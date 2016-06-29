/*
  TODO:
       - Handle exceptional conditions throughout the code
       - Improve options processing (using getopt?)
 */

#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <utime.h>
#include <bsd/stdlib.h>
#include <bsd/string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "include/list.h"
#include "include/uar.h"


/* Static declarations -- Main operations */
static int extract(char *ar_name, char *file_name);
static void print_table(char *arch_name, char *fname);
static int replace_or_add(char *ar_name, char *file_name);
static int delete_file(char *ar_name, char *file_name);

/* Static declarations -- Helper functions*/
static void print_err (char *fmt, ...);
static void print_usage (void);
static char get_op (int argc, char **argv);
static void check_archive (char *fname, bool ck_exist);
static int parse_archive(char *fname);
static uint32_t get_acc_size (uint32_t upto);

int
main(int argc, char **argv)
{
	char *arch_name;
	char op;

	setprogname(argv[0]);
	arch_name = argv[2];

	op = get_op(argc, argv);

	switch (op) {
		int i;
		case 'd':
		for (i = 3; i < argc; i++)
			delete_file (arch_name, argv[i]);
		break;
		case 'r':
		for (i = 3; i < argc; i++)
			replace_or_add(arch_name, argv[i]);
		break;
		case 'x':
		if (argc > 3)
			for (i = 3; i < argc; i++)
			extract(arch_name, argv[i]);
		else
			extract(arch_name, NULL);
		break;
		case 't':
		if (argc > 3)
			for (i = 3; i < argc; i++)
			print_table(arch_name, argv[i]);
		else
			print_table(arch_name, NULL);
		break;
		default:
		print_usage();
		return(1);
	}

	return 0;
}

static void 
print_err(char *fmt, ...) 
{
	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	list_free();
	exit(1);
}

static void 
print_usage() 
{
	printf(" Usage: %s [command] archive-file file(s)\n", getprogname());
	printf("\n commands:\n");
	printf("\td\t- delete file(s) from the archive\n");
	printf("\tr\t- replace or insert new file(s) into the archive\n");
	printf("\tx\t- extrat file(s) from the archive\n");
	printf("\tt\t- display contents of the archive\n");
}

static char
/* Handle invalid inputs and return the option argument */
get_op (int argc, char **argv) 
{
	char *op = argv[1];

	if (argc < 3) {
		print_usage();
		exit(1);
	}
	else if ( (argc == 3) && (op[0] == 'd') ) {
		print_usage();
		exit(1);
	}

	return op[0];
}

static void 
/* Make sure the archive exists and begins with the proper magic string */
check_archive(char *ar_name, bool err_noexist)
{
	int fd;
	char str[AR_MAGIC_SZ + 1];

	if ((fd = open(ar_name, O_RDONLY)) == -1)
		if (err_noexist)
			goto error;

	if (read(fd, str, AR_MAGIC_SZ + 1) == -1)
		goto close;

	if (strcmp(str, "!<arch>\n") != 0)
		print_err("%s: %s: File format not recognized\n",  getprogname(),
			ar_name);

close:
	close(fd);
error:
	print_err("%s: error code %d (%s)\n", getprogname, errno, strerror(errno));
	return;
}

static int 
/* Read the archive and constructs a linked list out of the headers; the use 
   of this list greatly simplifies some operations on the archive */
parse_archive(char *fname)
{
	FILE *f;
	struct header header;

	f = fopen(fname, "r");
	fseek(f, AR_MAGIC_SZ, SEEK_SET);

	list_free();

	while ((fscanf(f, "%16s %12s %6s %6s %8s %10s %2s", header.fname, header.date,
		    header.uid, header.gid, header.mode, header.fsize, 
		    header.magic)) == 7) {	
		fseek(f, strtoul(header.fsize, NULL, 10) + 1, SEEK_CUR);
		/* Take the "/" out of the file's name */
		header.fname[strlen(header.fname) - 1] = '\0';
		list_insert(header);
	}

	fclose(f);
	return 0;
}

static void
/* Print the contents of the archive. If a file name is passed, it's printed out
   only if it's contained in the archive; if NULL is passed, all the files' 
   names are printed out */
