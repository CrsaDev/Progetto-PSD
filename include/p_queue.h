#ifndef PQUEUE_H
#define PQUEUE_H

#include "report.h"

/* Hidden type for list management */
typedef struct c_pQueue *pQueue;

/* --- Memory management --- */

/* Allocates a new empty priority queue. Returns NULL in case of failure */
pQueue pQueue_create(void);

/* Deallocates the whole priority queue, the nodes and the internal reports will stay intact in memory */
void pQueue_destroy(pQueue q);

/* --- Queries --- */

/* Returns 1 if queue doesn't have elements, 0 if yes */
int pQueue_is_empty(pQueue q);

/* Returns the max report, else NULL */
report pQueue_get_max(pQueue q);

/* Deletes the max report with 1 indicating success, 0 failure */
int pQueue_delete_max(pQueue q);

/* Inserts a report in the priority queue with 1 indicating succes, 0 failure */
int pQueue_insert(pQueue q, report r);


#endif