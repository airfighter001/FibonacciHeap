#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

typedef struct node {
	int key;
	void* value;
	int marked;
	struct node * leftNode;
	struct node * rightNode;
	struct node * father;
	struct node * child;
} node_t;

typedef struct list {
	size_t size;
	struct node * head;
	struct node * tail;
} list_t;

list_t * createList() {
	list_t * list = (list_t *)malloc(sizeof(list_t));
	list->size = 0;
	return list;
}

node_t * createNode() {
	node_t * node = (node_t *)malloc(sizeof(node_t));
	node->key = -1;
	node->marked = 0;
	return node;
}

int deleteNode(node_t * node) {
	free(node);
	return 1;
}

int addToList(list_t * list, node_t * node) {
	if (list->head == NULL) {
		list->head = node;
		list->tail = node;
	}
	node->rightNode = list->head;
	node->leftNode = list->tail;
	list->head->leftNode = node;
	list->tail->rightNode = node;
	list->tail = node;
	list->size++;
	return 1;
}

int removeFromList(list_t * list, node_t * node) {
	if (node->leftNode == node) {
		list->head = NULL;
		list->tail = NULL;
	} else {
		node->leftNode->rightNode = node->rightNode;
		node->rightNode->leftNode = node->leftNode;
	}
	deleteNode(node);
	list->size--;
	return 1;
}

int deleteList(list_t * list) {
	while (list->head != NULL) {
		removeFromList(list, list->tail);
	}
	free(list);
	return 1;
}
