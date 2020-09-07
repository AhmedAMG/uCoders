/* Created By: Ahmed Mohammed Gamal Al-Din */
/*              4/8/2019                   */
/*         uCoders >>> ID : 10             */
/*            Project: Part2               */
/*           Database Manager              */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "CompanyDatabase.h"
#include "Utility.h"


int Add(DATABASE* database, char* newName, unsigned long long int newPhone, char* newEmail, DATE* newDate, char* newJob){

    //Reallocate some more memory for the new record to be stored
    database->List = (EMPLOYEE*)realloc(database->List, (database->DataBase_Size+1)*sizeof(EMPLOYEE));
    if(database->List == NULL){printf(MEMORY_ALLOCATION_FAILURE); return -1;}

    database->List[database->DataBase_Size].name = (char*)malloc(MAX_NAME_LENGTH*sizeof(char));
    if(database->List[database->DataBase_Size].name == NULL){printf(MEMORY_ALLOCATION_FAILURE); return -1;}
    database->List[database->DataBase_Size].email = (char*)malloc(MAX_EMAIL_LENGTH*sizeof(char));
    if(database->List[database->DataBase_Size].email == NULL){printf(MEMORY_ALLOCATION_FAILURE); return -1;}
    database->List[database->DataBase_Size].job_title = (char*)malloc(MAX_Job_LENGTH*sizeof(char));
    if(database->List[database->DataBase_Size].job_title == NULL){printf(MEMORY_ALLOCATION_FAILURE); return -1;}

    strcpy(database->List[database->DataBase_Size].name,newName);
    strcpy(database->List[database->DataBase_Size].email,newEmail);
    strcpy(database->List[database->DataBase_Size].job_title,newJob);
    database->List[database->DataBase_Size].phone = newPhone;

    database->List[database->DataBase_Size].birth_date.day = newDate->day;
    database->List[database->DataBase_Size].birth_date.month = newDate->month;
    database->List[database->DataBase_Size].birth_date.year = newDate->year;

    //Update the size of the database
    database->DataBase_Size = database->DataBase_Size + 1;

    return 1;
}

