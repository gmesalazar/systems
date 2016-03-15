#ifndef LIST_H
#define LIST_H

typedef struct words_list_data wlist_data_t;
typedef struct var_table_data table_data_t;
typedef struct hist_list_data hist_data_t;
typedef struct node struct_t;

struct words_list_data 
{
    char *word;
};

struct var_table_data 
{
    char *varName;
    char *varValue;
};

struct hist_list_data 
{
    struct_t *command;
};

enum struct_type 
{
    WORDS_LIST,
    TABLE,
    HIST_LIST
};

struct node 
{
    enum struct_type structType;
    struct node *prev;
    union {
	wlist_data_t wordData;
	table_data_t tableData;
	hist_data_t histData;
    };
    struct node *next;
};

int insertNode (struct_t **head, struct_t *newNode);
int deallocStruct (struct_t **head);
void printStruct (struct_t *head);
struct_t *cloneNode (struct_t *node);
struct_t *cloneList (struct_t *head);
struct_t *mergeLists (struct_t *stA, struct_t *stB);
struct_t *getNthNode (struct_t *head, int num);
struct_t *insertListIntoPos(struct_t *listA, struct_t *pos);

#endif
