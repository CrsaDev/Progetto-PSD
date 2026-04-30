#include "test_util.h"
#include <stdio.h>

FILE* fopen_at_line(const char *fname, int hop) {
    FILE *f = fopen(fname, "r");
    if (f == NULL) {
        printf("Attenzione: Impossibile aprire il file %s\n", fname);
        return NULL;
    }

    // Jumps n lines
    for (int i = 0; i < hop; i++) {
        if (fscanf(f, "%*[^\n]") == EOF) { // Loops until end of file, for \n
            fclose(f); 
            return NULL;
        }
        fgetc(f); // Consumes \n
    }

    return f; // Returns pointsr to the file at proper position
}


list finput_list(char *fname, int hop) {
    FILE *f = fopen_at_line(fname, hop);
    if (f == NULL) return NULL;

    list tmp = list_create();
    if (tmp == NULL) { fclose(f); return NULL; }
    
    int id, day, month, year, cat, prio, stat;
    char citizen[50], desc[100];
    
    // Reading buffer for line
    char line[256];

    // Adding elements to head
    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "%d %49s %d %99s %d/%d/%d %d %d", 
                   &id, citizen, &cat, desc, &day, &month, &year, &prio, &stat) == 9) {
            
            date dt = date_create(day, month, year);
            report r = report_create(id, citizen, (category)cat, desc, dt, prio, (status)stat);
            
            if (r != NULL) {
                // Adds to head
                list_add(tmp, r);
            }
        }
    }

    fclose(f);

    // Re-reversing in place so the list is in the correct order again.
    list_reversed(tmp); 
    
    return tmp;
}

int foutput_list(char *fname, list l) {
    FILE *f = fopen(fname, "w");
    if (f == NULL) {
        printf("Attenzione: Impossibile creare il file %s\n", fname);
        return 0;
    }

    // Printing list to the file
    if (!list_is_empty(l)) {
        list_print_file(l, f); 
    }

    fclose(f);
    return 1;
}

void get_test_filenames(const char *testcase_id, int n, char *in, char *out, char *oracle) {
    // Creates strings with specified formats.
    sprintf(in, "tests/testcases/%s/inputs/%s%d_input.txt", testcase_id, testcase_id, n);
    sprintf(out, "tests/testcases/%s/outputs/%s%d_output.txt", testcase_id, testcase_id, n);
    sprintf(oracle, "tests/testcases/%s/oracles/%s%d_oracle.txt", testcase_id, testcase_id, n);
}

int read_int(const char *fname, int hop) {
    int val = -1;
    FILE *f = fopen(fname, "r");
    if (f == NULL) {
        printf("Attenzione: Impossibile aprire il file %s\n", fname);
        return -1;
    }

    // Skipping hop lines
    for(int i = 0; i < hop; i++) {
        fscanf(f, "%*[^\n]"); 
        fgetc(f);             
    }

    fscanf(f,"%d",&val);
    fclose(f);

    return val;
}