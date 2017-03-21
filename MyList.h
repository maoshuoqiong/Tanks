/*
* A list template
*/

#ifndef MYLIST_H_
#define MYLIST_H_

#include <stdlib.h>

template<typename T>
class MyList
{

public:
	struct _MyNode;
	typedef struct _MyNode* MyNodePtr;
	struct _MyNode
	{
		MyNodePtr prev;
		MyNodePtr next;
		T val;
	};

public:
	MyList()
	{
		m_head = (MyNodePtr)malloc(sizeof(struct _MyNode));
		m_head->prev = m_head->next = NULL;
		m_count = 0;
	}

	~MyList()
	{
		MyNodePtr node = m_head->next;
		MyNodePtr temp;
		while(node)
		{
			temp = node;
			node = node->next;
			free(temp);
		}		

		free(m_head);
	}

public:
	MyNodePtr begin(){ return m_head->next; }	
	int count() { return m_count; }
	
public:
	void insert_front(T t)
	{
		MyNodePtr node_ptr = (MyNodePtr)malloc(sizeof(struct _MyNode));
		node_ptr->prev = m_head;
		node_ptr->next = m_head->next;
		node_ptr->val = t;
		if(m_head->next)
			m_head->next->prev = node_ptr;
		m_head->next = node_ptr;
		m_count++;
	}

	void erase( MyNodePtr ptr)
	{
		if(ptr == NULL)
			return;
		
		ptr->prev->next = ptr->next;
		if( ptr->next != NULL )
			ptr->next->prev = ptr->prev;	

		free(ptr);
		m_count--;
	}

	
private:
	MyNodePtr m_head; /* Head node */
	int m_count; /* count */

};

#endif

