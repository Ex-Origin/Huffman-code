#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include"tree.h"
#include"file.h"
#include"table.h"

int encode(const char *file_path)
{
	int length = fsize(file_path);
	if (length == -1)
	{
		fprintf(stderr, "The file is not exist\n");
		return -1;
	}

	unsigned char *file = (unsigned char *)malloc(length);
	file_read(file, file_path, length);

	unsigned int array_char[256] = { 0 };
	unsigned short char_sum = count_char(file, array_char, length);

	tree *head = NULL;
	create_tree_by_array(&head, array_char);
	//preorder_traversal(head);

	p_table *table = (p_table *)malloc(sizeof(p_table)*char_sum);
	initial_table(table, head);
	sort_table_by_amount_ascending(table,char_sum);

	free_tree(&head);

	unsigned char *arc_file = (unsigned char *)malloc((int)(length*1.2));
	

	int arc_length = start_encode((unsigned char *)file,length,arc_file + sizeof(arc_head), table,char_sum);

	arc_head h;
	h.length = length;
	h.table_num = char_sum; 
	get_file_postfix(h.postfix, file_path);
	arc_length += sizeof(arc_head);

	h.flag[0] = 'E';
	h.flag[1] = 'x';

	*(arc_head *)(arc_file) = h;

	save_file(arc_file, arc_length, (char *)file_path,"arc");
	free(arc_file);

	return 0;
}

int decode(const char *file_path)
{
	int arc_length = fsize(file_path);

	if (arc_length == -1)
	{
		fprintf(stderr, "The file is not exist\n");
		return -1;
	}

	unsigned char *arc_file = (unsigned char *)malloc(arc_length);
	file_read(arc_file, file_path, arc_length);
	
	arc_head *head = (arc_head *)arc_file;
	unsigned char *arc_file_body = arc_file+sizeof(arc_head);

	if (head->flag[0] != 'E' || head->flag[1] != 'x')
	{
		fprintf(stderr, "This is not a arc file\n");
		return -1;
	}

	tree *t_head = NULL;
	int table_offset = create_tree_by_file(&t_head, arc_file_body, head->table_num);
	//preorder_traversal(t_head);

	unsigned char *file = (unsigned char *)malloc(head->length);

	start_decode(arc_file_body + table_offset, file, t_head, head->length);
	
	save_file(file, head->length,(char *) file_path, head->postfix);

	return 0;
}

int main(int argc,char **argv)
{
	//#define TEST
#ifndef TEST
	if (argc != 2)
	{
		fprintf(stderr, "\nUsage: arc file_name\n");
		return -1;
	}

	if (strstr(argv[1], ".arc") == NULL)
	{
		encode(argv[1]);
	}
	else
	{
		decode(argv[1]);
	}
#else
	char *file = "1.docx";
	if (1)
	{
		encode(file);
	}
	else
	{
		decode(file);
	}

#endif // !TEST

	
	
	return 0;
}