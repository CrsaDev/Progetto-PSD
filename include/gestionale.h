#ifndef GESTIONALE_H
#define GESTIONALE_H

#include "list.h"
#include "p_queue.h"

typedef struct c_gestionale *gestionale;

/* --- Memory Management --- */

/* Allocates a new gestionale. Returns NULL in case of fail */
gestionale gestionale_create();

/* Deallocates the gestionale, the internal list, the priority queue, all the nodes and the internal reports */
void gestionale_destroy(gestionale g);

/* --- Queries --- */

/* Inserts a report in the gestionale. Returns 1 if the operation was successful and 0 if not  */
int gestionale_add_report(gestionale g, report r);

/* --- Operations --- */

/* Prints out all the reports in the gestionale  */
void gestionale_view_reports(gestionale g, int cat, int stat);

/* Prints a specified report */
void gestionale_find_report(gestionale g, int report_id);

/* Updates the status of a report if it's not resolved */
void gestionale_update_report_status(gestionale g, int report_id, status new_status);

/* Shows the most urgent report */
void gestionale_view_urgent(gestionale g);

/* Prints out a final sheet with important report details */
void gestionale_view_final_report(gestionale g);



#endif
