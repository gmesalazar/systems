#include <dirent.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void traverse(char *, int depth);

int main(int argc, char **argv) 
{
	if (argc == 1)
		traverse(".", 0);
	else
		traverse(argv[1], 0);

	return EXIT_SUCCESS;
}

static void
traverse(char *name, int n) 
{
    DIR *dir;
    struct dirent *ent;

	chdir(name);

    if (!(dir = opendir(".")))
		error(1, 0, "could not open current dir\n");

	for(;;) {
		int i;

		if (!(ent = readdir(dir)))
			break;
		if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
			continue;

		for (i = 0; i < n; i++)
			printf("\t");

		if (ent->d_type == DT_DIR) {
			printf("| d: %s\n", ent->d_name);
			traverse(ent->d_name, n + 1);
		} else if (ent->d_type == DT_REG) {
			printf("| f: %s\n", ent->d_name);
		}
	}

	if (strcmp(name, ".") != 0)
		chdir("../");
	closedir(dir);
}


