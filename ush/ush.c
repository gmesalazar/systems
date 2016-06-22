#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <setjmp.h>
#include <bsd/string.h>

#include "include/ush.h"
#include "include/list.h"
#include "util.h"

enum redir_op {
	REDIR_NO  = 0x0,
	REDIR_IN  = 0x1,
	REDIR_OUT = 0x2,
	REDIR_ERR = 0x4,
};

static int prompt(FILE *fp);
static int unset(char *varName);
static int histSubst(struct_t **words);
static int varExpansion(struct_t **words);
static int filenameCompl(struct_t *words);
static int getSetArgs(struct_t *words, char **varName, char **varValue);
static int execWrapper(struct_t *words);
static int execWithPipes(struct_t *words);
static int execCommand(struct_t *words);
static int execExec(struct_t *words);
static int handleRedir(char **argv);
static char* handleRedirOperator(char **argv, enum redir_op *);
static char* lookupPath(const char *bin);
static char **buildArgv(struct_t *words);
static struct_t *buildWordsList(char *str);
static struct_t *lookupTable(const char *varName);
static struct_t *set(char *varName, char *varValue);

static void commLineOptions(int argc, char **argv);
static void printCommand(struct_t *words);
static void setHandlers(void);
static void sigHandler(int sig);
static void printHelp(void);

static unsigned int histSizeGl = 0;
static struct_t *varTableGl = NULL;
static struct_t *historyGl = NULL;
static bool verbose = false;

static sigjmp_buf buf;

int
main(int argc, char **argv) 
{
	char *home;
	char *rcfname;
	FILE *stream;
	size_t sz;

	commLineOptions(argc, argv);
	setHandlers();

	if ((home = getenv("HOME")) == NULL)
		error(1, 0, "Could not retrieve HOME environment variable\n");
    
	sz = strlen(home) + strlen("/") + strlen(RC_FILE_NAME) + 1;
	rcfname = (char*) calloc(sz, sizeof(char));

	strlcat(rcfname, home, sz);
	strlcat(rcfname, "/", sz);
	strlcat(rcfname, RC_FILE_NAME, sz);

	if ((stream = fopen(rcfname, "r")) != NULL) {
		prompt(stream);
		fclose(stream);
		free(rcfname);
	}

	prompt(stdin);
	return 0;
}

static int
prompt(FILE *fp)
{
	struct_t *words;
	int status;
	size_t iniSize = 100;
	ssize_t nread;
	char *str = NULL;
	char *prmpt = "$";

	struct_t *ps1;

	while (true) {
		if (fp != stdin && feof(fp))
			break;
		if (sigsetjmp(buf, 1)) {
			free(str);
			deallocStruct(&words);
			printf("\n");
		}
		ps1 = lookupTable("PS1");
		if (fp == stdin && ps1)
			prmpt = ps1->tableData.varValue;
		if (fp == stdin && !ps1)
			ps1 = set("PS1", prmpt);
		if (fp == stdin) {
			printf(" %s ", prmpt);
			fflush(stdout);
		}
		str = (char *) calloc(iniSize + 1, sizeof(char));
		if (str == NULL)
			error(1, 0, "Could not allocate string of size %zu bytes\n", iniSize + 1);
		nread = getline (&str, &iniSize, fp);
		if (nread == -1) {
			free(str);
			continue;
		}
		if (str[nread - 1] == '\n')
			str[nread - 1] = '\0';
		words = buildWordsList(str);
		if (!words) {
			free(str);
			deallocStruct(&words);
			continue;
		}
		histSubst(&words);
		if (verbose)
			printCommand(words);
		varExpansion(&words);
		if (verbose)
			printCommand(words);
		
		if (filenameCompl(words) == 0) {
			free(str);
			deallocStruct(&words);
			continue;
		}
		if (strcmp(words->wordData.word, "exit") == 0) {
			free(str);
			deallocStruct(&words);
			break;
		}
		status = execCommand(words);
		if (fp == stdin)
			printf("%d\n", status);
		free(str);
		deallocStruct(&words);
	}
	if (fp == stdin) {
		deallocStruct(&varTableGl);
		deallocStruct(&historyGl);
	}
	return 0;
}

