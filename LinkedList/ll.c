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

Node *initialize() {
    Node *first = NULL, *ptr = NULL, *new_node = NULL; 
    int nrElAtInit = 0;
    printf("\nWith how many elements do you want to init the linked list? ");
    scanf("%d", &nrElAtInit);
    new_node = (Node*)malloc(sizeof(Node));
    if (new_node ==  NULL) {
        perror("Memory allocation error");
    }
    printf("\nEnter node data(int): ");
    scanf("%d", &new_node->data);
    new_node->next = NULL;
    first = new_node;
    ptr = first;
    for (int i = 0; i < nrElAtInit - 1; i++) {
        new_node = (Node *)malloc(sizeof(Node));
        if (new_node == NULL) {
            perror("Memory allocation error");
        }
        printf("\nEnter node data(int): ");
        scanf("%d", &new_node->data);
        new_node->next = NULL;
        ptr->next = new_node;
        ptr = ptr->next;
    }
    return first;
}

void display(Node *first) {
    FILE* fp;

    fp = fopen("list.txt","w");
    if (fp == NULL) {
        perror("error opening file");
    }
    
    Node *ptr;
    ptr = first;
    while (ptr != NULL) {
        fprintf(fp,"%d ", ptr->data);
        ptr = ptr->next;
        if (ptr != NULL) {
            fprintf(fp,"-> ");
        }
    }
    fprintf(fp,"\n");
    if (fclose(fp) == EOF) {
        perror("error closing the file");
    }
}

    Node *first = NULL;


void* display_thread(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        display(first);
        pthread_mutex_unlock(&mutex);
        sleep(1); // sleep for 1 second to avoid flooding the console
    }
    return NULL;
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
        printf("4. Exit\n");
        printf("Enter option: ");
        scanf("%s", input);
        if (strcmp(input, "reinitialize") == 0) {
            pthread_mutex_lock(&mutex);
            first = NULL;
            first = initialize();
            pthread_mutex_unlock(&mutex);
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