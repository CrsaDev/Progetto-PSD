#ifndef REPORT_H
#define REPORT_H

#include "date.h"

typedef enum {
    STREET,
    LIGHTNING,
    FAULT,
    WASTE
} category;

typedef enum {
    PENDING,
    IN_PROGRESS,
    RESOLVED,
    CLOSED
} status;

typedef enum {
    LOW,
    MEDIUM,
    HIGH,
    URGENT,
    CRITICAL
} priority;

/* Hidden type for report management */
typedef struct c_report *report;

/* --- Memory Management --- */

/* Allocates a new report. Returns NULL in case of fail */
report report_create(int id, char* citizen, category c, char* description, date d, priority p , status s);

/* Deallocates a given report */
void report_destroy(report r);

/* --- Getters --- */

/* Returns the id of a given report */
int report_id(report r);

/* Returns the citizen of a given report */
char* report_citizen(report r);

/* Returns the category of a given report */
int report_category(report r);

/* Returns the description of a given report */
char* report_description(report r);

/* Returns the date of a given report */
date report_date(report r);

/* Returns the priority of a given report */
int report_priority(report r);

/* Returns the status of a given report */
int report_status(report r);

/* --- Setters --- */

/* Updates the status of a given report */
void report_set_status(report r,status s);

/* --- Utils --- */
/* Creates a copy of the report giving a pointer to the new report - USEFUL FOR TESTING AND LIST CLONES */
report report_copy(report r);

/* Returns the id of a given report */
void report_formatted(report r);

/* Returns the report formatted string to write into a file */
char* report_file_string(report r);

/* --- Enums helpers --- */

char* category_to_string(category c);

char* status_to_string(status s);

char* priority_to_string(priority p);




#endif