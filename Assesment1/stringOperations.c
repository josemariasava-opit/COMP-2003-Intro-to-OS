/*2.
* String Operations (5 marks): Implement a C function that takes two strings as input and
* returns a new string that is the concatenation of the first string followed by the second.
* Ensure proper memory management to avoid buffer overflows.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// function  prototypes 
char* concatenateString(char *pString1, char *pString2);

int main(){ 

    char string1[100],string2[100];
    char *pConcatenatedString = NULL; 
    
    printf("Insert first string : ");
    fgets(string1,sizeof(string1),stdin); 

    printf("Insert second string : ");
    fgets(string2,sizeof(string2),stdin); 
    /*
    * Removing trailing newline character from fgets() 
    * strcspn() helps find the first occurrence of any character from a set in a given string.
    */
    string1[strcspn(string1,"\n")] = 0; 
    string2[strcspn(string2,"\n")] = 0; 

    // Calling concatenateString() function
    pConcatenatedString = concatenateString(string1,string2); 
    printf("---------------------------------------------------\n");
    printf("Concatenated String: %s \n", pConcatenatedString); 

    // Free the allocated memory
    free(pConcatenatedString);

    return 0; 
}


/*
* Argument of the function are the pointer to string1 and string2
* concatenateString() check lenght of both strings and allocate the memory for the new string
* the +1 is the size of the null terminator, that marks the end of the string  
* copies the content of the String1, char by char, inside the new String 
* concatenate the content of the String2, char by char, inside the new String
*/
char* concatenateString(char *pString1, char *pString2){

    // Allocate memory for the new string
    char *pStringResult = NULL; 
    size_t len1 = strlen(pString1); // get length of the String1 
    size_t len2 = strlen(pString2); // get length of the String2 
    size_t total = len1 + len2 +1; // string require a null terminator to mark the end

    pStringResult = malloc(total * sizeof(char)); 
    if (pStringResult== NULL){ 
        printf("Memory allocation error for new string. \n"); 
        return NULL;
    }

    // Copy char one by one from pString1
    for (int i = 0; i < len1; i++)
    {
        pStringResult[i] = pString1[i]; 
    }
    // concatenate char of pString2
    for (int j = 0; j < len2; j++)
    {
        pStringResult[len1+j] = pString2[j]; 
    }

    // Add the NULL terminator at the end of the string 
    pStringResult[len1+len2] = '\0'; 

    // return the new string
    return pStringResult; 
}