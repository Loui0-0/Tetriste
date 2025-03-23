#ifndef DOUBLE_LINKED_LIST_H
#define DOUBLE_LINKED_LIST_H

#include "pawn.h"

typedef struct Node {
	/*element fondamentale de DoubleLinkedList et LinkedList*/

    Pawn** pawnPtrPtr; 
    struct Node* previous;
    struct Node* next;

} Node;

typedef struct {
    /*simple implementation des liste doublement chainé avec boucle*/

    int size;
    Node* firstNode;
    Node* lastNode;
} DoubleLinkedList;

typedef struct {
    /*simple implementation des liste simplement chainé avec boucle*/

    int size;
    Node* firstNode;
    Node* lastNode;
} LinkedList;

DoubleLinkedList* newDoubleLinkedList();

Node* newNode(Pawn** pawnPtr);

void doubleLinkedList_appendFirst(DoubleLinkedList* list, Pawn** pawnPtrPtr);

void doubleLinkedList_appendLast(DoubleLinkedList* list, Pawn** pawnPtrPtr);

void doubleLinkedList_remove(DoubleLinkedList* list, Pawn* pawn);

int doubleLinkedList_isElem(DoubleLinkedList* list, Pawn** pawnPtrPtr);

Pawn** doubleLinkedList_pop(DoubleLinkedList* list);

void doubleLinkedList_clear(DoubleLinkedList *list);

void doubleLinkedList_displayDebug(DoubleLinkedList* list);

void doubleLinkedList_free(DoubleLinkedList* list);


LinkedList* newLinkedList();

void linkedList_appendFirst(LinkedList* list, Pawn** pawnPtrPtr);

void linkedList_appendLast(LinkedList* list, Pawn** pawnPtrPtr);

void linkedList_remove(LinkedList* list, Pawn* pawn);

int linkedList_isElem(LinkedList* list, Pawn** pawnPtrPtr);

Pawn** linkedList_pop(LinkedList* list);

void linkedList_clear(LinkedList *list);

void linkedList_displayDebug(LinkedList* list);

void linkedList_free(LinkedList *list);


#endif //DOUBLE_LINKED_LIST_H