/*7.
* Write Write a C program that simulates basic file
* operations, emphasizing appending data to a file and sequential reading. The program
* should offer two functionalities:
* • Append data to a text file. When this option is selected, the program should prompt the
* user to enter the data to be appended. The program will then write this data to the end of
* ‘example.txt’.
* • Read data from a text file. When this option is selected, the program should read and
* display the contents of 'example.txt' from the beginning.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* DEFINE */
#define FILE_NAME "example.txt"
#define DATA_SIZE 1024
#define WRITE "w"
#define READ "r"

typedef struct 
{
    FILE *pFile; 
    char name[20];
} basicFile; 

/* Function prototypes */
void initFile(basicFile* myFile);
void appendToFile(basicFile* myFile, char *data); 
void readFromFile(basicFile* myFile); 


int main(){
    /* Local vars */
    basicFile myFile; 
    int choice; 
    char dataWrite[DATA_SIZE]; 

    /* Calling initFile() */
    initFile(&myFile);


    while (1)
    {
        printf("\nFile operation menù \n"
                "1: Write to File \n"
                "2: Read from File \n"
                "3: Exit \n"
                "Choose an option: \n"
        );
        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input! Please enter a number \n");
            while (getchar() != '\n') /* clean the buffer*/
            continue;
        }

        getchar();

        switch (choice)
        {
        /* ask data from user and write it into file calling appendToFile() */
        case 1:
            printf("Enter data to write : "); 
            fgets(dataWrite,DATA_SIZE, stdin); 
            dataWrite[strcspn(dataWrite,"\n")] = 0; 
            appendToFile(&myFile,dataWrite); 
            break;
        /* Reading content from file, calling readFromFile() */
        case 2: 
            readFromFile(&myFile);
            break; 
        /* Exit the program */
        case 3: 
            printf("Exiting the program...\n"); 
            exit(0); 
        default:
            printf("Select a valud value from menù \n"); 
            break;
        }
    }
    


    return 0; 
}

/*
* Function name     : initFile
* Arguments         : basicFile*  = Pointer to the basicFile data structure
* Return value/s    : void        = does not return any value.
* Remarks           : Initializes the basicFile structure, creates the text file if not 
*                     already existing, and opens the file for writing. If the file cannot
*                     be created or opened, an error message is displayed.
*/
void initFile(basicFile* myFile){

    /* Init file structure */
    strcpy(myFile->name, FILE_NAME); 
    myFile->pFile = fopen(myFile->name,READ); 

    /* Check if file already exists */
    if (myFile->pFile)
    {
        char choice; 
        /* Notify ! */
        printf("File '%s' already exists. Do you want to reinitialize it? (y/n)",myFile->name); 
        scanf("%c", &choice); 
        getchar(); /* Clear the new line*/

        if (tolower(choice) == 'y')
        {
            myFile->pFile = fopen(myFile->name,WRITE); /* Re-init file */
            if (!myFile->pFile)
            {
                /* Notify ! */
                perror("Error reinitializing the file\n"); 
                return; 
            }
            /* Notify ! */
            printf("File '%s' has been reinitialized\n", myFile->name); 
            /* Always close the file after operation */
            fclose(myFile->pFile); 
        } else
        {
            /* Notify ! */
            printf("Using existing file \n"); 
        }
    }
    else
    {
        /* Create the file if does not exist */
        myFile->pFile = fopen(myFile->name, WRITE); 
        if (!myFile->pFile)
        {
            /* Notify ! */
            perror("Error creating the file\n"); 
            return; 
        }
        /* Notify ! */
        printf("File %s created successfully.\n", myFile->name);
        /* Always close the file after operation */
        fclose(myFile->pFile); 
    }
}

/*
* Function name     : appendToFile
* Arguments         : basicFile*  = Pointer to the basicFile data structure
*                     char* data  = String to be appended to the file
* Return value/s    : void        = does not return any value.
* Remarks           : Opens the file in append mode ("a+") and appends the given data to 
*                     the end of the file. If the file cannot be opened, an error message is displayed.
*/
void appendToFile(basicFile* myFile, char *data){

    myFile->pFile = fopen(myFile->name, "a+"); 

    if (!myFile->pFile)
    {
        /* Notify! */ 
        perror("Error opening the file \n");
        return; 
    }
    /*fputs(const char *str, FILE *stream);*/
    fputs(data,myFile->pFile);
    /* Append new line char for correct separation of data */
    fputs("\n", myFile->pFile);   
    printf("Datta appended to file successfully\n"); 
    /* Always close the file after operation */
    fclose(myFile->pFile); 
}
/*
* Function name     : readFromFile
* Arguments         : basicFile*  = Pointer to the basicFile data structure
* Return value/s    : void        = does not return any value.
* Remarks           : Opens the file in read mode ("r") and reads its contents line by line.
*                     If the file cannot be opened, an error message is displayed. 
*                     After reading the file, the contents are printed to the console.
*/
void readFromFile(basicFile* myFile){
    
    char buffer[DATA_SIZE];
    myFile->pFile = fopen(myFile->name, READ); 

    if (!myFile->pFile)
    {
        /* Notify! */ 
        perror("Error opening the file \n");
        return; 
    }
    
    while (fgets(buffer,sizeof(buffer), myFile->pFile))
    {
        printf("%s", buffer); 
    }

    /* Always close the file after operation */
    fclose(myFile->pFile);
}