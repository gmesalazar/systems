/*
 * List of TODO's
 *  - Ditch words list and work with a plain array of words?
 *  - Use editline history instead of my own?
 */

#include <ctype.h>
#include <dirent.h>
#include <err.h>
#include <errno.h>
#include <editline/readline.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <setjmp.h>
#include <unistd.h>

#include "include/ush.h"
#include "include/list.h"
#include "util.h"

enum redir_op {
	REDIR_NO  = 0x1,
	REDIR_IN  = 0x2,
	REDIR_OUT = 0x4,
	REDIR_ERR = 0x8,
};

/* prototypes */
static int prompt(FILE *);
static int unset(char *);
static int histSubst(struct_t**);
static int varExpansion(struct_t**);
static int getSetArgs(struct_t*, char**, char**);
static int execWrapper(struct_t*);
static int execWithPipes(struct_t*);
static int execCommand(struct_t*);
static int execExec(struct_t*);
static int handleRedir(char**);
static char*  handleRedirOperator(char**, enum redir_op*);
static char*  lookupPath(const char*);
static char** buildArgv(struct_t*);
static struct_t* buildWordsList(char*);
static struct_t* lookupTable(const char*);
static struct_t* set(const char*, const char*);
static void commLineOptions(int, char**);
static void printCommand(struct_t*);
static void runSubsts(struct_t**);
static void setHandlers(void);
static void sigHandler(int);
static void printHelp(void);
static void setPrompt(void);
/* prototypes */

/* globals */
static unsigned int histSizeGl = 0;
static struct_t* varTableGl = NULL;
static struct_t* historyGl = NULL;
static bool verbose = false;
static sigjmp_buf jmpbuf_gl;
static char* prompt_Gl = " $ ";
/* /globals */


/* ush main*/
int main(int argc, char* argv[])
{
	size_t sz;
	char* home;
	char* rcname;
	FILE* stream;

	// parse command line options
	commLineOptions(argc, argv);
	// set signal handlers
	setHandlers();

	if (!(home = getenv("HOME")))
		err(1, 0, "Could not retrieve HOME environment variable\n");

	// TODO save home into a global for later use
	// TODO factor this out
	sz = strlen(home) + strlen("/") + strlen(RC_FILE_NAME) + 1;
	rcname = calloc(sz, sizeof(char));

	strlcat(rcname, home, sz);
	strlcat(rcname, "/", sz);
	strlcat(rcname, RC_FILE_NAME, sz);

	if ((stream = fopen(rcname, "r")) != NULL) {
		prompt(stream);
		fclose(stream);
		free(rcname);
	}

	prompt(stdin);

	deallocStruct(&varTableGl);
	deallocStruct(&historyGl);

	return 0;
}

/* ush main */
static int
prompt(FILE* fp)
{
	int status;
	char* line = NULL;
	struct_t* words = NULL;

	// TODO unused for now
	// need to test config file loading
	(void)fp;

	while(true) {
		setPrompt();
		if (sigsetjmp(jmpbuf_gl, 1))
			goto clean;
		if (!(line = readline(prompt_Gl)))
			goto clean;
		if (strlen(line) == 0)
			goto clean;
		else
			add_history(line);

		// build a list of 'words' containing commands
		// and arguments
		words = buildWordsList(line);

		// perform several substitutions on the word
		// list
		runSubsts(&words);

		// on to execution...
		status = execCommand(words);
clean:
		// if something goes wrong, clear this iteration and continue
		free(line);
		deallocStruct(&words);
	}
	return 0;
}

static char**
buildArgv(struct_t* words)
{
	struct_t* aux = NULL;
	char** argv = NULL;
	size_t sz, i;

	if (!words)
		return NULL;

	for(aux = words, sz = 0; aux; aux = aux->next, sz++);
	if ((argv = calloc(sz + 1, sizeof(char*))) == NULL)
		return NULL;
	for (i = 0, aux = words; i < sz; i++, aux = aux->next)
		argv[i] = aux->wordData.word;
	argv[i] = NULL;

	return argv;
}

static int
/*
 * get arguments to the 'set' command -- variable nam
 * and value
 */
