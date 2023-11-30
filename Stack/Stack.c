/*
 * Author:  Cristian Mirea
 * Date :   20/11/2023
 * Brief:   Program that show ll priciples, academic purposes
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct Node {
    int data;
    struct Node *next;
} Node;

Node *top = NULL;
int counter = 1;

/*
 * pushes to stack
 */
Node *push(Node *top, int x) {
    Node *ptr = top;
    ptr = (Node*)malloc(sizeof(Node));
    if (ptr == NULL) {
        printf("Memory overflow\n");
        exit(EXIT_FAILURE);
    }
    counter++;
    ptr->data = x;
    ptr->next = top;
    top = ptr;
    return top;
}

/*
 * Displays the top value of the stack
 */
int peek(Node *top) {
    return top->data;
}

/*
 * pops from stack, returns popped value
 */
Node *pop(Node *top) {
    Node *ptr = top;
    if (top->next == NULL) {
        printf("Popping from a 1-element stack is forbidden\n");
        return top;
    }
    top = top->next;
    free(ptr);
    counter--;
    ptr = top;
    return top;
}

/*
 * Displays the menu
 */
void displayMenu() {
    printf("Menu:\n");
    printf("push (push [value])\n");
    printf("peek (peek)\n");
    printf("Exit (exit)\n");
}

/*
 * Displays the stack on the open stream
 */
void display(Node *top) {
    FILE *fp;

    fp = fopen("stack.txt","w");
    if (fp == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    Node *ptr = top;
    for (int i = 0; i < counter - 1; i++) {
        fprintf(fp,"|%d|",ptr->data);
        if (ptr->next != NULL) {
            fprintf(fp,"\n");
        }
        ptr = ptr->next;
    }
    
    if (fclose(fp) == EOF) {
        perror("Error closing file\n");
        exit(EXIT_FAILURE);
    }
}

/*
 * Thread function for calling display()
 */
void *display_thread(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        display(top);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    return NULL;
}

int	main(int argc, char **argv) {
    int key = 0, i = 0;
    char input[50];

    printf("Welcome to the stack program!😁\n");

    if (top == NULL) {
        printf("At first, at least one value must be pushed to the stack\n");
        printf("Enter the first value: ");  scanf("%d",&key);   printf("\n");
        top = push(top,key);
    }

    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, display_thread, top) != 0) {
        printf("Failed to create thread\n");
        return 1;
    }

    displayMenu();

    do {
        printf("Enter command: ");  scanf("%s",input);
        if (strcmp(input,"push") == 0) {
            scanf("%d",&key);
            top = push(top,key);
        } else if (strcmp(input,"peek") == 0) {
            printf("Top value: %d\n",peek(top));
        } else if (strcmp(input,"pop") == 0) {
            printf("⇡ (%d)\n", top->data);
            top = pop(top);
        } else if (strcmp(input,"exit") == 0) {
            free(top);
            FILE *fp;
            fp = fopen("stack.txt","w");
            if (fp == NULL) {
                perror("Error opening file");
                exit(EXIT_FAILURE);
            }
            fprintf(fp,"empty");
            if (fclose(fp) == EOF) {
                perror("Error closing file\n");
                exit(EXIT_FAILURE);
            }
            printf("Nice seeing you! \U0001F44B\n");
            return 0;
        } else {
            printf("Invalid command\n");
        }
    } while (1);

    return 0;
}
