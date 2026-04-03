#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "report.h"
#include "test_util.h"

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

int registration_test_case(char *testcase_id, int n)
{
    char input_fname[M], output_fname[M], oracle_fname[M];

    sprintf(input_fname, "tests/%s%d_input.txt", testcase_id, n);
    sprintf(output_fname, "tests/%s%d_output.txt", testcase_id, n);
    sprintf(oracle_fname, "tests/%s%d_oracle.txt", testcase_id, n);

    /* --- Step 1 --- */
    list input_list = list_create();
    list oracle_list = list_create();

    finput_list(oracle_fname, oracle_list, 0);
    oracle_list = list_reversed(oracle_list);

    
    list temp_l = list_create();
    finput_list(input_fname, temp_l, 0);
    report input_report = list_remove_head(temp_l);
    list_destroy(temp_l);

    if (input_report == NULL) // If for some reason the report is null, fail the test
    {
        list_destroy(input_list);
        list_destroy(oracle_list);
        return 0;
    }

    finput_list(input_fname, input_list, 2);
    input_list = list_reversed(input_list);

    /* --- Step 2 --- */
    list_add(input_list, input_report);

    /* --- Step 3 --- */
    foutput_list(output_fname, input_list);

    /* --- Step 4 --- */
    int compare = list_compare(oracle_list, input_list);

    // Destroying both lists to avoid wasting memory
    list_destroy(input_list);
    list_destroy(oracle_list);

    return compare;
}

/* -------------------------------------------------------------------------
   TODO
   ------------------------------------------------------------------------- */

int search_test_case(char *id, int n) { return 1; }
int status_update_test_case(char *id, int n) { return 1; }
int priority_test_case(char *id, int n) { return 1; }

/* -------------------------------------------------------------------------
   TEMPORARY TEST SUITE RUN AND MAIN
   ------------------------------------------------------------------------- */
   
int run_test_suite()
{
    int passed = 0;
    int num_tests = 3;
    char *tc_id = "RC";

    printf("\n=== ESECUZIONE REGISTRATION TEST SUITE ===\n");

    for (int i = 1; i <= num_tests; i++)
    {
        int result = registration_test_case(tc_id, i);

        printf("Test %s%d: ", tc_id, i);
        if (result)
        {
            printf("[PASS]\n");
            passed++;
        }
        else
        {
            printf("[FAIL]\n");
        }
    }

    printf("==========================================\n");
    printf("Risultato: %d/%d superati.\n\n", passed, num_tests);

    return (passed == num_tests);
}

int main(int argc, char *argv[])
{
    run_test_suite();

    return 0;
}