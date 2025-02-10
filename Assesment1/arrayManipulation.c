/*1.
* Array Manipulation and Functions (5 marks): Write a C program that implements a
* function to find the median of an array of integers. The program should take an array and its
* size as input, and return the median value of the integers.
*/
#include <stdio.h>
#include <stdlib.h>

// function  prototypes 
void bubbleSort(int *pArray, int size);
float findMedian(int *pArray, int size); 

int main(){ 
    int size; 
    int *pArray; 

    /*
    * Ask the user to input the size of the array - n of elements 
    */
   printf("Enter the size of the array :"); 
   scanf("%d", &size);
   /*
   * validate the input of the user  
   */ 
   if(size <= 0 || size == 1){
    printf("Error! Invalid input for the array size ! \n"); 
    return 1; // exit the program 
     }

    // Allocate memory for the array
    pArray = malloc(size * sizeof(int)); 
    /*
    * validate correct memory allocation for the array 
    * in case pointer is = NULL notify the user and exit the program 
    */
    if(pArray == NULL){
        printf("Error! Memory allocation failed for the array \n");
        return 1; 
    }

    /*
    * Ask the user to input the element of the array  
    */
    printf("Enter %d integers:\n", size);
    for (int i = 0; i < size; i++) {
        if (scanf("%d", &pArray[i]) != 1) {
            printf("Error: Invalid input for array element.\n");
            free(pArray); // Free the allocated memory
            return 1;   // Exit if input fails
        }
    }

    // Call the findMedian() function and print the result
    float median = findMedian(pArray, size);
    if (median != -1) {
        printf("The median is: %.2f\n", median);
    }

    // Free the allocated memory
    free(pArray);

    return 0; 
}


/*
* Argument of the function are the pointer to the array and the size
* BubbleSort compare the adjacent elements and swap them if they are in the wrong order
*/
void bubbleSort(int *pArray, int size){
    for(int i= 0; i<size-1; i++){
        for (int j = 0; j < size -i -1; j++)
        {
            if(pArray[j]>pArray[j+1]){
                // swap the value 
                int temp = pArray[j];
                pArray[j] = pArray[j+1];  
                pArray[j+1] = temp; 
            }
            // end of inner loop
        }
    // end of outer loop 
    }
}

/*
* Argument of the function is the pointer to the array and the size
* the function sort the given array and checks the size. 
* If odd the median is the element in the middle 
* if even the median is the average of the two elements that are closest to the center 
*/
float findMedian(int *pArray, int size){ 
    /* 
    * Safet First, check that the array and the size are valid 
    * In case notify user 
    */
   if(pArray == NULL || size <=0){
    printf("Error! Invalid pointer to array or wrong size");
    return 1; // exit the program 
   }

   // Sort the array by calling the function bubbleSort()
   bubbleSort(pArray,size); 

   // if the size is odd, return just the element in the middle 
   if (size % 2 != 0){ 
    return pArray[size/2]; 
   }
   else
   {
    // if the size is even, return the average of the two elements in the middle 
    return (pArray[(size/2) -1] + pArray[size/2]) / 2.0; 
   }
}

