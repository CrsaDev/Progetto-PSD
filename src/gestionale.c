#include "gestionale.h"
#include "test_util.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>

struct c_gestionale {
    list gestionale_list;
    pQueue gestionale_pQueue;
};

int gestionale_load_database(char *fname, gestionale g) {
    if (g == NULL) return 0;

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
                list_add(g->gestionale_list, r);
                // Loading in the pQueue only if not resolved
                if ((status)stat != RESOLVED) {
                    pQueue_insert(g->gestionale_pQueue, r);
                }
            }
        }
    }

    fclose(f);
    list_reversed(g->gestionale_list); 
    return 1;
}

int gestionale_save_database(char *fname, gestionale g) {
    if (g == NULL || g->gestionale_list == NULL) return 0;
    
    int success = foutput_list(fname, g->gestionale_list);
    
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

gestionale gestionale_create() {
    gestionale g = malloc(sizeof(*g));
    if(!g) return NULL;

    g->gestionale_list = list_create();
    g->gestionale_pQueue = pQueue_create();

    // Checks if they are both successfully allocated
    if (!g->gestionale_list || !g->gestionale_pQueue) {
        if (g->gestionale_list) list_destroy(g->gestionale_list);
        if (g->gestionale_pQueue) pQueue_destroy(g->gestionale_pQueue);
        free(g);
        return NULL;
    }

    // Loads up from the database
    clock_t start = clock();
    gestionale_load_database("../db.txt", g);
    clock_t end = clock();
    double tempo_impiegato = (double)(end - start) / CLOCKS_PER_SEC;
    printf("In: %.6f s\n", tempo_impiegato);


    return g;
}

void gestionale_destroy(gestionale g) {
    if (g) {
        // Saves everything to the database.
        gestionale_save_database("../db.txt",g);
        list_destroy(g->gestionale_list);
        pQueue_destroy(g->gestionale_pQueue);
        free(g);
    }
}

/* -------------------------------------------------------------------------
   OPERATIONS
   ------------------------------------------------------------------------- */

int gestionale_add_report(gestionale g, report r) {
    if (!g || !r) return 0;

    if (list_get_report(g->gestionale_list, report_id(r)) != NULL) {
        printf("Errore: Esiste gia' una segnalazione con ID %d!\n", report_id(r));
        return 0; 
    }

    int success_list = list_add(g->gestionale_list, r);
    int success_queue = 1; 
    
    if (report_status(r) != RESOLVED) {
        success_queue = pQueue_insert(g->gestionale_pQueue, r);
    }

    return (success_list && success_queue);
}

void gestionale_update_report_status(gestionale g, int report_id, status new_status) {
    if (!g) return;

    report r = list_get_report(g->gestionale_list, report_id);
    
    if (r) {
        if (report_status(r) == RESOLVED) {
            printf("Errore: Il report #%d risulta chiuso (RESOLVED) e non puo' essere riaperto o modificato.\n", report_id);
            return;
        }

        report_set_status(r, new_status);
        printf("Successo: Stato del report #%d aggiornato.\n", report_id);
        
    } else {
        printf("Errore: Report #%d non trovato.\n", report_id);
    }
}

/* -------------------------------------------------------------------------
   QUERIES
   ------------------------------------------------------------------------- */

void gestionale_find_report(gestionale g, int report_id) {
    if (!g) return;

    // Clock for mesuring time to find the report
    clock_t start = clock();
    report r = list_get_report(g->gestionale_list, report_id);
    clock_t end = clock();
    double tempo_impiegato = (double)(end - start) / CLOCKS_PER_SEC;
    if (r) {
        printf("Trovato: ");
        report_formatted(r); 
    } else {
        printf("Report con ID %d inesistente.\n", report_id);
    }
    printf("In: %.6fs\n", tempo_impiegato);
}
/* --- Queries --- */

void gestionale_view_urgent(gestionale g) {
    if (!g) return;

    report urgent = pQueue_get_max(g->gestionale_pQueue);
    
    if (urgent) {
        printf("=== SEGNALAZIONE PIU' URGENTE ===\n");
        report_formatted(urgent);
    } else {
        printf("Nessuna segnalazione urgente in sospeso.\n");
    }
}

void gestionale_view_reports(gestionale g, int cat, int stat) {
    if (!g) return;

    printf("\n--- ELENCO SEGNALAZIONI FILTRATE ---\n");
    
    list filtered = list_get_filtered(g->gestionale_list, cat, stat);
    
    if (list_is_empty(filtered)) {
        printf("Nessun report corrisponde ai criteri di ricerca.\n");
    } else {
        list_print_formatted(filtered);
    }
    
    list_destroy(filtered);
}

void gestionale_view_final_report(gestionale g) {
    if (!g || !g->gestionale_list) return;

    // Local variables to store counts
    int pending = 0, in_progress = 0, resolved = 0;
    int lightning = 0, street = 0, waste = 0, fault = 0;

    // Gathering the stats
    list_get_info_stats(g->gestionale_list, 
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