#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>


typedef char* elem_t;


#define PRINT_ELEM_T "s"



struct Tree_info
{
    size_t number_nodes;
    struct Tree_Node_t* Tree_root;
    struct Tree_Node_t* last;
};

struct Tree_Node_t 
{
    elem_t data;
    struct Tree_Node_t* Right;
    struct Tree_Node_t* Left;
    struct Tree_Node_t* Parrent;
};



int                   Dump_Graph             (struct Tree_Node_t* tree, FILE* graph_write, struct Tree_info* main_tree);
void                  Req_Graph              (struct Tree_Node_t* tree, FILE* graph_write, struct Tree_info* main_tree);
void                  Print_tree             (const struct Tree_Node_t* tree);


struct Tree_Node_t*   Tree_Constructor       (struct Tree_Node_t* parrent_tree, elem_t data, struct Tree_info* main_tree);
void                  Tree_Destructor        (struct Tree_Node_t* tree);


struct Tree_Node_t*   Add                    (struct Tree_Node_t* parrent_tree, elem_t data, struct Tree_info* main_tree, int choise);
struct Tree_Node_t*   Slap_right             (struct Tree_Node_t* parrent_tree, elem_t data, struct Tree_info* main_tree, int choise);
struct Tree_Node_t*   Slap_left              (struct Tree_Node_t* parrent_tree, elem_t data, struct Tree_info* main_tree, int choise);


int                   ReadFromFile           (const char* name_file, char** buffer);
void                  Save_Tree              (struct Tree_Node_t* node, FILE* file_save);
struct Tree_Node_t*   Load_Tree_From_Buffer  (char** ptr, struct Tree_Node_t* parrent, struct Tree_info* main_tree);


int                   Play_Akinator          (struct Tree_info* main_tree);
void                  Answer_No              (struct Tree_info* main_tree, struct Tree_Node_t* current);
void                  Running_Tree           (struct Tree_info* main_tree, struct Tree_Node_t** current);


void                  clear_buffer           ();