int Load(DATABASE* database, char* file_name){

    FILE *DataFile = fopen(file_name,"r");
    if (DataFile == NULL)
    {
        printf(OPEN_FILE_FAILURE);
        return -1;
    }
    int i = 0;

    //Needed variables to use the Add function to add new records
    char* line = (char*)malloc(120*sizeof(char));
    if(line == NULL){printf(MEMORY_ALLOCATION_FAILURE); return -1;}
    char** dataArray = (char**)malloc(5*sizeof(char*));
    if(dataArray == NULL){printf(MEMORY_ALLOCATION_FAILURE); return -1;}

    unsigned long long int newPhone = 0;
    DATE* newDate = (DATE*)malloc(sizeof(DATE));
    if(newDate == NULL){printf(MEMORY_ALLOCATION_FAILURE); return -1;}

    for(i = 0; i < 5; i++){
        dataArray[i] = (char*)malloc(MAX_LINE_LENGTH*sizeof(char));
        if(dataArray[i] == NULL){printf(MEMORY_ALLOCATION_FAILURE); return -1;}
    }

    //Redundant characters in the beginning of the file
    fgetc(DataFile);
    fgetc(DataFile);
    fgetc(DataFile);
    while(!feof(DataFile)){
        fscanf(DataFile,"%[^\n]%*c",line);
        line++;
        line[strlen(line) - 1] = '\0';

        //dataArray is an array of strings to store the 5 pieces of information of the 5 fields
        parseLine(line, dataArray);
        line--;

        //Handling Errors while trying adding records
        if(ParseName(dataArray[0]) == 0){
            printf("Skipping this record: %s, ...\n\n",dataArray[0]);
            continue;
        }else if(VerifyExisting(dataArray[0], database, name) == 1){
            printf(DATA_EXISTS_ALREADY);
            printf("Skipping this record: %s, ...\n\n",dataArray[0]);
            continue;
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////

        newPhone = ParsePhone(dataArray[1]);
        if(newPhone == 0){
            printf("Skipping this record: %s, ...\n\n",dataArray[0]);
            continue;
        }else if(VerifyExisting(dataArray[1], database, phone) == 1){
            printf(DATA_EXISTS_ALREADY);
            printf("Skipping this record: %s, ...\n\n",dataArray[0]);
            continue;
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////
        if(ParseEmail(dataArray[2]) == 0){
            printf("Skipping this record: %s, ...\n\n",dataArray[0]);
            continue;
        }else if(VerifyExisting(dataArray[2], database, email) == 1){
            printf(DATA_EXISTS_ALREADY);
            printf("Skipping this record: %s, ...\n\n",dataArray[0]);
            continue;
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////
        if(ParseDate(dataArray[3], newDate) == 0){
            printf("Skipping this record: %s, ...\n\n",dataArray[0]);
            continue;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        if(ParseJob(dataArray[4]) == 0){
            printf("Skipping this record: %s, ...\n\n",dataArray[0]);
            continue;
        }

        Add(database,dataArray[0],newPhone,dataArray[2],newDate,dataArray[4]);
        printf("Adding the record to the database: %s, ...\n\n",dataArray[0]);

    }

    //We have to free EVERY allocated memory and not to forget a single one not to be accumulated
    //and may cause errors and memory leak
    free(newDate);
    free(line);
    for(i = 0; i < 5; i++){
        free(dataArray[i]);
    }
    free(dataArray);

    return 1;
}

int Delete(DATABASE* database, char* theName){
    int i = 0;
    int dataBaseSize = database->DataBase_Size;

    //Check the format and the existence
    if(ParseName(theName) == 0){
        printf(NAME_INVALID_FORMAT);
        return 0;
    }else if(VerifyExisting(theName, database, name) == 0){
        printf(NAME_EXISTANCE);
        return 0;
    }

    char* tempLine1 = (char*)malloc(MAX_LINE_LENGTH*sizeof(char));
    char* tempLine2 = (char*)malloc(MAX_LINE_LENGTH*sizeof(char));
    strcpy(tempLine1,theName);
    lowerCase(tempLine1);

    for(i = 0; i < dataBaseSize; i++){
        strcpy(tempLine2,database->List[i].name);
        lowerCase(tempLine2);
        if(strcmp(tempLine1, tempLine2) == 0) break;
    }
    free(tempLine1);
    free(tempLine2);

    //Perform the omitting process and shift all the records up by one record which is deleted
    deleteRecord(database,i);
    return 1;

}

int Save(DATABASE* database, char* file_name){
    int i = 0, dataSize = database->DataBase_Size;
    FILE *DataFile = fopen(file_name,"w");
    if (DataFile == NULL)
    {
        printf(OPEN_FILE_FAILURE);
        return -1;
    }

    //Writing to the file record by record
    for(i = 0; i < dataSize; i++){

        fprintf(DataFile, "\"%s, ",database->List[i].name);
        fprintf(DataFile, "0%llu, ",database->List[i].phone);
        fprintf(DataFile, "%s, ",database->List[i].email);
        fprintf(DataFile, "%d/",database->List[i].birth_date.day);
        fprintf(DataFile, "%d/",database->List[i].birth_date.month);
        fprintf(DataFile, "%d, ",database->List[i].birth_date.year);
        //fprintf(DataFile, "%s\"\n",database->List[i].job_title);

        if(i + 1 != dataSize){
            fprintf(DataFile, "%s\"\n",database->List[i].job_title);
        }else{ //Not to have a new empty line in the file when reaching to the final record
            fprintf(DataFile, "%s\"",database->List[i].job_title);
        }

    }
    fclose(DataFile);
    return 1;

}

int Search(DATABASE* database, char* theName){
    int i = 0, dataSize = database->DataBase_Size;
    char* tempLine1 = (char*)malloc(MAX_LINE_LENGTH*sizeof(char));
    char* tempLine2 = (char*)malloc(MAX_LINE_LENGTH*sizeof(char));
    strcpy(tempLine1,theName);
    lowerCase(tempLine1);

    //Check format and existence
    if(ParseName(theName) == 0){
        printf(NAME_INVALID_FORMAT);
        return -1;
    }else if(VerifyExisting(theName, database, name) == 0){
        printf(NAME_EXISTANCE);
        return -1;
    }

    //Search for the name in all of the records
    for(i = 0; i < dataSize; i++){
        strcpy(tempLine2,database->List[i].name);
        lowerCase(tempLine2);
        if(strcmp(tempLine1,tempLine2) == 0){
            free(tempLine1);
            free(tempLine2);
            return i;
        }
    }

    //printf("Should NOT proceed to here!!!\n");
    return -1;

}

int Modify(DATABASE* database, char* theName, FIELD field){
    int i = 0;
    int dataSize = database->DataBase_Size;

    unsigned long long int newPhone = 0;
    char* line = (char*)malloc(MAX_LINE_LENGTH*sizeof(char));
    char* tempLine1 = (char*)malloc(MAX_LINE_LENGTH*sizeof(char));
    char* tempLine2 = (char*)malloc(MAX_LINE_LENGTH*sizeof(char));
    strcpy(tempLine1,theName);
    lowerCase(tempLine1);

    //Modification can done on phone, email and job title only
    switch(field){
    case phone:
        printf("New Phone Number: ");
        fflush(stdin);
        scanf("%[^\n]%*c", line);
        fflush(stdin);
        OmitTailSpaces(line);

        newPhone = ParsePhone(line);
        if(newPhone == 0){
            return 0;
        }else if(VerifyExisting(line, database, phone) == 1){
            printf(DATA_EXISTS_ALREADY);
            return 0;
        }

        for(i = 0; i < dataSize; i++){
            strcpy(tempLine2,database->List[i].name);
            lowerCase(tempLine2);
            if(strcmp(tempLine1,tempLine2) == 0){
                database->List[i].phone = newPhone;
                free(line);
                free(tempLine1);
                free(tempLine2);
                return 1;
            }
        }

        break;
    case email:
        printf("New Email: ");
        fflush(stdin);
        scanf("%[^\n]%*c", line);
        fflush(stdin);
        OmitTailSpaces(line);

        if(ParseEmail(line) == 0){
            return 0;
        }else if(VerifyExisting(line, database, email) == 1){
            printf(DATA_EXISTS_ALREADY);
            return 0;
        }

        for(i = 0; i < dataSize; i++){
            strcpy(tempLine2,database->List[i].name);
            lowerCase(tempLine2);
            if(strcmp(tempLine1,tempLine2) == 0){
                strcpy(database->List[i].email,line);
                free(line);
                free(tempLine1);
                free(tempLine2);
                return 1;
            }
        }

        break;
    case job_title:
        printf("New Job Title: ");
        fflush(stdin);
        scanf("%[^\n]%*c", line);
        fflush(stdin);
        OmitTailSpaces(line);

        if(ParseJob(line) == 0){
            return 0;
        }

        for(i = 0; i < dataSize; i++){
            strcpy(tempLine2,database->List[i].name);
            lowerCase(tempLine2);
            if(strcmp(tempLine1,tempLine2) == 0){
                strcpy(database->List[i].job_title,line);
                free(line);
                free(tempLine1);
                free(tempLine2);
                return 1;
            }
        }

        break;

    case name:
    case birth_date:
        break;
    }
    fflush(stdin);
    return 1;

}

void Sort(DATABASE* database, FIELD field, int direction){
    //We have 4 possible sorting paths:
    //Name or Date
    //Ascending or Descending
    if(direction == 2){
        if(field == name){
            qsort(database->List,database->DataBase_Size,sizeof(EMPLOYEE),descendingName);
        }else{
            qsort(database->List,database->DataBase_Size,sizeof(EMPLOYEE),descendingDate);
        }
    }else{
        if(field == name){
            qsort(database->List,database->DataBase_Size,sizeof(EMPLOYEE),ascendingName);
        }else{
            qsort(database->List,database->DataBase_Size,sizeof(EMPLOYEE),ascendingDate);
        }
    }
}

void Manual(void){
    printf("DISPLAY: Print the whole database into the console\n");
    printf("This Database Manager provides you with those functionalities:\n");
    printf("Add: To add any record with the full information to the database\n");
    printf("Load: To load records stored in a text file into the database\n");
    printf("Delete: To delete any record from the database\n");
    printf("Save: To save the whole database into a text file\n");
    printf("Search: To search for a specific record associated with a specific name in the database\n");
    printf("Modify: To modify and change any field from any record (names and birth dates are immune to be changed)\n");
    printf("Sort: To Sort the database according to the names or birth dates\n");
    printf("Quit: To quit and close the database manager\n");

}

//Print the whole database to the console
void printDataBase(DATABASE* FullDB){
    int i = 0;
    char* DateStr = (char*)malloc(MAX_Date_LENGTH*sizeof(char));
    for(i = 0; i < FullDB->DataBase_Size; i++){
        printf("\"%s, ",FullDB->List[i].name);
        printf("0%llu, ",FullDB->List[i].phone);
        printf("%s, ",FullDB->List[i].email);

        DateStr[0] = (FullDB->List[i].birth_date.day / 10) + 48;
        DateStr[1] = (FullDB->List[i].birth_date.day % 10) + 48;
        DateStr[2] = '/';
        DateStr[3] = (FullDB->List[i].birth_date.month / 10) + 48;
        DateStr[4] = (FullDB->List[i].birth_date.month % 10) + 48;
        DateStr[5] = '/';
        DateStr[6] = (FullDB->List[i].birth_date.year / 1000) + 48;
        DateStr[7] = ((FullDB->List[i].birth_date.year%1000) / 100) + 48;
        DateStr[8] = ((FullDB->List[i].birth_date.year%100) / 10) + 48;
        DateStr[9] = (FullDB->List[i].birth_date.year % 10) + 48;
        DateStr[10] = '\0';
        printf("%s, ",DateStr);
        printf("%s\"\n",FullDB->List[i].job_title);
    }
    free(DateStr);
}

//Delete a specific record in the database and rearrange them by shifting them up by the deleted record
void deleteRecord(DATABASE* database, int index){
    int i = 0, databaseSize = database->DataBase_Size;
    for(i = index; i < databaseSize - 1; i++){
        strcpy(database->List[i].name,database->List[i+1].name);
        strcpy(database->List[i].email,database->List[i+1].email);
        strcpy(database->List[i].job_title,database->List[i+1].job_title);
        database->List[i].phone = database->List[i+1].phone;
        database->List[i].birth_date.day = database->List[i+1].birth_date.day;
        database->List[i].birth_date.month = database->List[i+1].birth_date.month;
        database->List[i].birth_date.year = database->List[i+1].birth_date.year;

    }
    database->DataBase_Size = database->DataBase_Size - 1;
}
