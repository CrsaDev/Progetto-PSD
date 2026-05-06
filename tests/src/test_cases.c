#include "test_cases.h"
#include "test_util.h"
#include "report.h"
#include "dir_table.h"
#include "p_queue.h"
#include <stdlib.h>

#define M 50

int registration_test_case(char *testcase_id, int n) {
    char in_f[M], out_f[M], oracle_f[M];
    get_test_filenames(testcase_id, n, in_f, out_f, oracle_f);

    // Loading the files
    dirTable oracle_table = finput_dirTable(oracle_f, 0); // Skipping 0 lines
    dirTable input_table = finput_dirTable(in_f, 2); // Skipping 2 lines 

    // Reading the input report
    report input_report = read_report(in_f,0);  // Skipping 0 lines

    if (input_report != NULL) {
        if (!dirTable_add(input_table, input_report)) {
            report_destroy(input_report);
        }
    }

    // Comparing the two arrays
    int success = foutput_list(out_f, input_table);
    int compare = 0;

    if (success) {
        compare = dirTable_compare(oracle_table, input_table);
    }

    dirTable_destroy(input_table);
    dirTable_destroy(oracle_table);

    return compare;
}

int search_test_case(char *testcase_id, int n) {
    char in_f[M], out_f[M], oracle_f[M];
    get_test_filenames(testcase_id, n, in_f, out_f, oracle_f);

    // Loading lists
    report oracle_report = read_report(oracle_f,0);
    dirTable input_table = finput_dirTable(in_f, 2);

    // Reading id
    int target_id = read_int(in_f, 0);
    report output_report = NULL;

    // Creating output list
    if (target_id > 0 || !dirTable_is_empty(input_table)) {
        output_report = report_copy(dirTable_get_report(input_table, target_id));
    }

    // Writing in output
    int success = write_report(out_f,output_report);
    int compare = 0;

    // Confronting the arrays
    if (success) {
        compare = report_compare(oracle_report, output_report);
    }

    // Freeing the memory
    dirTable_destroy(input_table);
    report_destroy(oracle_report);
    free(output_report);

    return compare;
}

int status_update_test_case(char *testcase_id, int n)
{
    char in_f[M], out_f[M], oracle_f[M];
    get_test_filenames(testcase_id, n, in_f, out_f, oracle_f);

    int new_status = read_int(in_f,0);
    report oracle_report = read_report(oracle_f, 0);
    report input_report = read_report(in_f, 2);

    // Updating the status of the report
    report_set_status(input_report, (status)new_status);


    int success = write_report(out_f, input_report);
    int compare = 0;
    if (success) {
        compare = report_compare(oracle_report, input_report);
    }

    // Freeing memory
    free(oracle_report);
    free(input_report);
    
    return compare;
}

int priority_test_case(char *testcase_id, int n) {
    char in_f[M], out_f[M], oracle_f[M];
    get_test_filenames(testcase_id, n, in_f, out_f, oracle_f);

    report oracle_report = read_report(oracle_f, 0);
    dirTable input_table = finput_dirTable(in_f, 0);

    // Initiates the priority queue
    pQueue test_queue = pQueue_create();
    // MUST BE NULL TO WORK!
    report output_report = NULL;

    if (!dirTable_is_empty(input_table)) {
        for(int i = 0; i<= dirTable_get_size(input_table); i++)
        {
            report r = dirTable_get_report(input_table,i);
            if (report_status(r) != RESOLVED) {
                pQueue_insert(test_queue, r);
            } 
        } 
    }



    if (test_queue != NULL) {
        report max_rep = pQueue_get_max(test_queue);
        if (max_rep != NULL) {
            output_report = report_copy(max_rep);
        }
    }

    int success = write_report(out_f, output_report);
    int compare = 0;
    
    if (success) {
        compare = report_compare(oracle_report, output_report);
    }

    if(test_queue) pQueue_destroy(test_queue);
    if(input_table) dirTable_destroy(input_table);
    if(output_report) report_destroy(output_report);
    if(oracle_report) report_destroy(oracle_report);
    

    return compare;
}

int reports_filter_test_case(char *testcase_id, int n) {
    char in_f[M], out_f[M], oracle_f[M];
    get_test_filenames(testcase_id, n, in_f, out_f, oracle_f);

    dirTable oracle_table = finput_dirTable(oracle_f, 0);
    dirTable input_table = finput_dirTable(in_f, 3); // Skipping 3 lines because first two are for category and status

    int target_cat = read_int(in_f, 0);
    int target_stat = read_int(in_f, 1);

    dirTable result_list = dirTable_get_filtered(input_table, target_cat, target_stat);


    int success = foutput_list(out_f, result_list);
    int compare = 0;

    if (success && oracle_table != NULL && result_list != NULL) {
        compare = dirTable_compare(oracle_table, result_list);
    }
    
    if(oracle_table) dirTable_destroy(oracle_table);
    if(input_table) dirTable_destroy(input_table);
    if(result_list) dirTable_destroy(result_list);

    return compare;
}

int final_report_test_case(char *testcase_id, int n) {
    char in_f[M], out_f[M], oracle_f[M];
    get_test_filenames(testcase_id, n, in_f, out_f, oracle_f);

    FILE *fout = fopen(out_f, "w");
    if (fout == NULL) {
        printf("ERROR: Could not create the output file!\n");
        return 0;
    }

    dirTable input_list = finput_dirTable(in_f, 0);

    int pending = 0, in_prog = 0, res = 0;
    int light = 0, street = 0, waste = 0, fault = 0;

    dirTable_get_info_stats(input_list, 
                        &pending, &in_prog, &res, 
                        &light, &street, &waste, &fault);

    int s_vals[3] = {pending, in_prog, res};
    int c_vals[4] = {light, street, waste, fault};
    
    int valid = 1;

    for(int i = 0; i < 3; i++) {
        int oracle_val = read_int(oracle_f, i);
        fprintf(fout, "%d\n", s_vals[i]); 
        if(s_vals[i] != oracle_val) {
            valid = 0; 
        }
    }

    for(int i = 0; i < 4; i++) {
        int oracle_val = read_int(oracle_f, 3 + i);
        fprintf(fout, "%d\n", c_vals[i]);
        if(c_vals[i] != oracle_val) {
            valid = 0;
        }
    }

    fclose(fout);
    dirTable_destroy(input_list);

    return valid; 
}