getSetArgs(struct_t *words, char **varName, char **varValue)
{
	struct_t *aux = words->next;
	char *eq = NULL;

	if (!aux) {
		*varName = NULL;
		*varValue = NULL;
		return 0;
	}
	*varName = aux->wordData.word;

	aux = aux->next;
	if (!aux) {
		fprintf(stderr, "Malformed expression\n");
		return -1;
	}
	eq = aux->wordData.word;

	aux = aux->next;
	if (!aux) {
		fprintf(stderr, "Malformed expression\n");
		return -1;
	}
	*varValue = aux->wordData.word;

	if (strcmp(eq, "=") != 0) {
		fprintf(stderr, "Malformed expression\n");
		return -1;
	}

	return 0;
}

static int
/*
 * Check if the command is a builtin; if so, execute.
 * Otherwise, call execWrapper to handle pipes and exec
 */
execCommand(struct_t *words)
{
	char *varName  = NULL;
	char *varValue = NULL;
	extern char **environ;
	static char lastDir[MAXPATHLEN];
	struct_t *aux = words;

	if (strcmp(aux->wordData.word, "set") == 0) {
		if(getSetArgs(words, &varName, &varValue) == 0)
			set(varName, varValue);
	}
	else if (strcmp(aux->wordData.word, "unset") == 0) {
		if (!words->next)
			return -1;
		unset(words->next->wordData.word);
	}
	/*
	 * export an environment variable
	 * TODO: factor this out to a function
	 */
	else if (strcmp(aux->wordData.word, "setenv") == 0) {
		char **env;
		if (getSetArgs(words, &varName, &varValue) == 0) {
			if (!varName || !varValue)
				for (env = environ; *env; ++env)
					printf("%s\n", *env);
			setenv(varName, varValue, 1);
		}
	}
	else if (strcmp(aux->wordData.word, "unsetenv") == 0) {
		if (!words->next)
			return -1;
		unsetenv(words->next->wordData.word);
	}
	/*
	 * execute `cd dir`
	 * TODO: factor this out to a function
	 */
	else if ((strcmp(aux->wordData.word, "cd") == 0) && aux->next &&
			(strcmp(aux->next->wordData.word, "-") == 0)) {
		if (strlen(lastDir) == 0) {
			fprintf(stderr, "cd: Last dir variable not set\n");
			return -1;
		}
		else {
			char temp[MAXPATHLEN];
			getcwd(temp, sizeof(temp));
			if (chdir(lastDir) == -1) {
				fprintf(stderr, "%s\n", strerror(errno));
				return -1;
			}
			printf("%s\n", lastDir);
			strlcpy(lastDir, temp, MAXPATHLEN);
		}
	}
	/* execute `cd`
	 * TODO: factor this out to a function
	 */
	else if (strcmp(aux->wordData.word, "cd") == 0) {
		const char *home = NULL;
		getcwd(lastDir, sizeof(lastDir));

		if (!aux->next) {
			if (!(home = getenv("HOME"))) {
				fprintf(stderr, "%s\n", strerror(errno));
				return -1;
			}
			if (chdir(home) == -1) {
				fprintf(stderr, "%s\n", strerror(errno));
				return -1;
			}
		}
		else if (chdir(aux->next->wordData.word) == -1) {
			fprintf(stderr, "%s\n", strerror(errno));
			return -1;
		}
	}
	else if (strcmp(aux->wordData.word, "exit") == 0)
		exit(0);
	else if (strcmp(aux->wordData.word, "history") == 0)
		printStruct(historyGl);
	else if (strcmp(aux->wordData.word, "verbose") == 0)
		verbose = true;
	else if (strcmp(aux->wordData.word, "nonverbose") == 0)
		verbose = false;
	else
		return execWrapper(words);
	return 0;
}

static int
execWrapper(struct_t *words)
{
	struct_t *pipe = NULL;
	struct_t *aux = words;
	int status = -1;
	pid_t pid;

	while (aux) {
		if (strcmp(aux->wordData.word, "|") == 0) {
			pipe = aux;
			break;
		}
		aux = aux->next;
	}

	if (!pipe)
		status = execExec(words);
	else {
		if ((pid = vfork()) < 0)
			fprintf(stderr, "Could not fork\n");
		else if (pid == 0)
			execWithPipes(words);
		else {
			if (waitpid(pid, &status, 0) == -1) {
				fprintf(stderr, "%s\n", strerror(errno));
				return -1;
			}
			status = status >> 8;
		}

	}

	return status;
}

