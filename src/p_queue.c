#include "p_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INITIAL 50

typedef struct c_pQueue {
    report *queue;      
    int num_el;         
    int capacity;      
} c_pQueue;

/* -------------------------------------------------------------------------
   HELPER FUNCTIONS (STATIC)
   ------------------------------------------------------------------------- */

static void swap_reports(report *a, report *b) {
    report temp = *a;
    *a = *b;
    *b = temp;
}

// Useful for inserting new elements (Goes bottom->top)
static void up(pQueue q, int i) {
    // Base case: max heap height reached.
    if (i <= 1) return; 

    // Calculating the parent index to eventually go up.
    int parent = i / 2;

    // If the current node has higher priority than its parent.
    if (report_priority(q->queue[i]) > report_priority(q->queue[parent])) {
        // Swap the two nodes.
        swap_reports(&q->queue[i], &q->queue[parent]);

        // Recursion: We check if this node's parent can be swapped or not.
        up(q, parent);
    }

    // Important: if the function ends, it means the node found its place in the heap.
}

// Useful for element deletion (Goes top->bottom)
static void down(pQueue q, int i) {
    /* 
        Assumes the actual node is the largest, and calculate the child indexes.
    */
    int largest_node = i; 
    int left_index = 2 * i;
    int right_index = 2 * i + 1;

    // This is the last available index of the heap.
    int n = q->num_el; 

    /* 
        Checks if the left child exists via 'left_index <= n'.
        If it is bigger than the parent node, we update it as the new parent.
    */
    if (left_index <= n && report_priority(q->queue[left_index]) > report_priority(q->queue[largest_node])) {
        largest_node = left_index;
    }

    /* 
        Checks if the right child exists via 'right_index <= n'.
        If it is bigger than the parent node, we update it as the new parent.
    */
    if (right_index <= n && report_priority(q->queue[right_index]) > report_priority(q->queue[largest_node])) {
        largest_node = right_index;
    }

    // Base case: if parent node has the same index as the start, it means the heap is already ordered.
    if (largest_node != i) {
        // Swaps the original node, with the proper child.
        swap_reports(&q->queue[i], &q->queue[largest_node]);

        // The actual node might be able to go further down.
        down(q, largest_node);
    }
}

/* 
    Removes all the 'RESOLVED' reports that piled up on top of the heap
    Removes the parent each time it is not valid
*/
static void clean_resolved(pQueue q) {
    if (!q) return;

    // Keep looging as long as there are elements, and the top is "RESOLVED"
    while (q->num_el > 0 && report_status(q->queue[1]) == RESOLVED) {
        
        // Deleting the root
        q->queue[1] = q->queue[q->num_el];
        q->num_el--;
        
        // Re-heapify
        if (q->num_el > 0) {
            down(q, 1);
        }
    }
}

/* -------------------------------------------------------------------------
   MEMORY MANAGEMENT
   ------------------------------------------------------------------------- */

pQueue pQueue_create(void) {
    pQueue q = malloc(sizeof(struct c_pQueue));
    if (!q) return NULL;

    q->num_el = 0;
    q->capacity = INITIAL;
    
    // Capacity + 1, to use index 1 as the main index
    q->queue = malloc((q->capacity + 1) * sizeof(report));
    
    // If the array allocation fails, it fails the whole pQueue allocation.
    if (!q->queue) {
        free(q);
        return NULL;
    }
    return q;
}

void pQueue_destroy(pQueue q) {
    if (q) {
        // Only freeing the array and not the reports, as the ADT List acts as the main database.
        free(q->queue);
        free(q);
    }
}

/* -------------------------------------------------------------------------
   QUERIES
   ------------------------------------------------------------------------- */

int pQueue_is_empty(pQueue q) {
    if (!q) return 1;
    
    // Before returning if the heap is empty, we remove closed reports from it
    clean_resolved(q);
    
    return q->num_el == 0;
}

report pQueue_get_max(pQueue q) {
    if (pQueue_is_empty(q)) return NULL;
    
    return q->queue[1];
}

int pQueue_delete_max(pQueue q) {
    if (pQueue_is_empty(q)) return 0;

    // Replaces the heap root, with the last element and shortens the heap
    q->queue[1] = q->queue[q->num_el];
    q->num_el--;

    // Reorders the heap.
    down(q, 1);
    
    return 1;
}

int pQueue_insert(pQueue q, report r) {
    if (!q || !r) return 0;

    // Dinamically allocates new memory if the max capacity is reached, fails the operation if the reallocation fails
    if (q->num_el == q->capacity) {
        int new_capacity = q->capacity * 2;
        report *temp = realloc(q->queue, (new_capacity + 1) * sizeof(report));
        if (!temp) return 0; 
        
        q->queue = temp;
        q->capacity = new_capacity;
    }

    // Inserts the new element in the first available leaf node
    q->num_el++;
    q->queue[q->num_el] = r;

    // Reporders the heap.
    up(q, q->num_el); 
    
    return 1;
}
