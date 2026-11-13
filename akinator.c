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

int                 Dump_Graph            (struct Tree_Node_t* tree, FILE* graph_write, struct Tree_info* main_tree);
void                Req_Graph             (struct Tree_Node_t* tree, FILE* graph_write, struct Tree_info* main_tree);
void                Print_tree            (const struct Tree_Node_t* tree);

struct Tree_Node_t* Tree_Constructor      (struct Tree_Node_t* parrent_tree, elem_t data, struct Tree_info* main_tree);
void                Tree_Destructor       (struct Tree_Node_t* node);

struct Tree_Node_t* Add                   (struct Tree_Node_t* parrent_tree, elem_t data, struct Tree_info* main_tree, int choise);
struct Tree_Node_t* Slap_right            (struct Tree_Node_t* parrent_tree, elem_t data, struct Tree_info* main_tree, int choise);
struct Tree_Node_t* Slap_left             (struct Tree_Node_t* parrent_tree, elem_t data, struct Tree_info* main_tree, int choise);

int                 ReadFromFile          (const char* name_file, char** buffer);
void                Save_Tree             (struct Tree_Node_t* node, FILE* file);
struct Tree_Node_t* Load_Tree_From_Buffer (char** ptr, struct Tree_Node_t* parrent, struct Tree_info* main_tree);

int                 Play_Akinator         (struct Tree_info* main_tree);

int main()
{
    struct Tree_info Tree_1 = { };
    FILE* Graph_write = fopen("dump.dot", "w");
    char* buffer = NULL;

    int size = ReadFromFile("data_info", &buffer);

    if (size > 0 && buffer != NULL)
    {
        char* ptr = buffer; 
        Tree_1.Tree_root = Load_Tree_From_Buffer(&ptr, NULL, &Tree_1);
        printf("Tree loaded from file.\n");
        free(buffer);
    }
    
    if (Tree_1.Tree_root == NULL)
    {
        Tree_1.Tree_root = Tree_Constructor(NULL, "It is a animal?", &Tree_1);
        Tree_1.Tree_root->Right = Tree_Constructor(Tree_1.Tree_root, "Cat", &Tree_1);
        Tree_1.Tree_root->Left = Tree_Constructor(Tree_1.Tree_root, "Table", &Tree_1);
        printf("New tree created.\n");
    }

    Play_Akinator(&Tree_1);

    Dump_Graph(Tree_1.Tree_root, Graph_write, &Tree_1);
    Print_tree(Tree_1.Tree_root);

    FILE* save_file = fopen("data_info", "w");
    if (save_file != NULL)
    {
        Save_Tree(Tree_1.Tree_root, save_file);
        fclose(save_file);
        printf("\nTree saved.\n");
    }

    if (Graph_write != NULL) fclose(Graph_write);
    Tree_Destructor(Tree_1.Tree_root);

    return 0;
}

struct Tree_Node_t* Tree_Constructor(struct Tree_Node_t* parrent_tree, elem_t data, struct Tree_info* main_tree)
{
    assert(main_tree != NULL);
    assert(data != NULL);

    struct Tree_Node_t* point_tree = calloc(1, sizeof(struct Tree_Node_t));
    point_tree->Parrent = parrent_tree; 
    
    size_t len = strlen(data);
    point_tree->data = calloc(len + 1, sizeof(char));
    strcpy(point_tree->data, data);

    main_tree->last = point_tree;
    main_tree->number_nodes++;

    return point_tree;
}

void Tree_Destructor(struct Tree_Node_t* node)
{
    if (node == NULL)
        return;

    Tree_Destructor(node->Left);    
    Tree_Destructor(node->Right);   
    free(node->data); 
    free(node);               
}

struct Tree_Node_t* Add(struct Tree_Node_t* parrent_tree, elem_t data, struct Tree_info* main_tree, int choise)
{
    assert(parrent_tree != NULL);
    assert(main_tree != NULL);

    if (choise == 1)
    {
        return Slap_right(parrent_tree, data, main_tree, choise);
    }

