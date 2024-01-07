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

void mergeNodes(node_t * node1, node_t * node2, fibheap_t * fibHeap, node_t ** degrees) {
	if (node1->child == NULL) {
		list_t * list = (list_t *)malloc(sizeof(list_t));
		if (node2->key < node1->key) {
			node2->child = list;
		} else {
			node1->child = list;
		}
	}

	if (node2->key < node1->key) {
		removeFromList(node1->own, node1, 0);
		node1->own = node2->child;
		addToList(node2->child, node1);
		degrees[node2->child->size - 1] = NULL;
		if (degrees[node2->child->size] != NULL) {
			mergeNodes(node2, degrees[node2->child->size], fibHeap, degrees);
		} else {
			degrees[node2->child->size] = node2;
		}
	} else {
		removeFromList(node2->own, node2, 0);
		node2->own = node1->child;
		addToList(node1->child, node2);
		degrees[node1->child->size - 1] = NULL;
		if (degrees[node1->child->size] != NULL) {
			mergeNodes(node1, degrees[node1->child->size], fibHeap, degrees);
		} else {
			degrees[node1->child->size] = node1;
		}
	}
}

void consolidateFibHeap(fibheap_t * fibHeap) {
	size_t maxDegree = 5;
	node_t ** degrees = (node_t **)calloc(maxDegree, sizeof(node_t *));
	node_t * currentNode = fibHeap->root_list->head;
	do {
		size_t size;
		node_t * nextNode = currentNode->rightNode;
		if (currentNode->child == NULL) size = 0;
		else size = currentNode->child->size;
		if (degrees[size] != NULL) {
			if (size > maxDegree - 1) {
				maxDegree = size + 2;
				degrees = (node_t **)realloc(degrees, maxDegree * sizeof(node_t *));
			}
			mergeNodes(currentNode, degrees[size], fibHeap, degrees);		
		
		} else {
			degrees[size] = currentNode;
		}
		currentNode = nextNode;
	}while (currentNode != fibHeap->root_list->head);
	free(degrees);
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
	removeFromList(fibHeap->root_list, min, 0);

	consolidateFibHeap(fibHeap);

	//return min Node in case you actually want to do something with the value...
	return min;	
}

void printFibHeap(list_t * list, int depth) {
	node_t * currentNode = list->head;
	do {
		char * indent = malloc((depth + 1) * sizeof(char));
		for (int i = 0; i < depth; i++) {
			indent[i] = '\t';		
		}
		printf("%sDepth: %d Node-Key: %d\n", indent, depth, currentNode->key);
		if (currentNode->child != NULL) printFibHeap(currentNode->child, depth + 1);
		currentNode = currentNode->rightNode;
	}while (currentNode != list->head);	
}


int main() {
	fibheap_t * fib = createFibHeap();
	for (int i = 1; i < 17; i++) {
		//printf("Node: %d\n", i);
		node_t * node = createNode(i, NULL);
		insert(node, fib);
	}
	fib->min->child = (list_t *)malloc(sizeof(list_t));
	addToList(fib->min->child, createNode(10, NULL));
	//printf("List-Size:%ld\n", fib->root_list->size);
	//printf("Head-Key:%d\n", fib->root_list->head->key);

	extractMin(fib);
	//consolidateFibHeap(fib);

	printFibHeap(fib->root_list, 0);
	
	deleteFibHeap(fib);
	return 0;
}
