/* Created By: Ahmed Mohammed Gamal Al-Din */
/*              4/8/2019                   */
/*         uCoders >>> ID : 10             */
/*            Project: Part2               */
/*           Database Manager              */

///The code uses DMA intensively for saving data and do the required processing

/*
    This Database Manager provides you with those functionalities:
    DISPLAY: Print the whole database into the console
    Add: To add any record with the full information to the database
    Load: To load records stored in a text file into the database
    Delete: To delete any record from the database
    Save: To save the whole database into a text file
    Search: To search for a specific record associated with a specific name in the database
    Modify: To modify and change any field from any record (names and birth dates are immune to be changed)
    Sort: To Sort the database according to the names or birth dates
    Quit: To quit and close the database manager
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "CompanyDatabase.h"
#include "Utility.h"

int main()
{
    //Needed variables for processing
    int choice = 0, sortDirection = 0, verify = 0;
    char* line = (char*)malloc(MAX_LINE_LENGTH*sizeof(char));
    unsigned long long int newPhone = 0;

    //Create the database and initialize its size
    DATABASE* employees_data = (DATABASE*)malloc(sizeof(DATABASE));
    if(employees_data == NULL){printf(MEMORY_ALLOCATION_FAILURE); return 0;}
    employees_data->DataBase_Size = 0;
    employees_data->List = (EMPLOYEE*)malloc(sizeof(EMPLOYEE));
    if(employees_data->List == NULL){printf(MEMORY_ALLOCATION_FAILURE); return 0;}

    printf("--- Welcome to Facebook Database ---\n");
    while(1){

        //Display the available functions to the user
        printf("\nChoose the operation you want to perform:\n");
        printf("0.Display\t1.ADD\t\t2.LOAD\t\t3.DELETE\t4.SAVE\n");
        printf("5.SEARCH\t6.MODIFY\t7.HELP\t\t8.SORT\t\t9.QUIT\n\n");

        //Get the input from the user by one keystroke
        fflush(stdin);
        choice = getch();
        //Keeping in mind the (ctrl+c) control character to handle exiting the program.
        if(choice == 3) return 0;
        //Get the numeric value of the numbers not dealing with their ASCII values
        else choice = choice - 48;
        fflush(stdin);

        //Allocating some memory for the required variables needed for the processing
        char* newName = (char*)malloc(MAX_NAME_LENGTH*sizeof(char));
        if(newName == NULL){printf(MEMORY_ALLOCATION_FAILURE); return 0;}

        char* newEmail = (char*)malloc(MAX_EMAIL_LENGTH*sizeof(char));
        if(newEmail == NULL){printf(MEMORY_ALLOCATION_FAILURE); return 0;}

        DATE* newDate = (DATE*)malloc(sizeof(DATE));
        if(newDate == NULL){printf(MEMORY_ALLOCATION_FAILURE); return 0;}

        char* newJob = (char*)malloc(MAX_Job_LENGTH*sizeof(char));
        if(newJob == NULL){printf(MEMORY_ALLOCATION_FAILURE); return 0;}
        //Variables are JUST needed when adding a new record to the database >>> So we have to deallocate them as long as we
        //will not add a new record and do some other thing instead
        if(choice != 1){
            free(newName);
            free(newEmail);
            free(newDate);
            free(newJob);
        }

        //Choose which path I should execute according to the user choice
        switch(choice){
        case 0:
            //Print the whole Database into the console
            printDataBase(employees_data);
            break;
        case 1:
            //Scanning and store the name in the record we will add
            printf("Adding an employee to the database ...\n");
            printf("Please, Provide the following information about the employee:\n");
            //printf("NOTE: Names should have only alphabets and spaces\n");

            do{
                printf("Name: ");
                fflush(stdin);
                scanf("%[^\n]%*c", line);
                fflush(stdin);
                //Delete any space at the end of the string not to have problems checking it
                OmitTailSpaces(line);

                //Checking that the name does NOT violate any rule bounded to names
                //Checking whether there is a record already with that name in the database or not
                verify = VerifyExisting(line, employees_data, name);
                if(verify == 1) printf(DATA_EXISTS_ALREADY);
            }while(ParseName(line) == 0 || verify == 1);
            strcpy(newName,line);

            /////////////////////////////////////////////////////////////////////////////////////////////////////
            //Scanning and store the phone number in the record we will add
            //printf("NOTE: Phone numbers format: '01YXXXXXXXX' where X is numbers from 0 to 9 and Y is one of those 0,1,2,5\n");
            do{
                printf("Phone Number: ");
                fflush(stdin);
                scanf("%[^\n]%*c", line);
                fflush(stdin);
                OmitTailSpaces(line);

                //Convert that number from a string into an integer to make processing easier on it
                //Checking that the phone number does NOT violate any rule bounded to phone numbers
                newPhone = ParsePhone(line);

                verify = VerifyExisting(line, employees_data, phone);
                if(verify == 1) printf(DATA_EXISTS_ALREADY);
            }while(newPhone == 0 || verify == 1);

            //////////////////////////////////////////////////////////////////////////////////////////////////////
            //Scanning and store the email in the record we will add
            //printf("NOTE: Emails should be in the form: Something@website.com\n");
            do{
                printf("Email: ");
                fflush(stdin);
                scanf("%[^\n]%*c", line);
                fflush(stdin);
                OmitTailSpaces(line);

                //Checking that the email does NOT violate any rule bounded to emails
                //Checking whether there is a record already with that email in the database or not
                verify = VerifyExisting(line, employees_data, email);
                if(verify == 1) printf(DATA_EXISTS_ALREADY);
            }while(ParseEmail(line) == 0 || verify == 1);
            strcpy(newEmail,line);

            ///////////////////////////////////////////////////////////////////////////////////////////////////////
            //Scanning and store the birth date in the record we will add
            //printf("NOTE: Dates should be in the form: DD/MM/YYYY\n");
            do{
                printf("Birthday Date: ");
                fflush(stdin);
                scanf("%[^\n]%*c", line);
                fflush(stdin);
                OmitTailSpaces(line);
            //Checking that the birth date does NOT violate any constraints bounded to birth dates in that company
            }while(ParseDate(line, newDate) == 0);

            ////////////////////////////////////////////////////////////////////////////////////////////////////////
            //Scanning and store the job title in the record we will add
            //printf("NOTE: Job Titles that are valid are only those exist in the company's job list\n");
            do{
                printf("Job title: ");
                fflush(stdin);
                scanf("%[^\n]%*c", line);
                fflush(stdin);
                OmitTailSpaces(line);
            //Checking that the job title already exists in the job titles list of that company
            }while(ParseJob(line) == 0);
            strcpy(newJob,line);

            //Add this record to the database
            Add(employees_data, newName, newPhone, newEmail, newDate, newJob);
            printf("DONE.\n");

            //Deallocate all allocated variables needed here only
            free(newName);
            free(newEmail);
            free(newDate);
            free(newJob);

            break;
        case 2:
            //Load the whole database from a selected file
            printf("Please, Enter the file name: ");
            fflush(stdin);
            scanf("%[^\n]%*c", line);
            fflush(stdin);
            OmitTailSpaces(line);
            //Handling all kinds of errors that can happen from loading records into the database
            if(Load(employees_data,line) == -1) continue;
            printf("DONE.\n");

            break;
        case 3:
            //Delete a record from the database using the name associated with that record
            printf("Please, Enter the name in the required record to delete: ");
            fflush(stdin);
            scanf("%[^\n]%*c", line);
            fflush(stdin);
            OmitTailSpaces(line);
            //Handling all kinds of errors that can happen from trying deleting a record
            if(Delete(employees_data,line) == 0) continue;
            printf("DONE.\n");


            break;
        case 4:
            //Save the whole database in a text file
            printf("Please, Enter the name of the file to save the database in: ");
            fflush(stdin);
            scanf("%[^\n]%*c", line);
            fflush(stdin);
            OmitTailSpaces(line);
            //Handling all kinds of errors that can happen from saving the database in the file
            if(Save(employees_data,line) != 1) continue;
            printf("DONE.\n");

            break;
        case 5:
            //Searching for a specific record using the name associated with it
            printf("Please, Enter the name you want to search for: ");
            fflush(stdin);
            scanf("%[^\n]%*c", line);
            fflush(stdin);
            OmitTailSpaces(line);
            choice = Search(employees_data,line);
            //If there were no errors, print the record required by the user
            if(choice != -1){
                printf("\"%s, ",employees_data->List[choice].name);
                printf("0%llu, ",employees_data->List[choice].phone);
                printf("%s, ",employees_data->List[choice].email);
                printf("%d/",employees_data->List[choice].birth_date.day);
                printf("%d/",employees_data->List[choice].birth_date.month);
                printf("%d, ",employees_data->List[choice].birth_date.year);
                printf("%s\"\n",employees_data->List[choice].job_title);
            }

            break;
        case 6:
            //Modify and change a record's information (Names and birth dates can NOT be modified)
            printf("Please, Enter the name of the required record to modify:\n");
            printf("Name: ");
            fflush(stdin);
            scanf("%[^\n]%*c", line);
            fflush(stdin);
            OmitTailSpaces(line);

            if(ParseName(line) == 0){
                printf(NAME_INVALID_FORMAT);
                continue;
            }
            //Checking whether there is a record already with that name in the database or not
            else if(VerifyExisting(line, employees_data, name) == 0){
                printf(NAME_EXISTANCE);
                continue;
            }

            //Choosing the required field in the record to modify
            do{
                printf("Note: Names and birth dates are immune against any modification\n");
                printf("Please, Enter the field to modify:\n");
                printf("1.Phone\t2.Email\t3.Job Title\n");
                printf("Your choice: ");
                fflush(stdin);
                scanf("%d", &choice);
                fflush(stdin);
            }
            while(choice < phone || choice > birth_date);
            //Adjust the numbers to be easily refer using enum to make the code more understandable
            if(choice == birth_date) choice = job_title;
            //Handling all kinds of errors that can happen from modifying records
            if(Modify(employees_data,line,choice) != 1) continue;
            printf("DONE.\n");

            break;
        case 7:
            //Help the user with the provided functions functionalities
            Manual();
            break;

        case 8:
            //Sorting the database according to names OR birth dates
            do{
                printf("Want to sort the database according to names or birth dates?\n");
                printf("1.Names\n2.Birth dates\n");
                fflush(stdin);
                choice = getch() - '0';
                fflush(stdin);
            }while(choice != 1 && choice != 2);
            //Adjusting numbers to match enum values to use those words directly
            if(choice == 1) choice = name;
            else if(choice == 2) choice = birth_date;
            //Sorting the database according in an ascending order or a descending one
            do{
                printf("Want to sort the database in an ascending order or a descending one?\n");
                printf("1.Ascending\n2.Descending\n");
                fflush(stdin);
                sortDirection = getch() - '0';
                fflush(stdin);
            }while(sortDirection != 1 && sortDirection != 2);

            Sort(employees_data,choice,sortDirection);
            printf("DONE.\n");
            break;

        case 9:
            //Ask the user about the procedure he wants to do before exiting
            printf("Do you really want to quit?\n");
            printf("1.Save and Exit\n2.Exit without saving\n3.Cancel\n");

            fflush(stdin);
            choice = getch() - '0';
            fflush(stdin);

            switch(choice){
            case 1:
                printf("Saving and Exiting...\n");
                Save(employees_data,"DataBase.txt");
                return 0;
                break;
            case 2:
                printf("Exiting...\n");
                return 0;
                break;
            case 3:
                continue;
                break;
            }

            break;
        }
    }

    return 0;
}

