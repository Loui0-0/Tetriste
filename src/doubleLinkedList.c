#include <stdio.h>
#include <stdlib.h>
#include "doubleLinkedList.h"


Node *newNode(Pawn **pawnPtrPtr) {
	/*renvoie un  pointer vers un element node*/

	Node *node = (Node *)malloc(sizeof(Node));
	if (node == NULL) {
		return NULL;
	}
	node->pawnPtrPtr = pawnPtrPtr;
	node->next = NULL;
	node->previous = NULL;
	return node;
}


DoubleLinkedList* newDoubleLinkedList() {
	/*renvoie un pointer vers une liste doublement chainée*/
	DoubleLinkedList* list = malloc(sizeof(DoubleLinkedList));
	if (list == NULL) {
		return NULL;
	}

	list->size = 0;
	list->firstNode = NULL;
	list->lastNode = NULL;

	return list;
}

void doubleLinkedList_displayDebug(DoubleLinkedList *list) {
 
	if (list == NULL){
 
		printf("(NULL)\n");
 
		return;
 
	}
	
	printf("size : %d\n", list->size);

 
	Node* currentNode = list->firstNode;
	
 
	if (currentNode == NULL){
		printf("[]\n");
		return;
 
	}
	
	printf("[");
	while (currentNode != list->lastNode){
	
		int shapeId = pawn_getShapeId(currentNode->pawnPtrPtr);
		int colorId = pawn_getColorId(currentNode->pawnPtrPtr);
		printf("s:%d c:%d, ", colorId, shapeId);
		currentNode = currentNode->next; 
	}

	int shapeId = pawn_getShapeId(currentNode->pawnPtrPtr);
	int colorId = pawn_getColorId(currentNode->pawnPtrPtr);
	printf("s:%d c:%d]\n", colorId, shapeId);
}

void doubleLinkedList_appendFirst(DoubleLinkedList *list, Pawn** pawnPtrPtr){
	/*ajoute a gauche de la liste doublement chainé pawnPtrPtr alloue automatiquement la node*/
	
	Node *newFirstNode = newNode(pawnPtrPtr);
	if (newFirstNode == NULL) {
	return;
	}
	if (list->size == 0) {
	// lorsque la liste est vide on cree le list contenant un seul element qui
	// boucle sur lui meme;

	// la liste contient un seul element le 1er est donc aussi le dernier
	list->firstNode = newFirstNode;
	list->lastNode = newFirstNode;

	// on fait boucler la liste
	list->firstNode->previous = list->lastNode;
	list->lastNode->next = list->firstNode;

	} else {
	// fait suivre au nouveau 1er element l'ancien
	newFirstNode->next = list->firstNode;

	// l'ancien 1er est mtn précédé par le nouveau 1er
	list->firstNode->previous = newFirstNode;

	// on met a jour le 1er element de la liste
	list->firstNode = newFirstNode;

	// on fait boucler la liste
	list->firstNode->previous = list->lastNode;
	list->lastNode->next = list->firstNode;
	}

	list->size++;
}

void doubleLinkedList_appendLast(DoubleLinkedList *list, Pawn **pawnPtrPtr) {
	/*ajoute a droite de la liste doublement chainé pawnPtrPtr alloue automatiquement la node*/
	

	Node *newLastNode = newNode(pawnPtrPtr);

	if (newLastNode == NULL) {
	return;
	}

	if (list->size == 0) {
	// lorsque la liste est vide on cree la liste ne contenant qu'un seul
	// element qui boucle sur lui meme;

	// la liste contient un seul element le 1er est donc aussi le dernier
	list->firstNode = newLastNode;
	list->lastNode = newLastNode;

	// on fait boucler la liste
	list->firstNode->previous = list->lastNode;
	list->lastNode->next = list->firstNode;

	} else {

	// le nouveau dernier est mtn précédé par l'ancien dernier
	newLastNode->previous = list->lastNode;

	// l'ancien dernier a pour suivant le nouveau dernier
	list->lastNode->next = newLastNode;

	// on met a jour le sernier elem de la liste
	list->lastNode = newLastNode;

	// on fait boucler la liste
	list->firstNode->previous = list->lastNode;
	list->lastNode->next = list->firstNode;
	}

	list->size++;
}

