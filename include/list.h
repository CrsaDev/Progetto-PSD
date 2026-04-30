#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include "report.h"

typedef struct c_list *list;

/* --- Memory management --- */

/* Allocates a new empty list. Returns NULL in case of fail */
list list_create(void);

/* Deallocates the whole list, the nodes and the internal reports */
void list_destroy(list l);

/* --- Queries --- */

/* Returns 1 if list doesn't have elements, 0 if yes */
int list_is_empty(list l);

/* Returns the report if list has the report, else null */
report list_get_report(list l,int id);

/* Compares two lists and returns 1 if the elements are the same in the same order */
int list_compare(list l1, list l2);

/* Reverses the list in place */
void list_reversed(list l);

/* --- Operations --- */

/* Inserts a report as the new list head. Returns 1 if the operation was successful and 0 if not  */
int list_add(list l, report r);

/* Extracts the first report from the list */
report list_pop_head(list l);

/* Gets a filtered copy of the list */
list list_get_filtered(list l, int cat, int stat);

/* Assigns the counter of each category to each variable */
void list_get_info_stats(list l, int *pending, int *in_prog, int *res, int *light, int *street, int *waste, int *fault);


/* --- Output --- */

/* Prints the whole formatted list. */
void list_print_formatted(list l);

/* Writes the list on a file using the default format. */
void list_print_file(list l, FILE *stream);

/* Prints the formatted list filtering it by category and status */
void list_print_filtered(list l, int cat, int stat);

#endif