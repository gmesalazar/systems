#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "util.h"
#include "ulc_codegen.h"

extern FILE* yyin;
extern int yyparse();
extern int yylex_destroy();

#if YYDEBUG
extern int yydebug;
yydebug = 1;
#endif

int
main(int argc, char **argv)
{
	int i;
	bool sout = false;
	char *foname = NULL;
	FILE *fin = NULL;

	for (i = 2; i < argc; i++)
		if (strcmp(argv[i], "-s") == 0)
			sout = true;

	if (argc > 1) {
		fin = fopen(argv[1], "r");
		if (!fin)
			fatal("Could not open %s\n", argv[1]);
		foname = argv[1];
		foname[strlen(foname) - 1] = 'b';
		yyin = fin;
	}

	while (yyparse());

	if (sout)
		dump_code();
	dump_bcodes(foname);

	yylex_destroy();
	fclose(fin);

	return EXIT_SUCCESS;
}
