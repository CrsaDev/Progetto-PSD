#ifndef TEST_UTIL_H
#define TEST_UTIL_H
#include <stdio.h>
#include "dir_table.h"



/* Opens a file at a specific line */
FILE* fopen_at_line(const char *fname, int hop);

/* Reads a file containing a list and rerturns the list, after jumping hop positions */
dirTable finput_dirTable(char *fname, int hop);

/* Prints a list into a file*/
int foutput_list(char *fname, dirTable l);

/* Sets up the filenames */
void get_test_filenames(const char *testcase_id, int n, char *in, char *out, char *oracle);

/* Reads a int jumping hop positions in a file */
int read_int(const char *fname, int hop);

/* Reads a report jumping hop positions in a file */
report read_report(const char *fname, int hop);

/* Writes the report into the specified file */
int write_report(const char *fname, report r);

#endif