static int
execWithPipes(struct_t *words)
{
	int status = -1;
	struct_t* lastPipe = NULL;
	struct_t* aux;
	int pfd[2];
	pid_t pid;

	aux = words;
	while (aux) {
		if (strcmp(aux->wordData.word, "|") == 0)
			lastPipe = aux;
		aux = aux->next;
	}

	if (!lastPipe) {
		status = execExec(words);
		exit(status);
	} else {
		aux = lastPipe->next;
		lastPipe->prev->next = NULL;

		if (pipe(pfd) < 0) {
			fprintf(stderr, "Could not pipe\n");
			exit(1);
		}

		if ( (pid = vfork()) == - 1) {
			fprintf(stderr, "Could not fork\n");
			exit(1);
		}
		else if (pid == 0) {
			close(pfd[0]);
			dup2(pfd[1], STDOUT_FILENO);
			close(pfd[1]);
			execWithPipes(words);
		}
		else {
			close(pfd[1]);
			dup2(pfd[0], STDIN_FILENO);
			close(pfd[0]);
			status = execExec(aux);
			exit(status);
		}
	}

	return status;
}

static int
execExec(struct_t *words)
{
	int status = -1;
	char *filename = NULL;
	char **argv = buildArgv(words);
	char *compName = lookupPath(argv[0]);
	pid_t pid;

	if (!compName) {
		fprintf(stderr, "No command '%s' found\n", argv[0]);
		goto clean;
	}

	if ((pid = fork()) == -1) {
		fprintf(stderr, "Could not fork\n");
		goto clean;
	}
	// child, the program
	else if (pid == 0) {
		if (compName)
			handleRedir(argv);
		if (execv(compName, argv) == -1)
			fprintf(stderr, "%s\n", strerror(errno));
		goto clean;
	}
	// parent, the shell
	else {
		if (waitpid(pid, &status, 0) == -1)
			fprintf(stderr, "%s\n", strerror(errno));
		goto clean;
	}

clean:
	free(compName);
	free(filename);
	free(argv);
	return (status == -1 ? 127 : status);
}

static char*
lookupPath(const char *progName)
{
	char *env = strdup(getenv("PATH"));
	char *tok = NULL;
	char *compName = NULL;
	DIR *dirp = NULL;
	struct dirent *dire = NULL;
	size_t sz;

	for (tok = strtok(env, ":"); tok; tok = strtok(NULL, ":")) {
		if (!(dirp = opendir(tok)) || !(dire = readdir(dirp)))
			continue;

		while (dire) {
			if (strcmp(progName, dire->d_name) == 0) {
				sz = strlen(tok) + 1 + strlen(progName) + 1;
				compName = (char*) calloc(sz + 1, sizeof(char));
				strlcat(compName, tok, sz);
				strlcat(compName, "/", sz);
				strlcat(compName, progName, sz);
			}
			dire = readdir(dirp);
		}

		if (dirp)
			closedir(dirp);
	}
	free(env);
	return compName;
}

static int
handleRedir(char **argv)
{
	int fd;
	char *filename;
	enum redir_op redir;

	filename = handleRedirOperator(argv, &redir);

	if (redir & REDIR_IN) {
		if ((fd = open(filename, O_RDONLY)) == -1)
			goto error;
		dup2(fd, STDIN_FILENO);
	} else if (redir & (REDIR_OUT | REDIR_ERR)) {
		if ((fd = open(filename, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR)) == -1)
			goto error;
		dup2(fd, STDOUT_FILENO);
		if (redir & REDIR_ERR)
			dup2(fd, STDERR_FILENO);
	}

	return 0;
error:
	fprintf(stderr, "Redir failed (%s)\n", strerror(errno));
	return -1;
}

	static char*
handleRedirOperator(char **argv, enum redir_op *redir_op)
{
	char **aux;
	char *filename;

	filename  = NULL;
	*redir_op = REDIR_NO;

	for (aux = argv; *aux; aux++) {
		if (**aux == '<') {
			*redir_op = REDIR_IN;
			filename = strdup(*(aux + 1));
			*aux = NULL;
			break;
		}
		if (**aux == '>') {
			*redir_op = REDIR_OUT;
			filename = strdup(*(aux + 1));
			*aux = NULL;
			break;
		}
		if (**aux == '&' && *(*aux + 1) == '>') {
			*redir_op = REDIR_ERR;
			filename = strdup(*(aux + 1));
			*aux = NULL;
			break;
		}
	}

	return filename;
}

