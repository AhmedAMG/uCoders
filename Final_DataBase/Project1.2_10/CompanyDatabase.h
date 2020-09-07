/* Created By: Ahmed Mohammed Gamal Al-Din */
/*              4/8/2019                   */
/*         uCoders >>> ID : 10             */
/*            Project: Part2               */
/*           Database Manager              */

#ifndef __DATABASE_H_
#define __DATABASE_H_

typedef struct{
    int day;
    int month;
    int year;
}DATE;

typedef struct{
    char* name;
    unsigned long long int phone;
    char* email;
    DATE birth_date;
    char* job_title;
}EMPLOYEE;

typedef enum{
    name = 0, phone, email, birth_date, job_title
}FIELD;

typedef struct{
    int DataBase_Size;
    EMPLOYEE* List;
}DATABASE;


int Add(DATABASE* database, char* newName, unsigned long long int newPhone, char* newEmail, DATE* newDate, char* newJob);
int Load(DATABASE* database, char* file_name);
int Delete(DATABASE* database, char* name);
int Save(DATABASE* database, char* file_name);
int Search(DATABASE* database, char* name);
int Modify(DATABASE* database, char* name, FIELD field);
void Sort(DATABASE* database, FIELD field, int direction);
void Manual(void);
void printDataBase(DATABASE* FullDB);
void deleteRecord(DATABASE* database, int index);

#endif // __DATABASE_H_
