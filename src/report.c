#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "report.h"

struct c_report{
    int id;
    char citizen[50];
    category cat;
    char description[100];
    date d;
    priority prio;
    status stat;
};

/* -------------------------------------------------------------------------
   MEMORY MANAGEMENT
   ------------------------------------------------------------------------- */

report report_create(int id, char* citizen, category c, char* description, date d, priority p , status s) {
    report r = malloc(sizeof(*r));

    // Destroys the date to prevent the date from wasting memory since date is specifically related to the report
    if(r == NULL) {
        date_destroy(d);
        return NULL;
    };

    r->id = id;
    r->cat = c;
    r->stat = s;
    r->prio = p;
    r->d = d;

    strncpy(r->citizen,citizen,sizeof(r->citizen)-1);
    r->citizen[sizeof(r->citizen)-1] = '\0';

    strncpy(r->description,description,sizeof(r->description)-1);
    r->description[sizeof(r->description)-1] = '\0';


    return r;
}

void report_destroy(report r) {
    if(r) {
        date_destroy(r->d); // Destroys the date since each report will have a separate date
        free(r);
    }
}

/* --- Getters --- */

int report_id(report r) {
    return (r) ? r->id : -1;
}

char* report_citizen(report r) {
    return (r) ? r->citizen : NULL;
}

int report_category(report r) {
    return (r) ? r->cat : -1;
}

char* report_description(report r) {
    return (r) ? r->description : NULL;
}

date report_date(report r) {
    return (r) ? r->d : NULL;
}

int report_priority(report r) {
    return (r) ? r->prio : -1;
}

int report_status(report r) {
    return (r) ? r->stat : -1;
}

/* -------------------------------------------------------------------------
   SETTERS
   ------------------------------------------------------------------------- */

void report_set_status(report r,status s) {
    if(!r)  return;
    r->stat = s;
}

/* -------------------------------------------------------------------------
   OUTPUT & UTILITIES
   ------------------------------------------------------------------------- */

void report_formatted(report r) {
    if (!r) return;

    printf("%d\t%s\t%s\t%s\t", r->id, r->citizen,
           category_to_string(r->cat), r->description);

    date_print(r->d);  // prints in DD/MM/YYYY format
    printf("\t%s\t%s\n", status_to_string(r->stat), priority_to_string(r->prio));
}

char* report_file_string(report r) {
    if (!r) return NULL;
    
    // Checks the size to allocate for the string
    int record_size = snprintf(NULL,0, "%d %s %d %s %02d/%02d/%d %d %d", 
        r->id, r->citizen,r->cat, r->description,
        date_day(r->d), date_month(r->d), date_year(r->d),
        r->prio, r->stat
    );

    if(record_size < 0) return NULL;

    // Allocates the string
    char *s = malloc((record_size+1) * sizeof(char));
    if(!s) return NULL;
    sprintf(s, "%d %s %d %s %02d/%02d/%d %d %d", 
        r->id, r->citizen, r->cat, r->description,
        date_day(r->d), date_month(r->d), date_year(r->d),
        r->prio, r->stat
    );

    return s;
}

/* -------------------------------------------------------------------------
   ENUM HELPERS
   ------------------------------------------------------------------------- */

char* status_to_string(status s) {
    switch(s) {
        case PENDING: return "Pending";
        case IN_PROGRESS: return "In Progress";
        case RESOLVED: return "Resolved";
        case CLOSED: return "Closed";
        default: return "Unknown";
    }
}

char* category_to_string(category c) {
    switch(c) {
        case LIGHTNING: return "Streetlight Issue";
        case STREET: return "Pothole";
        case WASTE: return "Illegal Waste";
        case FAULT: return "Public Facility Fault";
        default: return "Unknown";
    }
}

char* priority_to_string(priority p) {
    switch(p) {
        case LOW: return "Low";
        case MEDIUM: return "Medium";
        case HIGH: return "High";
        case URGENT: return "Urgent";
        case CRITICAL: return "Critical";
        default: return "Unknown";
    }
}
