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
    if (year < 2000 || month < 1 || month > 12 || day < 1) return NULL;

    int days_in_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Bisestile year
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        days_in_month[2] = 29;
    }

    if (day > days_in_month[month]) return NULL;

    date d = malloc(sizeof(*d));
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

void date_print(date d) {
    if(!d) return;

    printf("%02d/%02d/%04d",d->day,d->month,d->year);
}

int date_is_valid(date d) {
    return (d != NULL);
}