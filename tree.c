#include<stdio.h>
#include<malloc.h>
#include"tree.h"
#include"table.h"
#include"link.h"

int create_tree_by_file(tree **out, unsigned char *file,unsigned short amount)
{
	int file_offset = 0;
	tree *head = (tree *)malloc(sizeof(tree));
	head->lchild = NULL;
	head->rchild = NULL;
	for (int i = 0; i < amount; i++)
	{
		unsigned char data = file[file_offset++];

		int tree_length = file[file_offset++];
		int binary;
		int bit = tree_length / 8;
		if (tree_length % 8 != 0)
		{
			bit++;
		}
		

		switch (bit)
		{
		case 1:
			binary = (int)file[file_offset++] << 24;
			break;
		case 2:
			binary = (int)(*(short *)&file[file_offset]) << 16;
			file_offset += 2;
			break;
		case 3:
		case 4:
			binary = *(int *)&file[file_offset];
			file_offset += 4;
			break;
		default:
			fprintf(stderr, "Decode overflow\n");
			return -1;
		}

		tree *temp = head;
		for (int ii = 0; ii < tree_length; ii++)
		{
			if (binary & 0x80000000)//1
			{
				if (temp->rchild == NULL)
				{
					temp->rchild = (tree *)malloc(sizeof(tree));
					temp = temp->rchild;
					temp->lchild = NULL;
					temp->rchild = NULL;
				}
				else
				{
					temp = temp->rchild;
				}
			}
			else//0
			{
				if (temp->lchild == NULL)
				{
					temp->lchild = (tree *)malloc(sizeof(tree));
					temp = temp->lchild;
					temp->lchild = NULL;
					temp->rchild = NULL;
				}
				else
				{
					temp = temp->lchild;
				}
			}
			binary <<= 1;
		}

		temp->data = data;
	}

	*out = head;
	return file_offset;
}

void preorder_traversal(tree *in)
{
	if (in != NULL)
	{
		if (in->lchild == NULL && in->rchild == NULL)
		{
			printf("%c", in->data);
			return;
		}
		preorder_traversal(in->lchild);
		preorder_traversal(in->rchild);
	}
}

void create_tree_by_array(tree **out, unsigned int *array)
{
	link *head_link = NULL;
	for (int i = 0; i < 256; i++)
	{
		if (array[i] > 0)
		{
			link *link_temp = (link *)malloc(sizeof(link));
			tree *tree_temp= (tree *)malloc(sizeof(tree));
			tree_temp->lchild = NULL;
			tree_temp->rchild = NULL;
			tree_temp->data = i;
			tree_temp->amount = array[i];

			link_temp->data = tree_temp;

			link_temp->next = NULL;
			add_by_amount(&head_link, link_temp);
		}
	}
	int length = count_link(head_link);
	 
	tree *huffman = NULL;
	while (head_link->next!=NULL)
	{
		tree *temp_tree = (tree *)malloc(sizeof(tree));
		link *temp_link = head_link;
		head_link = head_link->next;

		unsigned int amount = temp_link->data->amount;

		temp_tree->lchild = temp_link->data;
		free(temp_link);

		temp_link = head_link;
		head_link = head_link->next;
		amount += temp_link->data->amount;

		temp_tree->rchild = temp_link->data;
		temp_tree->amount = amount;

		temp_link->data = temp_tree;
		temp_link->next = NULL;
		add_by_amount(&head_link, temp_link);
	}

	*out = head_link->data;
	int max;
	get_max_path(*out, &max);
	free(head_link);
}

void _get_max_path(tree *in, int *max, int level)
{
	if (level > *max)
	{
		*max = level;
	}

	if (in->lchild != NULL)
	{
		_get_max_path(in->lchild, max, level + 1);
	}

	if (in->rchild != NULL)
	{
		_get_max_path(in->rchild, max, level + 1);
	}
}

void get_max_path(tree *in, int *out)
{
	*out = 0;
	if (in != NULL)
	{
		_get_max_path(in, out, 1);
	}
}

void _free_tree(tree *head)
{
	if (head->lchild != NULL)
	{
		_free_tree(head->lchild);
	}

	if (head->rchild != NULL)
	{
		_free_tree(head->rchild);
	}

	free(head);
}

void free_tree(tree **head)
{
	_free_tree(*head);
	*head = NULL;
}