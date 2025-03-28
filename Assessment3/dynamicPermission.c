/*8.
* Develop a C program that allows users to
* selectively change read, write, and execute permissions of a given file. The program should
* prompt the user for the file name and the specific permissions to be modified. The program
* must then display the original permissions, make the change in permissions, and then
* display the new permissions.
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h> /* Contains functions for file status retrieval */
#include <string.h>

/* function prototypes */
int isValidFile(char* fileName); 

int main(){

    /* Local vars */
    char fileName[256]; 
    int newPermission, originalPermission; 
    struct stat st; 

    /* Get fileName from user */
    printf("Enter the file name: ");
    /* Check if the file name is valid */
    if (fgets(fileName, sizeof(fileName),stdin) == NULL)
    {
        printf("Error reading the file name \n"); 
        return 1; 
    }
    /* Remove newline char */
    fileName[strcspn(fileName, "\n")] = 0; 

    /* Check if the file exist calling isValidFile()*/
    if (!isValidFile(fileName))
    {
        printf("Error! File does not exists\n"); 
        return 1; 
    }
    /* Get new permission from user */
    printf("Enter new permission in octal (e.g., 0644) : "); 
    scanf("%o", &newPermission);

    /* Get original permission from given file */
    stat(fileName, &st); 
    /* masks to get only permission bits */
    originalPermission = st.st_mode & 0777; 
    
    /* Notify original permission */
    printf("Before changing -> permission: %04o \n", originalPermission); 

    /* Change permission */
    if (chmod(fileName,newPermission) != 0)
    {
        /* Notify error */
        perror("Error changing permission \n"); 
    }
    
    /* Notify new permissions */
    stat(fileName, &st); 
    int tempPermission = st.st_mode & 0777; 
    printf("After changing -> permission: %04o \n", tempPermission); 

    return 0; 
}

/*
* Function name     : isValidFile
* Arguments         : char*       = Pointer to the filename string to check
* Return value      : int = 1 if file exists, 0 if file does not exist
* Remarks           : Uses stat() to check if the file exists by attempting to retrieve file status
*/
int isValidFile(char* fileName){
    struct stat st; 
    return stat(fileName, &st) == 0;
}
