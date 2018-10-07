
#ifndef LINK
#define LINK
#include"tree.h"

typedef struct link_list
{
	tree *data;
	struct link_list *next;
}link;

//Insert sequentially, the default is descending order
void add_by_amount(link **head, link *in);

//Calculate length of the list
int count_link(link *in);

#endif // !LINK



