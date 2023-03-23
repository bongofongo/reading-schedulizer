#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>

const unsigned int month_days[12] = {31, 29, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30};
const unsigned int MAX_AUTHOR = 31, MAX_TITLE = 61, MAX_FILE_LINE = 121, MAX_READINGS = 30;

struct date {
    unsigned int day;
    unsigned int month;
    unsigned int year;
}; 
typedef struct date date;

struct reading {
    unsigned int ppd;
    date* start_d;
    date* end_d;
    char* author;
    char* title;
};
typedef struct reading reading;

void free_reading(reading* r);

void free_arr(reading** arr, int size);

unsigned int days_calc(date* start, date* end);

unsigned int ppd_calc(date* dstart, date* dend, unsigned int pages);

reading* make_reading(unsigned int ppd, date* s, date* e, char *a, char *t);

date* make_date(unsigned int m, unsigned int d, unsigned int y);

char* date_formatter(date* d);

reading** looper(FILE* tp, int *size);

void print_out(reading** r, int size);

