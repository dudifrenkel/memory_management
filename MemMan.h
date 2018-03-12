#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include "LinkedList.h"

#define MAX_STORAGE (uint64_t)(pow(2,35))             // in bytes

#define MAX_FREE_NODE (size_t)(pow(2,14)) // each node represent 2MB of space

#define CHUNCK_SIZE (pow(2,21))

void manager_init();
void manager_fini();
uint64_t manager_malloc(size_t size);
void manager_free(uint64_t alloc_id);
void merge_if_pos(Node* wanted);
int merge(Node* left, Node* right);
int searchNode(uint64_t alloc_id,LList list, Node** wanted);
int nextFit(Node** ret,size_t chunksNum);
void* runner(void* arg);
