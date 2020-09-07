/* Created By: Ahmed Mohammed Gamal Al-Din */
/*              4/8/2019                   */
/*         uCoders >>> ID : 10             */
/*            Project: Part2               */
/*           Database Manager              */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "Utility.h"

//Deleting any spaces at the end of any string
void OmitTailSpaces(char* str){
    int i = strlen(str) - 1;
    while(*str == ' '){
        str++;
    }

    while(str[i]==' '){
        str[i] = '\0';
        i--;
    }
}

//Check whether the name or the phone or the email resides already in the database not to have replicates and to
//check if the required record from the user exist or not
int VerifyExisting(char* line, DATABASE* employees_data, FIELD field){
    int i = 0;
    char* phoneStr = (char*)malloc(12*sizeof(char));
    //We will need those variables to ignore cases in letters and alphabets
    char* tempLine1 = (char*)malloc(MAX_LINE_LENGTH*sizeof(char));
    char* tempLine2 = (char*)malloc(MAX_LINE_LENGTH*sizeof(char));
    strcpy(tempLine1,line);
    //Comparison will done after making every letter a small letter not a capital one
    lowerCase(tempLine1);

    switch(field){
    case name:
        for(i = 0; i < employees_data->DataBase_Size; i++){
            strcpy(tempLine2,employees_data->List[i].name);
            lowerCase(tempLine2);
            if(strcmp(tempLine1, tempLine2) == 0){
                //We have to free EVERY allocated memory and not to forget a single one not to be accumulated
                //and may cause errors and memory leak
                free(phoneStr);
                free(tempLine1);
                free(tempLine2);
                return 1;
            }
        }
        break;
    case phone:
        for(i = 0; i < employees_data->DataBase_Size; i++){
            itoa(employees_data->List[i].phone, phoneStr, 10);
            if(strcmp(phoneStr, line+1) == 0){
                //We have to free EVERY allocated memory and not to forget a single one not to be accumulated
                //and may cause errors and memory leak
                free(phoneStr);
                free(tempLine1);
                free(tempLine2);
                return 1;
            }
        }
        break;
    case email:
        for(i = 0; i < employees_data->DataBase_Size; i++){
            strcpy(tempLine2,employees_data->List[i].email);
            lowerCase(tempLine2);
            if(strcmp(tempLine1, tempLine2) == 0){
                //We have to free EVERY allocated memory and not to forget a single one not to be accumulated
                //and may cause errors and memory leak
                free(phoneStr);
                free(tempLine1);
                free(tempLine2);
                return 1;
            }
        }
        break;

    case birth_date:
    case job_title:
        break;
    }
    //We have to free EVERY allocated memory and not to forget a single one not to be accumulated
    //and may cause errors and memory leak
    free(phoneStr);
    free(tempLine1);
    free(tempLine2);


    //If Exist is one that means that a record is found with the specified data
    return 0;
}

//Check that the name does NOT violate any of the rules associated with names
//Can have ONLY alphabets. Can contain spaces. Ignore Case.
int ParseName(char* line){
    int i = 0, lineLength = strlen(line);
    for(i = 0; i < lineLength; i++){
        if( !((line[i] >= 'A' && line[i] <= 'Z') || (line[i] >= 'a' && line[i] <= 'z') || (line[i] == ' ')) ){
            printf(NAME_INVALID_FORMAT);
            return 0;
        }
    }
    return 1;
}

//Check that the phone number does NOT violate any of the rules associated with phone numbers
//All of it should contain only numbers and starts with 010 or 011 or 012 or 015 only and contains 11 digits exactly
int ParsePhone(char* line){
    unsigned long long int phoneNum = 0;
    int i = 0, lineLength = strlen(line);
    for(i = 0; i < lineLength; i++){
        if(!(line[i] >= '0' && line[i] <= '9')){
            printf(PHONE_INVALID_FORMAT);
            return 0;
        }
    }

    phoneNum = atoi(line);
    if( !((phoneNum >= 1000000000 && phoneNum <= 1299999999) || (phoneNum >= 1500000000 && phoneNum <= 1599999999)) ){

        phoneNum = 0;
        printf(PHONE_INVALID_FORMAT);
    }
    return phoneNum;
}

