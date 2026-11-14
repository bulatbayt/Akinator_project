#include "akinator.h"
// удаление del
// сделать интерактивное меню  с выбором сохранения и чтения + exit 
// выбор с картинками ( можно рейлиб) + можно озвучку
// проработать выбор 
int main()
{
    struct Tree_info Tree_1 = { };
    FILE* Graph_write = fopen ("dump.dot", "w");
    char* buffer = NULL;



    int size = ReadFromFile ("data_info.txt", &buffer);

     
    printf ("size = %d, point buffer = %p\n", size, buffer);

    if (size > 0 && buffer != NULL)
    {
        Tree_1.Tree_root = Load_Tree_From_Buffer(&buffer, NULL, &Tree_1); ///!!!!!!!!!!!!!!
        free(buffer);
    }
    

    if (Tree_1.Tree_root == NULL)
    {
        Tree_1.Tree_root        = Tree_Constructor (NULL, "It is a animal?", &Tree_1);
        Tree_1.Tree_root->Right = Tree_Constructor (Tree_1.Tree_root, "Cat", &Tree_1);
        Tree_1.Tree_root->Left  = Tree_Constructor (Tree_1.Tree_root, "Ded", &Tree_1);
        
        printf ("New tree created.\n");
    }


    int choise = 1;
    

    while (choise != 0)
    {
        Play_Akinator (&Tree_1);
        clear_buffer ();
        printf ("Enter 1 to continue, 0 to stop\n");
        scanf ("%d", &choise);
    }

    Dump_Graph (Tree_1.Tree_root, Graph_write, &Tree_1);
    Print_tree (Tree_1.Tree_root);

    FILE* file_save = fopen ("data_info.txt", "w");
    Save_Tree (Tree_1.Tree_root, file_save);
    fclose (file_save);
    
    Tree_Destructor (Tree_1.Tree_root);
    fclose (Graph_write);


    return 0;
}


int               Dump_Graph                  (struct Tree_Node_t* tree, FILE* graph_write, struct Tree_info* main_tree)
{
    assert (tree != NULL);
    assert (graph_write != NULL);
    assert (main_tree != NULL);
 
    fprintf (graph_write, "digraph G {\n");
    fprintf (graph_write, "  splines = false;\n"); 
    fprintf (graph_write, "  rankdir = HR;\n");
    fprintf (graph_write, "  node [shape = Mrecord; style = filled;];\n");
    fprintf (graph_write, "  edge [color = darkblue;];\n\n");

    Req_Graph (tree, graph_write, main_tree);

    fprintf (graph_write, "}");

    return 0;
}

void              Req_Graph                   (struct Tree_Node_t* tree, FILE* graph_write, struct Tree_info* main_tree)
{
    assert (graph_write != NULL);
    assert (main_tree != NULL);

    if (tree == NULL) 
        return;

    const char* fillcolor = "#C0E0FF";
     
    if (tree == main_tree->last) 
        fillcolor = "#de781f"; 

    fprintf (graph_write,
            "  N%p [label = \"{ %s | %p | Parrents:%p | {<L> %p | <R> %p} }\"; fillcolor = \"%s\"; color = \"#0000FF\"];\n", 
            (void*)tree, tree->data, (void*)tree, (void*)tree->Parrent, (void*)tree->Left, (void*)tree->Right, fillcolor);

    if (tree->Left)  
        fprintf (graph_write, "  N%p:L -> N%p [dir=both; color =\"#00bc7e\"; arrowsize = 0.7]; \n", (void*)tree, (void*)tree->Left);

    if (tree->Right) 
        fprintf (graph_write, "  N%p:R -> N%p [dir=both; color = \"#00bc7e\"; arrowsize = 0.7];\n", (void*)tree, (void*)tree->Right);
    
    if (tree->Left && tree->Right) 
        fprintf (graph_write, "  { rank=same; N%p; N%p; }\n", (void*)tree->Left, (void*)tree->Right);

    Req_Graph (tree->Left, graph_write, main_tree);
    Req_Graph (tree->Right, graph_write, main_tree);
}

void              Print_tree                  (const struct Tree_Node_t* tree)
{
    assert (tree != NULL);

    printf ("( ");
    printf ("%" PRINT_ELEM_T " ", tree->data);

    if (tree->Left != NULL)  
        Print_tree (tree->Left);
    
    if (tree->Right != NULL) 
        Print_tree(tree->Right);

    printf(")");
}



struct Tree_Node_t*   Tree_Constructor        (struct Tree_Node_t* parrent_tree, elem_t data, struct Tree_info* main_tree)
{
    assert (main_tree != NULL);
    assert (data != NULL);

    struct Tree_Node_t* point_tree = calloc (1, sizeof(struct Tree_Node_t));
    point_tree->Parrent = parrent_tree; 
    
    size_t len = strlen (data);
    point_tree->data = calloc (len + 1, sizeof(char)); 
    strcpy (point_tree->data, data);

    main_tree->last = point_tree;
    main_tree->number_nodes++;

    return point_tree;
}

