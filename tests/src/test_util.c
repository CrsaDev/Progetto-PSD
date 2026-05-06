#include "test_util.h"
#include <stdio.h>

#include <stdio.h>
#include <stdlib.h>

FILE* fopen_at_line(const char *fname, int hop) {
    FILE *f = fopen(fname, "r");
    if (!f) {
        printf("Attenzione: Impossibile aprire il file %s\n", fname);
        return NULL;
    }

    char buffer[256]; // Temp buffer for lines
    for (int i = 0; i < hop; i++) {
        // check if NULL isreturned before hops
        if (fgets(buffer, sizeof(buffer), f) == NULL) {
            fclose(f);
            return NULL;
        }
    }

    return f; 
}

dirTable finput_dirTable(char *fname, int hop) {
    FILE *f = fopen_at_line(fname, hop);
    if (f == NULL) return NULL;

    dirTable tmp = dirTable_create();
    if (tmp == NULL) { 
        fclose(f); 
        return NULL; 
    }
    
    int id, day, month, year, cat, prio, stat;
    char citizen[50], desc[100];
    char line[256];

    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "%d;%49[^;];%d;%99[^;];%d/%d/%d;%d;%d", 
                   &id, citizen, &cat, desc, &day, &month, &year, &prio, &stat) == 9) {
            
            date dt = date_create(day, month, year);
            if (dt == NULL) continue; // If date isn't valid, skip

            report r = report_create(id, citizen, (category)cat, desc, dt, prio, (status)stat);
            
            if (r != NULL) {
                // Tenta l'inserimento
                if (!dirTable_add(tmp, r)) {
                    report_destroy(r); // Aviods memory leaks
                    continue; // Skips to next line
                }
            } else {
                date_destroy(dt); 
            }
        }
    }

    fclose(f);
    return tmp;
}

int foutput_list(char *fname, dirTable l) {
    FILE *f = fopen(fname, "w");
    if (f == NULL) {
        printf("Attenzione: Impossibile creare il file %s\n", fname);
        return 0;
    }

    // Printing list to the file
    if (!dirTable_is_empty(l)) {
        dirTable_print_file(l, f); 
    }

    fclose(f);
    return 1;
}

void get_test_filenames(const char *testcase_id, int n, char *in, char *out, char *oracle) {
    // Creates strings with specified formats.
    sprintf(in, "../tests/testcases/%s/inputs/%s%d_input.txt", testcase_id, testcase_id, n);
    sprintf(out, "../tests/testcases/%s/outputs/%s%d_output.txt", testcase_id, testcase_id, n);
    sprintf(oracle, "../tests/testcases/%s/oracles/%s%d_oracle.txt", testcase_id, testcase_id, n);
}

int read_int(const char *fname, int hop) {
    int val = -1;
    FILE *f = fopen(fname, "r");
    if (f == NULL) {
        printf("Attenzione: Impossibile aprire il file %s\n", fname);
        return -1;
    }

    // Skipping "hop" lines
    for(int i = 0; i < hop; i++) {
        fscanf(f, "%*[^\n]"); 
        fgetc(f);             
    }

    fscanf(f,"%d",&val);
    fclose(f);

    return val;
}

report read_report(const char *fname, int hop) {
    FILE *f = fopen_at_line(fname, hop);
    if (f == NULL) return NULL;

    int id, day, month, year, cat, prio, stat;
    char citizen[50], desc[100];
    char line[256];
    report r = NULL;

    // Reads a single line
    if (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "%d;%49[^;];%d;%99[^;];%d/%d/%d;%d;%d", 
                   &id, citizen, &cat, desc, &day, &month, &year, &prio, &stat) == 9) {
            
            date dt = date_create(day, month, year);
            if (dt != NULL) {
                r = report_create(id, citizen, (category)cat, desc, dt, prio, (status)stat);
                
                if (r == NULL) {
                    date_destroy(dt); 
                }
            }
        }
    }

    fclose(f);
    return r;
}

int write_report(const char *fname, report r) {
    FILE *f = fopen(fname, "w");
    if (f == NULL) {
        printf("Attenzione: Impossibile creare il file %s\n", fname);
        return 0;
    }

    char *report = report_file_string(r);
    
    if (report != NULL) {
        fprintf(f, "%s\n", report);
        free(report); // Freeing for memory
    }

    fclose(f);
    return 1;
}