void
setPrompt(void)
{
	struct_t* ps1;
	if ((ps1 = lookupTable("PS1")))
		prompt_Gl = ps1->tableData.varValue;
}

void
runSubsts(struct_t **words) {
	// perform history substitution
	histSubst(words);
	if (verbose)
		printCommand(*words);

	// perform variable expansion
	varExpansion(words);
	if (verbose)
		printCommand(*words);
}


static struct_t *
buildWordsList(char *str)
{
	char *token = NULL;
	struct_t *words = NULL;
	struct_t *word = NULL;
	size_t sz = 0;

	if (!str)
		return NULL;

	for (token = strtok(str, " "); token; token = strtok(NULL, " ")) {
		if (!(word = (struct_t*) calloc(1, sizeof(struct_t))))
			return NULL;

		word->structType = WORDS_LIST;
		sz = strlen(token) + 1;
		word->wordData.word = (char*) calloc(sz, sizeof(char));
		strlcpy(word->wordData.word, token, sz);

		if ((insertNode(&words, word) == -1))
			return NULL;
	}

	return words;
}

static int
histSubst(struct_t **words)
{
	struct_t *aux = NULL;
	struct_t *newList = NULL;
	struct_t *new = NULL;
	struct_t *hist = NULL;
	char *ptr = NULL;

	if (!*words)
		return -1;

	hist = lookupTable("HISTSIZE");
	aux  = *words;

	while (aux) {
		if (!historyGl)
			break;
		// 'last command' substitution
		if ((ptr = strstr(aux->wordData.word, "!!"))) {
			struct_t *lastCom = cloneList(historyGl->histData.command);
			struct_t *newStart = insertListIntoPos(lastCom, aux);
			if (!aux->prev)
				*words = newStart;
			free(aux->wordData.word);
			free(aux);
			aux = newStart;
		}
		// 'last word' substitution
		else if ((ptr = strstr(aux->wordData.word, "!$"))) {
			struct_t *lastWord = historyGl->histData.command;
			struct_t *newStart = NULL;

			while (lastWord->next)
				lastWord = lastWord->next;

			lastWord = cloneNode(lastWord);
			newStart = insertListIntoPos(lastWord, aux);
			if (!aux->prev)
				*words = newStart;
			free(aux->wordData.word);
			free(aux);
			aux = newStart;
		}
		// 'nth command' or 'starting with' history substitution
		else if ((ptr = strstr(aux->wordData.word, "!"))) {
			int num = 0;
			char *str = NULL;
			struct_t *match = NULL;
			struct_t *newStart = NULL;
			struct_t *naux = NULL;

			if (strlen(ptr) == 1)
				return -1;
			// 'nth command'
			if ( (isNumber(++ptr)) ) {
				num = strtol(ptr, NULL, 10);
				if (num <= 0)
					continue;
				match = getNthNode(historyGl, num);
				if ( !match )
					break;
				match = cloneList(match->histData.command);
				newStart = insertListIntoPos(match, aux);
				if (!aux->prev)
					*words = newStart;
				free(aux->wordData.word);
				free(aux);
				aux = newStart;
			}
			// 'command starting with'
			else {
				str = ptr;
				naux = historyGl;
				while (naux) {
					if (strstr(naux->histData.command->wordData.word, str))
						match = cloneList(naux->histData.command);
					naux = naux->next;
				}
				newStart = insertListIntoPos(match, aux);
				if (!aux->prev)
					*words = newStart;
				free(aux->wordData.word);
				free(aux);
				aux = newStart;
			}
		}
		aux = aux->next;
	}

	if (hist && (histSizeGl < strtoul(hist->tableData.varValue, NULL, 10))) {
		newList = cloneList(*words);
		new = (struct_t*) calloc(1, sizeof(struct_t));
		if (!new)
			err(1, 0, "Could not allocate new node\n");
		new->structType = HIST_LIST;
		new->histData.command = newList;
		new->next = NULL;
		insertNode(&historyGl, new);
		histSizeGl++;
	}
	return 0;
}

