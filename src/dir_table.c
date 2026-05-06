#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dir_table.h"

#define INITIAL_CAP 500 // Initial CAP for array size

struct c_dirTable {
    report *array;
    int size;         // Number of elements in the array
    int capacity;     // Size of the array
    int max_id;      // Highest ID memorized, to optimize looping
};

/* -------------------------------------------------------------------------
   GESTIONE MEMORIA
   ------------------------------------------------------------------------- */

dirTable dirTable_create(void) {
    dirTable l = malloc(sizeof(struct c_dirTable));
    if(l == NULL) return NULL;

    l->size = 0;
    l->max_id = -1; // Since the table is empty
    l->capacity = INITIAL_CAP;
    l->array = calloc(l->capacity, sizeof(report));

    if (l->array == NULL) {
        free(l);
        return NULL;
    }

    return l;
}

void dirTable_destroy(dirTable d) {
    if(d == NULL) return;

    // Scanning up until the MAX ID, as it is the last element that occupates the array
    for (int i = 0; i <= d->max_id; i++) {
        if (d->array[i] != NULL) {
            report_destroy(d->array[i]);
        }
    }
    
    free(d->array);
    free(d);
}

/* -------------------------------------------------------------------------
   QUERY
   ------------------------------------------------------------------------- */

int dirTable_get_size(dirTable d) {
    return (d == NULL) ? -1 : d->size;
}

int dirTable_get_max_id(dirTable d) {
    return (d == NULL) ? -1 : d->max_id;
}

int dirTable_is_empty(dirTable d) {
    return d == NULL || d->size == 0;
}

report dirTable_get_report(dirTable d, int id) {
    // Checking if the given id is inbounds
    if (d == NULL || id < 0 || id > d->max_id) return NULL;
    return d->array[id];
}

int dirTable_compare(dirTable l1, dirTable l2) {
    if (!l1 || !l2) return -1;
    // If either size or max_id are different, it's easy to say the lists are different
    if (l1->size != l2->size || l1->max_id != l2->max_id) return 0;

    for (int i = 0; i <= l1->max_id; i++) {
        report r1 = l1->array[i];
        report r2 = l2->array[i];

        if (r1 != NULL && r2 != NULL) {
            if (report_compare(r1, r2) == 0) return 0; // Mismatching reports
        } else if (r1 != r2) { 
            return 0; // One is null and the other isn't - causes easy mistmatch without compare
        }
    }
    return 1;
}

/* -------------------------------------------------------------------------
   OPERAZIONI
   ------------------------------------------------------------------------- */

int dirTable_add(dirTable d, report r) {
    if(!d || !r) return 0;

    int index = report_id(r); 

    if (index >= d->capacity) {
        int new_capacity = index + 500;  // Not allocating too much memory
        
        report *temp = realloc(d->array, new_capacity * sizeof(report));
        if (!temp) return 0; 
        
        // Cleaning the new allocated memory
        memset(temp + d->capacity, 0, (new_capacity - d->capacity) * sizeof(report));

        d->array = temp;
        d->capacity = new_capacity;
    }

    // If the index is already allocated, then fail
    if (d->array[index] != NULL) return 0;

    d->array[index] = r; 
    d->size++;

    // Updating to the new max id
    if (index > d->max_id) {
        d->max_id = index;
    }

    return 1;
}

/* -------------------------------------------------------------------------
   FILTRAGGIO E STATISTICHE
   ------------------------------------------------------------------------- */

dirTable dirTable_get_filtered(dirTable d, int cat, int stat) {
    if (d == NULL) return NULL;

    dirTable final_l = dirTable_create(); 
    if (final_l == NULL) return NULL;

    // Scanning up to max_id as it is the last element
    for (int i = 0; i <= d->max_id; i++) {
        if (d->array[i] == NULL) continue; 

        int r_cat = report_category(d->array[i]);
        int r_stat = report_status(d->array[i]);

        int cat_match = (cat == -1 || r_cat == cat);
        int stat_match = (stat == -1 || r_stat == stat);

        if (cat_match && stat_match) {
            report copy = report_copy(d->array[i]);
            if (!dirTable_add(final_l, copy)) {
                report_destroy(copy);
            }
        }
    }
    return final_l;
}

void dirTable_print_filtered(dirTable d, int cat, int stat) {
    if (d == NULL || d->size == 0) {
        printf("No reports available.\n");
        return;
    }

    int found = 0;

    for (int i = 0; i <= d->max_id; i++) {
        // Skipping empty
        if (d->array[i] == NULL) {
            continue; 
        }

        // Extractuing the values
        int r_cat = report_category(d->array[i]);
        int r_stat = report_status(d->array[i]);

        // Matching
        int cat_match = (cat == -1 || r_cat == cat);
        int stat_match = (stat == -1 || r_stat == stat);

        if (cat_match && stat_match) {
            report_formatted(d->array[i]);
            printf("\n"); 
            found++;
        }
    }

    if (found == 0) {
        printf("No reports found for filters (Category: %d, Status: %d).\n", cat, stat);
    }
}

void dirTable_get_info_stats(dirTable d, int *pending, int *in_prog, int *res, int *light, int *street, int *waste, int *fault) {
    if (d == NULL) return;

    // Scanning up to max id
    for (int i = 0; i <= d->max_id; i++) {
        if (d->array[i] == NULL) continue;

        status s = report_status(d->array[i]);
        if (s == PENDING) (*pending)++;
        else if (s == IN_PROGRESS) (*in_prog)++;
        else if (s == RESOLVED) (*res)++;

        category c = report_category(d->array[i]);
        if (c == LIGHTNING) (*light)++;
        else if (c == STREET) (*street)++;
        else if (c == WASTE) (*waste)++;
        else if (c == FAULT) (*fault)++;
    }
}

/* -------------------------------------------------------------------------
   OUTPUT
   ------------------------------------------------------------------------- */

void dirTable_print_file(dirTable d, FILE *stream) {
    if(dirTable_is_empty(d)) return;

    for (int i = 0; i <= d->max_id; i++) {
        if (d->array[i] == NULL) continue;

        char *curr_record = report_file_string(d->array[i]);
        if(curr_record != NULL) {
            fprintf(stream, "%s\n", curr_record);
            free(curr_record);
        }
    }
}

void dirTable_print_formatted(dirTable d) {
    if(dirTable_is_empty(d)) {
        printf("No reports available.\n");
        return;
    }

    for (int i = 0; i <= d->max_id; i++) {
        if(d->array[i] != NULL) {
            report_formatted(d->array[i]);
            printf("\n");
        }
    }
}