//Check that the email does NOT violate any of the rules associated with emails
//Should be in the form of something@website.com. Ignore Case. Can NOT start with a number
int ParseEmail(char* line){
    int i = 0, flag = 0, lineLength = strlen(line), temp = 0;
    if( (lineLength < 7) || (strcmp(line + lineLength - 4,".com") != 0) || (line[0] >= '0' && line[0] <= '9') ){
        printf(EMAIL_INVALID_FORMAT);
        return 0;
    }
    for(i = 0; i < lineLength; i++){
        if(line[i] == '@'){
            flag = 1;
            temp = i;
            break;
        }
    }
    if(flag == 1){
        for(i = temp + 1; i < lineLength - 4; i++){
            if( !((line[i] >= 'A' && line[i] <= 'Z') || (line[i] >= 'a' && line[i] <= 'z')) ){
                printf(WEBSITE_INVALID);
                return 0;
            }
        }
    }else printf(EMAIL_INVALID_FORMAT);

    return flag;
}

//Check that the birth date does NOT violate any of the rules associated with birth dates
//Check whether this date exists on the Calendar and take into account leap years.
//Arrange day, month and year in a structure to use in other further work
int ParseDate(char* line, DATE* newDate){
    int i = 0, lineLength = strlen(line), leap = 0;
    if(lineLength != 10 || line[2] != '/' || line[5] != '/'){
        printf(DATE_INVALID_FORMAT);
        return 0;
    }

    for(i = 0; i < lineLength; i++){
        if(i == 2 || i == 5){
            continue;
        }else if(!(line[i] >= '0' && line[i] <= '9')){
            printf(DATE_INVALID_FORMAT);
            return 0;
        }
    }

    char* YYYY = (char*)malloc(5*sizeof(char));
    char* MM = (char*)malloc(3*sizeof(char));
    char* DD = (char*)malloc(3*sizeof(char));

    DD[0] = line[0]; DD[1] = line[1]; DD[2] = '\0';
    MM[0] = line[3]; MM[1] = line[4]; MM[2] = '\0';
    YYYY[0] = line[6]; YYYY[1] = line[7]; YYYY[2] = line[8]; YYYY[3] = line[9]; YYYY[4] = '\0';

    newDate->day = atoi(DD);
    newDate->month = atoi(MM);
    newDate->year = atoi(YYYY);

    if(newDate->year < 1959 || newDate->year > 1996){
        printf(YEAR_INVALID);
        return 0;
    }

    if(newDate->year % 4 == 0){
        if(newDate->year % 100 == 0){
            if(newDate->year % 400 == 0){
                leap = 1;
            }else leap = 0;
        }else leap = 1;
    }else leap = 0;

    if(newDate->month == 1 || newDate->month == 3 || newDate->month == 5 || newDate->month == 7 || newDate->month == 8 ||
       newDate->month == 10 || newDate->month == 12){
        if(newDate->day > 31 || newDate->day < 1){
            printf(DATE_EXISTANCE);
            return 0;
        }
    }else if(newDate->month == 4 || newDate->month == 6 || newDate->month == 9 || newDate->month == 11){
        if(newDate->day > 30 || newDate->day < 1){
            printf(DATE_EXISTANCE);
            return 0;
        }
    }else if(newDate->month == 2){
        if(leap == 1){
            if(newDate->day > 29 || newDate->day < 1){
                printf(DATE_EXISTANCE);
                return 0;
            }
        }else{
            if(newDate->day > 28 || newDate->day < 1){
                printf(DATE_EXISTANCE);
                return 0;
            }
        }

    }else{
        printf(DATE_EXISTANCE);
        return 0;
    }

    //We have to free EVERY allocated memory and not to forget a single one not to be accumulated
    //and may cause errors and memory leak
    free(YYYY);
    free(MM);
    free(DD);

    return 1;
}

