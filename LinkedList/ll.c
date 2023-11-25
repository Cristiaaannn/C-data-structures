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

Node *first = NULL;
Node *last = NULL;

int counter = 1;

/*
 * Initializes the linked list
 */
Node *initialize() {
    int nrElAtInit = 0;

    Node *new_node = NULL, *ptr = NULL;

    printf("\nWith how many elements do you want to init the list?: ");
    scanf("%d", &nrElAtInit);
    new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) {
        perror("Error allocating memory\n");
        exit(EXIT_FAILURE);
    }
    printf("Enter node(%d) data: ", counter++);
    scanf("%d", &new_node->data);
    new_node->next = NULL;
    first = new_node;
    ptr = first;
    if (nrElAtInit == 1) {
        return first;
    } else {
        for (int i = 0; i < nrElAtInit - 1; i++) {
            Node *new_node = malloc(sizeof(Node));
            if (new_node == NULL) {
                perror("Error allocating memory\n");
                exit(EXIT_FAILURE);
            }
            printf("Enter node(%d) data: ", counter++);
            scanf("%d", &new_node->data);
            new_node->next = NULL;
            ptr->next = new_node;
            ptr = ptr->next;
            last = ptr;
        }
    }
    // flush the input buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
    return first;
}
/*
 * Displays the menu
 */
void displayMenu() {
    printf("Menu:\n");
    printf("Reinitialize (reinitialize)\n");
    printf("How many elements does the list have? (count)\n");
    printf("Which is the max. number? (max)\n");
    printf("Check if the linked list is sorted (check sorted)\n");
    printf("Sum of the linked list's elements (sum)\n");
    printf("Find an element in the linked list (searcht / find[val])\n");
    printf("Insert an element before the first (insert first / head[val])\n");
    printf("Insert an element after certain pos (insert after pos[pos] [value]\n");
    printf("Insert an element after certain value (insert after val [value] [value-to-be-inserted]\n");
    printf("Delete the first element (delete head / first)\n");
    printf("Delete an element after certain pos (delete after pos[pos])\n");
    printf("Delete an element after certain val (delete after val[val])\n");
    printf("Delete a certain value (delete [val])\n");
    printf("Remove duplicates (remove duplicates)\n");
    printf("Exit (exit)\n");
}
/*
 * Displays the linked list on the open stream
 */
void display(Node *first) {
    FILE *fp;

    fp = fopen("list.txt", "w");
    if (fp == NULL) {
        perror("Error allocating memory\n");
        exit(EXIT_FAILURE);
    }

    Node *ptr = first;
    for (int i = 0; i < counter - 1; i++) {
        fprintf(fp, "%d ", ptr->data);
        if (ptr->next != NULL) {
            fprintf(fp, "-> ");
        }
        ptr = ptr->next;
    }
    if (fclose(fp) == EOF) {
        perror("Error allocating memory\n");
        exit(EXIT_FAILURE);
    }
}

/*
 * Thread function for calling display()
 */
