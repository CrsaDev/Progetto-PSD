#include <stdio.h>
#include <stdlib.h>

#include "date.h"
#include "report.h"
#include "list.h"
#include "p_queue.h"

int main() {
    printf("--- Inizializzazione Sistema Gestione Report ---\n\n");

    date d1 = date_create(15, 10, 2023);
    date d2 = date_create(20, 10, 2023);
    date d3 = date_create(1, 11, 2023);

    report r1 = report_create(101, "Mario Rossi", STREET, "Buca profonda in Via Roma", d1, 32, PENDING);
    report r2 = report_create(102, "Luigi Verdi", LIGHTNING, "Lampione spento in Piazza", d2, 12, IN_PROGRESS);
    report r3 = report_create(103, "Giulia Bianchi", STREET, "Asfalto rovinato", d3, 1, PENDING);

    list my_list = list_create();
    pQueue my_queue = pQueue_create();
    
    printf("Aggiunta report al Database (Lista) e alla Coda di Priorità...\n");
    

    list_add(my_list, r1); pQueue_insert(my_queue, r1);
    list_add(my_list, r2); pQueue_insert(my_queue, r2);
    list_add(my_list, r3); pQueue_insert(my_queue, r3);

    printf("\n--- Tutti i Report (dal Database) ---\n");
    list_print_formatted(my_list);

    printf("\n--- Filtraggio: Solo problemi stradali (STREET) ---\n");
    list_print_by_category(my_list, STREET);


    printf("\n--- Estrazione dalla Priority Queue ---\n");
    
    report max_rep = pQueue_get_max(my_queue);
    printf("Report più urgente attuale: %s (Prio: %d, ID: %d)\n", 
           report_description(max_rep), report_priority(max_rep), report_id(max_rep));

    printf("\n--- Lazy Deletion ---\n");
    report rep_to_close = list_get_report(my_list, 101);
    if (rep_to_close) {
        report_set_status(rep_to_close, RESOLVED);
        printf("Stato del report 101 impostato a RESOLVED.\n");
    }
    max_rep = pQueue_get_max(my_queue);
    if (max_rep) {
        printf("NUOVO report più urgente: %s (Prio: %d, ID: %d)\n", 
               report_description(max_rep), report_priority(max_rep), report_id(max_rep));
    } else {
        printf("Nessun report urgente rimasto.\n");
    }

    printf("\nChiusura sistema e pulizia memoria...\n");

    max_rep = pQueue_get_max(my_queue);
    if (max_rep) {
        printf("NUOVO report più urgente: %s (Prio: %d, ID: %d)\n", 
               report_description(max_rep), report_priority(max_rep), report_id(max_rep));
    } else {
        printf("Nessun report urgente rimasto.\n");
    }

    pQueue_delete_max(my_queue);
    pQueue_delete_max(my_queue);
    max_rep = pQueue_get_max(my_queue);
    if (max_rep) {
        printf("NUOVO report più urgente: %s (Prio: %d, ID: %d)\n", 
               report_description(max_rep), report_priority(max_rep), report_id(max_rep));
    } else {
        printf("Nessun report urgente rimasto.\n");
    }
    
    pQueue_destroy(my_queue);
    list_destroy(my_list);

    printf("Operazione completata con successo.\n");
    return 0;
}