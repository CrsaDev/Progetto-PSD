#ifndef LIST_H
#define LIST_H

#include "report.h"

typedef struct list *list;

/* --- Memory Management --- */

// Allocates and returns a new list instance
list list_create();

// Frees the memory allocated for the given list
void list_destroy(list l);

// Returns 1 if the list is empty
int list_is_empty(list l);

// Pushes a report to the head of the list and returns the success of the operation
int list_add(list l, report r);

// Deletes a specified report from the list
int list_delete_id(list l, int id);

// Prints all the reports
void list_print(list l);

// Returns a specified report from the list
report* list_search_by_id(list l,int id);

// Prints the reports of a selected category
void list_print_by_category(list l, category c);



#endif