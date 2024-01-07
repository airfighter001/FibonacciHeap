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

	//if nodes have degree 0, there's no child list yet, create and assign child list
	//to node with smaller key
	if (node1->child == NULL) {
		list_t * list = (list_t *)malloc(sizeof(list_t));
		if (node2->key < node1->key) {
			node2->child = list;
		} else {
			node1->child = list;
		}
	}
	
	//remove bigger node from current list (always root list)
	//set list of bigger node to child list of smaller node
	//add bigger node to child list of smaller node
	//set entry to degree list at degree of both nodes before merging to NULL
	//if there's an entry in degree list at new degree, call merge again with 
	//node in degree list and merged node
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
	//initialize list of nodes by degree with 5
	size_t maxDegree = 5;
	node_t ** degrees = (node_t **)calloc(maxDegree, sizeof(node_t *));
	//initialize noce to check with head of root list
	node_t * currentNode = fibHeap->root_list->head;
	// go through all nodes of the root list
	do {
		size_t size;

		//remember the next node so it's fine if the current node becomes a child of another node
		node_t * nextNode = currentNode->rightNode;

		//if the current node has no child list, its degree is 0
		if (currentNode->child == NULL) size = 0;
		else size = currentNode->child->size;
		
		//if degree of node is too big for list, reallocate list for more space
		if (size > maxDegree - 1) {
			maxDegree = size + 2;
			degrees = (node_t **)realloc(degrees, maxDegree * sizeof(node_t *));
		}
		//if there's an entry in the degree-list, nodes have to be merged
		//else put current node into list
		if (degrees[size] != NULL) {
			mergeNodes(currentNode, degrees[size], fibHeap, degrees);		
		
		} else {
			degrees[size] = currentNode;
		}

		//when done, move on to next node
		currentNode = nextNode;
	}while (currentNode != fibHeap->root_list->head);
	//free memory resserved for degree list
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
	//remove min node from root list
	removeFromList(fibHeap->root_list, min, 0);
	
	//call function to minimize the number of nodes in the root list
	consolidateFibHeap(fibHeap);
	
	//find new minimum
	//set head to min, then go through root list and set min to node if smaller
	node_t * currentNode = fibHeap->root_list->head;
	fibHeap->min = currentNode;
	do {
		if (currentNode->key < fibHeap->min->key) fibHeap->min = currentNode;
		currentNode = currentNode->rightNode;
	}while (currentNode != fibHeap->root_list->head);

	//return min Node in case you actually want to do something with the value...
	return min;	
}

void cutNode(fibheap_t * fibHeap, node_t * node) {
	//set pointer to father node for marking/cutting later
	node_t * father = NULL;
	if (node->own->father != NULL) father = node->own->father;

	//remove node from child list of father and move to root list if not already there
	//then check if node key is smaller than min key, if it is, set node as min
	if (node->own != fibHeap->root_list) {
		removeFromList(node->own, node, 0);
		node->marked = 0;
		addToList(fibHeap->root_list, node);
		if (node->key < fibHeap->min->key) fibHeap->min = node;
	}
	
	// mark father node if not marked already (having lost a child)
	// if already marked, recursively cut out father node as well
	if (!father->marked) {
		father->marked = 1;
	} else {
		cutNode(fibHeap, father);
	}
}

void decreaseKey(fibheap_t * fibHeap, node_t * node, int newKey) {
	//set key of node to new value
	node->key = newKey;
	//if key of node is still bigger than key of father, nothing to do, return
	if (node->own->father != NULL && newKey >= node->own->father->key) return;
	cutNode(fibHeap, node);

}

void delete(fibheap_t * fibHeap, node_t * node) {
	//to delete a node, simply decrease its key to below the minimal min key value
	//then call extract key to remove it from the fibHeap
	decreaseKey(fibHeap, node, -1);
	extractMin(fibHeap);
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
		node_t * node = createNode(i, NULL);
		insert(node, fib);
	}
	fib->min->child = (list_t *)malloc(sizeof(list_t));
	addToList(fib->min->child, createNode(10, NULL));

	node_t * min = extractMin(fib);
	free(min);

	printFibHeap(fib->root_list, 0);
	
	deleteFibHeap(fib);
	return 0;
}
