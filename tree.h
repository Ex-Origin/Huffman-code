
#ifndef TREE
#define TREE

#include"table.h"

//Huffman tree, be used to decode
typedef struct binary_tree
{
	unsigned char data;
	unsigned int amount;
	struct binary_tree *lchild;
	struct binary_tree *rchild;
}tree;

//if only one or zero element is non-zero
//In other words,there is one or no character in the file to be compressed
//it will be wrong
void create_tree_by_array(tree **out, unsigned int *array);

void get_max_path(tree *in, int *out);

void free_tree(tree **head);

//"file" is body of the arc file 
int create_tree_by_file(tree **out, unsigned char *file, unsigned short amount);

void preorder_traversal(tree *in);

#endif // !TREE
