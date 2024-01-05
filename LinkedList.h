#include <stdlib.h>
#include <ctype.h>

typedef struct node {
	int key;
	void* value;
	int marked;
	struct node * leftNode;
	struct node * rightNode;
	struct list * own;
	struct list * child;
} node_t;

typedef struct list {
	size_t size;
	struct node * head;
	struct node * tail;
	struct node * father;
} list_t;

node_t * createNode(int key, void* value) {
	node_t * node = (node_t *)malloc(sizeof(node_t));
	node->key = key;
	node->marked = 0;
	return node;
}

int deleteNode(node_t * node) {
	free(node);
	return 1;
}

list_t * createList() {
	list_t * list = (list_t *)malloc(sizeof(list_t));
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
	return list;
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
	node->own = list;
	list->size++;
	return 1;
}

int removeFromList(list_t * list, node_t * node, int fullDelete) {
	if (node->leftNode == node) {
		list->head = NULL;
		list->tail = NULL;
	} else {
		node->leftNode->rightNode = node->rightNode;
		node->rightNode->leftNode = node->leftNode;
	}
	if (fullDelete) deleteNode(node);
	list->size--;
	return 1;
}

int deleteList(list_t * list) {
	while (list->head != NULL) {
		if (list->tail->child != NULL) {
			deleteList(list->tail->child);
		}
		removeFromList(list, list->tail, 1);
	}
	free(list);
	return 1;
}

node_t * findNode(list_t * list, int searchKey) {
	node_t * currentNode = list->head;
	do {
		if (currentNode->key == searchKey) {
			return currentNode;
		}
		currentNode = currentNode->rightNode;

	}while (currentNode->rightNode != list->head);
	return NULL;
}
