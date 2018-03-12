#ifndef UNTITLED_LINKEDLIST_H
#define UNTITLED_LINKEDLIST_H

#include <stdint.h>
#include <stdlib.h>

struct Node{
    uint16_t ID;
    struct Node* next;
    struct Node* prev;
    size_t size;
}typedef Node;

struct LList{
    Node* head;
    Node* lastAlloc;
}typedef LList;

void insert(LList* list, Node* newNode);
int removeNo(Node* node);
int createNode(Node** node, uint16_t ID, Node* next, Node* prev, size_t size);
void freeList(LList* list);


#endif //UNTITLED_LINKEDLIST_H
