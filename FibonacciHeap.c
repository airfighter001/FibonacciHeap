#include <stdio.h>
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

int insert(node_t * node, fibheap_t * fibheap) {
	addToList(fibheap->root_list, node);
	if (fibheap->min == NULL || node->key < fibheap->min->key) {
		fibheap->min = node;
	}
	return 1;
}

void deleteFibHeap(fibheap_t * fibHeap) {
	deleteList(fibHeap->root_list);
	free(fibHeap);
}

void mergeNodes(node_t * node1, node_t * node2, fibheap_t * fibHeap) {
	
	
}

void consolidateFibHeap(fibheap_t * fibHeap) {
	size_t maxDegree = 5;
	node_t ** degrees = (node_t **)calloc(maxDegree, sizeof(node_t *));
	node_t * currentNode = fibHeap->root_list->head;
	do {
		size_t size;
		if (currentNode->child == NULL) size = 0;
		else size = currentNode->child->size;
		while (degrees[size] != NULL && degrees[size] != currentNode) {
			if (size > maxDegree - 1) {
				maxDegree = size + 1;
				degrees = (node_t **)realloc(degrees, maxDegree * sizeof(node_t *));
			}
			mergeNodes(currentNode, degrees[size], fibHeap);		
		
		}
		degrees[size] = currentNode;

	}while (currentNode->rightNode != fibHeap->root_list->head);
}

node_t * extractMin(fibheap_t * fibHeap) {
	node_t * min = fibHeap->min;

	// if min Node has a child list, that list needs to be handled
	if (fibHeap->min->child != NULL) {
		node_t * currentNode = fibHeap->min->child->head;
		list_t * childList = fibHeap->min->child;

		// set List reference of all childs to root list
		do {
			currentNode->own = fibHeap->root_list;
			currentNode = currentNode->rightNode;
		}while (currentNode->rightNode != currentNode);

		//attach child list at the end of the root list, fix pointers
		fibHeap->root_list->tail->rightNode = childList->head;
		childList->head->leftNode = fibHeap->root_list->tail;
		fibHeap->root_list->head->leftNode = childList->head;
		childList->tail->rightNode = fibHeap->root_list->head;

		//set tail of root list to end of child list (which is now at the end of the root list)
		fibHeap->root_list->tail = childList->tail;

		//set head of child list to NULL, effectively emptying it to then delete
		childList->head = NULL;
		deleteList(childList);

	
	}

	//return min Node in case you actually want to do something with the value...
	return min;	
}

void printFibHeap(list_t * list, int depth) {
	node_t * currentNode = list->head;
	do {
		printf("%d %d\n", depth, currentNode->key);
		if (currentNode->child != NULL) printFibHeap(currentNode->child, depth + 1);
		currentNode = currentNode->rightNode;
	}while (currentNode != list->head);	
}


int main() {
	fibheap_t * fib = createFibHeap();
	for (int i = 1; i < 10; i++) {
		printf("Node: %d\n", i);
		node_t * node = createNode(i, NULL);
		insert(node, fib);
	}
	fib->min->child = (list_t *)malloc(sizeof(list_t));
	addToList(fib->min->child, createNode(10, NULL));
	printf("List-Size:%ld\n", fib->root_list->size);
	printf("Head-Key:%d\n", fib->root_list->head->key);

	printFibHeap(fib->root_list, 0);
	
	deleteFibHeap(fib);
	return 0;
}
