/*
  TODO:
       - Handle exceptional conditions throughout the code
       - Improve options processing (using getopt?)
 */

#include <err.h>
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
#include <sys/stat.h>
#include <sys/types.h>

#include "include/list.h"
#include "include/uar.h"


/* Main operations */
static int extract(char *ar_name, char *file_name);
static void print_table(char *arch_name, char *fname);
static int replace_or_add(char *ar_name, char *file_name);
static int delete_file(char *ar_name, char *file_name);

/* Helper functions*/
static void print_usage (void);
static char get_op (int, char**);
static void check_archive (char*);
static int parse_archive(char*);
static uint32_t get_acc_size (uint32_t);
static void dispatch(int, char*[]);

enum {
	OP_DEL = 0x01,
	OP_ADD = 0x02,
	OP_EXT = 0x04,
	OP_TAB = 0x08,
	OP_CHK = 0x10
};

static int flag = 0;

int
main(int argc, char **argv)
{
	int ch = 0;

	setprogname(argv[0]);

	while((ch = getopt(argc, argv, "drxtc")) != -1) {
		switch (ch) {
			case 'd':
				flag |= OP_DEL;
				break;
			case 'r':
				flag |= OP_ADD;
				break;
			case 'x':
				flag |= OP_EXT;
				break;
			case 't':
				flag |= OP_TAB;
				break;
			case 'c':
				flag |= OP_CHK;
				break;
			case '?':
				print_usage();
		}
	}

	argc -= optind;
	argv += optind;

	dispatch(argc, argv);

	return 0;
}

static void
dispatch(int argc, char *argv[]) {
	if (flag & OP_DEL) {
		if (argc < 2)
			print_usage();
	}
	if (flag & OP_ADD) {
		if (argc < 2)
			print_usage();
	}
	if (flag & OP_EXT) {
		if (argc < 1)
			print_usage();
	}
	if (flag & OP_TAB) {
		if (argc < 1)
			print_usage();
	}
	if (flag & OP_CHK) {
		if (argc < 1)
			print_usage();
		check_archive(argv[0]);
	}
}

static void 
print_usage() 
{
	fprintf(stderr, "Usage: %s [command] archive-file [files]\n", getprogname());
	fprintf(stderr, "\n commands:\n");
	fprintf(stderr, "\t-d\t- delete files\n");
	fprintf(stderr, "\t-r\t- replace or insert new files\n");
	fprintf(stderr, "\t-x\t- extrat files from the archive; defaults to all\n");
	fprintf(stderr, "\t-t\t- display contents of the archive\n");
	fprintf(stderr, "\t-c\t- check the archive exists and has the magic\n");
	exit(1);
}

static void 
/* Make sure the archive exists and begins with the proper magic string */
check_archive(char *ar_name)
{
	int fd;
	char str[AR_MAGIC_SZ + 1];

	if ((fd = open(ar_name, O_RDONLY)) == -1)
		goto error;

	if (read(fd, str, AR_MAGIC_SZ) == -1)
		goto close;

	if (strcmp(str, "!<arch>\n") != 0)
		err(1, 0, "%s: %s: File format not recognized\n");

close:
	close(fd);
	return;
error:
	err(errno, NULL);
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

	check_archive(arch_name);
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

	check_archive(ar_name);
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
		err(1, "%s: No such file or directory\n", file_name);

	if ( !(arch = fopen(ar_name, "r+")) ) {
		printf("%s: creating %s\n", getprogname(), ar_name);
		arch = fopen(ar_name, "w+");
		fprintf(arch, "%s", AR_MAGIC_STR);
		fflush(arch);
	}

	check_archive(ar_name);
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
			err(1, "Could not create a temp file; aborting\n");

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
			err(1, "%s: Could not complete the replace operation\n",
				file_name);

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

	check_archive(ar_name);
	parse_archive(ar_name);

	if ( !(arch = fopen(ar_name, "r+")) )
		err(1, "%s: Could not open the archive\n", ar_name);

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
			err(1, "%s: Could not delete file\n", file_name);

		fseek(arch, offset_header, SEEK_SET);
		fwrite(bkpbuff, sizeof(char), bkpbuff_sz, arch);

		if ((ftruncate(fileno(arch), ftell(arch))) == -1)
			err(1, "%s: Could not complete deletion of file\n",
				file_name);

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