static char**
buildArgv(struct_t *words) 
{
    struct_t *aux = NULL;
    char **argv = NULL;
    size_t sz, i;

    if (!words)
	return NULL;

    for(aux = words, sz = 0; aux; aux = aux->next, sz++);
    if ( !(argv = (char**) calloc(sz + 1, sizeof(char*))) )
	return NULL;
    for (i = 0, aux = words; i < sz; i++, aux = aux->next)
	argv[i] = aux->wordData.word;
    argv[i] = NULL;
    
    return argv;
}

static int
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
execCommand(struct_t *words) 
{
    struct_t *aux = words;
    char *varName;
    char *varValue;
    #define PATH_SZ 1024
    static char lastDir[PATH_SZ] = "\0";

    extern char **environ;

    if (strcmp(aux->wordData.word, "set") == 0) {
	if(getSetArgs(words, &varName, &varValue) == 0)
	    set(varName, varValue);
    }
    else if (strcmp(aux->wordData.word, "unset") == 0) {
	if (!words->next)
	    return -1;
	unset(words->next->wordData.word);
    }
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
    else if ((strcmp(aux->wordData.word, "cd") == 0) && aux->next &&
	     (strcmp(aux->next->wordData.word, "-") == 0)) {

	if (strlen(lastDir) == 0) {
	    fprintf(stderr, "cd: Last dir variable not set\n");
	    return -1;
	} else {
	    char temp[PATH_SZ];
	    getcwd(temp, sizeof(temp));
	    if (chdir(lastDir) == -1) {
		fprintf(stderr, "%s\n", strerror(errno));
		return -1;
	    }
	    printf("%s\n", lastDir);
	    strlcpy(lastDir, temp, PATH_SZ);
	}
    }
    else if (strcmp(aux->wordData.word, "cd") == 0) {
	const char *home = NULL;
	if (!getcwd(lastDir, sizeof(lastDir)));
	
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
    pid_t pid;
    char **argv = buildArgv(words);
    char *compName = lookupPath(argv[0]);
    char *filename = NULL;

    if (!compName) {
	fprintf(stderr, "No command '%s' found\n", argv[0]);
	goto clean;
    }

    if ( (pid = fork()) == -1) {
	fprintf(stderr, "Could not fork\n");
	goto clean;
    } 
    else if (pid == 0) {
	if (compName)
	    handleRedir(argv);
	if (execv(compName, argv) == -1)
	    fprintf(stderr, "%s\n", strerror(errno));
	goto clean;
    }
    else {
	if (waitpid(pid, &status, 0) == -1) {
	    fprintf(stderr, "%s\n", strerror(errno));
	    goto clean;
	} else
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

    tok = strtok(env, ":");
    while (tok) {
	dirp = opendir(tok);
	dire = readdir(dirp);

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
	closedir(dirp);
	tok = strtok(NULL, ":");
    }
    free(env);
    return compName;
}

static int
handleRedir(char **argv)
{
	int fd;
	char *filename;
	enum redir_op redir_op;

	filename = handleRedirOperator(argv, &redir_op);

	if (redir_op & REDIR_IN) {
		if ((fd = open(filename, O_RDONLY)) == -1)
			goto error;
		dup2(fd, 0);
	} else if (redir_op & (REDIR_OUT | REDIR_ERR)) {
		if ((fd = open(filename, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR)) == -1)
			goto error;
		dup2(fd, 1);
		if (redir_op & REDIR_ERR)
			dup2(fd, 2);
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
			break;
		}
		if (**aux == '>') {
			*redir_op = REDIR_OUT;
			filename = strdup(*(aux + 1));
			break;
		}
		if (**aux == '&' && *(*aux + 1) == '>') {
			*redir_op = REDIR_ERR;
			filename = strdup(*(aux + 1));
			break;
		}
	}

	return filename;
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

    token = strtok(str, " ");
    while (token) {

	if ( !(word = (struct_t*) calloc(1, sizeof(struct_t))) )
	    return NULL;

	word->structType = WORDS_LIST;
	sz = strlen(token) + 1;
	word->wordData.word = (char*) calloc(sz, sizeof(char));
	strlcpy(word->wordData.word, token, sz);

	if ( (insertNode(&words, word) == -1) )
	    return NULL;

	token = strtok(NULL, " ");
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

    aux = *words;

    while (aux) {
	if (!historyGl)
	    break;
	// 'last command' substitution
	if ( (ptr = strstr(aux->wordData.word, "!!")) ) {
	    struct_t *lastCom = cloneList(historyGl->histData.command);
	    struct_t *newStart = insertListIntoPos(lastCom, aux);
	    if (!aux->prev)
		*words = newStart;
	    free(aux->wordData.word);
	    free(aux);
	    aux = newStart;
	}
	// 'last word' substitution
	else if ( (ptr = strstr(aux->wordData.word, "!$")) ) {
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
	else if ( (ptr = strstr(aux->wordData.word, "!")) ) {
	    int num = 0;
	    char *str = NULL;
	    struct_t *match = NULL;
	    struct_t *newStart = NULL;
	    struct_t *naux = NULL;
	    
	    if (strlen(ptr) == 1)
		return -1;

	    // 'nth command'
	    if ( (isNumber(++ptr)) ) {
		num = atoi(ptr);
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

    if (hist && (histSizeGl < atoi(hist->tableData.varValue))) {

	newList = cloneList(*words);
	new = (struct_t*) calloc(1, sizeof(struct_t));
	if (!new)
	    error(1, 0, "Could not allocate new node\n");
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

static int
filenameCompl(struct_t *words) 
{
    struct_t *aux = NULL;
    char *compname = NULL;
    char *path = NULL;
    char *filename = NULL;
    size_t strSz;
    int pathStrSz;

    DIR *dirp = NULL;
    struct dirent *dire;

    if (!words)
	return -1;

    aux = words;
    while (aux->next)
	aux = aux->next;
    compname = aux->wordData.word;

    strSz = strlen(compname);

    if (!words->next) {
	if (compname[strSz - 1] == '\t')
	    compname[strSz - 1] = '\0';
	return -1;
    }

    if (compname[strSz - 1] != '\t')
	return -1;

    compname[strSz - 1] = '\0';
    
    if (strstr(compname, "/")) {
	
	pathStrSz = strlen(compname);
	while (pathStrSz > 0) {
	    if (compname[pathStrSz - 1] == '/')
		break;
	    --pathStrSz;
	}
	
	path = (char*) calloc(pathStrSz, sizeof(char));
	strSz = strlen(compname) - pathStrSz + 1;
	filename = (char*) calloc(strSz, sizeof(char));
	
	strlcpy(path, compname, pathStrSz);
	strlcpy(filename, compname + pathStrSz, strSz);

	dirp = opendir(path);
    } 
    else {
	dirp = opendir(".");
	filename = strdup(compname);
    }

    if (!dirp) {
	free(path);
	free(filename);
	return -1;
    }

    dire = readdir(dirp);
    while (dire) {
	if (startsWith(dire->d_name, filename) || (strSz == 1)) {
	    if (strcmp(dire->d_name, ".") != 0 && 
		strcmp(dire->d_name, "..") != 0)
		printf(" %s\n", dire->d_name);
	}
	dire = readdir(dirp);
    }

    free(filename);
    free(path);
    closedir(dirp);

    return 0;
}

static struct_t *
set(char *varName, char *varValue) 
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
	error(1, 0, "Could not allocate table node");
    new->tableData.varName = strdup(varName);
    new->tableData.varValue = strdup(varValue);
    insertNode(&varTableGl, new);

    return new;
}

static int 
unset(char *varName) 
{
    struct_t *aux = lookupTable(varName);

    if (!aux)
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
    int i;
    for (i = 1; i < argc; i++) {
	if (strcmp(argv[i], "-verbose") == 0)
	    verbose = true;
	else if (strcmp(argv[i], "-version") == 0) {
	    printf("mysh, version 1.0\n");
	    exit(0);
	} else {
	    printHelp();
	    exit(0);
	}
    }
}

static void
printHelp(void) 
{
    printf("mysh, version 1.0\n\n");
    printf("Command line options:\n");
    printf("\t-verbose\n");
    printf("\t-help\n");
    printf("\t-version\n\n");
    printf("Builtin commands:\n");
    printf("\tset [var = value]\n");
    printf("\tunset var\n");
    printf("\tsetenv [var = value]\n");
    printf("\tunsetenv var\n");
    printf("\tcd [dirs]\n");
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
    siglongjmp(buf, 1);
}
