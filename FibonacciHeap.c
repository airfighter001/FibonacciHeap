#include <stdio.h>
#include <stdlib.h>
#include "LinkedList.h"

typedef struct fibheap {
	struct node * min;
	struct list * root_list;
} fibheap_t;

fibheap_t * createFibHeap() {
	fibheap_t * fibHeap = (fibheap_t *)malloc(sizeof(fibheap_t));
	fibHeap->root_list = (list_t *)malloc(sizeof(list_t));
	return fibHeap;
}

int getMin(fibheap_t fibheap) {
	return fibheap.min->key;
}

int insert(node_t * node, fibheap_t fibheap) {
	addToList(fibheap.root_list, node);
	return 1;
}


int main() {
	node_t * head = createNode(10, NULL);
	head->leftNode = head;
	head->rightNode = head;

	list_t * root = createList();
	addToList(root, head);


	printf("List-Size:%ld\n", root->size);
	printf("Head-Key:%d\n", root->head->key);


	free(head);
	free(root);
	return 0;
}
