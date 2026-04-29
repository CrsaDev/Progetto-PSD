#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include "report.h"

/* Hidden type for list management */
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

/* 
    Returns the count of the reports of a specified field.
    -'c': Category
    -'s': Status
    -'p': Priority
*/
int list_report_field_count(list l, char field, int value);


/* Returns an inverted copy of l leaving l untouched*/
list list_reversed(list l);


/* --- Operations --- */

/* Inserts a report as the new list head. Returns 1 if the operation was successful and 0 if not  */
int list_add(list l, report r);

/* Removes a report from the list and the deallocates both the record and the node if found. Returns 1 if found and -1 if not */
int list_delete_id(list l, int id);

/* Extracts the first report from the list */
report list_remove_head(list l);


/* --- Output --- */

/* Prints the whole formatted list. */
void list_print_formatted(list l);

/* Writes the Scrive la lista su uno stream (file o stdout) usando il formato specificato. */
void list_print_file(list l, FILE *stream);

/* Prints the formatted list filtering it by category */
void list_print_by_category(list l, category c);

#endif