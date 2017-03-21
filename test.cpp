#include <iostream>
#include <stdio.h>
#include "MyList.h"

using namespace std;

int main(int argc, char ** argv)
{
	MyList<int> list;
	for(int i=0;i<10;i++)
		list.insert_front(i);
	
	printf("list count: %d.\n",list.count());	
	
	MyList<int>::MyNodePtr first = list.begin();
	MyList<int>::MyNodePtr temp;

	while(first != NULL)
	{
		temp = first;
		first = first->next;
		if(temp->val % 3 == 0)
		{
			list.erase(temp);
			printf("list count: %d.\n",list.count());	
			continue;
		}
		
		printf("%d.\n",temp->val);
	}

	return 0;
}
