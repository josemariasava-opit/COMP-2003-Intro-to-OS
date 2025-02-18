/*
* 1. Simple Scheduling Algorithm (5 marks): Write a C program to implement a basic
* First-Come, First-Served (FCFS) scheduling algorithm. The program should accept the burst
* times of a set of processes (from the command line) and calculate their waiting times and
* turnaround times
*/
#include <stdio.h>
#include <stdlib.h>

/* function  prototypes*/
int* convertCommandArguments( int size, int *pBurst, char *argv[]);
void processTimes(int size, int *pBurst, int *pWaiting, int *pTurnaround); 
void printResults(int size, int *pBurst, int *pWaiting, int *pTurnaround);

int main(int argc, char *argv[]){ 

    /* local vars*/
    int *pBurstTime = NULL;
    int *pWaitingTime = NULL; 
    int *pTurnaroundTime = NULL; 
    int size = 0; 



    /* At least one burst time is provided by command-line */
    if(argc < 2 ){
        printf("Usage: %s <burst_time1> <burst_time2> ...\n", argv[0]);
        return 1; 
    }

    /* size = number of command-line args*/
    size = argc - 1;
    /* allocate memory for each array */
    pBurstTime = malloc(size * sizeof(int));
    pWaitingTime = malloc(size * sizeof(int));
    pTurnaroundTime = malloc(size * sizeof(int)); 
    
    /*  
    * Safety first !
    */
    if (pBurstTime == NULL || pWaitingTime == NULL || pTurnaroundTime == NULL)
    {
        printf("Memory Allocation failed \n");
        return 1; 
    }
    
    /* 
    * calling functions
    * convert from string to int values from command-line
    */
    convertCommandArguments(size,pBurstTime,argv);
    /* calculate times for each array*/
    processTimes(size,pBurstTime,pWaitingTime,pTurnaroundTime);
    /* print the results*/
    printResults(size,pBurstTime,pWaitingTime,pTurnaroundTime);


    /* Free the allocated memory*/
    free(pBurstTime);
    free(pWaitingTime);
    free(pTurnaroundTime);

    return 0; 
}


    /*
    * Function name     :   convertCommandArguments
    * Arguments         :   size        = Pointer to a string
    *                       *pBurst     = A pointer to an integer array
    *                       *argv[]     = command-line arguments (array of strings)
    * Return values/s   :   pBurst        
    * Remarks           :   takes command-line arguments (as strings), converts them into integers, and stores them in an integer array.               
    *                       If sscanf successfully reads one number, it returns 1 (meaning the string is a valid number).
    *                       If it fails (e.g., the string contains letters or invalid characters), it returns 0.
    */
   int* convertCommandArguments( int size, int *pBurst,char *argv[]){
    for (int i = 0; i < size; i++)
    {
        char *pEnd;
        /*
        * strtol syntax
        * strtol(const char *str, char **endptr, int base);
        * strtol() conversion returns a long integer, and cast it to integer. Used base 10 specify the decimal numbers. 
        */
        pBurst[i] = strtol(argv[i+1],&pEnd,10);  
        /*
        * *pEnd check that the char was converted correctly to a valid integer. 
        * points to the first character in the string that was not converted. 
        * if *pEnd points to the null terminator ('\0') indicates the end of the string, so the char is valid and the conversion was successful
        * if *pEnd points to a char the character given by command-line is invalid 
        */
        if (*pEnd != '\0'){
            printf("Wrong/Invalid input : %s is not a valid integer.\n", argv[i+1]);
            free(pBurst);
            return NULL; 
        }
    }
    return pBurst;
   }

/*
    * Function name     :   calculateWaitingAndTurnaround
    * Arguments         :   size          = Number of processes
    *                       *pBurst       = A pointer to an integer array containing burst times
    *                       *pWaiting     = A pointer to an integer array storing waiting times
    *                       *pTurnaround  = A pointer to an integer array storing turnaround times
    * Return values/s   :   ---
    * Remarks           :   Computes waiting and turnaround times based on burst times 
    *                       Waiting time: Sum of previous burst times + waiting times
    *                       Turnaround time: Waiting time + burst time.
    */
   void processTimes(int size, int *pBurst, int *pWaiting, int *pTurnaround){

    pWaiting [0] = 0; 
    pTurnaround[0] = pBurst[0]; 
    for (int i = 0; i < size; i++)
    {
        pWaiting[i+1] = pWaiting[i] + pBurst[i];
        pTurnaround[i+1] = pWaiting[i+1] + pBurst[i+1]; 
    }
   }

   /*
    * Function name     :   printResults
    * Arguments         :   size          = Number of processes
    *                       *pBurst       = A pointer to an integer array containing burst times
    *                       *pWaiting     = A pointer to an integer array storing waiting times
    *                       *pTurnaround  = A pointer to an integer array storing turnaround times
    * Return values/s   :   ---
    * Remarks           :   Prints process information in a tabular format.
    *                       Displays Process ID, Burst Time, Waiting Time, and Turnaround Time.
    */
   void printResults(int size, int *pBurst, int *pWaiting, int *pTurnaround){

    printf("Process \tBurst Time\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < size; i++)
    {
        printf("%d \t\t%d \t\t%d \t\t%d \n", i+1, pBurst[i],pWaiting[i],pTurnaround[i]);
    }
   }