#ifndef TEST_UTIL_H
#define TEST_UTIL_H
#include <stdio.h>
#include "dyn_arr.h"



// Opens a file at a specific line
FILE* fopen_at_line(const char *fname, int hop);

/* Reads a file containing a list and rerturns the list, after jumping hop positions */
dynArr finput_list(char *fname, int hop);

/* Prints a list into a file*/
int foutput_list(char *fname, dynArr l);

/* Sets up the filenames */
void get_test_filenames(const char *testcase_id, int n, char *in, char *out, char *oracle);

/* Reads a int jumping hop positions in a file */
int read_int(const char *fname, int hop);

#endif