#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "LinkedList.h"

//typedef struct node {
//	int key;
//	void* value;
//	int marked;
//	struct node * leftNode;
//	struct node * rightNode;
//	struct node * father;
//	struct node * child;
//} node_t;

//typedef struct list {
//	size_t size;
//	struct node * head;
//	struct node * tail;
//} list_t;

typedef struct fibheap {
	struct node * min;
	struct list * root_list;
} fibheap_t;

int addNode(int key, void* value, node_t* leftNode, node_t* rightNode, node_t* father, node_t* child) {
	
	return 1;
}

int main() {
	//node_t * head = NULL;
	//head = (node_t *) malloc(sizeof(node_t));
	node_t * head = createNode();
	if (head->leftNode == NULL) {
		puts("test");
	}
	head->leftNode = head;
	head->rightNode = head;
	//head->child = NULL;
	//head->father = NULL;
	head->key = 10;
	list_t * root = createList();
	root->head = head;
	root->tail = head;
	root->size++;

	printf("List-Size:%ld\n", root->size);
	printf("Head-Key:%d\n", root->head->key);


	free(head);
	free(root);
	return 0;
}
