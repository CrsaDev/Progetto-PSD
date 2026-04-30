#include "test_cases.h"
#include "test_util.h"
#include "report.h"
#include "list.h"
#include "p_queue.h"

#define M 50

/*
    GENERAL RULES

    /------------------------------------------/
        !!Important - File format

        Record to add - Id Citizen Category Description DD/MM/YYYY Priority Status
        
        ----- Separator

        List (Multiple records one under another)
    /------------------------------------------/

    1. Input reading
        Read the input into eventual records
        Re-enter file, skip first n lines (generally two) if there's a list too
        Copy from test_input.txt into the list
        Open test_oracle.txt
        Copy from test_oracle.txt into another list
        Reverse both lists to follow file order

    2. Make the intended operation
        Could be list_add, list_remove, etc..

    3. Write the output file by using the updated input list

    4. Compare the input list with the oracle list

    Return 1 if they are the same and 0 if not
*/

int registration_test_case(char *testcase_id, int n) {
    char in_f[M], out_f[M], oracle_f[M];
    get_test_filenames(testcase_id, n, in_f, out_f, oracle_f);

    // Lists can be NULL
    list oracle_list = finput_list(oracle_f, 0);
    list input_list = finput_list(in_f, 2);

    // The input can be NULL
    list temp_l = finput_list(in_f, 0);
    report input_report = list_pop_head(temp_l); 
    list_destroy(temp_l);

    // Its possible to try and insert a NULL value
    list_add(input_list, input_report);
    
    int success = foutput_list(out_f, input_list);
    int compare = 0;

    if (success) {
        compare = list_compare(oracle_list, input_list);
    }

    list_destroy(input_list);
    list_destroy(oracle_list);

    return compare;
}

int search_test_case(char *testcase_id, int n) {
    char in_f[M], out_f[M], oracle_f[M];
    get_test_filenames(testcase_id, n, in_f, out_f, oracle_f);

    list oracle_list = finput_list(oracle_f, 0);
    list input_list = finput_list(in_f, 2);
    int target_id = read_int(in_f, 0);

    list output_list = list_create(); 
    
    if (target_id != -1 && input_list != NULL) {
        report found = list_get_report(input_list, target_id);
        
        if (found != NULL) {
            list_add(output_list, report_copy(found)); 
        }
    }

    int success = foutput_list(out_f, output_list);
    int compare = 0;

    if (success) {
        compare = list_compare(oracle_list, output_list);
    }

    list_destroy(input_list);
    list_destroy(output_list);
    list_destroy(oracle_list);

    return compare;
}

int status_update_test_case(char *testcase_id, int n)
{
    char in_f[M], out_f[M], oracle_f[M];
    get_test_filenames(testcase_id, n, in_f, out_f, oracle_f);

    list oracle_list = finput_list(oracle_f, 0);
    list input_list = finput_list(in_f, 2);

    int new_status = read_int(in_f,0);

    report to_update = list_pop_head(input_list);
    report_set_status(to_update, (status)new_status);
    list_add(input_list, to_update);

    int success = foutput_list(out_f, input_list);
    int compare = 0;
    if (success) {
        compare = list_compare(oracle_list, input_list);
    }

    list_destroy(input_list);
    list_destroy(oracle_list);
    
    return compare;
}

int priority_test_case(char *testcase_id, int n) {
    char in_f[M], out_f[M], oracle_f[M];
    get_test_filenames(testcase_id, n, in_f, out_f, oracle_f);

    list oracle_list = finput_list(oracle_f, 0);
    list input_list = finput_list(in_f, 0);

    pQueue test_queue = pQueue_create();
    list output_list = list_create(); 

    if (input_list != NULL) {
    while (!list_is_empty(input_list)) {
            report r = list_pop_head(input_list);
            
            if (report_status(r) != RESOLVED) {
                pQueue_insert(test_queue, r);
            } else {
                report_destroy(r); 
            }
        }
    }

    if (test_queue != NULL) {
        report max_rep = pQueue_get_max(test_queue);
        if (max_rep != NULL) {
            list_add(output_list, report_copy(max_rep));
        }
    }

    int success = foutput_list(out_f, output_list);
    int compare = 0;
    
    if (success) {
        compare = list_compare(oracle_list, output_list);
    }

    list_destroy(input_list);
    list_destroy(output_list);
    list_destroy(oracle_list);
    pQueue_destroy(test_queue);

    return compare;
}

int reports_filter_test_case(char *testcase_id, int n) {
    char in_f[M], out_f[M], oracle_f[M];
    get_test_filenames(testcase_id, n, in_f, out_f, oracle_f);

    list oracle_list = finput_list(oracle_f, 0);
    list input_list = finput_list(in_f, 3);

    int target_cat = read_int(in_f, 0);
    int target_stat = read_int(in_f, 1);

    list result_list = list_get_filtered(input_list, target_cat, target_stat);

    int success = foutput_list(out_f, result_list);
    int compare = 0;

    if (success) {
        compare = list_compare(oracle_list, result_list);
    }
    
    list_destroy(oracle_list);
    list_destroy(input_list);
    list_destroy(result_list);

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

    list input_list = finput_list(in_f, 0);

    int pending = 0, in_prog = 0, res = 0;
    int light = 0, street = 0, waste = 0, fault = 0;

    list_get_info_stats(input_list, 
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
    list_destroy(input_list);

    return valid; 
}