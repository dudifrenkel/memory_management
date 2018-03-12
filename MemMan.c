#include "MemMan.h"

//void * testT();

LList* freeS;
LList* takenS;
pthread_mutex_t lock;

#define NUM_THREADS 5

int main() {
    manager_init();
    return 0;
}

/* Initializes the memory allocator. */
void manager_init() {
    Node *frHead;
    if(createNode(&frHead, 0, NULL, NULL, MAX_FREE_NODE)){
        printf("failed initializing manager");
        return;
    }
    pthread_mutex_lock(&lock);

    freeS = malloc(sizeof(LList));
    takenS = malloc(sizeof(LList));
    freeS->head = frHead;
    takenS->head = NULL;
    freeS->lastAlloc = frHead;

    pthread_mutex_unlock(&lock);
}


/* Release all resources acquired by the memory allocator. */
void manager_fini(){
    pthread_mutex_lock(&lock);

    freeList(freeS);
    freeList(takenS);

    pthread_mutex_unlock(&lock);
    manager_init();
}

/*Attempts to allocate <size> bytes of memory.
Returns a non-zero value (indicating the allocation ID) if successful and 0 if there are no available
physically-contiguous memory blocks.
<size> may not be a multiple of 2 megabytes.*/
uint64_t manager_malloc(size_t size){

    /* test for illegal inputs */
    if(size<=0||size>MAX_STORAGE){ return 0;}
    if (&freeS==NULL){ return 0; }

    Node* curr = freeS->head;
    size_t chuncksNum = ceil(size/CHUNCK_SIZE), remainder;
    uint64_t base;

    if(nextFit(&curr,chuncksNum)){
        return 0;
    }

    base = curr->ID;
    remainder = (curr->size) - chuncksNum;

    Node* takenNode;
    if(createNode(&takenNode,base,NULL,NULL,chuncksNum)){
        printf("failed creating node");
        return 0;
    }

    pthread_mutex_lock(&lock);  // start of critical sec

    insert(takenS,takenNode);
    if(remainder > 0){          // takes only part from the block
        curr->ID = base + chuncksNum;
        curr->size = remainder;
        pthread_mutex_unlock(&lock); // end of critical sec
        return base;
    }
    /* removing curr from freeS */
    (curr->prev)->next = curr->next;
    (curr->next)->prev = curr->prev;

    pthread_mutex_unlock(&lock); // end of critical sec

    free(curr);
    return base;
}



/* Deallocate the memory corresponding to that allocation. */
void manager_free(uint64_t alloc_id){
    Node* wanted;
    if(searchNode(alloc_id,*takenS,&wanted)) {
        printf("didn't found requested id");
        return;
    }

    pthread_mutex_lock(&lock);  // critical section

    removeNo(wanted);       // remove from takenS
    insert(freeS,wanted);   // insert into freeS
    merge_if_pos(wanted);

    pthread_mutex_unlock(&lock);
}

/* The function try to merge the "wanted" node with his neighbors */
void merge_if_pos(Node* wanted){
    if(!merge(wanted->prev,wanted)){     // merged between wanted and his left node
        merge(wanted->prev,wanted->next);
        free(wanted);
    }
    else {
        if(!merge(wanted, wanted->next)){    // merged *only* between wanted and his right node
            free(wanted);
        }
    }
}

/* try to "merge" two neighboring nodes to one */
int merge(Node* left, Node* right){
    if( (left==NULL)||(right==NULL) ){ return 1; }  // at least one of the node is NULL
    if( (left->ID) + (left->size) == right->ID){    // checks if the right node is right after the left one
        left->size = left->size + right->size;
        removeNo(right);
        return 0;
    }
    return 1;
}

/* The function looking for wanted node according to given alloc_id */
int searchNode(uint64_t alloc_id, LList list, Node** wanted){
    Node* curr = list.head;
    while (curr){
        if (curr->ID == alloc_id){
            *wanted = curr;
            return 0;
        }
        curr = curr->next;
    }
    return 1;   // didn't found requested id
}

/* The function looking for the first place in memory that big enough according to given chunksNum from
 * the last place allocated */
int nextFit(Node** ret,size_t chunksNum){
    Node* curr = freeS->lastAlloc;
    do{
        if(curr->size >= chunksNum) {     // founds place big enough
            *ret = curr;
            freeS->lastAlloc = curr;
            return 0;
        }
        curr = curr->next;
        if (!curr) {
            curr = freeS->head;
        }
    }
    while (curr!=freeS->lastAlloc);
    return 1;
}

///* for tests */
//void* runner(void* arg){
//    int *tN = (int*)(arg);
//    int t = *tN;
//    uint64_t a = manager_malloc(pow(2,20)*t);
//    printf("t%d: %lld\n",t,a);
//    pthread_exit(0);
//}