static int
varExpansion(struct_t **words)
{
	size_t sz = 0;
	struct_t *aux = NULL;
	char *ptr1, *ptr2, *varName = NULL;
	struct_t *var = NULL;

	if (!*words)
		return -1;

	aux = *words;

	while (aux) {
		if (!varTableGl)
			break;

		if ( (ptr1 = strstr(aux->wordData.word, "$(")) &&
				(ptr2 = strstr(aux->wordData.word, ")")) ) {
			sz = ptr2 - (ptr1 + 2) + 1;
			if (sz == 0)
				return -1;
			varName = (char*) calloc(sz, sizeof(char));
			strlcpy(varName, aux->wordData.word + 2, sz);

		} else if ( (ptr1 = strstr(aux->wordData.word, "$")) ) {
			sz = strlen(aux->wordData.word + 1) + 1;
			if (sz == 1)
				return -1;
			varName = (char*) calloc(sz, sizeof(char));
			strlcpy(varName, aux->wordData.word + 1, sz);
		}

		if (varName) {
			if ( (var = lookupTable(varName)) ) {
				free(aux->wordData.word);
				aux->wordData.word = strdup(var->tableData.varValue);
			} else {
				free(aux->wordData.word);
				aux->wordData.word = strdup("");
			}
			free(varName);
		}
		aux = aux->next;
	}

	return 0;
}

static struct_t *
set(const char *varName, const char *varValue)
{
	struct_t *aux = NULL;
	struct_t *new = NULL;
	table_data_t data;

	if (varName == NULL || varValue == NULL) {
		aux = varTableGl;
		while (aux) {
			data = aux->tableData;
			printf("%s=%s\n", data.varName, data.varValue);
			aux = aux->next;
		}
		return varTableGl;
	}

	aux = lookupTable(varName);
	if (aux) {
		free(aux->tableData.varValue);
		aux->tableData.varValue = strdup(varValue);
		return aux;
	}

	new = (struct_t*) calloc(1, sizeof(struct_t));
	new->structType = TABLE;
	if (new == NULL)
		err(1, 0, "Could not allocate table node");
	new->tableData.varName  = strdup(varName);
	new->tableData.varValue = strdup(varValue);
	insertNode(&varTableGl, new);

	return new;
}

static int
unset(char *varName)
{
	struct_t *aux = lookupTable(varName);

	if (aux == NULL)
		return -1;

	if (!aux->prev)
		varTableGl = aux->next;
	else
		aux->prev->next = aux->next;

	if (!aux->next) {
		if (aux->prev)
			aux->prev->next = NULL;
	} else
		aux->next->prev = aux->prev;

	free(aux->tableData.varName);
	free(aux->tableData.varValue);
	free(aux);

	return 0;
}

static struct_t*
lookupTable(const char *varName)
{
	struct_t *aux = varTableGl;
	while (aux && ((strcmp(aux->tableData.varName, varName) != 0)) )
		aux = aux->next;
	return aux;
}

static void
printCommand(struct_t *words)
{
	struct_t *aux = words;
	while (aux) {
		printf("%s", aux->wordData.word);
		if (aux->next)
			printf(",");
		aux = aux->next;
	}
	printf("\n");
}

static void
commLineOptions(int argc, char **argv)
{
	int ch;
	while((ch = getopt(argc, argv, "vh")) != -1) {
		switch(ch) {
			case 'v':
				verbose = true;
				break;
			case 'h':
				printHelp();
				exit(0);
				break;
		}
	}
}

	static void
printHelp(void)
{
	printf("\tush beta\n\n");
	printf("Command line options:\n");
	printf("\t-v enable verbose output\n");
	printf("\t-h print help info\n");
	printf("Builtin commands:\n");
	printf("\tset var = value\n");
	printf("\tunset var\n");
	printf("\tsetenv var = value\n");
	printf("\tunsetenv var\n");
	printf("\tcd dir\n");
	printf("\thistory\n");
	printf("\tverbose\n");
	printf("\tnonverbose\n");
}

	static void
setHandlers(void)
{
	struct sigaction new;

	new.sa_handler = sigHandler;
	sigemptyset(&new.sa_mask);
	new.sa_flags = 0;

	sigaction(SIGINT, &new, NULL);
	sigaction(SIGQUIT, &new, NULL);
	sigaction(SIGTSTP, &new, NULL);
}

	static void
sigHandler(int sig)
{
	(void) sig;
	siglongjmp(jmpbuf_gl, 1);
}
