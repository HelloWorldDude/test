#include <stdio.h>
#include <cstdlib>

struct Data
{
	Data *next;
	// Data section
	int value;
};

Data *head = nullptr;

Data *newNode(Data *next, int value)
{
	Data *d = (Data *)malloc(sizeof(Data));
	d->next = next;
	d->value = value;
	return d;
}

void enqueue(Data **node, int value)
{
	Data *temp = newNode(*node, value);

	*node = temp;
}

void dequeue(Data **node)
{
	if (*node != nullptr)
	{
		Data *temp = *node;
		*node = (*node)->next;
		free(temp);
	}
	else
	{
		printf("dequeue: list is empty\n");
	}
}

void reverse(Data **head)
{
	Data *temp = *head;
	Data *next = (*head)->next;
	Data *prev = nullptr;
	while (next != nullptr)
	{
		(*head) = next;
		temp->next = prev;
		prev = temp;
		next = (*head)->next;
		(*head)->next = temp;
		temp = (*head);
	}
}

void print(Data *node)
{
	while (node != nullptr)
	{
		printf("%d\n", node->value);
		node = node->next;
	}
}

bool listEmpty()
{
	if (head == nullptr)
		return true;
	else
		return false;
}

void testList()
{
	for (int i = 0; i < 1000000; i++)
	{
		enqueue(&head, i);
	}

	reverse(&head);

	for (int i = 0; i < 1000000; i++)
	{
		dequeue(&head);
	}

	if (!listEmpty())
		printf("test: error\n");
}

int main()
{
	testList();
}