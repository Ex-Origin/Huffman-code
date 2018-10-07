#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include"file.h"
#include"table.h"

void start_decode(unsigned char *arc_body, unsigned char *des_file, tree *huffman, unsigned int des_file_length)
{
	unsigned char binary=*(arc_body++);
	int t = 1;
	int offset_bit = 0;
	for (unsigned int i = 0; i < des_file_length; i++)
	{
		tree *temp = huffman;
		int key = 1;
		while (key==1)
		{
			if (offset_bit == 8)
			{
				offset_bit = 0;
				binary = *(arc_body++);
				t++;
			}
			
			for (  ; offset_bit < 8; offset_bit++)
			{
				if (temp->lchild == NULL && temp->rchild == NULL)
				{
					des_file[i] = temp->data;
					key = 0;
					break;
				}
				if (binary & 0b10000000)//1
				{
					temp = temp->rchild;
				}
				else//0
				{
					temp = temp->lchild;
				}
				binary <<= 1;
			}
		}
		
	}

	int i = 0;
}

void save_file(unsigned char *file, int length,char *file_path,char *postfix)
{
	char filename[64];
	int temp = strlen(file_path);
	for (int i = temp - 1; i >= 0 ; i--)
	{
		if (file_path[i] == '.')
		{
			temp = i;

			break;
		}
	}
	for (int i=0; i < temp; i++)
	{
		filename[i] = file_path[i];
	}
	filename[temp++] = '.';
	strcpy(filename + temp, postfix);

	FILE *fp = fopen(filename, "wb");
	fwrite(file, length, 1, fp);
	fclose(fp);
}

unsigned int start_encode(unsigned char *source_file,unsigned int source_file_length,unsigned char *arc_file,p_table *table,unsigned short table_length)
{
	unsigned int p_file = 0;
	int over_bit = 0;
	for (int i = 0; i < table_length; i++)
	{
		arc_file[p_file++] = table[i].data;
		unsigned char binary_length = table[i].num;
		arc_file[p_file++] = binary_length;
		if (binary_length % 8 == 0)
		{
			binary_length /= 8;
		}
		else
		{
			binary_length = binary_length / 8 + 1;
		}
		switch (binary_length)
		{
		case 1:
			arc_file[p_file++] = (unsigned char)(table[i].binary>>24);
			break;
		case 2:
			*(short *)&arc_file[p_file] = (short)(table[i].binary >> 16);
			p_file += 2;
			break;
		case 3:
		case 4:
			*(int *)&arc_file[p_file] = (int)(table[i].binary);
			p_file += 4;
			break;
		default:
			fprintf(stderr, "Overflow\n");
			break;
		}
	}

	int temp = 0;
	unsigned char *temp_ch = ((unsigned char *)&temp) + 3;
	for (unsigned int i = 0; i < source_file_length; i++)
	{
		int position = 0;
		while (source_file[i] != table[position].data)
		{
			position++;
		}

		
		temp += (int)(table[position].binary >> over_bit);
		over_bit += table[position].num;

		//32-7=25 more than 25 will be wrong
		while (over_bit / 8 > 0)
		{
			over_bit -= 8;
			arc_file[p_file++] = *temp_ch;
			temp <<= 8;
		}
	}

	arc_file[p_file++] = *temp_ch;
	temp <<= 8;

	return p_file;
}

void get_file_postfix(char *file,const char *file_path)
{
	file[0] = 0;
	int length = strlen(file_path);
	int pre = length;
	for (int i = length - 1, ii = 0; i >= 0 && ii < 8; i--, ii++)
	{
		if (file_path[i] == '.')
		{
			strcpy(file, &file_path[i + 1]);

			break;
		}
	}
}

int fsize(const char *file_path)
{
	FILE *fp;
	fp = fopen(file_path, "rb");
	if (fp == NULL)
	{
		return -1;
	}

	fseek(fp, 0, SEEK_END);
	int length = ftell(fp);
	fclose(fp);
	return length;
}

void file_read(unsigned char *file, const char *file_path, int length)
{
	FILE *fp;
	fp = fopen(file_path, "rb");
	fread(file, length, 1, fp);
	fclose(fp);
}

unsigned short count_char(unsigned char *file, unsigned int *array_char, int length)
{
	for (int i = 0; i < length; i++)
	{
		array_char[*file++]++;
	}

	unsigned short sum = 0;
	for (int i = 0; i < 256; i++)
	{
		if (array_char[i] != 0)
		{
			sum++;
		}
	}

	return sum;
}