void doubleLinkedList_remove(DoubleLinkedList *list, Pawn *pawn) { 
	/*si l'element passé en argument est trouvé dans la liste il est supprimé free automatiquement la node*/

	// cas particulié si la liste est de taille 1
	if (list->size == 0) {
	return;


	} else if (list->size == 1) {

		if (*list->firstNode->pawnPtrPtr == pawn) {
			free(list->firstNode);
			list->firstNode = NULL;
			list->lastNode = NULL;
			list->size--;
			return;
		}
	}

	Node *currentNode = list->firstNode;

	// parcours de la liste de gauche a droite
	while (currentNode != list->lastNode) {

	// suppression si la Node est trouvé
	if (*currentNode->pawnPtrPtr == pawn) {
		// on lie doublement les 2 nodes autour de current node
		currentNode->previous->next = currentNode->next;
		currentNode->next->previous = currentNode->previous;

		if (currentNode == list->firstNode) {
		// si le 1er elem est supr c'est sont suivant qui devient alors 1er
		list->firstNode = currentNode->next;
		}

		list->size--;
		free(currentNode);
		return;
	}

	currentNode = currentNode->next;
	}

	// mise a jour de list->firstNode si il se trouve que c'est la node supr

	// cas particulié si l'element a supr est le dernier de la liste
	if (*list->lastNode->pawnPtrPtr == pawn) {
	list->lastNode->previous->next = list->firstNode;
	list->firstNode->previous = list->lastNode->previous;
	list->lastNode = list->lastNode->previous;
	list->size--;
	free(currentNode);
	}	
}


int doubleLinkedList_isElem(DoubleLinkedList* list, Pawn** pawnPtrPtr){
	/*renvoie 1 si pawnPtrPtr est dans la liste 0 sinon*/
	
	if (list == NULL){
		return 0;
	}

	if (list->size == 0){
		return 0;
	}

	Node* currentNode = list->firstNode;
	
	do {
		if (currentNode->pawnPtrPtr == pawnPtrPtr){
			return 1;
		}
		currentNode = currentNode->next;
	} while (currentNode != list->firstNode);

	return 0;
}

Pawn** doubleLinkedList_pop(DoubleLinkedList* list){
	/*renvoie le pawn** du premier element de la liste libere la memoire automatiquement*/

	if (list == NULL){
	return NULL;
	}
	if (list->size == 0){
	return NULL;
	}

	Pawn** toReturn = list->firstNode->pawnPtrPtr;
	Node* toFree = list->firstNode;

	list->firstNode = list->firstNode->next;
	list->lastNode->next = list->firstNode;
	list->firstNode->previous = list->lastNode; 

	list->size--;

	if (list->size == 0){
	list->firstNode = NULL;
	list->lastNode = NULL;
	}

	free(toFree);
	return toReturn;
}



void doubleLinkedList_clear(DoubleLinkedList *list){
	/*vide la liste donné en argument libere automatiquement la memoire*/
	if (list == NULL){
		return;
	}
	
	if (list->size == 0){
		return;
	}

	while(list->firstNode){
		doubleLinkedList_pop(list);
	}
}

void doubleLinkedList_free(DoubleLinkedList *list) {
	/*libere de la memoire la liste aussi que les nodes de celle ci (mais pas les elements qu'elle contient (pawn**))*/

	if (list==NULL){
		return;
	}
	
	if (list->size > 0){
		doubleLinkedList_clear(list);
	}

	free(list);
}


/* les fonctions suivantes sont similaires aux precedente donc seront moins detaillé */

LinkedList *newLinkedList() {
	/*revoie un pointer vers une liste simplement chainé*/

	LinkedList *list = malloc(sizeof(LinkedList));
	if (list == NULL) {
	return NULL;
	}

	list->size = 0;
	list->firstNode = NULL;
	list->lastNode = NULL;
	return list;
}

void linkedList_appendFirst(LinkedList *list, Pawn **pawnPtrPtr) {


	Node *newFirstNode = newNode(pawnPtrPtr);
	if (newFirstNode == NULL) {
	return;
	}

	if (list->size == 0) {
	list->firstNode = newFirstNode;
	list->lastNode = newFirstNode;
	} else {
	newFirstNode->next = list->firstNode;
	list->firstNode = newFirstNode;
	}

	// boucle
	list->lastNode->next = list->firstNode;
	list->size++;
	return;
}

