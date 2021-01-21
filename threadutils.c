
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef struct Node Node;
struct Node {
    uint64_t val;
    Node* next;
};

pthread_mutex_t lock;
pthread_mutex_t alock;

struct info {
    Node* root;
    int index;
    uint64_t val;
};

void* list_write(void* args)
{
    pthread_mutex_lock(&lock);
    struct info* a = (struct info*) args;
    printf("%p %d %llu\n", a->root, a->index, a->val);    
    Node* root = a->root;
    for (int i = 0; i < a->index; i++) root = root->next;
    root->val=a->val;
    pthread_mutex_unlock(&lock);
    printf("Wrote index %d of %p\n", a->index, a->root);
}

void* list_read(void* args)
{
    pthread_mutex_lock(&lock);
    struct info* a = (struct info*) args;
    Node* root = a->root;
    for (int i = 0; i < a->index; i++) root = root->next;    
    printf("Read index %d of %p and got %llu\n", a->index, a->root, root->val);
    pthread_mutex_unlock(&lock);
}

void* list_append(void* args)
{
    pthread_mutex_lock(&alock);
    struct info* a = (struct info*) args;
    Node* root = a->root;
    for (int i = 0; root->next != NULL; i++) root = root->next;
    Node* next = (Node*)malloc(sizeof(Node));
    next->val = a->val;
    next->next = NULL;
    root->next = next;   
    pthread_mutex_unlock(&alock);
}

int main()
{
    Node* root = (Node*)malloc(sizeof(Node));
    pthread_t writer1;
    pthread_t writer2;
    struct info* args = (struct info*)malloc(sizeof(struct info));
    args->root = root;
    args->index = 0;
    args->val = 17;
    pthread_create(&writer1, NULL, list_write, (void*)args);
    pthread_create(&writer2, NULL, list_read, (void*)args);
    pthread_join(writer1, NULL);
    pthread_join(writer2, NULL);
}
