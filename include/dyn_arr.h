#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include "report.h"

typedef struct c_dynArr *dynArr;

/* --- Memory management --- */

/* Allocates a new dynamic array. Returns NULL in case of fail */
dynArr dynArr_create(void);

/* Deallocates the whole list, the nodes and the internal reports */
void dynArr_destroy(dynArr d);

/* --- Queries --- */

/* Returns the size of the list */
int dynArr_get_size(dynArr d);

/* Returns 1 if list doesn't have elements, 0 if yes */
int dynArr_is_empty(dynArr d);

/* Returns the report if list has the report, else null */
report dynArr_get_report(dynArr d,int id);

/* Compares two lists and returns 1 if the elements are the same in the same order */
int dynArr_compare(dynArr d1, dynArr d2);

/* Reverses the list in place */
void dynArr_reversed(dynArr d);

/* --- Operations --- */

/* Inserts a report as the new list head. Returns 1 if the operation was successful and 0 if not  */
int dynArr_add(dynArr d, report r);

/* Extracts the first report from the list */
report dynArr_pop_head(dynArr d);

/* Gets a filtered copy of the list */
dynArr dynArr_get_filtered(dynArr d, int cat, int stat);

/* Assigns the counter of each category to each variable */
void dynArr_get_info_stats(dynArr d, int *pending, int *in_prog, int *res, int *light, int *street, int *waste, int *fault);


/* --- Output --- */

/* Prints the whole formatted list. */
void dynArr_print_formatted(dynArr d);

/* Writes the list on a file using the default format. */
void dynArr_print_file(dynArr d, FILE *stream);

/* Prints the formatted list filtering it by category and status */
void dynArr_print_filtered(dynArr d, int cat, int stat);

#endif