void linkedList_appendLast(LinkedList *list, Pawn **pawnPtrPtr) {

	Node *newLastNode = newNode(pawnPtrPtr);
	if (newLastNode == NULL) {
	return;
	}

	if (list->size == 0) {
	list->firstNode = newLastNode;
	list->lastNode = newLastNode;
	} else {
	list->lastNode->next = newLastNode;
	list->lastNode = newLastNode;
	}

	// boucle
	list->lastNode->next = list->firstNode;
	list->size++;
}

void linkedList_remove(LinkedList *list, Pawn* pawn) {

	if (list == NULL) {
		return ;
	}

	//cas 1 : l'element a supprimer est le seul de la liste
	if (list->size == 1){
		if (*list->firstNode->pawnPtrPtr == pawn){
			free(list->firstNode);
			list->firstNode = NULL;
			list->lastNode = NULL;
			list->size = 0;
			return;
		}
	}


	Node* previousNode = NULL;
	Node* currentNode = list->firstNode;

	while (currentNode != list->lastNode) {

		if (*currentNode->pawnPtrPtr == pawn){
			
			if (previousNode) { //cas 2 l'element a supr est dans le corps de la liste 
				previousNode->next = currentNode->next;
			
			} else { //cas 3 l'element a supr est le 1er de la liste
				list->firstNode      = list->firstNode->next;
				list->lastNode->next = list->firstNode;

			}
			list->size--;
			free(currentNode);
			return;
		}
		previousNode = currentNode;
		currentNode = currentNode->next;
	}

	// cas 4 l'element a supr est le dernier de la liste
	if (*list->lastNode->pawnPtrPtr == pawn) { 
		list->lastNode = previousNode;
		list->lastNode->next = list->firstNode;
		list->size--;
		free(currentNode);
		return;
	}
	
	return;
}

void linkedList_displayDebug(LinkedList *list) {
	if (list == NULL){
	printf("(NULL)\n");
	return;
	}

	printf("size : %d\n", list->size);
	Node* currentNode = list->firstNode;

	if (currentNode == NULL) {
	printf("[]\n");
	return;
	}

	printf("[");
	while (currentNode != list->lastNode){
	int shapeId = pawn_getShapeId(currentNode->pawnPtrPtr);
	int colorId = pawn_getColorId(currentNode->pawnPtrPtr);
	printf("s:%d c:%d, ", colorId, shapeId);
	currentNode = currentNode->next; 
	}
	int shapeId = pawn_getShapeId(currentNode->pawnPtrPtr);
	int colorId = pawn_getColorId(currentNode->pawnPtrPtr);
	printf("s:%d c:%d]\n", colorId, shapeId);
}

int linkedList_isElem(LinkedList* list, Pawn** pawnPtrPtr){
	
	if (list == NULL){
		return 0;
	}
	if (list->size == 0){
		return 0;
	}

	Node* currentNode = list->firstNode;

	do {
		if (currentNode->pawnPtrPtr == pawnPtrPtr){
			return 1;
		}
		currentNode = currentNode->next;
	} while (currentNode != list->firstNode);

	return 0;
}

Pawn** linkedList_pop(LinkedList* list){
	if (list == NULL){
		return NULL;
	}
	
	if (list->size == 0){
		return NULL;
	}

	Pawn** toReturn = list->firstNode->pawnPtrPtr;
	Node* toFree = list->firstNode; 
	
	if (list->size == 1){
							
		list->firstNode = NULL;
		list->lastNode = NULL;					  
	
	} else {

		list->firstNode = list->firstNode->next;
		list->lastNode->next = list->firstNode;
	}

	list->size--;

	free(toFree);
	return toReturn;
}

void linkedList_clear(LinkedList* list){
	while(list->firstNode){
		linkedList_pop(list);
	}
}

void linkedList_free(LinkedList *list) {
	if (list==NULL){
		return;
	}
	linkedList_clear(list);
	free(list);
}