print_table(char *arch_name, char *fname)
{
	struct node *node = NULL;

	check_archive(arch_name, true);
	parse_archive(arch_name);

	if (!fname)
	while ( (node = list_next(node)) )
	    printf("%s\n", node->header.fname);
    else {
	node = list_lookup(fname);
	if (node)
	    printf("%s\n", fname);
	else
	    printf("no entry %s found\n", fname);
    }
}

static int
extract_aux(char *ar_name, struct node *node)
{
	FILE *file;
	FILE *arch;
	size_t acc_size;
	size_t offset;
	size_t filesz;
	struct utimbuf tbuff;

	arch = fopen(ar_name, "r");
	file = fopen(node->header.fname, "w");

	acc_size = get_acc_size(node->seq);
	offset = AR_MAGIC_SZ + node->seq * F_HDR_SZ + acc_size;

	fseek(arch, offset, SEEK_SET);

	filesz = strtoul(node->header.fsize, NULL, 10);

	while (filesz--) {
		char ch = getc(arch);
		putc(ch, file);
	}

	fclose(arch);
	fclose(file);

	tbuff.actime = tbuff.modtime = strtoul(node->header.date, NULL, 10);

	utime(node->header.fname, &tbuff);
	chmod(node->header.fname, (unsigned short) strtoul(node->header.mode,
						       NULL, 8));

    return 0;
}

static int
extract(char *ar_name, char *file_name)
{
	struct node *ptr = NULL;

	check_archive(ar_name, true);
	parse_archive(ar_name);

	if (!file_name)
	while ((ptr = list_next(ptr)) != NULL)
		extract_aux(ar_name, ptr);

	else {
		if (!(ptr = list_lookup(file_name))) {
			printf("%s: no entry %s found\n", getprogname(), file_name);
			exit(0);
		}
		extract_aux(ar_name, ptr);
	}

	return 0;
}

static int
/* Add or replace a file into the archive. */
replace_or_add(char *ar_name, char *file_name)
{    
	FILE *file;
	FILE *arch;
	FILE *bkpfile;

	struct stat file_st;
	struct stat arch_st;

	struct node *ptr;

	size_t acc_size;
	size_t bkpbuff_sz;

	uint32_t offset;
	uint32_t offset_next;

	size_t aux;

	int ch;

	char tmpstr[FNAME_SZ + 1];
	strlcpy(tmpstr, file_name, FNAME_SZ);
	strlcat(tmpstr, "/", FNAME_SZ + 1);

	if (!(file = fopen(file_name, "r")))
		print_err("%s: %s: No such file or directory\n", getprogname(),
		  file_name);

	if ( !(arch = fopen(ar_name, "r+")) ) {
		printf("%s: creating %s\n", getprogname(), ar_name);
		arch = fopen(ar_name, "w+");
		fprintf(arch, "%s", AR_MAGIC_STR);
		fflush(arch);
	}

	check_archive(ar_name, false);
	parse_archive(ar_name);

	fstat(fileno(file), &file_st);

	/* The file is not in the archive */
	if ((ptr = list_lookup(file_name)) == NULL) {
		fseek(arch, 0, SEEK_END);
		fprintf(arch, "%-16s%-12ld%-6u%-6u%-8o%-10zu%c%c", tmpstr,
			file_st.st_mtime, file_st.st_uid, file_st.st_gid,
			file_st.st_mode, file_st.st_size, 0x60, 0x0A);

		rewind(file);
		while ((ch = getc(file)) != EOF)
			putc(ch, arch);
	}

	/* The file is in the archive, let's replace it */
	else {
		fstat(fileno(arch), &arch_st);
		/* Summation of each file's body size, up to a given element in the
		   headers list -- in this case, up to (not including) the file to be
		   replaced */
		acc_size = get_acc_size(ptr->seq);

		/* The offset of the file to be replaced, i.e., number of bytes from the
		   beginning of the file up to the start of the file's header */
		offset = AR_MAGIC_SZ + (ptr->seq - 1) * F_HDR_SZ + acc_size;

		/* The offset of the next file, after the one to be replaced; we need
		   this information to save and relocate subsequent contents of the
		   archive */
		offset_next = offset + F_HDR_SZ + strtoul(ptr->header.fsize, NULL, 10);

		/* Going to the right point to start backing up the contents of the
		   archive */
		fseek(arch, offset_next, SEEK_SET);

		bkpbuff_sz = arch_st.st_size - ftell(arch);

		if ( !(bkpfile = tmpfile()) )
			print_err("%s: Could not create a temp file; aborting\n");

		aux = bkpbuff_sz;
		while (aux--) {
			ch = getc(arch);
			putc(ch, bkpfile);
		}

		/* Seeking back to the file that's going to be replaced */
		fseek(arch, offset, SEEK_SET);
		fprintf(arch, "%-16s%-12ld%-6u%-6u%-8o%-10zu%c%c", tmpstr,
			file_st.st_mtime, file_st.st_uid, file_st.st_gid,
			file_st.st_mode, file_st.st_size, 0x60, 0x0A);

		while ((ch = getc(file)) != EOF)
			putc(ch, arch);

		/* We may need to truncate the archive if the size of file being
		   replaced is less than its previous size */
		if (file_st.st_size < strtol(ptr->header.fsize, NULL, 10))
			if ( (ftruncate(fileno(arch), ftell(arch))) == -1)
			print_err("%s: %s: Could not complete the replace operation\n",
				  getprogname(), file_name);

		/* Write the backed up portion of the archive back to it */
		rewind(bkpfile);
		aux = bkpbuff_sz;
		while(aux--) {
			ch = getc(bkpfile);
			putc(ch, arch);
		}

		/* We need to update the size of the file in the list of headers*/
		snprintf(ptr->header.fsize, sizeof(ptr->header.fsize), "%lu",
			 file_st.st_size);

		fclose(bkpfile);
	}

	fclose(file);
	fclose(arch);

	return 0;
}

