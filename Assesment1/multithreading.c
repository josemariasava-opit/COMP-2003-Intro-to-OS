/* 
* Multithreading (5 marks). Write a C program for Linux OS that uses two threads to
* calculate the sum of two halves of an integer array. Your program should divide an array of
* integers into two halves. Then, create two threads: one to calculate the sum of the first half,
* and the other to calculate the sum of the second half. Finally, the main thread should wait for
* these two threads to complete their execution and then calculate and display the total sum of
* the array. Use the POSIX thread library (pthread) for creating and managing threads. (NOTE:
* when compiling, do not forget to use the -lpthread switch)
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/*
* Struct to pass data to threads
*/
typedef struct{
    int* array;
    int startPoint; 
    int endPoint; 
    int sum; 
}dataThread; 

/* Function prototypes */
void* calculateSum(void* args);
int* populateArray(int size); 

int main(){ 

    /* local vars*/
    int size; 
    int *pArray; 
    int totalSum; 


    pthread_t firstHalf; 
    pthread_t secondHalf; 

    dataThread firstData, secondData; 


    printf("Insert size of the array : "); 
    scanf("%d", &size);
    
    pArray = populateArray(size); 

    firstData.array = pArray; 
    firstData.startPoint = 0; 
    firstData.endPoint = size/2; 
    firstData.sum = 0; 

    secondData.array = pArray; 
    secondData.startPoint = size/2; 
    secondData.endPoint = size; 
    secondData.sum = 0; 

    /* Create Thread */
    if (pthread_create(&firstHalf, NULL,calculateSum, &firstData) != 0){
        perror("Failed to create thread"); 
        return 1; 
    }
    
    if (pthread_create(&secondHalf,NULL, calculateSum, &secondData)!=0){
        perror("Failed to create thread"); 
        return 1; 
    }

    pthread_join(firstHalf, NULL ); 
    pthread_join(secondHalf, NULL );
    

    totalSum = firstData.sum + secondData.sum; 

    printf("Sum of first halve is : %d \n", firstData.sum); 
    printf("Sum of second halve is : %d \n", secondData.sum); 

    printf("Total sum is: %d \n", totalSum); 

    free(pArray); 

    return 0; 
}

/*
* Function name     :   alculateSum 
* Arguments         :   void* args     = generic pointer that allows any type of argument to be passed in
* Return value/s    :   NULL           = returns null because the function is void* 
* Remarks           :   Iterates from startPoint to endPoint, summing up elements from array within this range. 
*/
void* calculateSum(void* args){

    /* 
    * Casting args to dataThread
    * In this way is possible to access the dataThread structure
    */
    dataThread* data = (dataThread* ) args; 

    data->sum = 0; 

    /* Loops through the portion of the Array*/
    for (int i = data->startPoint; i < data->endPoint; i++)
    {
        data->sum+= data->array[i]; 
    }

    return NULL; 
}

/*
* Function name     :   populateArray
* Arguments         :   size     = size of the Array - number of elements
* Return value/s   :   *pArray  = pointer to an array of integer
* Remarks           :   Alocates memory for the array, ask the user to input integer values and populate the array. 
*                       Return the pointer to the array                    
*/
int* populateArray(int size){

    int *pArray; 

    /* Allocate memory for the array*/
    pArray = malloc(size *sizeof(int)); 

    /* Validate correct memory allocation */
    if (pArray == NULL)
    {
        printf("Error! Memory allocation failed for the array \n"); 
        return NULL; 
    }
    /* 
    * Ask the user to input the element of the array 
    */
    printf("Enter %d integer: \n", size); 
    for (int i = 0; i < size; i++){
        if (scanf("%d", &pArray[i]) != 1)
        {
            printf("Error: Invalid input for array element. \n");
            free(pArray); 
            return NULL; 
        }
    }

    return pArray; 
}