//Check that the job title exists in the job titles list of the company
int ParseJob(char* line){
    char* jobStr = (char*)malloc(MAX_Job_LENGTH*sizeof(char));
    char* tempLine = (char*)malloc(MAX_LINE_LENGTH*sizeof(char));
    strcpy(tempLine,line);

    int flag = 0;
    FILE *jobs = fopen(JobTitles,"r");
    if (jobs == NULL)
    {
        printf(OPEN_FILE_FAILURE);
        return -1;
    }

    lowerCase(tempLine);
    fscanf(jobs,"%[^\n]%*c",jobStr);
    while(!feof(jobs)){
        fscanf(jobs,"%[^\n]%*c",jobStr);
        strcpy(line,jobStr);
        lowerCase(jobStr);
        if(strcmp(jobStr,tempLine) == 0){
            flag = 1;
            break;
        }
    }

    if(flag == 0) printf(JOB_INVALID);

    //We have to free EVERY allocated memory and not to forget a single one not to be accumulated
    //and may cause errors and memory leak
    fclose(jobs);
    free(jobStr);
    free(tempLine);
    return flag;
}

//Convert each Capital letter into a small one in the same string
void lowerCase(char* line){
    int i = 0, lineLength = strlen(line);
    for(i = 0; i < lineLength; i++){
        if(line[i] >= 65 && line[i] <= 90){
            line[i] = line[i] + 32;
        }
    }
}

//Partitioning the read line from a file to parse its data as name, phone number, email, birth date and job title
void parseLine(char* line,char* dataArray[5]){
    int i = 0;
    char* token = strtok(line, ",");
    OmitTailSpaces(token);
    while (token != NULL) {
        //printf("%s.", token);
        strcpy(dataArray[i],token);
        i = i + 1;
        token = strtok(NULL, ",");
        if(token != NULL){
            while(*token == ' ') token++;
            OmitTailSpaces(token);
        }
    }
}

///The following 4 functions acts as the compare function that the qsort needs to operate well

//Sort the database in an ascending order according to the name
int ascendingName(const void *first, const void *second){
    int res = 0;
    char* C1 = (char*)malloc(MAX_NAME_LENGTH*sizeof(char));
    char* C2 = (char*)malloc(MAX_NAME_LENGTH*sizeof(char));
    strcpy(C1,((EMPLOYEE*)first)->name);
    lowerCase(C1);
    strcpy(C2,((EMPLOYEE*)second)->name);
    lowerCase(C2);

    while(*C1 == *C2 && *C1 != '\0' && *C2 != '\0'){
        C1++;
        C2++;
    }
    res = (*C1 - *C2);
    free(C1);
    free(C2);
    return res;
}

//Sort the database in a descending order according to the name
int descendingName(const void *first, const void *second){
    int res = 0;
    char* C1 = (char*)malloc(MAX_NAME_LENGTH*sizeof(char));
    char* C2 = (char*)malloc(MAX_NAME_LENGTH*sizeof(char));
    strcpy(C1,((EMPLOYEE*)first)->name);
    lowerCase(C1);
    strcpy(C2,((EMPLOYEE*)second)->name);
    lowerCase(C2);

    while(*C1 == *C2 && *C1 != '\0' && *C2 != '\0'){
        C1++;
        C2++;
    }
    res = (*C2 - *C1);
    free(C1);
    free(C2);
    return res;
}

//Sort the database in an ascending order according to the birth date
int ascendingDate(const void *first, const void *second){
    int res = 0;
    int C1 = ((EMPLOYEE*)first)->birth_date.year;
    int C2 = ((EMPLOYEE*)second)->birth_date.year;
    res = C1 - C2;
    if(res == 0){
        C1 = ((EMPLOYEE*)first)->birth_date.month;
        C2 = ((EMPLOYEE*)second)->birth_date.month;
        res = C1 - C2;
        if(res == 0){
            C1 = ((EMPLOYEE*)first)->birth_date.day;
            C2 = ((EMPLOYEE*)second)->birth_date.day;
            res = C1 - C2;
        }
    }
    return res;
}

//Sort the database in a descending order according to the birth date
int descendingDate(const void *first, const void *second){
    int res = 0;
    int C1 = ((EMPLOYEE*)first)->birth_date.year;
    int C2 = ((EMPLOYEE*)second)->birth_date.year;
    res = C1 - C2;
    if(res == 0){
        C1 = ((EMPLOYEE*)first)->birth_date.month;
        C2 = ((EMPLOYEE*)second)->birth_date.month;
        res = C1 - C2;
        if(res == 0){
            C1 = ((EMPLOYEE*)first)->birth_date.day;
            C2 = ((EMPLOYEE*)second)->birth_date.day;
            res = C1 - C2;
        }
    }
    return -1*res;
}

