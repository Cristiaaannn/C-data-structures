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

/*
 * Initializes the linked list
 */
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
/*
 * Displays the menu
 */
void displayMenu() {
    printf("Menu:\n");
    printf("Reinitialize (reinitialize)\n");
    printf("Add (add)\n");
    printf("Remove (remove)\n");
    printf("How many elements does the list have? (count)\n");
    printf("Sum of the linked list's elements (sum)\n");
    printf("Find an element in the linked list (search)\n");
    printf("Exit (exit)\n");
}
/*
 * Displays the linked list on the open stream
 */
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

/*
 * Thread function for calling display()
 */
void* display_thread(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        display(first);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    return NULL;
}

/*
 * Returns the sum of the linked list's elements
 */
int sum(Node* first) {

    Node *ptr = first;
    int sum = 0;

    for (int i = 0; i < counter - 1; i++) {
        sum += ptr->data;
        ptr = ptr->next;
    }
    return sum;
}

/*
 * Returns the max. element of the linked list
 */
int max(Node* first) {

    Node *ptr = first;
    int max = 0;

    for (int i = 0; i < counter - 1; i++) {
        if (ptr->data > max) {
            max = ptr->data;
        }
        ptr = ptr->next;
    }
    return max;
}

/*
 * Returns 1 if the key was found in the linked list, 0 otherwise
 * Found keys are transposed with the previous key (transposition optimization) 
 */
int search(Node *first, int key) {
    Node *swap = NULL, *ptr = first, *q = first;
    int found = 0;

    if (ptr->data == key) {
        found = 1;
        return found;
    } else {
        ptr = ptr->next;
        for (int i = 0; i < counter - 2; i++) {
            if (ptr->data != key) {
                q = q->next;
                ptr = ptr->next;
            } else {
                swap = (Node *)malloc(sizeof(Node));
                swap->data = q->data;
                q->data = ptr->data;
                ptr->data = swap->data;
                free(swap);
                found = 1;
            }
        }
    }
    return found;
}

int main(int argc, char **argv) {
    int option, key;
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

    displayMenu();

    do {
        if (strcmp(input,"menu") == 0) {
        displayMenu();
        }
        printf("Enter cmd: ");
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
            printf("The \u03A3 of the %d elements is %d\n", counter - 1, sum(first));
        }
        if (strcmp(input, "max") == 0) {
            printf("The max. of the %d elements is %d \U0001F60E\n", counter - 1, max(first));
        }
        if (strcmp(input, "search") == 0) {
            scanf("%d", &key);
            if (search(first, key)) {
                printf("%d was found \033[0;32m\u2713\033[0m\n", key); // Green checkmark for found
            } else {
                printf("%d was not found \033[0;31m\u2717\033[0m\n", key); // Red cross for not found
            }
        }
        if (strcmp(input, "add") == 0) {
            printf("Adding\n");
        }
        if (strcmp(input, "remove") == 0) {
            printf("Removing\n");
        }
        if (strcmp(input, "exit") == 0) {
            printf("Nice seeing you! \U0001F44B\n");
            return 0;
        }
    } while (1);

    return 0;
}
