#include<stdio.h>
#include<malloc.h>
#include"link.h"
#include"tree.h"

void add_by_amount(link **head,link *in)
{
	if (*head == NULL)
	{
		*head = in;
	}
	else
	{
		link *t_head = *head;
		link *_head = *head;
		link *pre_temp = NULL;
		while (t_head->data->amount < in->data->amount)
		{
			pre_temp = t_head;
			t_head = t_head->next;
			if (t_head == NULL)
			{
				break;
			}
		}

		if (pre_temp != NULL)
		{
			pre_temp->next = in;
		}
		else
		{
			_head = in;
		}

		link *temp = t_head;
		t_head = in;
		t_head->next = temp;

		*head = _head;
	}
}

int count_link(link *in)
{
	int sum = 0;
	while (in != NULL)
	{
		sum++;
		in = in->next;
	}

	return sum;
}
