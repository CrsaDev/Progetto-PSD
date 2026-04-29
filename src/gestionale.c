#include "gestionale.h"
#include <stdlib.h>
#include <string.h>

struct c_gestionale {
    list gestionale_list;
    pQueue gestionale_pQueue;
};

/* -------------------------------------------------------------------------
   MEMORY MANAGEMENT
   ------------------------------------------------------------------------- */

gestionale gestionale_create() {
    gestionale g = malloc(sizeof(*g));
    if(!g) return NULL;

    g->gestionale_list = list_create();
    g->gestionale_pQueue = pQueue_create();

    // If either the queue or the list fail the allocation, the gestionale fails its allocation too.
    if (!g->gestionale_list || !g->gestionale_pQueue) {
        if (g->gestionale_list) list_destroy(g->gestionale_list);
        if (g->gestionale_pQueue) pQueue_destroy(g->gestionale_pQueue);
        free(g);
        return NULL;
    }

    return g;
}

void gestionale_destroy(gestionale g) {
    if (g) {
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
            printf("Errore: Il report #%d è già chiuso (RESOLVED) e non può essere riaperto o modificato.\n", report_id);
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

    report r = list_get_report(g->gestionale_list, report_id);
    if (r) {
        printf("Trovato: ");
        report_formatted(r); 
    } else {
        printf("Report con ID %d inesistente.\n", report_id);
    }
}
/* --- Queries --- */

void gestionale_view_urgent(gestionale g) {
    if (!g) return;

    report urgent = pQueue_get_max(g->gestionale_pQueue);
    
    if (urgent) {
        printf("=== SEGNALAZIONE PIÙ URGENTE ===\n");
        report_formatted(urgent);
    } else {
        printf("Nessuna segnalazione urgente in sospeso.\n");
    }
}

void gestionale_view_reports(gestionale g, int cat, int stat) {
    if (!g) return;
    
    printf("\n--- ELENCO SEGNALAZIONI FILTRATE ---\n");
    list_print_filtered(g->gestionale_list, cat, stat);
}

void gestionale_view_final_report(gestionale g) {
    if (!g) return;

    printf("\n========================================\n");
    printf("        DASHBOARD STATISTICHE           \n");
    printf("========================================\n");

    // We get the count for each category
    int pending = list_report_field_count(g->gestionale_list, 's', PENDING);
    int in_progress = list_report_field_count(g->gestionale_list, 's', IN_PROGRESS);
    int resolved = list_report_field_count(g->gestionale_list, 's', RESOLVED);
    int total = pending + in_progress + resolved;

    printf("--- STATO SEGNALAZIONI ---\n");
    printf("Totale Segnalazioni : %d\n", total);
    printf("In attesa (PENDING) : %d\n", pending);
    printf("In lavorazione      : %d\n", in_progress);
    printf("Chiuse (RESOLVED)   : %d\n\n", resolved);

    printf("--- RIPARTIZIONE PER CATEGORIA ---\n");
    printf("Lampioni (LIGHTNING): %d\n", list_report_field_count(g->gestionale_list, 'c', LIGHTNING));
    printf("Strade (STREET)     : %d\n", list_report_field_count(g->gestionale_list, 'c', STREET));
    printf("Rifiuti (WASTE)     : %d\n", list_report_field_count(g->gestionale_list, 'c', WASTE));
    printf("Guasti (FAULT)      : %d\n", list_report_field_count(g->gestionale_list, 'c', FAULT));
    
    printf("========================================\n\n");
}