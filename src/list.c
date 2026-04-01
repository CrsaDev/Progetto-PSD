#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

struct list_node {
    report item;
    struct list_node *next;
};

struct list {
    struct list_node *head;
    int size;
};

/* --- Memory Management --- */

static struct list_node* list_node_create(report r) {
    struct list_node* n = malloc(sizeof(*n));
    if (n == NULL) return NULL;

    n->item = r;
    n->next = NULL;
    return n;
}

// Allocates and returns a new list instance
list list_create() {
    list l = malloc(sizeof(*l));
    if(l == NULL) return NULL;

    l->head = NULL;
    l->size = 0;

    return l;
}

// Frees the memory allocated for the given list
void list_destroy(list l) {
    if(list_is_empty(l)) {
        return;
    }

    struct list_node *curr = l->head;
    struct list_node *next_node;

    while(curr != NULL) {
        next_node = curr->next;

        report_destroy(curr->item);
        free(curr);
        curr = next_node;
    }

    free(l);
}

// Returns 1 if the list is empty
int list_is_empty(list l) {
    return l == NULL || l->head == NULL;
}

// Pushes a report to the head of the list
int list_add(list l, report r) {
    struct list_node *node = list_node_create(r);
    if(node == NULL) {
        return 0;
    }

    node->next = l->head;
    l->head = node;
    l->size++;

    return 1;
}

// Deletes a specified report from the list
int list_delete_id(list l, int id) {
    struct list_node *curr = l->head;
    struct list_node *prev = NULL;

    while(curr != NULL && report_id(curr->item) != id) {
        prev = curr;
        curr = curr->next;
    }

    // If the current node is null, it means we couldn't find the report
    if(curr == NULL) return 0;

    // If the previous is null, it means that the element it's the head else it's in the list
    if(prev == NULL) {
        l->head = curr->next;
    } else {
        prev->next = curr->next;
    }

    report_destroy(curr->item);
    free(curr);
    l->size--;

    return 1;
}

// Prints all the reports
void list_print(list l) {
    struct list_node *curr = l->head;

    while(curr != NULL) {
        report_to_string(curr->item);
        printf("\n");
        curr = curr->next;
    }
}

// Returns a specified report from the list
report list_search_by_id(list l,int id) {
    if(list_is_empty(l)) {
        return NULL;
    }

    struct list_node *curr = l->head;

    while(curr != NULL && report_id(curr->item) != id) {
        curr = curr->next;
    }

    //The element isn't in the list
    if(curr == NULL) return NULL;

    return curr->item;
}

// Prints the reports of a selected category
void list_print_by_category(list l, category c) {
    struct list_node *curr = l->head;

    while(curr != NULL) {
        if(report_category(curr->item) == c) {
            report_to_string(curr->item);
            printf("\n");
        }

        curr = curr->next;
    }
}

