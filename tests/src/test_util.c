#include "test_util.h"

void finput_list(char *fname, list l, int hop) {
    FILE *f = fopen(fname, "r");
    if (f == NULL) {
        printf("Attenzione: Impossibile aprire il file %s\n", fname);
        return;
    }

    for(int i = 0; i < hop; i++) {
        fscanf(f, "%*[^\n]"); 
        fgetc(f);             
    }

    int id, day, month, year, cat, prio, stat;
    char citizen[50], desc[100];

    while (fscanf(f, "%d %s %d %s %d/%d/%d %d %d", 
                  &id, citizen, &cat, desc, &day, &month, &year, &prio, &stat) == 9) {
        
        date dt = date_create(day, month, year);
        report r = report_create(id, citizen, (category)cat, desc, dt, (priority)prio, (status)stat);
        
        if (r != NULL) {
            list_add(l, r);
        }
    }
    fclose(f);
}

void foutput_list(char *fname, list l) {
    FILE *f = fopen(fname, "w");
    if (f == NULL) {
        printf("Attenzione: Impossibile creare il file %s\n", fname);
        return;
    }

    if (!list_is_empty(l)) {
        list_print_file(l, f); 
    }

    fclose(f);
}