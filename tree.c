#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef int elem_t;

#define PRINT_ELEM_T "d"


/*
сделать проверки в начале функций
*/

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

int                  Dump_Graph       (struct Tree_Node_t* tree, FILE* grapf_write, struct Tree_info* main_tree); // название
void                 Req_Graph        (struct Tree_Node_t* tree, FILE* graph_write, struct Tree_info* main_tree);
void                 Print_tree       (const struct Tree_Node_t* tree);

struct Tree_Node_t*  Tree_Constructor (struct Tree_Node_t* parrent_tree, elem_t data, struct Tree_info* main_tree);
void                 Tree_Destructor  (struct Tree_Node_t* node); // название tree
/*
нужно оббежать все дерево начиная с листьев, освобождая их, если освобождать родительский -> дочерние станут недоступны
*/
struct Tree_Node_t*  Add        (struct Tree_Node_t* parrent_tree, elem_t data, struct Tree_info* main_tree);
struct Tree_Node_t*  Slap_right (struct Tree_Node_t* parrent_tree, elem_t data, struct Tree_info* main_tree);
struct Tree_Node_t*  Slap_left  (struct Tree_Node_t* parrent_tree, elem_t data, struct Tree_info* main_tree);

int                  Tree_Verificetor (struct Tree_Node_t* parrent_tree, elem_t data, struct Tree_info* main_tree);







int main()
{
    struct Tree_info Tree_1 = { };
    FILE* dump_write = fopen ("Tree_dump.txt", "w");
    FILE* Graph_write = fopen ("dump.dot", "w");

    Tree_1.Tree_root = Tree_Constructor (NULL, 50, &Tree_1);

    Add (Tree_1.Tree_root, 20, &Tree_1);
    Add (Tree_1.Tree_root, 60, &Tree_1);
    Add (Tree_1.Tree_root, 10, &Tree_1);
    Add (Tree_1.Tree_root, 30, &Tree_1);
    Add (Tree_1.Tree_root, 52, &Tree_1);
    Add (Tree_1.Tree_root, 67, &Tree_1);

    Add (Tree_1.Tree_root, 68, &Tree_1);
    Add (Tree_1.Tree_root, 47, &Tree_1);
    Add (Tree_1.Tree_root, 27, &Tree_1);
    Add (Tree_1.Tree_root, 617, &Tree_1);
    Add (Tree_1.Tree_root, 64, &Tree_1);
    Add (Tree_1.Tree_root, 39, &Tree_1);
    Add (Tree_1.Tree_root, 7 , &Tree_1);
    Add (Tree_1.Tree_root, 8 , &Tree_1);
    Add (Tree_1.Tree_root, 6 , &Tree_1);

    Dump_Graph (Tree_1.Tree_root, Graph_write, &Tree_1);

    fclose (dump_write);
    fclose (Graph_write);

    Tree_Destructor (Tree_1.Tree_root);

    return 0;
}


struct Tree_Node_t* Tree_Constructor (struct Tree_Node_t* parrent_tree, elem_t data, struct Tree_info* main_tree)
{
    assert (main_tree    != NULL);

    struct Tree_Node_t* point_tree = calloc (1, sizeof(struct Tree_Node_t));
    point_tree->Parrent = parrent_tree; //так как корень
    point_tree->data = data;

    main_tree->last = point_tree;

    return point_tree;
}

void                Tree_Destructor  (struct Tree_Node_t* node)
{

    if (node == NULL)
        return;

    Tree_Destructor(node->Left);    
    Tree_Destructor(node->Right);   
    free(node);               
}



struct Tree_Node_t*  Add        (struct Tree_Node_t* parrent_tree, elem_t data, struct Tree_info* main_tree)
{
    assert (parrent_tree != NULL);
    assert (main_tree !=  NULL);

    if (data >= parrent_tree->data)
    {
        return Slap_right (parrent_tree, data, main_tree);
    }

    if (data < parrent_tree->data)
    {
        return Slap_left  (parrent_tree, data, main_tree);
    }

    return 0;
}

struct Tree_Node_t*  Slap_right (struct Tree_Node_t* parrent_tree, elem_t data, struct Tree_info* main_tree) // нужно еще убедиться что он свободный 
{
    assert (parrent_tree != NULL);
    assert (main_tree    != NULL);


