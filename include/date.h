#ifndef DATE_H
#define DATE_H

typedef struct date *date;

/* --- Memory Management --- */

// Allocates and returns a new date instance
date date_create(int day, int month, int year);

// Frees the memory allocated for the given date
void date_destroy(date d);

/* --- Getters --- */

// Returns the day of the date
int date_day(date d);

// Returns the month of the date
int date_month(date d);

// Returns the year of the date (YYYY format)
int date_year(date d);

/* --- Utilities & Comparison --- */

// Converts the date to a YYYYMMDD long value for easy comparisons
long date_to_long(date d);

// Prints the date in DD/MM/YYYY format
void date_print(date d);

// Returns 1 if the date is valid, 0 otherwise
int date_is_valid(date d);

#endif