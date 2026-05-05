#include "manager.h"
#include "test_util.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>

struct c_manager {
    dynArr manager_list;
    pQueue manager_pQueue;
};

int manager_load_database(char *fname, manager m) {
    if (m == NULL) return 0;

    FILE *f = fopen(fname, "r");
    if (f == NULL) return 1; // DB file does not exist, not an error.

    int id, day, month, year, cat, prio, stat;
    char citizen[50], desc[100];
    char line[256];

    // fgets + sscanf for line reading
    while (fgets(line, sizeof(line), f)) {
        // Reading the report
        if (sscanf(line, "%d %49s %d %99s %d/%d/%d %d %d", 
                   &id, citizen, &cat, desc, &day, &month, &year, &prio, &stat) == 9) {
            
            date dt = date_create(day, month, year);
            report r = report_create(id, citizen, (category)cat, desc, dt, prio, (status)stat);
            
            if (r != NULL) {
                dynArr_add(m->manager_list, r);
                // Loading in the pQueue only if not resolved
                if ((status)stat != RESOLVED) {
                    pQueue_insert(m->manager_pQueue, r);
                }
            }
        }
    }

    fclose(f);
    printf("\n %d Segnalazioni caricate",dynArr_get_size(m->manager_list));
    return 1;
}

int manager_save_database(char *fname, manager m) {
    if (m == NULL || m->manager_list == NULL) return 0;
    
    int success = foutput_list(fname, m->manager_list);
    
    if (success) {
        printf("Database salvato con successo su %s\n", fname);
    } else {
        printf("Errore durante il salvataggio del database!\n");
    }

    return success;
}


/* -------------------------------------------------------------------------
   MEMORY MANAGEMENT
   ------------------------------------------------------------------------- */

manager manager_create() {
    manager m = malloc(sizeof(*m));
    if(!m) return NULL;

    m->manager_list = dynArr_create();
    m->manager_pQueue = pQueue_create();

    // Checks if they are both successfully allocated
    if (!m->manager_list || !m->manager_pQueue) {
        if (m->manager_list) dynArr_destroy(m->manager_list);
        if (m->manager_pQueue) pQueue_destroy(m->manager_pQueue);
        free(m);
        return NULL;
    }

    // Loads up from the database
    clock_t start = clock();
    manager_load_database("../db.txt", m);
    clock_t end = clock();
    double tempo_impiegato = (double)(end - start) / CLOCKS_PER_SEC;
    printf(", in:%.6fs\n", tempo_impiegato);


    return m;
}

void manager_destroy(manager m) {
    if (m) {
        // Saves everything to the database.
        manager_save_database("../db.txt",m);
        dynArr_destroy(m->manager_list);
        pQueue_destroy(m->manager_pQueue);
        free(m);
    }
}

/* -------------------------------------------------------------------------
   OPERATIONS
   ------------------------------------------------------------------------- */

int manager_get_size(manager m) {
    return dynArr_get_size(m->manager_list);
}

int manager_add_report(manager m, report r) {
    if (!m || !r) return 0;

    if (dynArr_get_report(m->manager_list, report_id(r)) != NULL) {
        printf("Error: A report with ID: %d already exists!\n", report_id(r));
        return 0; 
    }

    if (!dynArr_add(m->manager_list, r)) {
        return 0;
    }

    if (report_status(r) != RESOLVED) {
        if (!pQueue_insert(m->manager_pQueue, r)) {
            printf("Error: Failed to insert report %d into the priority queue.\n", report_id(r));
            return 0;
        }
    }

    manager_save_database("../db.txt",m);
    return 1;
}

void manager_update_report_status(manager m, int report_id, status new_status) {
    if (!m) return;

    report r = dynArr_get_report(m->manager_list, report_id);
    
    if (r) {
        if (report_status(r) == RESOLVED) {
            printf("Error: Report #%d is RESOLVED and cannot be modified.\n", report_id);
            return;
        }

        report_set_status(r, new_status);
        manager_save_database("../db.txt",m);
        printf("Success: Report #%d updated.\n", report_id);
        
    } else {
        printf("Error: Report #%d not found.\n", report_id);
    }
}

