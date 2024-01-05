#include <stdio.h>
#include "LinkedList.h"

typedef struct fibheap {
	struct node * min;
	struct list * root_list;
} fibheap_t;

int addNode(int key, void* value, node_t* leftNode, node_t* rightNode, node_t* father, node_t* child) {
	
	return 1;
}

int main() {
	node_t * head = createNode();
	head->leftNode = head;
	head->rightNode = head;
	head->key = 10;

	printf("Node-Key:%d\n", head->key);

	list_t * root = createList();
	printf("%ld\n", root->size);
	addToList(root, head);
	puts("test");
	printf("List-Size:%ld\n", root->size);
	printf("Head-Key:%d\n", root->head->key);


	free(head);
	free(root);
	return 0;
}