void *display_thread(void *arg) {
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
int sum(Node *first) {
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
int max(Node *first) {
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
 */
int search(Node *first, int key) {
    Node *ptr = first;
    int found = 0;

    if (ptr->data == key) {
        found = 1;
        return found;
    } else {
        ptr = ptr->next;
        for (int i = 0; i < counter - 2; i++) {
            if (ptr->data != key) {
                ptr = ptr->next;
            } else {
                found = 1;
            }
        }
    }
    return found;
}
/*
 * Returns 1 if the key was found in the linked list, 0 otherwise
 * Found keys are transposed with the previous key (transposition optimization)
 */
int searcht(Node *first, int key) {
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
                if (swap == NULL) {
                    perror("Error allocating memory\n");
                    exit(EXIT_FAILURE);
                }
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

/*
 * Inserts a new node before the first node
 */
Node *insertBeforeFirst(Node *first, int x) {
    Node *t = NULL;
    t = (Node *)malloc(sizeof(Node));
    if (t == NULL) {
        perror("Error allocating memory\n");
        exit(EXIT_FAILURE);
    }
    t->data = x;
    t->next = first;
    first = t;
    printf("🚀 %d was inserted before the first node\n", x);
    counter++;
    return first;
}

/*
 * Inserts a new node after a certain position
 */
Node *insertAfterPos(Node *first, int pos, int x) {
    if (pos > counter - 1) {
        printf("\033[0;31m\u2717\033[0m %d is not a valid pos\n", pos);
        return first;
    }
    
    Node *ptr = first, *t = NULL;
    
    for (int i = 0; i < pos -1; i++) {
        ptr = ptr->next;
    }
        
    t = (Node *)malloc(sizeof(Node));
    if (t == NULL) {
        perror("Error allocating memory\n");
        exit(EXIT_FAILURE);
    }
    t->data = x;
    t->next = ptr->next;
    ptr->next = t;
    counter++;
    printf("🚀 %d was inserted after pos %d\n", x, pos);
    return first;
}

/*
 * Inserts a new node after a certain value
 */
Node *insertAfterValue(Node *first, int value, int x) {
    if ((search(first, value) == 0)) {
        printf("\033[0;31m\u2717\033[0m %d is not in the list\n", value);
        return first;
    }
    Node *ptr = first, *t = NULL;

    for (int i = 0; i < counter - 1; i++) {
        if (ptr->data == value) {
            break;
        } else {
            ptr = ptr->next;
        }
    }
        t = (Node *)malloc(sizeof(Node));
        if (t == NULL) {
            perror("Error allocating memory\n");
            exit(EXIT_FAILURE);
        }
        t->data = x;
        t->next = ptr->next;
        ptr->next = t;
        counter++;
        printf("🚀 %d was inserted after %d\n", x, value);
    return first;
}

/*
 * Deleting the first node
 */
Node *delete_first(Node *first) {
    if (first->next == NULL) {
        printf("\033[0;31m\u2717\033[0m Deletion of the first element when it has only 1 element is forbidden\n");
        return first;
    }
    Node *ptr = first;
    first = first->next;
    counter--;
    printf("⤼ '%d' was deleted\n", ptr->data);
    free(ptr);
    return first;
}

/*
 * Deleting after certain pos
 */
Node *deleteAfterPos(Node *first, int pos) {
    if (pos > counter - 1) {
        printf("\033[0;31m\u2717\033[0m %d is not a valid pos\n", pos);
        return first;
    }
    Node *ptr = first, *q = first;
    for (int i = 0; i < pos; i++) {
        q = ptr;
        ptr = ptr->next;
    }
    q->next = ptr->next;
    printf("⤼ '%d' was deleted\n",ptr->data);
    free(ptr);
    counter--;
    return first;
}

/*
 * Deleting after certain value
 */
Node *deleteAfterValue(Node *first, int value) {
    if ((search(first, value) == 0)) {
        printf("\033[0;31m\u2717\033[0m %d is not in the list\n", value);
        return first;
    }
    Node *ptr = first, *q = first;
    for (int i = 0; i < counter - 1; i++) {
        if (ptr->data == value) {
            break;
        } else {
            q = ptr;
            ptr = ptr->next;
        }
    }
    q = ptr;
    ptr = ptr->next;
    q->next = ptr->next;
    counter--;
    printf("⤼ '%d' was deleted\n", ptr->data);
    free(ptr);
    return first;
}
/*
 * Deleting certain value
 */
Node *deleteValue(Node *first, int value) {
    if ((search(first, value) == 0)) {
        printf("\033[0;31m\u2717\033[0m %d is not in the list\n", value);
        return first;
    }
    if (first->data == value) {
        first = delete_first(first);
        return first;
    }

    Node *ptr = first, *t = NULL;

    for (int i = 0; i < counter - 1; i++) {
        if (ptr->data == value) {
            break;
        } else {
            t = ptr;
            ptr = ptr->next;
        }
    }
    t->next = ptr->next;
    counter--;
    printf("⤼ '%d' was deleted\n", ptr->data);
    free(ptr);
    return first;
}

/*
 * Checks if the linked list is sorted
 * Returns: 1 if in ascneding, 2 if in descending, 0 if not
 */
int checkIfSorted(Node *first) {
    int sorted = 1;
    Node *ptr = first, *q = NULL;
    q = ptr;
    ptr = ptr->next;
    if (q->data == ptr->data) {
        q = ptr;
        ptr = ptr->next;
    }
    if (q->data < ptr->data) {          // Ascending order
        while (ptr->next != NULL) {
            q = ptr;
            ptr = ptr->next;
            if (q->data > ptr->data) {
                sorted = 0;
                return sorted;
            }
        }
    } else {                            // Descending order
        sorted = 2;
        while (ptr->next != NULL) {
            q = ptr;
            ptr = ptr->next;
            if (q->data < ptr->data) {
                sorted = 0;
                return sorted;
            }
        }
    }
    return sorted;
}
/*
 * Remove duplicates from the LL
 */
void *removeDuplicates(Node *first) {
    Node *ptr = first, *q = NULL;
    int i = 0;
    bool* seen = (bool*)calloc(10000, sizeof(bool));
    while (ptr->next != NULL) {
        if (seen[ptr->data]) {
            q->next = ptr->next;
            printf("⤼ '%d' was deleted\n", ptr->data);
            free(ptr);
            counter--;
            ptr = q->next;
        } else {
            seen[ptr->data] = true;
            q = ptr;
            ptr = ptr->next;
        }
    }
    if (seen[ptr->data]) {
        first = deleteAfterPos(first,counter-2);
    }
}

int main(int argc, char **argv) {
    int key, i = 0;
    char input[50];

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
        printf("Enter cmd: ");
        fflush(NULL);
        fgets(input, 50, stdin);          // read the entire line
        input[strcspn(input, "\n")] = 0;  // remove the newline character
        if (strcmp(input, "menu") == 0) {
            displayMenu();
        }
        if (strncmp(input, "insert head", 11) == 0) {
            sscanf(input, "insert head %d", &key);  // parse the argument from the input string
            first = insertBeforeFirst(first, key);
        }
        if (strncmp(input, "insert first", 12) == 0) {
            sscanf(input, "insert first %d", &key);  // parse the argument from the input string
            first = insertBeforeFirst(first, key);
        }
        if (strncmp(input, "insert after pos", 16) == 0) {
            sscanf(input, "insert after pos %d %d", &i, &key);
            first = insertAfterPos(first, i, key);
        }
        if (strncmp(input, "insert after val", 16) == 0) {
            sscanf(input, "insert after val %d %d", &i, &key);
            first = insertAfterValue(first, i, key);
        }
        if (strncmp(input, "delete after pos", 16) == 0) {
            sscanf(input, "delete after pos %d", &key);
            first = deleteAfterPos(first, key);
        }
        if (strncmp(input, "delete after val", 16) == 0) {
            sscanf(input, "delete after val %d", &key);
            first = deleteAfterValue(first, key);
        }
        if (strncmp(input, "remove duplicates", 17) == 0) {
            removeDuplicates(first);
        }
        if (strncmp(input, "delete first", 12) == 0 || strncmp(input, "delete head", 11) == 0) {
            first = delete_first(first);
        }
        if (strncmp(input, "delete val", 10) == 0) {
            sscanf(input, "delete val %d", &key);
            first = deleteValue(first, key);
        }
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
            printf("The ⅀ of the %d elements is %d\n", counter - 1, sum(first));
        }
        if (strcmp(input, "max") == 0) {
            printf("The max. of the %d elements is %d \U0001F60E\n", counter - 1, max(first));
        }
        if (strncmp(input, "searcht", 7) == 0 || strncmp(input, "find", 4) == 0) {
            sscanf(input, "%*s %d", &key);  // parse the argument from the input string
            if (searcht(first, key)) {
                printf(" \033[0;32m\u2713\033[0m\n");  // Green checkmark for found
            } else {
                printf(" \033[0;31m\u2717\033[0m\n");  // Red X for not found
            }
        }
        if (strcmp(input, "check sorted") == 0) {
            switch (checkIfSorted(first)) {
                case 0:
                    printf("The list is not sorted\n");
                    break;
                case 1:
                    printf("The list is sorted in ascending order\n");
                    break;
                case 2:
                    printf("The list is sorted in descending order\n");
                    break;
                default:
                    printf("\033[0;31m\u2717\033[0mError: could not determine");
                    break;
            }
        }
        if (strcmp(input, "exit") == 0) {
            free(first);
            printf("Nice seeing you! \U0001F44B\n");
            // clear the file
            FILE *fp;
            fp = fopen("list.txt", "w");
            if (fp == NULL) {
                perror("Error allocating memory\n");
                exit(EXIT_FAILURE);
            }
            fprintf(fp, "empty");
            if (fclose(fp) == EOF) {
                perror("Error allocating memory\n");
                exit(EXIT_FAILURE);
            }
            return 0;
        }
    } while (1);

    return 0;
}
