/* Prints out a calendar of a month or an entire year
 * depending on what the user inputs.
 *
 * Filip Belik
 * fbelik@gustavus.edu
 * 09 / 26 / 2019
 */

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/* Compilation: gcc cal.c -lm -o cal
 * Execution:   cal [month] [year]*/

typedef char day; /* define a day to be a character */
typedef char month; /* define a month to be a character */
typedef int year; /* define a year to be an integer */

day cday;
month cmonth;
year cyear; /* Current day, month, and year variables */

char months[13][10] =
{"january","february","march","april","may","june",
"july","august","september","october","november","december"};

char shortMonths[13][10] =
{"jan","feb","mar","apr","may","jun","jul","aug","sep","oct","nov","dec"};

void printMonth(month,year);               // Print out a given month of a given year
void getDayMonthYear(day*, month*, year*); // Set day, month, and year to current d,m,y
void printYear(year);                      // Print a given year
void lowerCase(char*);                     // Lowercase a string

char color = 0; // Contains value of what color month(s) will be printed in

int main(int argc, char **argv) {
	month monthVal = -1;
	year yearVal = -1;
	int c;
    /* Interpret flagged command line arguments */
	while ((c = getopt (argc, argv, "rgbpcom:")) != -1) {
	    switch (c) {
			case 'r':
				color = 1;
				break;
			case 'g':
				color = 2;
				break;
			case 'b':
				color = 3;
				break;
			case 'p':
				color = 4;
				break;
			case 'c':
				color = 5;
				break;
			case 'o':
				color = 6;
				break;
			case 'm': // Check what month the user inputs after -m (num or word)
				if (optarg[0] >= '0' && optarg[0] <= '9') {
					monthVal = (month) atoi(optarg);
				}
				else {
					month i;
					lowerCase(optarg);
					for (i = 0; i < 12; i++) {
						if (strcmp(optarg, shortMonths[i]) == 0) {
							monthVal = i+1;
							break;
						}
						else if (strcmp(optarg, months[i]) == 0) {
							monthVal = i+1;
							break;
						}
					}
					if (monthVal == -1) {
						printf("error: must enter a valid month (%s)\n", optarg);
						return 1;
					}
				}
				break;
			case '?':
				if (optopt == 'm')
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				else if (isprint (optopt))
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf (stderr,
			           "Unknown option character `\\x%x'.\n",
			           optopt);
					return 1;
			default:
				abort();
		}
	}
	if (monthVal == -1 && optind == argc - 2) {
		lowerCase(argv[optind]);
		month i;
		for (i = 0; i < 12; i++) {
			if (strcmp(argv[optind], shortMonths[i]) == 0) {
				monthVal = i+1;
				break;
			}
			if (strcmp(argv[optind], months[i]) == 0) {
				monthVal = i+1;
				break;
			}
		}
		if (monthVal == -1) {
			monthVal = (month) atoi(argv[optind]);
		}
		yearVal = (year) atoi(argv[optind + 1]);
        if (yearVal < 1753 || yearVal > 9999) {
            printf("error: year value must be in [1753,9999]\n");
            return 1;
        }
	}
	else if (optind == argc - 1) {
		yearVal = (year) atoi(argv[optind]);
        if (yearVal < 1753 || yearVal > 9999) {
            printf("error: year value must be in [1753,9999]\n");
            return 1;
        }
	}
	getDayMonthYear(&cday,&cmonth,&cyear);
	if (monthVal != -1 && yearVal != -1) {
		printMonth(monthVal, yearVal);
	}
	else if (monthVal != -1) {
		printMonth(monthVal, cyear);
	}
	else if (yearVal != -1) {
		printYear(yearVal);
	}
	else {
		printMonth(cmonth, cyear);
	}
}

/* Lowercases the word that is inputted, changing capital letters to lowercase */
void lowerCase(char *word) {
	while (*word != '\0') {
		if (*word >= 'A' && *word <= 'Z') {
			*word = *word - 'A' + 'a';
		}
		word++;
	}
}

/* Function that returns the number of days in specific month from specific year */
int daysInMonth(month m, year y) {
	if (m == 2) {
		if ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0) {
			return 29;
		}
		else {
			return 28;
		}
	}
	else {
		return 31 - ((m-1) % 7) % 2;
	}
}

/* Function that returns the day of the week of the first day of a specific year */
day fDOYear(year y) {
	long returnval = ((y - 2017) % 7 + (int) floor((y - 2017) / 4.0) - (int) floor((y - 2017) / 100.0) + (int) floor((y - 2017) / 400.0)) % 7;
	return (returnval >= 0) ? returnval + 1 : returnval + 8;
}