    if (choise == 0)
    {
        return Slap_left(parrent_tree, data, main_tree, choise);
    }

    return NULL;
}

struct Tree_Node_t* Slap_right(struct Tree_Node_t* parrent_tree, elem_t data, struct Tree_info* main_tree, int choise)
{
    assert(parrent_tree != NULL);
    assert(main_tree != NULL);

    if (parrent_tree->Right == NULL)
    {
        struct Tree_Node_t* new_R_tree = Tree_Constructor(parrent_tree, data, main_tree);
        parrent_tree->Right = new_R_tree;
        return new_R_tree;
    }
    else 
    {
        return Add(parrent_tree->Right, data, main_tree, choise);
    }
}

struct Tree_Node_t* Slap_left(struct Tree_Node_t* parrent_tree, elem_t data, struct Tree_info* main_tree, int choise) 
{
    assert(parrent_tree != NULL);
    assert(main_tree != NULL);

    if (parrent_tree->Left == NULL)
    {
        struct Tree_Node_t* new_L_tree = Tree_Constructor(parrent_tree, data, main_tree);
        parrent_tree->Left = new_L_tree;
        return new_L_tree;
    }
    else 
    {
        return Add(parrent_tree->Left, data, main_tree, choise);
    }
}

int Dump_Graph(struct Tree_Node_t* tree, FILE* graph_write, struct Tree_info* main_tree)
{
    assert(tree != NULL);
    assert(graph_write != NULL);
    assert(main_tree != NULL);
 
    fprintf(graph_write, "digraph G {\n");
    fprintf(graph_write, "  splines = false;\n"); 
    fprintf(graph_write, "  rankdir = HR;\n");
    fprintf(graph_write, "  node [shape = Mrecord; style = filled;];\n");
    fprintf(graph_write, "  edge [color = darkblue;];\n\n");

    Req_Graph(tree, graph_write, main_tree);

    fprintf(graph_write, "}");

    return 0;
}

void Req_Graph(struct Tree_Node_t* tree, FILE* graph_write, struct Tree_info* main_tree)
{
    assert(graph_write != NULL);
    assert(main_tree != NULL);

    if (tree == NULL) return;

    const char* fillcolor = "#C0E0FF"; 
    if (tree == main_tree->last) fillcolor = "#de781f"; 

    fprintf(graph_write,
            "  N%p [label = \"{ %s | %p | Parrents:%p | {<L> %p | <R> %p} }\"; fillcolor = \"%s\"; color = \"#0000FF\"];\n", 
            (void*)tree, tree->data, (void*)tree, (void*)tree->Parrent, (void*)tree->Left, (void*)tree->Right, fillcolor);

    if (tree->Left)  
        fprintf(graph_write, "  N%p:L -> N%p [dir=both; color =\"#00bc7e\"; arrowsize = 0.7]; \n", (void*)tree, (void*)tree->Left);

    if (tree->Right) 
        fprintf(graph_write, "  N%p:R -> N%p [dir=both; color = \"#00bc7e\"; arrowsize = 0.7];\n", (void*)tree, (void*)tree->Right);
    
    if (tree->Left && tree->Right) 
        fprintf(graph_write, "  { rank=same; N%p; N%p; }\n", (void*)tree->Left, (void*)tree->Right);

    Req_Graph(tree->Left, graph_write, main_tree);
    Req_Graph(tree->Right, graph_write, main_tree);
}

void Print_tree(const struct Tree_Node_t* tree)
{
    assert(tree != NULL);

    printf("( ");
    printf("%" PRINT_ELEM_T " ", tree->data);

    if (tree->Left)  Print_tree(tree->Left);
    if (tree->Right) Print_tree(tree->Right);

    printf(")");
}