/* -------------------------------------------------------------------------
   QUERIES
   ------------------------------------------------------------------------- */

void manager_find_report(manager m, int report_id) {
    if (!m) return;
    if(report_id <= 0 || report_id > dynArr_get_size(m->manager_list)) {
        printf("Report con ID %d inesistente.\n", report_id);
        return;
    }

    // Clock for mesuring time to find the report
    clock_t start = clock();
    report r = dynArr_get_report(m->manager_list, report_id);
    clock_t end = clock();
    double tempo_impiegato = (double)(end - start) / CLOCKS_PER_SEC;

    if (r) {
        printf("Report: ");
        report_formatted(r); 
    } else {
        printf("Error: Reprot #%d not found.\n", report_id);
    }
    printf("Found in: %.6fs\n", tempo_impiegato);
}
/* --- Queries --- */

void manager_view_urgent(manager m) {
    if (!m) return;

    report urgent = pQueue_get_max(m->manager_pQueue);
    
    if (urgent) {
        printf("=== SEGNALAZIONE PIU' URGENTE ===\n");
        report_formatted(urgent);
    } else {
        printf("No urgent reports in queue.\n");
    }
}

void manager_view_reports(manager m, int cat, int stat) {
    if (!m) return;

    printf("\n--- ELENCO SEGNALAZIONI FILTRATE ---\n");
    
    dynArr filtered = dynArr_get_filtered(m->manager_list, cat, stat);
    
    if (dynArr_is_empty(filtered)) {
        printf("No report was found.\n");
    } else {
        dynArr_print_formatted(filtered);
    }
    
    dynArr_destroy(filtered);
}

void manager_view_final_report(manager m) {
    if (!m || !m->manager_list) return;

    // Local variables to store counts
    int pending = 0, in_progress = 0, resolved = 0;
    int lightning = 0, street = 0, waste = 0, fault = 0;

    // Gathering the stats
    dynArr_get_info_stats(m->manager_list, 
                        &pending, &in_progress, &resolved, 
                        &lightning, &street, &waste, &fault);

    int total = pending + in_progress + resolved;

    // Most frequent category
    int max_cat = lightning;
    const char* freq_cat_name = "Lampioni (LIGHTNING)";
    
    if (street > max_cat) {
        max_cat = street;
        freq_cat_name = "Strade (STREET)";
    }
    if (waste > max_cat) {
        max_cat = waste;
        freq_cat_name = "Rifiuti (WASTE)";
    }
    if (fault > max_cat) {
        max_cat = fault;
        freq_cat_name = "Guasti (FAULT)";
    }

    // --- SHOWING DASHBOARD ---
    printf("\n========================================\n");
    printf("         DASHBOARD STATISTICHE          \n");
    printf("========================================\n");

    printf("--- STATO SEGNALAZIONI ---\n");
    printf("Totale Segnalazioni : %d\n", total);
    printf("In attesa (PENDING) : %d\n", pending);
    printf("In lavorazione      : %d\n", in_progress);
    printf("Chiuse (RESOLVED)   : %d\n\n", resolved);

    printf("--- VISUALIZZAZIONE PER CATEGORIA ---\n");
    printf("Lampioni (LIGHTNING): %d\n", lightning);
    printf("Strade (STREET)     : %d\n", street);
    printf("Rifiuti (WASTE)     : %d\n", waste);
    printf("Guasti (FAULT)      : %d\n\n", fault);
    
    printf("--- TIPOLOGIA PIU' FREQUENTE ---\n");
    if (total > 0) {
        printf("%s con %d segnalazioni\n", freq_cat_name, max_cat);
    } else {
        printf("Nessuna segnalazione registrata.\n");
    }
    
    printf("========================================\n\n");
}