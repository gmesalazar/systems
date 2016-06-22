#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static void
fatal(char *str)
{
    fprintf(stderr, "%s\n", str);
    exit(EXIT_FAILURE);
}

static void
traverse(char *name, int isdir, int n) 
{
    DIR *dir;
    struct dirent *ent;

    if (isdir)
		chdir(name);

    if (!(dir = opendir(".")))
		fatal("problema ao abrir o diretorio");

    for(;;) {

		if (!(ent = readdir(dir)))
			break;

		if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
			continue;
		
		int i;
		for (i = 0; i < n; i++)
			printf("\t");

		if (ent->d_type == DT_DIR) {
			printf("| d: %s\n", ent->d_name);
			traverse(ent->d_name, 1, n + 1);
		} else if (ent->d_type == DT_REG) {
			printf("| f: %s\n", ent->d_name);
		}
	
    }

    if (isdir && strcmp(name, ".") != 0)
		chdir("../");
    closedir(dir);
}

int main(int argc, char **argv) 
{
	if (argc == 1)
		traverse(".", 1, 0);
	else
		traverse(argv[1], 1, 0);

    return EXIT_SUCCESS;
}
