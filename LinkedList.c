#include "LinkedList.h"

/* The function insert the given node in the correct place in the given list according to ID */
void insert(LList* list, Node* newNode){
    Node* curr = list->head;
    if (curr == NULL){   // takenS is empty
        list->head = newNode;
        return;
    }
    if( (newNode->ID) < (curr->ID) ){   // The correct place is the first place of the list
        newNode->prev = NULL;
        newNode->next = curr;
        curr->prev = newNode;
        list->head = newNode;
        return;
    }
    Node* currP;
    while( curr!=NULL ){
        currP = curr;
        if( (newNode->ID) < (curr->ID) ){   // The correct place in the middle of the list
            newNode->prev = curr->prev;
            newNode->next = curr;
            (newNode->prev)->next = newNode;
            (curr->prev) = newNode;
            return;
        }
        curr = (curr->next);
    }
    currP->next = newNode;
    newNode->prev = currP;
    return;
}

/* The function remove the given node from the given list */
int removeNo(Node* node){
    if(node->prev != NULL){
        (node->prev)->next = node->next;
    }
    if(node->next != NULL){
        (node->next)->prev = node->prev;
    }
}

/* The function create Node with malloc and update all the given attributes */
int createNode(Node** node, uint16_t ID, Node* next, Node* prev, size_t size){
    (*node) = malloc(sizeof(Node));
    if(!(*node)){
        return 1;
    }
    (*node)->ID = ID;
    (*node)->next = next;
    (*node)->prev = prev;
    (*node)->size = size;
    return 0;
}

/* The function free all the nodes in the given list */
void freeList(LList* list){
    Node* curr = list->head;
    Node* prev;
    if(!curr) {
        free(list);
        return;
    }
    prev = curr;
    curr = curr->next;
    while (curr){
        free(prev);
        prev = curr;
        curr=curr->next;
    }
    free(prev); // only 1 node on the list
    free(list);
}
