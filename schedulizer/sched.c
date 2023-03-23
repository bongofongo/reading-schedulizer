#include "sched.h"

// Calculates the days 
// Needs months to be from 0 - 11 indexed. 
unsigned int days_calc(date* start, date* end) {
    unsigned int year_diff = end->year - start->year;
    if (year_diff == 0) {
        if (end->month - start->month == 0) {
            return end->day - start->day;
        }
        else if (end->month - start->month > 0) {
            unsigned int sum = 0;
            for (int i = start->month; i < end->month; i++) {
                sum += month_days[i]; 
            } 
            return sum + end->day - start->day;
        } else {
            fprintf(stderr, "Month inputs are incorrect, try again please...\n");    
            exit(1);
        }
    }
    else if (year_diff > 0) {
        unsigned int sum = 0;
        unsigned int years_left = year_diff;

        for (int i = start->month; i < 12; i++) {
            sum += month_days[i];
        }
        years_left--;
        sum += years_left * 365;
        for (int i = 0; i < end->month; i++) {
            sum += month_days[i];
        }
        return sum + end->day - start->day;
    }
    else {
        fprintf(stderr, "Year inputs are incorrect, try again please...\n");
        exit(1);
    }
}

unsigned int ppd_calc(date* dstart, date* dend, unsigned int pages) {
    return (unsigned int) (pages / days_calc(dstart, dend));
}

reading* make_reading(unsigned int ppd, date* s, date* e, char *a, char *t) {
    reading* res = (reading*) malloc(sizeof(reading));
    res->ppd = ppd;
    res->start_d = s; 
    res->end_d = e;
    if (strlen(a) > 30) {
        fprintf(stderr, "Author name too long. Make it shorter.\n");
        exit(1);
    }
    if (strlen(t) > 60) {
        fprintf(stderr, "Title too long. Make it shorter.\n");
        exit(1);
    }

    res->author = (char*)malloc(MAX_AUTHOR * sizeof(char));
    strcpy(res->author, a);
    res->title = (char*)malloc(MAX_TITLE * sizeof(char));
    strcpy(res->title, t);
    return res;
} 

void free_reading(reading* r) {
    free(r->title);
    free(r->author);
    free(r);
}

date* make_date(unsigned int m, unsigned int d, unsigned int y) {
    date* dres = (date*) malloc(sizeof(date));
    dres->month = m, dres->day = d, dres->year = y;
    return dres;
}

char* date_formatter(date* d) {
    char* date_str = (char*) malloc(9 * sizeof(char));
    sprintf(date_str, "%02u-%02u-%02u", d->month+1, d->day, d->year);
    return date_str;
}

reading** looper(FILE* tp, int *size) {
    char file_line[MAX_FILE_LINE]; 
        char* author = (char*) malloc(MAX_AUTHOR * sizeof(char));
        char* title = (char*) malloc(MAX_TITLE * sizeof(char));
        unsigned int month_s, month_e, day_s, day_e, year_s, year_e;
        unsigned int pages;
        reading** rres = (reading**)malloc(MAX_READINGS * sizeof(reading*));

        int i = 0;
        while (fgets(file_line, sizeof(file_line), tp)) {
            sscanf(file_line, "%[^,], %[^,], %u, %u-%u-%u, %u-%u-%u", author, title, &pages, &month_s, &day_s, &year_s, &month_e, &day_e, &year_e);
            if (i > MAX_READINGS) {
                printf("The amount of file inputs (lines) has exceeded the capacity of the program, which is: %u.\n", MAX_READINGS);
                break;
            }
            month_s--, month_e--;
            date* dstart = make_date(month_s, day_s, year_s);
            date* dend = make_date(month_e, day_e, year_e);
            rres[i] = make_reading(ppd_calc(dstart, dend, pages), dstart, dend, author, title);
            i++;
        }
        *size = i;
        return rres;

}

void print_out(reading** r, int size) {
    for (int i = 0; i < size; i++) {
        char* dstart = date_formatter(r[i]->start_d);
        char* dend = date_formatter(r[i]->end_d);
        printf("%s, %s. %s - %s. Pages/Day: %u\n", r[i]->author, r[i]->title, dstart, dend, r[i]->ppd);
    }
}

void free_arr(reading** arr, int size) {
    for (int i = 0; i < size; i++) {
        free_reading(arr[i]);
    }
    free(arr);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "You just need to put the file as the first input, and that's all.\n Try again.\n");
        exit(1);
    }

    FILE* t = fopen(argv[1], "r");
    if (t == NULL) {
        fprintf(stderr, "File not found.");
        exit(1);
    }

    reading** r_arr = NULL;
    int size;

    r_arr = looper(t, &size);
    print_out(r_arr, size);
    free_arr(r_arr, size);
}
