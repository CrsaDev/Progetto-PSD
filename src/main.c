#include <stdio.h>
#include <stdlib.h>

#include "date.h"
#include "report.h"
#include "list.h"

int main() {
    printf("--- Inizializzazione Sistema Gestione Report ---\n\n");

    // 1. Creazione delle date
    date d1 = date_create(15, 10, 2023);
    date d2 = date_create(20, 10, 2023);
    date d3 = date_create(1, 11, 2023);

    // 2. Creazione dei report
    report r1 = report_create(101, "Mario Rossi", STREET, "Buca profonda in Via Roma", d1, HIGH, PENDING);
    report r2 = report_create(102, "Luigi Verdi", LIGHTNING, "Lampione spento in Piazza", d2, LOW, IN_PROGRESS);
    report r3 = report_create(103, "Giulia Bianchi", STREET, "Asfalto rovinato", d3, MEDIUM, PENDING);

    list my_list = list_create();
    
    printf("Aggiunta report alla lista...\n");
    list_add(my_list, r1);
    list_add(my_list, r2);
    list_add(my_list, r3);

    printf("\n--- Tutti i Report ---\n");
    list_print(my_list);

    printf("\n--- Filtraggio: Solo problemi stradali (STREET) ---\n");
    list_print_by_category(my_list, STREET);

    printf("\nChiusura sistema e pulizia memoria...\n");

    list_delete_id(my_list,102);
    
    list_destroy(my_list);



    printf("Operazione completata con successo.\n");
    return 0;
}