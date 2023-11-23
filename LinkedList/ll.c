/*
 * Author:  Cristian Mirea
 * Date :   20/11/2023
 * Brief:   Program that show ll priciples, academic purposes
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct Node {
    int data;
    struct Node *next;
} Node;

Node *first = NULL;
Node *last = NULL;

int counter = 1;

Node* initialize() {
    int nrElAtInit = 0;

    Node* new_node = NULL, *ptr = NULL;

    printf("\nWith how many elements do you want to init the list?: ");
    scanf("%d", &nrElAtInit);
    new_node = (Node*)malloc(sizeof(Node));
    printf("Enter node(%d) data: ", counter++);
    scanf("%d", &new_node->data);
    new_node->next = NULL;
    first = new_node;
    ptr = first;
    if (nrElAtInit == 1) {
        return first;
    } else {
        for (int i = 0; i < nrElAtInit - 1; i++) {
            Node *new_node = (Node *)malloc(sizeof(Node));
            printf("Enter node(%d) data: ", counter++);
            scanf("%d", &new_node->data);
            new_node->next = NULL;
            ptr->next = new_node;
            ptr = ptr->next;
            last = ptr;
        }
    }
    return first;
}

void display(Node* first) {

    FILE* fp;

    fp = fopen("list.txt","w");
    if (fp == NULL) {
        perror("\nerror opening file");
    }

    Node *ptr = first;
    for (int i = 0; i < counter - 1; i++) {
        fprintf(fp,"%d ", ptr->data);
        if (ptr->next != NULL) {
            fprintf(fp,"-> ");
        }
        ptr = ptr->next;
    }
    if (fclose(fp) == EOF) {
        perror("\nerror closing file");
    }
}

void* display_thread(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        display(first);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    return NULL;
}

int sum(Node* first) {

    Node *ptr = first;
    int sum = 0;

    for (int i = 0; i < counter - 1; i++)
    {
        sum += ptr->data;
        ptr = ptr->next;
    }
    return sum;
}

int main(int argc, char **argv) {
    int option;
    char input[20];


    if (first == NULL) {
        printf("\nLinked list is empty, initializing \n");
        first = initialize();
    }

    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, display_thread, first) != 0) {
        printf("Failed to create thread\n");
        return 1;
    }

    do {
        printf("Menu:\n");
        printf("1. Reinitialize\n");
        printf("2. Add\n");
        printf("3. Remove\n");
        printf("4. How many elements does the list have? (count)\n");
        printf("5. Sum of the linked list's elements\n");
        printf("6. Exit\n");
        printf("Enter option: ");
        scanf("%s", input);
        if (strcmp(input, "reinitialize") == 0) {
            pthread_mutex_lock(&mutex);
            counter = 1;
            first = NULL;
            first = initialize();
            pthread_mutex_unlock(&mutex);
        }
        if (strcmp(input, "count") == 0) {
            printf("The list has %d elements\n", counter - 1);
        }
        if (strcmp(input, "sum") == 0) {
            printf("The sum of the %d elements is %d\n", counter - 1, sum(first));
        }
        if (strcmp(input, "add") == 0) {
            printf("Adding\n");
        }
        if (strcmp(input, "remove") == 0) {
            printf("Removing\n");
        }
        if (strcmp(input, "exit") == 0) {
            printf("Exiting\n");
            return 0;
        }
    } while (1);

    return 0;
}