/* Function that returns the day of the week of the first day of a specific month from a year */
day fDOMonth(month m, year y) {
	int theDay = fDOYear(y)-1, i;
	for (i = 1; i < m; i++) {
		theDay += daysInMonth(i, y);
	}
	return (theDay >= 0) ? theDay % 7 + 1: theDay % 7 + 8;
}

/* Function that returns the string of month name concatenated with year and white space
 * Example: concatMY(1,2000,myStr) puts  "   January 2000   " in myStr */
void concatMY(month m, year y, char *result) {
	if (m < 1 || m > 12) {
		puts("error: must enter a valid month");
		return;
	}
	char concat[21];
	sprintf(concat, "%s %d", months[m-1], y);
	int length = strlen(concat), i;
	for(i = 0; i < ((21 - length) / 2); i++) {
		*result = ' ';
		result++;
	}
	*result = concat[0] - 'a' + 'A';
	result++;
	for (i = 1; i < length; i++) {
		*result = concat[i];
		result++;
	}
	for(i = 0; i < ((21 - length) / 2); i++) {
		*result = ' ';
		result++;
	}
	if (length % 2 == 1) {
		result--;
		*result = '\0';
	}
	else {
		*result = '\0';
	}
}

/* Set 'month' (in the caller) to current month, and
 * set 'year' (in the caller) to current year. */
void getDayMonthYear(day *day, month *month, year *year) {
    time_t timenow;
    struct tm *current;
    time(&timenow);
    current = gmtime(&timenow);
    *day = current->tm_mday;
    *month = current->tm_mon+1;
    *year = current->tm_year+1900;
}

/* Changes text color to red */
void red() {
	printf("\033[0;31m");
}

/* Changes text color to green */
void green() {
	printf("\033[0;32m");
}

/* Changes text color to blue */
void blue() {
	printf("\033[0;34m");
}

/* Changes text color to magenta */
void magenta() {
	printf("\033[0;35m");
}

/* Changes text color to cyan */
void cyan() {
	printf("\033[0;36m");
}

/* Changes text color to yellow */
void yellow() {
	printf("\033[0;33m");
}

/* Changes text color to bold yellow */
void byellow() {
	printf("\033[01;33m");
}

/* Changes text color to original color */
void reset() {
	printf("\033[0m");
}

/* Changes the color of the text based on the color variable */
void changeColor() {
	switch(color) {
		case 0:
			reset();
			break;
		case 1:
			red();
			break;
		case 2:
			green();
			break;
		case 3:
			blue();
			break;
		case 4:
			magenta();
			break;
		case 5:
			cyan();
			break;
		case 6:
			yellow();
			break;
	}
}

/* Function that prints out a month from a specific year */
void printMonth(month m, year y) {
	if (m < 1 || m > 12) {
		puts("error: must enter a valid month");
		return;
	}
    if (color > 0) {
	    changeColor();
    }
	char prtStr[21];
	concatMY(m, y, prtStr);
	printf("%s\n", prtStr);
	puts("Su Mo Tu We Th Fr Sa");
	// Print the data
	day theDay, days = daysInMonth(m,y), fDay = fDOMonth(m,y);
	for (theDay = 1; theDay < (fDay-1); theDay++) {
		printf("   ");
	}
    if (fDay != 1) {
        printf("  ");
    }
	for (theDay = 1; theDay <= days; theDay++) {
		if (theDay != 1 && (theDay + fDay - 2) % 7 == 0) {
			if (y == cyear && m == cmonth && theDay == cday) {
				byellow();
				printf("\n%2d", theDay);
				changeColor();
			}
			else {
				printf("\n%2d", theDay);
			}
		}
		else {
			if (y = cyear && m == cmonth && theDay == cday) {
				byellow();
                if (theDay == 1 && fDay == 1) {
				    printf("%2d", theDay);
                }
                else {
                    printf(" %2d", theDay);
                }
				changeColor();
			}
			else {
                if (theDay == 1 && fDay == 1) {
				    printf("%2d", theDay);
                }
                else {
                    printf(" %2d", theDay);
                }
			}
		}
	}
    for (; (theDay+fDay-2) % 7 != 0; theDay++) {
        printf("   ");
    }
	if (color > 0) {
		reset();
	}
    printf("\n");
	puts("                    ");
}