int ReadFromFile(const char* name_file, char** buffer)
{
    FILE* data_info = fopen(name_file, "rb");

    if (data_info == NULL)
    {
        return -1; 
    }

    int file_dis = fileno(data_info);
    struct stat stat_file;
    fstat(file_dis, &stat_file);
    size_t size_file = stat_file.st_size;

    *buffer = calloc(size_file + 1, sizeof(char)); 
    fread(*buffer, sizeof(char), size_file, data_info); 
    
    fclose(data_info);

    return (int)size_file;
}

int Play_Akinator(struct Tree_info* main_tree)
{
    struct Tree_Node_t* current = main_tree->Tree_root;
    char answer[10] = {0};

    while (current->Left != NULL || current->Right != NULL) 
    {
        printf("%s (yes/no): ", current->data);
        scanf("%9s", answer);
        
        if (strcmp(answer, "yes") == 0 || strcmp(answer, "y") == 0) 
        {
            if (current->Right != NULL) current = current->Right; 
        } 
        else 
        {
            if (current->Left != NULL) current = current->Left;
        }
    }

    printf("Is it %s? (yes/no): ", current->data);
    scanf("%9s", answer);

    if (strcmp(answer, "yes") == 0 || strcmp(answer, "y") == 0) 
    {
        printf("Hooray! I guessed it!\n");
    } 
    else 
    {
        char new_answer[256] = {0};
        char new_question[256] = {0};
        
        printf("I give up. What did you guess? ");
        scanf(" %255[^\n]", new_answer); 

        printf("What question distinguishes %s from %s? (yes for %s)\n", 
               new_answer, current->data, new_answer);
        scanf(" %255[^\n]", new_question);

        char* old_answer_text = current->data;

        current->data = calloc(strlen(new_question) + 1, sizeof(char));
        strcpy(current->data, new_question);

        current->Right = Tree_Constructor(current, new_answer, main_tree);
        current->Left  = Tree_Constructor(current, old_answer_text, main_tree);
        
        free(old_answer_text); 

        printf("Thank you! I will remember that.\n");
    }

    return 0;
}

void Save_Tree(struct Tree_Node_t* node, FILE* file)
{
    if (node == NULL) 
    {
        fprintf(file, "( ) ");
        return;
    }

    fprintf(file, "( %s ", node->data);
    
    Save_Tree(node->Left, file);
    Save_Tree(node->Right, file);
    
    fprintf(file, ") ");
}

struct Tree_Node_t* Load_Tree_From_Buffer(char** ptr, struct Tree_Node_t* parrent, struct Tree_info* main_tree)
{
    while (**ptr == ' ' || **ptr == '\n' || **ptr == '\r' || **ptr == '\t') (*ptr)++;

    if (**ptr == '\0') return NULL;

    if (**ptr == '(') 
    {
        (*ptr)++;
        while (**ptr == ' ' || **ptr == '\n' || **ptr == '\r' || **ptr == '\t') (*ptr)++;

        if (**ptr == ')') 
        {
            (*ptr)++;
            return NULL; 
        }

        char buffer[1024] = {0};
        int i = 0;

        while (**ptr != '(' && **ptr != ')' && **ptr != '\0' && i < 1023) 
        {
            buffer[i++] = **ptr;
            (*ptr)++;
        }
        buffer[i] = '\0';

        while (i > 0 && (buffer[i-1] == ' ' || buffer[i-1] == '\n' || buffer[i-1] == '\r' || buffer[i-1] == '\t')) 
        {
            buffer[i-1] = '\0';
            i--;
    }

    struct Tree_Node_t* node = Tree_Constructor(parrent, buffer, main_tree);

    while (**ptr == ' ' || **ptr == '\n' || **ptr == '\r' || **ptr == '\t') 
        (*ptr)++;

    node->Left  = Load_Tree_From_Buffer(ptr, node, main_tree);
    node->Right = Load_Tree_From_Buffer(ptr, node, main_tree);

    while (**ptr == ' ' || **ptr == '\n' || **ptr == '\r' || **ptr == '\t') 
        (*ptr)++;

    if (**ptr == ')') 
        (*ptr)++;
    
    return node;

    }

    return NULL;
}