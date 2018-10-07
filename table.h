

#ifndef TABLE
#define TABLE
#include"tree.h"

//comparison table, be used to encode
typedef struct parallel_table
{
	unsigned int binary;
	unsigned char num;
	unsigned char data;
	unsigned int amount;
}p_table;

//Turn the huffman tree into a comparsion table
//You need to allocate space of the p_table in stack or heap
void initial_table(p_table *out,void *tree_in);

//Arrange by size of the "amount" of the p_table ascendingly
//length is the length of the p_table 
void sort_table_by_amount_ascending(p_table *in, unsigned short length);

#endif // !TABLE