    if (parrent_tree->Right == 0)
    {
        struct Tree_Node_t* new_R_tree = Tree_Constructor(parrent_tree, data, main_tree);
        parrent_tree->Right = new_R_tree;
        return new_R_tree;
    }

    else 
    {
        return Add (parrent_tree->Right, data, main_tree);
    }
 
}

struct Tree_Node_t*  Slap_left  (struct Tree_Node_t* parrent_tree, elem_t data, struct Tree_info* main_tree) 
{
    assert (parrent_tree != NULL);
    assert (main_tree    != NULL);

    if (parrent_tree->Left == 0)
    {
        struct Tree_Node_t* new_L_tree = Tree_Constructor(parrent_tree, data, main_tree);
        parrent_tree->Left = new_L_tree;
        return new_L_tree;
    }

    else 
    {
        return Add (parrent_tree->Left, data, main_tree);
    }
}




int   Dump_Graph        (struct Tree_Node_t* tree, FILE* graph_write, struct Tree_info* main_tree)
{
    assert (tree        != NULL);
    assert (graph_write != NULL);
    assert (main_tree   != NULL);
 
        
    fprintf (graph_write, "digraph G {\n");
    fprintf (graph_write, "  splines = false;\n"); 
    fprintf (graph_write, "  rankdir = HR;\n");
    fprintf (graph_write, "  node [shape = Mrecord; style = filled;];\n");
    fprintf (graph_write, "  edge [color = darkblue;];\n\n");


    Req_Graph (tree, graph_write, main_tree);


    fprintf (graph_write, "}");


    Print_tree (tree);

    return 0;
}

void  Req_Graph         (struct Tree_Node_t* tree, FILE* graph_write, struct Tree_info* main_tree)
{
    assert (graph_write != NULL);
    assert (main_tree   != NULL);

    if (tree == NULL)
        return;

    if (tree == main_tree->last)
    {
        fprintf(graph_write, 
                "  N%p [label = \"{ %d | %#x | Parrents:%#x | {<L> %#x | <R> %#x} }\"; fillcolor = \"#de781f\"; color = \"#0000FF\"];\n", 
                (void*)tree, tree->data, (void*)tree, (void*)tree->Parrent, (void*)tree->Left, (void*)tree->Right);
    }

    else 
    {
        fprintf(graph_write, 
                "  N%p [label = \"{ %d | %#x | Parrents:%#x | {<L> %#x | <R> %#x} }\"; fillcolor = \"#C0E0FF\"; color = \"#0000FF\"];\n", 
                (void*)tree, tree->data, (void*)tree, (void*)tree->Parrent, (void*)tree->Left, (void*)tree->Right);
    }

    
    if (tree->Left)  
    {
        if (tree->Left->Parrent == tree)
            fprintf(graph_write, "  N%p:L -> N%p [dir=both; color =\"#00bc7e\"; arrowsize = 0.7]; \n", (void*)tree, (void*)tree->Left);

        else
            fprintf(graph_write, "  N%p:L -> N%p [dir=both; color =\"#bc003e\"; arrowsize = 0.7];\n", (void*)tree, (void*)tree->Left);
    }

    if (tree->Right) 
    {
        if (tree->Right->Parrent == tree)
            fprintf(graph_write, "  N%p:R -> N%p [dir=both; color = \"#00bc7e\"; arrowsize = 0.7];\n", (void*)tree, (void*)tree->Right);

        else 
            fprintf(graph_write, "  N%p:R -> N%p [dir=both; color =\"#bc003e\"; arrowsize = 0.7];\n", (void*)tree, (void*)tree->Right);
    }
    if (tree->Left && tree->Right) 
    {
        fprintf(graph_write, "  { rank=same; N%p; N%p; }\n", 
                (void*)tree->Left, (void*)tree->Right);
    }

    Req_Graph(tree->Left, graph_write, main_tree);
    Req_Graph(tree->Right, graph_write, main_tree);
}

void  Print_tree        (const struct Tree_Node_t* tree)
{
    assert (tree        != NULL);


    printf ("( ");
    printf ("%" PRINT_ELEM_T, tree->data);//как вставить в % elem_t

    if (tree->Left)  Print_tree (tree->Left);
    if (tree->Right) Print_tree (tree->Right);

    printf  (")");
}