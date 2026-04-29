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
        report_destroy(curr->item);
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
        if(report_id(c1->item) != report_id(c2->item)) return 0;
        c1 = c1->next;
        c2 = c2->next;
    }
    return 1;
}

/* 
    Returns the count of the reports of a specified field.
    -'c': Category
    -'s': Status
    -'p': Priority
*/
int list_report_field_count(list l, char field, int value) {
    if (list_is_empty(l)) return 0;

    int count = 0;
    struct list_node *curr = l->head;

    while (curr != NULL) {
        switch (field) {
            case 'c':
                if ((int)report_category(curr->item) == value) count++;
                break;
            case 's':
                if ((int)report_status(curr->item) == value) count++;
                break;
            case 'p':
                if ((int)report_priority(curr->item) == value) count++;
                break;
            default:
                return -1; // Invalid field
        }
        curr = curr->next;
    }
    return count;
}

/* Only for testing purposes */
list list_reversed(list l) {
    if (l == NULL) return NULL;

    /* Creates a new list, and copies all the data in the list traversing the main one in order to reverse it. */

    list reversed_l = list_create();
    if (reversed_l == NULL) return NULL; 

    struct list_node *curr = l->head;

    while (curr != NULL) {
        report cloned_report = report_copy(curr->item);
        
        if (cloned_report != NULL) {
            list_add(reversed_l, cloned_report); 
        }
        
        curr = curr->next;
    }
    
    return reversed_l;
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

int list_delete_id(list l, int id) {
    if(!l) return 0;

    struct list_node *curr = l->head;
    struct list_node *prev = NULL;

    /* Searching for the node to delete keeping the previous node: O(n) */
    while(curr != NULL && report_id(curr->item) != id) {
        prev = curr;
        curr = curr->next;
    }

    if(curr == NULL) return 0; // Not found

    /* Removing in head case */
    if(prev == NULL) {
        l->head = curr->next;
    } else {
        prev->next = curr->next;
    }

    report_destroy(curr->item); // To avoid wasting memory
    free(curr);
    l->size--;

    return 1;
}

report list_remove_head(list l) {
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
   OUTPUT
   ------------------------------------------------------------------------- */

void list_print_by_category(list l, category c) {
    if(list_is_empty(l)) {
        printf("The list is empty!");
        return;
    }

    if(c<0 || c>4) {
        printf("The selected category is not valid");
        return;
    }

    struct list_node *curr = l->head;

    while(curr != NULL) {

        if(report_category(curr->item) == c) {
            report_formatted(curr->item);
            printf("\n");
        }

        curr = curr->next;
    }

}

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

    struct list_node *current = l->head; 
    int found = 0;

    while (current != NULL) {
        int match_cat = (cat == -1 || report_category(current->item) == cat);
        int match_stat = (stat == -1 || report_status(current->item) == stat);

        if (match_cat && match_stat) {
            report_formatted(current->item);
            found++;
        }
        current = current->next;
    }

    if (found == 0) {
        printf("Nessun report corrisponde ai criteri di ricerca.\n");
    }
}