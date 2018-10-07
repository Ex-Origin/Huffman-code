#include<stdio.h>
#include<malloc.h>
#include"table.h"
#include"tree.h"

typedef struct parameter
{
	p_table *out;
	unsigned char s[32];//stack
	unsigned char position;
}para;
void _initial_table(tree *in, para *p,unsigned char level)
{
	p_table *out=p->out;

	if (in->lchild != NULL)
	{
		p->s[level] = 0b01111111;
		_initial_table(in->lchild,p, level+1);
	}

	if (in->rchild != NULL)
	{
		p->s[level] = 0b10000000;
		_initial_table(in->rchild, p, level+1);
	}

	if (in->lchild == NULL && in->rchild == NULL)
	{
		int temp = 0;
		char *ptr = ((char *)&temp) + 3;

		for (int i = level-1; i >= 0 ; i--)
		{
			temp >>= 1;

			unsigned char ch_temp = p->s[i];
			if (ch_temp == 0b01111111)//set to zero
			{
				*ptr = *ptr & ch_temp;
			}
			else if (ch_temp == 0b10000000)//set to one
			{
				*ptr = *ptr | ch_temp;
			}
			else
			{
				fprintf(stderr, "Error 2\n");
			}
		}

		out[p->position].num = level;
		out[p->position].data = in->data;
		out[p->position].amount = in->amount;
		out[p->position].binary = temp;
		
		p->position++;
	}
}

void initial_table(p_table *out, void *tree_in)
{
	tree *trees = (tree *)tree_in;
	if (trees == NULL)
	{
		return;
	}

	para p;
	p.position = 0;
	p.out = out;
	_initial_table(trees,&p, 0);
}

void sort_table_by_amount_ascending(p_table *in,unsigned short length)
{
	for (unsigned short i = 0; i < length-1; i++)
	{
		p_table *max = in + i;
		for (unsigned int ii = i + 1; ii < length; ii++)
		{
			if (max->amount < in[ii].amount)
			{
				max = in + ii;
			}
		}
		if (max != in + i)
		{
			p_table temp = *max;
			*max = in[i];
			in[i] = temp;
		}
	}
}
