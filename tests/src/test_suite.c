#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test_cases.h"

#define M 50

// Typedef to pass pointr to function
typedef int (*test_function)(char*, int);

int run_suite(const char* suite_name, char* prefix, int num_tests, test_function test_func)
{
    int passed = 0;
    printf("\n=== ESECUZIONE %s ===\n", suite_name);

    for (int i = 1; i <= num_tests; i++)
    {
        printf("Test %s%d: ", prefix, i);
        
        int result = test_func(prefix, i);

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

    printf("------------------------------------------\n");
    printf("Risultato: %d/%d superati.\n", passed, num_tests);
    return (passed == num_tests);
}

int main(int argc, char *argv[])
{
    printf("Avvio Test Suite di Sistema...\n");

    run_suite("REGISTRATION TEST", "RC", 5, registration_test_case);
    run_suite("SEARCH TEST", "SC", 8, search_test_case);
    run_suite("STATUS UPDATE TEST", "UC", 5, status_update_test_case);
    run_suite("PRIORITY QUEUE TEST", "PC", 6, priority_test_case);
    run_suite("FILTER TEST", "FC", 3, reports_filter_test_case); 
    run_suite("FINAL REPORT TEST", "GC", 2, final_report_test_case);

    printf("\nTesting concluso.\n");
    getchar();
    return 0;
}