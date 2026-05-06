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
    int prio;
    status stat;
};

/* -------------------------------------------------------------------------
   MEMORY MANAGEMENT
   ------------------------------------------------------------------------- */

report report_create(int id, char* citizen, category c, char* description, date d, int p , status s) {
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

/* -------------------------------------------------------------------------
   GETTERS
   ------------------------------------------------------------------------- */

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

report report_copy(report r) {
    if (r == NULL) return NULL;

    date original_date = report_date(r);
    date new_date = date_create(
        date_day(original_date), 
        date_month(original_date), 
        date_year(original_date)
    );

    report copy = report_create(
        report_id(r),
        report_citizen(r), 
        report_category(r),
        report_description(r), 
        new_date,
        report_priority(r),
        report_status(r)
    );

    if (copy == NULL) {
        date_destroy(new_date);
        return NULL;
    }

    return copy;
}

report report_input(int last_index) {
    int prio, cat_int, stat_int;
    char citizen[50], desc[100];
    int dd, mm, yy;

    printf("\n--- Inserimento Nuovo Report ---\n");
    
    // Generazione automatica dell'ID basata sull'ultimo indice
    int id = last_index + 1;
    printf("Report ID (Automatically generated): %d\n", id);
    
    printf("Citizen name: "); 
    scanf(" %[^\n]s", citizen); 

    printf("Category (0:STRADE, 1:ILLUMINAZIONE, 2:RIFIUTI, 3:MALFUNZIONAMENTI): ");
    if (scanf("%d", &cat_int) != 1 || cat_int < 0 || cat_int > 3) {
        printf("\nError: Invalid category. Must be between 0 and 3.\n");
        printf("Insert failed.\n");
        return NULL;
    }

    printf("Descrizione: "); 
    scanf(" %[^\n]s", desc);

    printf("Data (formato GG MM AAAA): ");
    if (scanf("%d %d %d", &dd, &mm, &yy) != 3) {
        printf("\nErrore: Formato data non valido.\n");
        printf("Inserimento annullato.\n");
        return NULL;
    }

    date new_date = date_create(dd, mm, yy);
    if (!new_date) {
        printf("\nErrore: La data inserita (%02d/%02d/%04d) non esiste o non e' valida!\n", dd, mm, yy);
        printf("Inserimento annullato.\n");
        return NULL; 
    }

    printf("Priorita' (Valore numerico maggiore di 0): ");
    if (scanf("%d", &prio) != 1 || prio <= 0) {
        printf("\nErrore: La priorita' deve essere un numero intero maggiore di 0.\n");
        printf("Inserimento annullato.\n");
        date_destroy(new_date); // Prevents memory leaks
        return NULL;
    }

    printf("Stato (0:PENDING, 1:IN_PROGRESS, 2:RESOLVED): ");
    if (scanf("%d", &stat_int) != 1 || stat_int < 0 || stat_int > 2) {
        printf("\nErrore: Stato non valido. Deve essere un numero tra 0 e 2.\n");
        printf("Inserimento annullato.\n");
        date_destroy(new_date);
        return NULL;
    }

    // Returning the report
    return report_create(id, citizen, (category)cat_int, desc, new_date, prio, (status)stat_int);
}

void report_formatted(report r) {
    if (!r) return;

    // Used to allign text
    printf("%-4d | %-20s | %-18s | %-25s | ", 
           r->id, 
           r->citizen,
           category_to_string(r->cat), 
           r->description);

    date_print(r->d);  
    
    printf(" | %-12s | Priority: %-3d\n", 
           status_to_string(r->stat), 
           r->prio);
}

char* report_file_string(report r) {
    if (!r) return NULL;
    
    // Checks the size to allocate for the string
    int record_size = snprintf(NULL, 0, "%d;%s;%d;%s;%02d/%02d/%d;%d;%d", 
        r->id, r->citizen, r->cat, r->description,
        date_day(r->d), date_month(r->d), date_year(r->d),
        r->prio, r->stat
    );

    if(record_size < 0) return NULL;

    // Allocates the string
    char *s = malloc((record_size + 1) * sizeof(char));
    if(!s) return NULL;

    // Writes into the string with the delimiters
    sprintf(s, "%d;%s;%d;%s;%02d/%02d/%d;%d;%d", 
        r->id, r->citizen, r->cat, r->description,
        date_day(r->d), date_month(r->d), date_year(r->d),
        r->prio, r->stat
    );

    return s;
}

int report_compare(report r1, report r2) {
    if (r1 == r2) return 1; 
    
    if (!r1 || !r2) return 0; 

    if (r1->id != r2->id) return 0;
    if (r1->prio != r2->prio) return 0;
    if (r1->stat != r2->stat) return 0;
    if (r1->cat != r2->cat) return 0;

    if (strcmp(r1->citizen, r2->citizen) != 0) return 0;
    if (strcmp(r1->description, r2->description) != 0) return 0;

    if (date_to_long(r1->d) != date_to_long(r2->d)) return 0;

    return 1;
}

/* -------------------------------------------------------------------------
   ENUM HELPERS
   ------------------------------------------------------------------------- */

char* status_to_string(status s) {
    switch(s) {
        case PENDING: return "Pending";
        case IN_PROGRESS: return "In Progress";
        case RESOLVED: return "Resolved";
        default: return "Unknown";
    }
}

char* category_to_string(category c) {
    switch(c) {
        case LIGHTNING: return "Illuminazione";
        case STREET: return "Strade/Buche";
        case WASTE: return "Rifiuti";
        case FAULT: return "Guasti generici";
        default: return "Unknown";
    }
}
