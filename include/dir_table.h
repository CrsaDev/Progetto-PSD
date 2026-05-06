#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include "report.h"

typedef struct c_dirTable *dirTable;

/* --- Memory management --- */

/* Allocates a new dynamic array. Returns NULL in case of fail */
dirTable dirTable_create(void);

/* Deallocates the whole list, the nodes and the internal reports */
void dirTable_destroy(dirTable d);

/* --- Queries --- */

/* Returns the size of the list */
int dirTable_get_size(dirTable d);

/* Returns the max id allocated */
int dirTable_get_max_id(dirTable d);

/* Returns 1 if list doesn't have elements, 0 if yes */
int dirTable_is_empty(dirTable d);

/* Returns the report if list has the report, else null */
report dirTable_get_report(dirTable d,int id);

/* Compares two lists and returns 1 if the elements are the same in the same order */
int dirTable_compare(dirTable d1, dirTable d2);

/* --- Operations --- */

/* Inserts a report as the new list head. Returns 1 if the operation was successful and 0 if not  */
int dirTable_add(dirTable d, report r);

/* Extracts the first report from the list */
report dirTable_pop_head(dirTable d);

/* Gets a filtered copy of the list */
dirTable dirTable_get_filtered(dirTable d, int cat, int stat);

/* Assigns the counter of each category to each variable */
void dirTable_get_info_stats(dirTable d, int *pending, int *in_prog, int *res, int *light, int *street, int *waste, int *fault);


/* --- Output --- */

/* Prints the whole formatted list. */
void dirTable_print_formatted(dirTable d);

/* Writes the list on a file using the default format. */
void dirTable_print_file(dirTable d, FILE *stream);

/* Prints the formatted list filtering it by category and status */
void dirTable_print_filtered(dirTable d, int cat, int stat);

#endif