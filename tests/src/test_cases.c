#include "test_cases.h"
#include "test_util.h"
#include "report.h"
#include "dyn_arr.h"
#include "p_queue.h"

#define M 50

int registration_test_case(char *testcase_id, int n) {
    char in_f[M], out_f[M], oracle_f[M];
    get_test_filenames(testcase_id, n, in_f, out_f, oracle_f);

    // Loading the files
    dynArr oracle_list = finput_list(oracle_f, 0);
    dynArr input_list = finput_list(in_f, 2);

    dynArr temp_l = finput_list(in_f, 0);
    if (temp_l == NULL) {
        dynArr_destroy(oracle_list);
        dynArr_destroy(input_list);
        return 0;
    }

    // Reading the input report
    report input_report = dynArr_pop_head(temp_l); 
    
    dynArr_destroy(temp_l);

    if (input_report != NULL) {
        if (!dynArr_add(input_list, input_report)) {
            report_destroy(input_report);
        }
    }
    
    // Comparing the two arrays
    int success = foutput_list(out_f, input_list);
    int compare = 0;

    if (success) {
        compare = dynArr_compare(oracle_list, input_list);
    }

    dynArr_destroy(input_list);
    dynArr_destroy(oracle_list);

    return compare;
}

int search_test_case(char *testcase_id, int n) {
    char in_f[M], out_f[M], oracle_f[M];
    get_test_filenames(testcase_id, n, in_f, out_f, oracle_f);

    // Loading lists
    dynArr oracle_list = finput_list(oracle_f, 0);
    dynArr input_list = finput_list(in_f, 2);
    
    // Reading id
    int target_id = read_int(in_f, 0);

    // Creating output list
    dynArr output_list = dynArr_create(); 
    
    if (target_id != -1 && input_list != NULL && output_list != NULL) {
        report found = dynArr_get_report(input_list, target_id);
        
        if (found != NULL) {
            report copy = report_copy(found);
            if (!dynArr_add(output_list, copy)) {
                // Free the copy if it fails
                report_destroy(copy);
            }
        }
    }

    // Writing in output
    int success = foutput_list(out_f, output_list);
    int compare = 0;

    // Confronting the arrays
    if (success) {
        compare = dynArr_compare(oracle_list, output_list);
    }

    // Freeing the memory
    dynArr_destroy(input_list);
    dynArr_destroy(output_list);
    dynArr_destroy(oracle_list);

    return compare;
}

int status_update_test_case(char *testcase_id, int n)
{
    char in_f[M], out_f[M], oracle_f[M];
    get_test_filenames(testcase_id, n, in_f, out_f, oracle_f);

    dynArr oracle_list = finput_list(oracle_f, 0);
    dynArr input_list = finput_list(in_f, 2);

    int new_status = read_int(in_f,0);

    report to_update = dynArr_pop_head(input_list);
    report_set_status(to_update, (status)new_status);
    dynArr_add(input_list, to_update);

    int success = foutput_list(out_f, input_list);
    int compare = 0;
    if (success) {
        compare = dynArr_compare(oracle_list, input_list);
    }

    dynArr_destroy(input_list);
    dynArr_destroy(oracle_list);
    
    return compare;
}

int priority_test_case(char *testcase_id, int n) {
    char in_f[M], out_f[M], oracle_f[M];
    get_test_filenames(testcase_id, n, in_f, out_f, oracle_f);

    dynArr oracle_list = finput_list(oracle_f, 0);
    dynArr input_list = finput_list(in_f, 0);

    pQueue test_queue = pQueue_create();
    dynArr output_list = dynArr_create(); 

    if (input_list != NULL) {
    while (!dynArr_is_empty(input_list)) {
            report r = dynArr_pop_head(input_list);
            
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
            dynArr_add(output_list, report_copy(max_rep));
        }
    }

    int success = foutput_list(out_f, output_list);
    int compare = 0;
    
    if (success) {
        compare = dynArr_compare(oracle_list, output_list);
    }

    dynArr_destroy(input_list);
    dynArr_destroy(output_list);
    dynArr_destroy(oracle_list);
    pQueue_destroy(test_queue);

    return compare;
}

int reports_filter_test_case(char *testcase_id, int n) {
    char in_f[M], out_f[M], oracle_f[M];
    get_test_filenames(testcase_id, n, in_f, out_f, oracle_f);

    dynArr oracle_list = finput_list(oracle_f, 0);
    dynArr input_list = finput_list(in_f, 3);

    int target_cat = read_int(in_f, 0);
    int target_stat = read_int(in_f, 1);

    dynArr result_list = dynArr_get_filtered(input_list, target_cat, target_stat);

    int success = foutput_list(out_f, result_list);
    int compare = 0;

    if (success) {
        compare = dynArr_compare(oracle_list, result_list);
    }
    
    dynArr_destroy(oracle_list);
    dynArr_destroy(input_list);
    dynArr_destroy(result_list);

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

    dynArr input_list = finput_list(in_f, 0);

    int pending = 0, in_prog = 0, res = 0;
    int light = 0, street = 0, waste = 0, fault = 0;

    dynArr_get_info_stats(input_list, 
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
    dynArr_destroy(input_list);

    return valid; 
}