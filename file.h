#ifndef FILEH
#define FILEH
#include"table.h"

typedef struct arc_file_head//16Bits
{
	char postfix[8];
	unsigned int length;//source file length
	unsigned short table_num;
	char flag[2];
}arc_head;

//Gets the file length size
int fsize(const char *file_path);

//Read the file from the path of "file_path" to the memory "file"
void file_read(unsigned char *file, const char *file_path, int length);

//Count how many differents characters(Bits)
unsigned short count_char(unsigned char *file, unsigned int *array_char, int length);

//Get the postfix of file name 
void get_file_postfix(char *file,const char *file_path);

//Return the offset of the arc_file
unsigned int start_encode(unsigned char *source_file, unsigned int source_file_length, unsigned char *arc_file, p_table *table, unsigned short table_length);


void save_file(unsigned char *file, int length, char *file_path, char *postfix);


void start_decode(unsigned char *arc_body, unsigned char *des_file, tree *huffman, unsigned int bits);

#endif // !FILEH