void                  Tree_Destructor         (struct Tree_Node_t* tree)
{
    if (tree == NULL)
        return;

    Tree_Destructor(tree->Left);    
    Tree_Destructor(tree->Right);   
    free(tree->data); 
    free(tree);               
}



struct Tree_Node_t*   Add                     (struct Tree_Node_t* parrent_tree, elem_t data, struct Tree_info* main_tree, int choise)
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

struct Tree_Node_t*   Slap_right              (struct Tree_Node_t* parrent_tree, elem_t data, struct Tree_info* main_tree, int choise)
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

struct Tree_Node_t*   Slap_left               (struct Tree_Node_t* parrent_tree, elem_t data, struct Tree_info* main_tree, int choise) 
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



int                   ReadFromFile            (const char* name_file, char** buffer)
{
    FILE* data_info = fopen(name_file, "r");

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

void                  Save_Tree               (struct Tree_Node_t* node, FILE* file_save)
{
    if (node == NULL) 
    {
        fprintf(file_save, "( ) ");
        return;
    }

    fprintf(file_save, "( %s ", node->data);
    
    Save_Tree(node->Left, file_save);
    Save_Tree(node->Right, file_save);
    
    fprintf(file_save, ") ");

    //printf("\nTree saved.\n");
}

struct Tree_Node_t*   Load_Tree_From_Buffer   (char** ptr, struct Tree_Node_t* parrent, struct Tree_info* main_tree) //divide to fuction; optimize parser
{
    while (**ptr == ' ' || **ptr == '\n') 
        (*ptr)++;

    if (**ptr == '\0') 
        return NULL;

    if (**ptr == '(') 
    {
        (*ptr)++;
        while (**ptr == ' ' || **ptr == '\n') 
            (*ptr)++;

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

        while (i > 0 && (buffer[i-1] == ' ' || buffer[i-1] == '\n')) 
        {
            buffer[i-1] = '\0';
            i--;
        }
    
    struct Tree_Node_t* node = Tree_Constructor(parrent, buffer, main_tree);

    while (**ptr == ' ') 
        (*ptr)++;

    node->Left  = Load_Tree_From_Buffer(ptr, node, main_tree);
    node->Right = Load_Tree_From_Buffer(ptr, node, main_tree);

    while (**ptr == ' ' || **ptr == '\n') 
        (*ptr)++;

    if (**ptr == ')') 
        (*ptr)++;
    
    return node;

    }

    return NULL;
}

/*struct Tree_Node_t*   Load_Tree_From_Buffer   (char** ptr, struct Tree_Node_t* parrent, struct Tree_info* main_tree)
{
    assert (ptr != NULL);
    assert (main_tree != NULL);

    char string[25] = " ";

    while (*ptr != '(')
    {
        *ptr ++;
    }

    int i = 0;
    while (*ptr != '(' || *ptr != '\n' || *ptr != '\0' || *ptr != ')' || *ptr != ' ')
    {
        string[i] = *(*ptr++);

    }
    

} */

int                   Play_Akinator           (struct Tree_info* main_tree)
{
    struct Tree_Node_t* current = main_tree->Tree_root;
    char answer[10] = {0};

    Running_Tree (main_tree, &current);

    printf("Is it %s? (yes/No): ", current->data);
    scanf("%9s", answer);

    if (strcmp(answer, "yes") == 0 || strcmp(answer, "y") || strcmp(answer, "Yes") || strcmp(answer, "Y")  == 0) 
    {
        printf("I win\n");
    } 
    else 
    {
        Answer_No (main_tree, current);
    }

    return 0;
}


void                  Answer_No               (struct Tree_info* main_tree, struct Tree_Node_t* current)
{
    char new_answer[256] = {0};
    char new_question[256] = {0};
    
    printf("What it? ");
    scanf(" %s", new_answer); 

    printf("What question diference %s from %s? (yes for %s)\n", 
            new_answer, current->data, new_answer);
    scanf(" %s", new_question);

    char* old_answer_text = current->data;

    current->data = calloc(strlen(new_question) + 1, sizeof(char));
    strcpy(current->data, new_question);

    current->Right = Tree_Constructor(current, new_answer, main_tree);
    current->Left  = Tree_Constructor(current, old_answer_text, main_tree);
    
    free (old_answer_text); 

    printf ("Thank you! I will remember that.\n");
}

void                  Running_Tree            (struct Tree_info* main_tree, struct Tree_Node_t** current)
{
    char answer[10] = {0};

    while ((*current)->Left != NULL || (*current)->Right != NULL) 
    {
        printf("%s (yes/no): ", (*current)->data);
        scanf("%9s", answer);
        
        if (strcmp(answer, "yes") == 0 || strcmp(answer, "y") == 0) 
        {
            if ((*current)->Right != NULL) 
                (*current) = (*current)->Right; 
        } 
        else 
        {
            if ((*current)->Left != NULL) 
                (*current) = (*current)->Left;
        }
    }

}



void                  clear_buffer ()
{
    while (getchar () != '\n')
        continue;
}
