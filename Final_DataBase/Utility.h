/* Created By: Ahmed Mohammed Gamal Al-Din */
/*              4/8/2019                   */
/*         uCoders >>> ID : 10             */
/*            Project: Part2               */
/*           Database Manager              */

#ifndef __UTILITY_H_
#define __UTILITY_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "DataBase.h"

#define MAX_NAME_LENGTH 40
#define MAX_EMAIL_LENGTH 60
#define MAX_Job_LENGTH 40
#define MAX_Date_LENGTH 12
#define MAX_LINE_LENGTH 60

#define MEMORY_ALLOCATION_FAILURE "Memory Allocation Failure\nExiting...\n"
#define TRY_AGAIN "Please, try again ...\n"
#define OPEN_FILE_FAILURE "Error while opening the file.\n"
#define DATA_EXISTS_ALREADY "Replicates are NOT allowed in the database\n"
#define NAME_EXISTANCE "Name does NOT exist in the database\n"
#define NAME_INVALID_FORMAT "Names should contain ONLY alphabets\n"
#define EMAIL_INVALID_FORMAT "Emails should be in the form: Something@website.com\n"
#define DATE_INVALID_FORMAT "Dates should be in the form: DD/MM/YYYY\n"
#define PHONE_INVALID_FORMAT "Phone numbers should be in the format '01YXXXXXXXX', where X is numbers from 0 to 9 and Y is one of those 0,1,2,5\n"
#define DATE_EXISTANCE "This Date does NOT exist\n"
#define JOB_INVALID "This job title does NOT exsist in the company's job titles\n"
#define WEBSITE_INVALID "The website in 'Something@website.com' should contains ONLY alphabets\n"
#define YEAR_INVALID "Years should be between 1959 and 1996\n"

#define JobTitles "Job_Titles.txt"

void OmitTailSpaces(char* str);
int VerifyExisting(char* line, DATABASE* employees_data, FIELD field);

int ParseName(char* line);
int ParsePhone(char* line);
int ParseEmail(char* line);
int ParseDate(char* line, DATE* newDate);
int ParseJob(char* line);
void parseLine(char* line,char* dataArray[5]);
void lowerCase(char* line);
void clearStr(char* line, int sizeStr);
int ascendingName(const void *first, const void *second);
int descendingName(const void *first, const void *second);
int ascendingDate(const void *first, const void *second);
int descendingDate(const void *first, const void *second);

#endif // __UTILITY_H_
