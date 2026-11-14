#include "akinator.h"
#include "color.h"
/* 
удаление del --------------------------------------------------------------------achived
сделать интерактивное меню  с выбором файла для сохранения и чтения + exit через switch case выбрать 
выбор с картинками ( можно рейлиб) + можно озвучку
проработать выбор ( 1 или 0) -----------------------------------------------------achived
функции add, slap right, slap left, -----------------------------------------------------achived
парсить то, что пользователь будет вводить скобочки при ответе --------------------------achived

стрелочки зеленые - yes, нет - no -----------------------------------------------------achived

dot запускать через system        -----------------------------------------------------achieved
 */
int main()
{
    struct Tree_info Tree_1 = { };

    char* buffer = NULL;
    char database[256] = "data_info.txt"; // обработать 256 - a waste of memory
    char save_file[256]     = "data_info.txt"; 

    Interactive_menu (database, save_file, &buffer, &Tree_1);
    
    Tree_Destructor  (Tree_1.Tree_root);
    
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

    Run_Draw (tree, graph_write, main_tree);

    fprintf (graph_write, "}");

    return 0;
}

void              Run_Draw                   (struct Tree_Node_t* tree, FILE* graph_write, struct Tree_info* main_tree)
{
    assert (graph_write != NULL);
    assert (main_tree != NULL);

    if (tree == NULL) 
        return;

    const char* fillcolor = "#C0E0FF";
     
    if (tree == main_tree->last) 
        fillcolor = "#de781f"; 

    #ifdef ONDEBUG
    fprintf (graph_write,
            "  N%p [label = \"{ %s | %p | Parrents:%p | {<L> %p | <R> %p} }\"; fillcolor = \"%s\"; color = \"#0000FF\"];\n", 
            (void*)tree, tree->data, (void*)tree, (void*)tree->Parrent, (void*)tree->Left, (void*)tree->Right, fillcolor);

     if (tree->Left)  
        fprintf (graph_write, "  N%p:L -> N%p [label=\"No\"; dir=both; color =\"#DC143C\"; arrowsize = 0.7]; \n", (void*)tree, (void*)tree->Left);

    if (tree->Right) 
        fprintf (graph_write, "  N%p:R -> N%p [label=\"Yes\";dir=both; color = \"#00bc7e\"; arrowsize = 0.7];\n", (void*)tree, (void*)tree->Right);
    
    if (tree->Left && tree->Right) 
        fprintf (graph_write, "  { rank=same; N%p; N%p; }\n", (void*)tree->Left, (void*)tree->Right);

    #endif

    #ifndef ONDEBUG
    fprintf (graph_write,
            "  N%p [label = \" %s \"; fillcolor = \"%s\"; color = \"#0000FF\"];\n", 
            (void*)tree, tree->data, fillcolor);

    if (tree->Left)  
        fprintf (graph_write, "  N%p -> N%p [label=\"No\"; dir=both; color =\"#DC143C\"; arrowsize = 0.7]; \n", (void*)tree, (void*)tree->Left);

    if (tree->Right) 
        fprintf (graph_write, "  N%p -> N%p [label=\"Yes\";dir=both; color = \"#00bc7e\"; arrowsize = 0.7];\n", (void*)tree, (void*)tree->Right);
    
    if (tree->Left && tree->Right) 
        fprintf (graph_write, "  { rank=same; N%p; N%p; }\n", (void*)tree->Left, (void*)tree->Right);

    #endif

    
    Run_Draw (tree->Left, graph_write, main_tree);
    Run_Draw (tree->Right, graph_write, main_tree);
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

int                   Delete                  (struct Tree_info* main_tree, const char* target)
{
    assert(main_tree != NULL);
    assert(target != NULL);

    struct Tree_Node_t* node = Find_Node(main_tree->Tree_root, target);

    if (node == NULL)
    {
        printf("Node '%s' not found.\n", target);
        return -1;
    }

    int deleted_count = Count_Nodes(node);

    
    if (node->Parrent == NULL)
    {
        main_tree->Tree_root = NULL;
    }
    else if (node->Parrent->Left == node)
    {
        node->Parrent->Left = NULL;
    }
    else if (node->Parrent->Right == node)
    {
        node->Parrent->Right = NULL;
    }

    main_tree->last = node->Parrent;
    main_tree->number_nodes -= deleted_count;

    
    Tree_Destructor(node);

    return 0;
}




void                  Upload_Data             (char** buffer, struct Tree_info* Tree_1, const char* database)
{
    int size = ReadFromFile (database, buffer);
 
//    printf ("size = %d, point buffer = %p\n", size, buffer);

    if (size > 0 && *buffer != NULL)
    {
        char* ptr = *buffer;
        Tree_1->Tree_root = Load_Tree_From_Buffer(&ptr, NULL, Tree_1, 0);
        free(*buffer);
    }
    

    if (Tree_1->Tree_root == NULL) 
    {
        Tree_1->Tree_root        = Tree_Constructor (NULL, "It is a animal?", Tree_1);
        Tree_1->Tree_root->Right = Tree_Constructor (Tree_1->Tree_root, "Cat", Tree_1);
        Tree_1->Tree_root->Left  = Tree_Constructor (Tree_1->Tree_root, "Ded", Tree_1);
        
        printf ("New tree created.\n");
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

struct Tree_Node_t*   Load_Tree_From_Buffer   (char** ptr, struct Tree_Node_t* parrent, struct Tree_info* main_tree, int choise)
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
    
        struct Tree_Node_t* node = NULL;
        
        if (parrent == NULL)
            node = Tree_Constructor(NULL, buffer, main_tree); 
        else
            node = Add(parrent, buffer, main_tree, choise);   

        while (**ptr == ' ') 
            (*ptr)++;

        node->Left  = Load_Tree_From_Buffer(ptr, node, main_tree, 0);
        node->Right = Load_Tree_From_Buffer(ptr, node, main_tree, 1);

        while (**ptr == ' ' || **ptr == '\n') 
            (*ptr)++;

        if (**ptr == ')') 
            (*ptr)++;
    
        return node;
    }

    return NULL;
}




int                   Play_Akinator           (struct Tree_info* main_tree)
{
    int choise = 1;
    while (choise != 0)
    {
        struct Tree_Node_t* current = main_tree->Tree_root;
        char answer[10] = {0};

        Running_Tree (main_tree, &current);

        printf("Is it %s? ("GREEN"yes"RESET_COLOR"/"RED"No"RESET_COLOR"): ", current->data);
        scanf("%9s", answer);

        if (strcmp(answer, "yes") == 0 || strcmp(answer, "y") == 0 || strcmp(answer, "Yes") == 0 || strcmp(answer, "Y")  == 0) 
        {
            printf("I win\n");
        } 
        else 
        {
            Answer_No (main_tree, current);
        }
        
        printf ("Enter 1 to continue// 0 to stop\n");
        if (scanf ("%d", &choise) != 1)
        {
            continue;
        }
    }
    
    return 0;
}

void                  Answer_No               (struct Tree_info* main_tree, struct Tree_Node_t* current) 
{
    char new_answer[256] = {0};
    char new_question[256] = {0};
    
    clear_buffer();
    printf("What is it? ");
    read_line(new_answer, sizeof(new_answer));

    printf("What question differentiates %s from %s? (yes for %s)\n", 
           new_answer, current->data, new_answer);
    read_line(new_question, sizeof(new_question));

    char* old_answer_text = current->data;

    current->data = calloc(strlen(new_question) + 1, sizeof(char));
    strcpy(current->data, new_question);

    Add(current, old_answer_text, main_tree, 0);
    Add(current, new_answer, main_tree, 1); 
    
    free(old_answer_text); 

    printf("Thank you! I will remember that.\n");
}

void                  Running_Tree            (struct Tree_info* main_tree, struct Tree_Node_t** current)
{
    char answer[10] = {0};

    while ((*current)->Left != NULL || (*current)->Right != NULL) 
    {
        printf("%s ("GREEN"yes"RESET_COLOR"/"RED"No"RESET_COLOR"): ", (*current)->data);
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

void                  read_line    (char* string, int max_size)
{
    int i = 0;
    int ch = 0;
    while ((ch = getchar()) != '\n' && ch != EOF) 
    {
        if (ch == '(') 
            ch = '['; 
        else if (ch == ')') 
            ch = ']';

        if (i < max_size - 1) 
            string[i++] = (char)ch;
    }
    string[i] = '\0';
}

struct Tree_Node_t*   Find_Node    (struct Tree_Node_t* node, const char* target)
{
    if (node == NULL || target == NULL)
        return NULL;

    if (strcmp(node->data, target) == 0)
        return node;

    struct Tree_Node_t* found_left = Find_Node(node->Left, target);
    if (found_left != NULL)
        return found_left;

    return Find_Node(node->Right, target);
}

int                   Count_Nodes  (const struct Tree_Node_t* node)
{
    if (node == NULL)
        return 0;

    return 1 + Count_Nodes(node->Left) + Count_Nodes(node->Right);
}

void                  clear_string (char* string)
{
    for (size_t i = 0; string[i] != '\0'; i++)
    {
        string [i] = '\0';
    }
}



void                  Interactive_menu        (char* database, char* save_file, char** buffer, struct Tree_info* main_tree) // выбор для записи картинки  // для удаления элемента// выход 
{ 
    int choise = -1;
    
    while ( choise != 0)
    {
        printf ("\n==============================================Akinator Game==============================================\n\n");
        printf (YELLOW "mode menu:\n\n" RESET_COLOR);
        printf (BLUE   "Enter 1:"RESET_COLOR" Choise Database file          (If you do not make a selection, the default one will be used.)\n\n");
        printf (BLUE   "Enter 2:"RESET_COLOR" Choise file for save database (If you do not make a selection, the default one will be used.)\n\n");
        printf (BLUE   "Enter 3:"RESET_COLOR" Start Akinator Game\n\n");
        printf (BLUE   "Enter 4:"RESET_COLOR" Save database \n\n");
        printf (BLUE   "Enter 5:"RESET_COLOR" Graph Dump Tree\n\n");
        printf (BLUE   "Enter 6:"RESET_COLOR" Deleete element\n\n");
        printf (YELLOW "Enter 0:"RESET_COLOR" Exit game\n\n");

        printf ("Your Choise: ");

        while (scanf("%d", &choise) == 0 || (choise <0 ) || choise >6 )
        {
            printf ("Please, do correct choise\n");
        }

    
        switch (choise)
        {
            case 1: 
            {
                printf ("Data_base file:\n");
                clear_string (database);
                fgets (database, 256, stdin);
                break;
            }

            case 2:
            {
                printf ("Save file:\n");
                clear_string (save_file);
                fgets (save_file, 256, stdin);
                break;
            }

            case 3:
            {
                printf ("Let's start\n");

                Upload_Data (buffer, main_tree, database);

                Play_Akinator (main_tree);  
                
                break;
            }

            case 4:
            {
                FILE* save = fopen (save_file, "w");
                Save_Tree (main_tree->Tree_root, save);
                fclose (save);
                
                break;
            }

            case 5:
            {
                FILE* Graph_write = fopen ("dump.dot", "w");

                Dump_Graph (main_tree->Tree_root, Graph_write, main_tree);
                Print_tree (main_tree->Tree_root);             

                fclose (Graph_write);

                system   ("dot -Tpng dump.dot -o dump.png");
                
                break;
            }

            case 6:
            {
                printf (RED"Name for Delete: "RESET_COLOR);
                char target[20];// !!!!!!!!!!!!!!! оптимизировать
                scanf ("%s", target);
                clear_buffer;
                Delete (main_tree, target);

                break;
            }
        }

    }
}  

