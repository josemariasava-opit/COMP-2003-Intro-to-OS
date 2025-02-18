/*3.
* Command Line Argument Processing (5 marks): Write a program in C that takes
* command-line arguments representing integer values and sorts them in ascending order.
* The sorted numbers should be printed to the console.
*/
#include <stdio.h>
#include <stdlib.h>

/* function  prototypes */
int* bubbleSort(int *pArray, int size);


int main(int argc, char *argv[]){

    int *pArray = NULL;
    int size = 0; 

    /* At leat one values is provided by command line*/
    if(argc < 2 ){
        printf("Usage: %s <Integer_values1> <Integer_values2> ...\n", argv[0]);
        return 1; 
    }

    /*
    * argc count all the command-line arguments + the program name 
    * argv[argc=0] is the program name itself
    */
    size  = argc - 1; 
    /* Allocate memory for the Array*/
    pArray = malloc(size * sizeof(int)); 
    
    /* Check correct memory allocation*/
    if (pArray == NULL)
    {
        printf("Memory Allocation failed \n");
        return 1; 
    }


    /*
    * convert from char to int and populate array using strtol()
    * strtol() conversion returns a long integer, and cast it to integer. Used base 10 specify the decimal numbers. 
    * *pEnd check that the char was converted correctly to a valid integer. 
    * points to the first character in the string that was not converted. 
    */
    for (int i = 0; i < size; i++)
    {
        char *pEnd;
        /*
        * strtol syntax
        * strtol(const char *str, char **endptr, int base);
        */
        pArray[i] = strtol(argv[i+1],&pEnd,10);  

        /*
        * if *pEnd points to the null terminator ('\0') indicates the end of the string, so the char is valid and the conversion was successful
        * if *pEnd points to a char the character given by command-line is invalid 
        */
        if (*pEnd != '\0'){
            printf("Wrong/Invalid input : %s is not a valid integer.\n", argv[i+1]);
            free(pArray);
            return 1; 
        }
    }

    /* Sort the array calling the function bubbleSort() */
    bubbleSort(pArray,size); 

    /* loops the array and notify user with the sorted numbers. */
    printf("Here the sorted numbers : ");
    for (int i = 0; i < size; i++)
    {
        printf("%d ", pArray[i]);
    }
    printf("\n");
    
    /* Free the allocated memory*/
    free(pArray);

    return 0; 
}

/*
* Function name     :   bubbleSort
* Arguments         :   *pArray     = Pointer to Array
*                       size        = size of the Array - number of elements 
* Return values/s   :   *pArray     = sorted array       
* Remarks           :   BubbleSort compare the adjacent elements of the array  and swap them if they are in the wrong order 
*                       
*/
int* bubbleSort(int *pArray, int size){
    for(int i= 0; i<size-1; i++){
        for (int j = 0; j < size -i -1; j++)
        {
            if(pArray[j]>pArray[j+1]){
                /* swap the value */
                int temp = pArray[j];
                pArray[j] = pArray[j+1];  
                pArray[j+1] = temp; 
            }
            /* end of inner loop */
        }
    /* end of outer loop  */ 
    }

    return pArray;
}

