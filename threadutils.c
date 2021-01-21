
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

void list_write(void* args)
{
    pthread_mutex_lock(&lock);
    for (int i = 0; i <= index; i++) root = root->next;
    root->val=new;
    pthread_mutex_unlock(&lock);
    printf("Wrote index %d of %p", index, root);
}

void list_append(void* args)
{
    pthread_mutex_lock(&alock);
    for (int i = 0; root->next != NULL; i++) root = root->next;
    Node* next = (Node*)malloc(sizeof(Node));
    next->val = new;
    next->next = NULL;
    root->next = next;   
    pthread_mutex_unlock(&alock);
}

int main()
{
    Node root = {23, NULL};
    struct info a1 = {&root, NULL, 42};
    struct info a2 = {&root, NULL, 53};
    struct info a3 = {&root, NULL, 12};
    list_append((void*)&a1);   
    list_append((void*)&a2);
    list_append((void*)&a3);
    pthread_t writer1;
    pthread_t writer2;
    struct info args = {&root, 1, 17};
    pthread_create(&writer1, NULL, list_write, (void*)&args);
    pthread_create(&writer2, NULL, list_write, (void*)&args);
    pthread_join(writer1, NULL);
    pthread_join(writer2, NULL);
}