/* Centers the month name in a string 20 characters long */
void centerMonth(month m, char *monthStr) {
	int length = strlen(months[m-1]), i;
	char *initMonth;
	initMonth = monthStr;
	for (i = 0; i < (20 - length) / 2; i++) {
		*monthStr = ' ';
		monthStr++;
	}
	*monthStr = months[m-1][0] - 'a' + 'A';
	monthStr++;
	for (i = 1; i < length; i++) {
		*monthStr = months[m-1][i];
		monthStr++;
	}
	for (i = 0; i < (20 - length) / 2; i++) {
		*monthStr = ' ';
		monthStr++;
	}
	if (monthStr - initMonth == 20) {
		*monthStr = '\0';
	}
	else if (monthStr - initMonth == 19) {
		*monthStr = ' ';
		monthStr++;
		*monthStr = '\0';
	}
}

/* Function that prints out three months from a specific year horizontally */
void printThreeMonths(month m1, year y) {
	if (m1 < 1 || m1 > 10) {
		puts("error: must enter a valid month");
		return;
	}
	month m2=m1+1, m3=m1+2;
	char prtStr1[21];
	char prtStr2[21];
	char prtStr3[21];
	centerMonth(m1, prtStr1);
	centerMonth(m2, prtStr2);
	centerMonth(m3, prtStr3);
    printf("%s   ", prtStr1);
	printf("%s   ", prtStr2);
	printf("%s\n", prtStr3);
	printf("Su Mo Tu We Th Fr Sa   Su Mo Tu We Th Fr Sa   Su Mo Tu We Th Fr Sa\n");
	// Print the data
	day days1 = daysInMonth(m1,y), fd1 = fDOMonth(m1, y), days2 = daysInMonth(m2,y), fd2 = fDOMonth(m2, y), days3 = daysInMonth(m3,y), fd3 = fDOMonth(m3, y);
	day cday1, cday2, cday3;
    char max;
	if (days1 + fd1 - 1 > 35 || days2 + fd2 - 1 > 35 || days3 + fd3 - 1 > 35) {
		max = 6;
	}
	else {
		max = 5;
	}
    for (cday1=2-fd1; cday1 < 9-fd1; cday1++) {
        if (cday1 > 0) {
            if (y == cyear && m1 == cmonth && cday1 == cday) {
				    byellow();
				    printf("%2d ", cday1);
				    changeColor();
			    }
                else
                    printf("%2d ",cday1);
        }
        else {
            printf("   ");
        }
    }
    printf("  ");
    for (cday2=2-fd2; cday2 < 9-fd2; cday2++) {
        if (cday2 > 0) {
            if (y == cyear && m2 == cmonth && cday2 == cday) {
				    byellow();
				    printf("%2d ", cday2);
				    changeColor();
			    }
                else
                    printf("%2d ",cday2);
        }
        else {
            printf("   ");
        }
    }
    printf("  ");
    for (cday3=2-fd3; cday3 < 9-fd3; cday3++) {
        if (cday3 > 0) {
            if (y == cyear && m3 == cmonth && cday3 == cday) {
				    byellow();
				    printf("%2d ", cday3);
				    changeColor();
			    }
                else
                    printf("%2d ",cday3);
        }
        else {
            printf("   ");
        }
    }
    puts("");
    day week;
	for (week = 2; week <= max; week++) {
	    for (; cday1 < 2+7*week-fd1; cday1++) {
            if (cday1 <= days1) {
                if (y == cyear && m1 == cmonth && cday1 == cday) {
				    byellow();
				    printf("%2d ", cday1);
				    changeColor();
			    }
                else
                    printf("%2d ",cday1);
            }
            else
                printf("   ");
        }
        printf("  ");
        for (; cday2 < 2+7*week-fd2; cday2++) {
            if (cday2 <= days2) { 
                if (y == cyear && m2 == cmonth && cday2 == cday) {
				    byellow();
				    printf("%2d ", cday2);
				    changeColor();
			    }
                else
                    printf("%2d ",cday2);
            }
            else
                printf("   ");
        }
        printf("  ");
        for (; cday3 < 2+7*week-fd3; cday3++) {
            if (cday3 <= days3) {
                if (y == cyear && m3 == cmonth && cday3 == cday) {
				    byellow();
				    printf("%2d ", cday3);
				    changeColor();
			    }
                else
                    printf("%2d ",cday3);
            }
            else
                printf("   ");
        }
        puts("");
    }
}

/* Method used to print an entire year */
void printYear(year y) {
	changeColor();
	printf("                               %d\n\n", y);
	month m;
	for (m = 1; m <= 12; m+=3) {
		printThreeMonths(m, y);
		if (m < 10) {
			puts("");
		}
	}
	if (color > 0) {
		reset();
	}
	puts("");
}
