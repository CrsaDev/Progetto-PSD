#include <stdio.h>
#include <stdlib.h>
#include "date.h"

struct date {
    int day;
    int month;
    int year;
};

/* --- Memory Management --- */

date date_create(int day, int month, int year) {
    date d = malloc(sizeof(*d));
    
    // Returns null in case of unsuccesful memory allocation
    if(d == NULL) return NULL;

    d->day = day;
    d->month = month;
    d->year = year;

    return d;
}

void date_destroy(date d) {
    if (d) free(d);
}

/* --- Getters --- */


int date_day(date d) {
    return (d) ? d->day : -1;
}

int date_month(date d) {
    return (d) ? d->month : -1;
}

int date_year(date d) {
    return (d) ? d->year : -1;
}

/* --- Utilities & Comparison --- */

long date_to_long(date d) {
    if(!d) return -1;

    return (long)d->year*10000 + (long)d->month*100 + d->day;
}

void date_print(date d) {
    if(!d) return;

    printf("%02d/%02d/%04d",d->day,d->month,d->year);
}

// Validates the inserted date
int date_is_valid(date d) {
    if (!d) return 0;

    if (d->year < 2000 || d->month < 1 || d->month > 12 || d->day < 1) return 0;

    int days_in_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Checks for a leap year
    if ((d->year % 4 == 0 && d->year % 100 != 0) || (d->year % 400 == 0)) {
        days_in_month[2] = 29;
    }

    return d->day <= days_in_month[d->month];
}