static int
delete_file (char *ar_name, char *file_name)
{
	FILE *arch;
	char *bkpbuff;

	size_t acc_size;
	size_t bkpbuff_sz;

	uint32_t offset_header;
	uint32_t offset_next;

	struct node *ptr;
	struct stat arch_st;

	check_archive(ar_name, true);
	parse_archive(ar_name);

	if ( !(arch = fopen(ar_name, "r+")) )
		print_err("%s: %s: Could not open the archive\n", getprogname(),
		  ar_name);

	if ( !(ptr = list_lookup(file_name)) ) {
		printf("%s: no entry %s found\n", getprogname(), file_name);
		exit(0);
	}

	else {
		acc_size = get_acc_size(ptr->seq);
		offset_header = AR_MAGIC_SZ + (ptr->seq - 1) * F_HDR_SZ +
			acc_size;
		offset_next = offset_header + F_HDR_SZ +
			strtoul(ptr->header.fsize, NULL, 10);

		fseek(arch, offset_next, SEEK_SET);
		fstat(fileno(arch), &arch_st);
		bkpbuff_sz = arch_st.st_size - ftell(arch);
		bkpbuff = (char*) calloc(bkpbuff_sz + 1, sizeof(char));

		if ((fread(bkpbuff, sizeof(char), bkpbuff_sz, arch)) != bkpbuff_sz)
			print_err("%s: %s: Could not delete file\n", getprogname(), file_name);

		fseek(arch, offset_header, SEEK_SET);
		fwrite(bkpbuff, sizeof(char), bkpbuff_sz, arch);

		if ((ftruncate(fileno(arch), ftell(arch))) == -1)
			print_err("%s: %s: Could not complete deletion of file\n",
				  getprogname(), file_name);

		free(bkpbuff);
	}

	return 0;
}

static uint32_t
/* Iterate over the list of headers (initialized in the function parse_archive)
   and return the summation of files sizes, up to (not including) a given file
   identified by its position in the archive and list of headers */
get_acc_size (uint32_t upto)
{
	size_t acc = 0;
	struct node *aux = NULL;

	while ((aux = list_next(aux)) != NULL && (aux->seq < upto))
		acc += strtoul(aux->header.fsize, NULL, 10);
	while ((aux = list_next(aux)) != NULL);

	if (acc <= 1)
		return 0;
	else
		return acc;
}
