#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "ulc_codegen.h"

#if YYDEBUG
extern int yydebug = 1;
#endif

static int compile(const char*);
static void show_help();

/* Print bytecodes to standard out? */
static bool stdoutFlag = false;

int main(int argc, char *argv[]) {
	int opt;

	setprogname(argv[0]);

	while ((opt = getopt(argc, argv, "d")) != -1) {
		switch(opt) {
			case 'd':
				stdoutFlag = true;
				break;
			case '?':
				show_help();
				break;
		}
	}

	argc -= optind;
	argv += optind;

	for (int i = 0; i < argc; i++)
		compile(argv[i]);

	if (argc == 0)
		fatal("%s: need a damn file name to compile!\n", getprogname());

	return EXIT_SUCCESS;
}

static int
compile(const char* source) {
	extern FILE* yyin;
	extern int yyparse();
	char *fout = NULL;
	size_t fsz;

	yyin = fopen(source, "r");
	if (!yyin)
		fatal("Could not open %s\n", source);

	/* Call the parser; currently a bison-generated parser */
	yyparse();

	if (stdoutFlag)
		prnt_code();

	fsz = strlen(source) + 2;
	if (!(fout = calloc(1, fsz)))
		fatal("%s: could not allocate memory\n", getprogname());
	strlcpy(fout, source, fsz);
	strlcat(fout, "b", fsz);

	save_code(fout);

	if (yyin)
		fclose(yyin);
	if (fout)
		free(fout);

	return 0;
}

static void
show_help()
{
	fprintf(stderr, "%s:  [-d] source file\n", getprogname());
	fprintf(stderr, "\t-d: show debugging info\n");
	exit(EXIT_FAILURE);
}
