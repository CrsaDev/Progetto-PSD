#ifndef manager_H
#define manager_H

#include "dir_table.h"
#include "p_queue.h"

typedef struct c_manager *manager;

/* --- Memory Management --- */

/* Allocates a new manager. Returns NULL in case of fail */
manager manager_create();

/* Deallocates the manager, the internal list, the priority queue, all the nodes and the internal reports */
void manager_destroy(manager m);

/* --- Queries --- */

/* Returns the number of reports contained in the manager */
int manager_get_max_id(manager m);

/* Inserts a report in the manager. Returns 1 if the operation was successful and 0 if not  */
int manager_add_report(manager m, report r);

/* --- Operations --- */

/* Prints out all the reports in the manager  */
void manager_view_reports(manager m, int cat, int stat);

/* Prints a specified report */
void manager_find_report(manager m, int report_id);

/* Updates the status of a report if it's not resolved */
void manager_update_report_status(manager m, int report_id, status new_status);

/* Shows the most urgent report */
void manager_view_urgent(manager m);

/* Prints out a final sheet with important report details */
void manager_view_final_report(manager m);



#endif
