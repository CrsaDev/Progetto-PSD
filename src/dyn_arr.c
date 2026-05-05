#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dyn_arr.h"

#define INITIAL_CAP 10

struct c_dynArr {
    report *array;    // Dinamic array of reports
    int size;         // Current number of elements
    int capacity;     // Maximum capacity before reallocation
};

/* -------------------------------------------------------------------------
   HELPER FUNCTIONS (STATIC)
   ------------------------------------------------------------------------- */

// Quickly swaps two reports (useful for dynArr_reversed)
static void swap_reports(report *a, report *b) {
    report temp = *a;
    *a = *b;
    *b = temp;
}

/* -------------------------------------------------------------------------
   MEMORY MANAGEMENT
   ------------------------------------------------------------------------- */

dynArr dynArr_create(void) {
    dynArr l = malloc(sizeof(struct c_dynArr));
    if(l == NULL) return NULL;

    l->size = 0;
    l->capacity = INITIAL_CAP;
    l->array = malloc(l->capacity * sizeof(report));

    if (l->array == NULL) {
        free(l);
        return NULL;
    }

    return l;
}

void dynArr_destroy(dynArr d) {
    if(d == NULL) return;

    /* Destructs the reports along with the array to avoid wasting memory */
    for (int i = 0; i < d->size; i++) {
        if (d->array[i] != NULL) {
            report_destroy(d->array[i]);
        }
    }
    
    free(d->array);
    free(d);
}

/* -------------------------------------------------------------------------
   QUERIES
   ------------------------------------------------------------------------- */

int dynArr_get_size(dynArr d) {
    return (d == NULL) ? -1 : d->size;
}

int dynArr_is_empty(dynArr d) {
    return d == NULL || d->size == 0;
}

report dynArr_get_report(dynArr d, int id) {
    if (d == NULL) return NULL;

    // Translating id to index
    int index = id - 1;

    // Checking bounds
    if (index < 0 || index >= d->size) {
        return NULL; // Means the id isn't valid 
    }

    // Returns the report
    return d->array[index];
}

int dynArr_compare(dynArr l1, dynArr l2) {
    if(!l1 || !l2) return -1;
    if(l1->size != l2->size) return 0;

    for (int i = 0; i < l1->size; i++) {
        if (report_compare(l1->array[i], l2->array[i]) == 0) {
            return 0; // Mismatch found
        }
    }

    return 1; // Arrays are identical
}

/* -------------------------------------------------------------------------
   QUERY & UTILITIES
   ------------------------------------------------------------------------- */

int dynArr_add(dynArr d, report r) {
    if(!d || !r) return 0;

    // Checks if the report already exists
    if (dynArr_get_report(d,report_id(r))) {
        return 0; 
    }

    // Reallocs the array if max size was reached
    if (d->size == d->capacity) {
        int new_capacity = d->capacity + (d->capacity / 2); // Increase of 1.5
        report *temp = realloc(d->array, new_capacity * sizeof(report));
        if (!temp) return 0; 
        
        d->array = temp;
        d->capacity = new_capacity;
    }

    // Inserting in the end O(1)
    d->array[d->size] = r; 
    d->size++;

    return 1;
}

report dynArr_pop_head(dynArr d) {
    if(dynArr_is_empty(d)) return NULL;

    report r = d->array[0];

    // Shift everything left by 1 to fill the gap of the popped head
    d->size--;
    if (d->size > 0) {
        memmove(&d->array[0], &d->array[1], d->size * sizeof(report));
    }

    return r;
}

/* -------------------------------------------------------------------------
   MOSTLY FOR TESTING 
   ------------------------------------------------------------------------- */

void dynArr_reversed(dynArr d) {
    // If the list is empty or has 1 element, nothing to reverse
    if (dynArr_is_empty(d) || d->size == 1) return;

    int start = 0;
    int end = d->size - 1;

    // Swap elements from the outside moving inwards
    while (start < end) {
        swap_reports(&d->array[start], &d->array[end]);
        start++;
        end--;
    }
}

dynArr dynArr_get_filtered(dynArr d, int cat, int stat) {
    if (d == NULL) return NULL;

    dynArr final_l = dynArr_create();
    if (final_l == NULL) return NULL;

    for (int i = 0; i < d->size; i++) {
        // Matching
        int cat_match = (cat == -1 || (int)report_category(d->array[i]) == cat);
        int stat_match = (stat == -1 || (int)report_status(d->array[i]) == stat);

        if (cat_match && stat_match) {
            report copy = report_copy(d->array[i]);
            
            // If adding fails return the partial output
            if (!dynArr_add(final_l, copy)) {
                report_destroy(copy);
                
                break; 
            }
        }
    }
    
    return final_l;
}

void dynArr_get_info_stats(dynArr d, int *pending, int *in_prog, int *res, int *light, int *street, int *waste, int *fault) {
    if (dynArr_is_empty(d)) return;

    for (int i = 0; i < d->size; i++) {
        // Counting states
        status s = report_status(d->array[i]);
        if (s == PENDING) (*pending)++;
        else if (s == IN_PROGRESS) (*in_prog)++;
        else if (s == RESOLVED) (*res)++;

        // Counting categories
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

void dynArr_print_file(dynArr d, FILE *stream) {
    if(dynArr_is_empty(d)) {
        printf("The list is empty!\n");
        return;
    }

    // Print into file
    for (int i = 0; i < d->size; i++) {
        char *curr_record = report_file_string(d->array[i]);

        if(curr_record != NULL) {
            fprintf(stream, "%s\n", curr_record);
            free(curr_record); // To reset the string
        }
    }
}

void dynArr_print_formatted(dynArr d) {
    if(dynArr_is_empty(d)) {
        printf("There are currently no reports available.\n");
        return;
    }

    // Print all the reports
    for (int i = 0; i < d->size; i++) {
        report_formatted(d->array[i]);
        printf("\n");
    }
}

void dynArr_print_filtered(dynArr d, int cat, int stat) {
    if (dynArr_is_empty(d)) {
        printf("There are currently no reports available.\n");
        return;
    }

    int found = 0;

    for (int i = 0; i < d->size; i++) {
        // For matches
        int match_cat = (cat == -1 || report_category(d->array[i]) == cat);
        int match_stat = (stat == -1 || report_status(d->array[i]) == stat);

        if (match_cat && match_stat) {
            report_formatted(d->array[i]);
            found++;
        }
    }

    if (found == 0) {
        printf("No report was found.\n");
    }
}