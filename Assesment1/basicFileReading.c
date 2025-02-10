/*4.
* Basic File Reading (5 marks): 
* Create a C program that reads a text file containing a list of
* numbers, one per line, and calculates their sum. 
* The file name should be passed as a command-line argument.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// assuming that the default extension for the file is .txt
#define DEFAULT_EXTENSION ".txt"

// function  prototypes 
double calculateSum(FILE *pFile);
int validateNumber(char *pLine);


int main(int argc, char *argv[]){

    FILE *pFile; 


    // Check if the file name is provided as command-line arg. 
    if (argc !=2) {
        printf("Usage: %s <filename> \n", argv[0]);
        return 1; 
    }

    /*
    * Create a copy of the file name provided as command-line arg. 
    * Is not possible to modify directly argv[1] 
    * strdup() duplicates a string by allocating new memory and copying the original string into it.
    */
    char *pFileName = strdup(argv[1]);
    if(pFileName == NULL){
        printf("Memory allocation failed \n");
        return 1; 
    }
    
    /*
    * strchr() searches for the first occurrence of a character in a string.
    */
    if(strchr(pFileName,'.') == NULL) { 
        // If no extension provided, append the default one as #define 
        size_t new_length = strlen(pFileName)+strlen(DEFAULT_EXTENSION)+1;
        pFileName = realloc(pFileName,new_length);
        strcat(pFileName, DEFAULT_EXTENSION);
    }

    // Open the file --> read mode 
    pFile = fopen(pFileName,"r");
    /*
    * Safety First 
    * Check if the pointer to file is NULL 
    */
    if(pFile == NULL){
        /*
        * Notify error with a description using the function perror() 
        */
        perror("Erorr opening file");
        // free the dynamically allocated memory for the pointer to fileName
        free(pFileName); 
        return 1; 
    }

    /*
    * Calculate the sum by calling the function calculateSum() and notify 
    */
    printf("The sum of the numbers is: %.2f \n", calculateSum(pFile));

    // Close the file 
    fclose(pFile);

    return 0; 
}


/*
* Argument of the function is the pointer to the file that has to be parsed
* Reads the given file line by line, check the number with the function validateNumber()
* comvert the string value in double and return the sum of numbers
*/
double calculateSum(FILE *pFile){
    // Buffer to store each line 
    char line[256]; 
    double sum = 0.0;

    // Read the file line by line 
    while(fgets(line,sizeof(line),pFile)){
        // Remove the /n char at the end of the line 
        line[strcspn(line,"\n")] = 0; 
        //Check that the value of the new line is a number
        if(validateNumber(line)){
            //convert the string to a number with atof() function and add to the var sum 
            sum += atof(line);
        }
    }


    return sum; 
}

/*
* Argument of the function is the pointer to the line of the file 
* Check if the line contains a valid number (1.0 or -10)
* use sscanf() to try parsing the line as number, if succeeds, the functions returns 1
* so the line contains a valid number
*/
int validateNumber(char *pLine){

    double temp; 
    /*
    */
    return sscanf(pLine, "%lf", &temp) == 1; 
}

