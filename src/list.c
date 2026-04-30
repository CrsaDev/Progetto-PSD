#include <stdlib.h>
#include <string.h>
#include "list.h"

/* List node structure */
struct list_node {
    report item;
    struct list_node *next;
};

/* List structure */
struct c_list {
    struct list_node *head;
    int size;
};

/* -------------------------------------------------------------------------
   HELPER FUNCTIONS (STATIC)
   ------------------------------------------------------------------------- */

static struct list_node* list_node_create(void) {
    struct list_node* n = malloc(sizeof(*n));
    if (n == NULL) return NULL; // Checks if the allocation was successful

    n->item = NULL;
    n->next = NULL;
    return n;
}

/* -------------------------------------------------------------------------
   MEMORY MANAGEMENT
   ------------------------------------------------------------------------- */

list list_create(void) {
    list l = malloc(sizeof(*l));
    if(l == NULL) return NULL;

    l->head = NULL;
    l->size = 0;
    return l;
}

void list_destroy(list l) {
    if(l == NULL) return;

    struct list_node *curr = l->head;
    while(curr != NULL) {
        struct list_node *next_node = curr->next;

        /* Destructs the reports along with the nodes to avoid wasting memory */
        if(curr->item != NULL) {
            report_destroy(curr->item);
        }
        free(curr);
        curr = next_node;
    }
    free(l);
}

/* -------------------------------------------------------------------------
   QUERIES
   ------------------------------------------------------------------------- */

int list_is_empty(list l) {
    return l == NULL || l->head == NULL;
}

report list_get_report(list l,int id) {
    if(list_is_empty(l)) return 0;

    struct list_node * curr = l->head;

    while(curr != NULL) {
        if(report_id(curr->item) == id) return curr->item;
        curr = curr->next;
    }

    return NULL; // The item is not in the list
}

int list_compare(list l1, list l2) {
    if(!l1 || !l2) return -1;
    if(l1->size != l2->size) return 0;

    struct list_node *c1 = l1->head;
    struct list_node *c2 = l2->head;

    while(c1 != NULL) {
        if (report_compare(c1->item, c2->item) == 0) {
            return 0; 
        }
        c1 = c1->next;
        c2 = c2->next;
    }

    return 1;
}

/* -------------------------------------------------------------------------
   QUERY & UTILITIES
   ------------------------------------------------------------------------- */

int list_add(list l, report r) {
    if(!l || !r) return 0; // Checks if wheter the list or the report exist

    if(list_get_report(l,report_id(r))) return 0; // The report is already in the list

    struct list_node *node = list_node_create();
    if(node == NULL) return 0;
    
    node->item = r;

    /* Inserting in head: O(1) */
    node->next = l->head;
    l->head = node;
    l->size++;

    return 1;
}

report list_pop_head(list l) {
    if(list_is_empty(l)) return NULL;

    struct list_node *temp = l->head;
    report r = temp->item;

    l->head = temp->next;
    l->size--;
    
    /* Deallocating the node but not the report */
    free(temp);

    return r;
}

/* -------------------------------------------------------------------------
   MOSTLY FOR TESTING 
   ------------------------------------------------------------------------- */

void list_reversed(list l) {
    // Safety check: if the list or the head is empty, nothing to reverse
    if (l == NULL || l->head == NULL) return;

    struct list_node *prev = NULL;    // Pointer to the previous node (starts at NULL)
    struct list_node *curr = l->head; // Pointer to the node we are currently processing
    struct list_node *next = NULL;    // Temporary pointer to store the next node

    while (curr != NULL) {
        // Store the next node (the rest of the list is not lost)
        next = curr->next; 
        
        // Reverse the link (make the current node point backwards to 'prev')
        curr->next = prev; 
        
        // Moves the pointers one step forward for the next iteration
        prev = curr;     // 'curr' ise 'prev' for the next node
        curr = next;     // Moves to next node for next iteration
    }

    // Update the list head to point to the new first node (which was the last) since in this case current now is NULL
    l->head = prev;
}

list list_get_filtered(list l, int cat, int stat) {
    if (l == NULL) return NULL;

    list final_l = list_create();
    if (final_l == NULL) return NULL;

    struct list_node *curr = l->head;
    struct list_node *tail = NULL; // Keeping a pointer to the last node

    while (curr != NULL) {
        int cat_match = (cat == -1 || (int)report_category(curr->item) == cat);
        int stat_match = (stat == -1 || (int)report_status(curr->item) == stat);

        if (cat_match && stat_match) {
            struct list_node *new_node = list_node_create();
            if (new_node == NULL) break; 

            // Cloning the report
            new_node->item = report_copy(curr->item);
            
            if (final_l->head == NULL) {
                // First element
                final_l->head = new_node;
            } else {
                // Adding other elements after the tail
                tail->next = new_node;
            }
            // Setting the tail to be the new node
            tail = new_node;
            final_l->size++;
        }
        // Moving onto next elment
        curr = curr->next;
    }
    
    return final_l;
}

void list_get_info_stats(list l, int *pending, int *in_prog, int *res, int *light, int *street, int *waste, int *fault) {
    if (l == NULL || l->head == NULL) return;

    struct list_node *curr = l->head;
    while (curr != NULL) {
        // Counting states
        status s = report_status(curr->item);
        if (s == PENDING) (*pending)++;
        else if (s == IN_PROGRESS) (*in_prog)++;
        else if (s == RESOLVED) (*res)++;

        // Counting categories
        category c = report_category(curr->item);
        if (c == LIGHTNING) (*light)++;
        else if (c == STREET) (*street)++;
        else if (c == WASTE) (*waste)++;
        else if (c == FAULT) (*fault)++;

        curr = curr->next;
    }
}

/* -------------------------------------------------------------------------
   OUTPUT
   ------------------------------------------------------------------------- */

void list_print_file(list l, FILE *stream) {
    if(list_is_empty(l)) {
        printf("The list is empty!");
        return;
    }

    struct list_node *curr = l->head;

    while(curr != NULL) {
        char *curr_record = report_file_string(curr->item);

        if(curr_record != NULL) {
            fprintf(stream,"%s\n",curr_record);
            free(curr_record); //To reset the string
        }

        curr = curr->next;
    }

}

void list_print_formatted(list l) {
    if(list_is_empty(l)) {
        printf("The list is empty!");
        return;
    }

    struct list_node *curr = l->head;

    while(curr != NULL) {
        report_formatted(curr->item);
        printf("\n");
        curr = curr->next;
    }
}

void list_print_filtered(list l, int cat, int stat) {
    if (list_is_empty(l)) {
        printf("Nessun report presente nel database.\n");
        return;
    }

    struct list_node *curr = l->head; 
    int found = 0;

    while (curr != NULL) {
        int match_cat = (cat == -1 || report_category(curr->item) == cat);
        int match_stat = (stat == -1 || report_status(curr->item) == stat);

        if (match_cat && match_stat) {
            report_formatted(curr->item);
            found++;
        }
        curr = curr->next;
    }

    if (found == 0) {
        printf("Nessun report corrisponde ai criteri di ricerca.\n");
    }
}