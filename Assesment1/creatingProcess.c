/*2.Creating Processes (5 marks): In a UNIX/Linux environment, write a C program to simulate
* a simple process scheduler using the First-Come, First-Served (FCFS) approach. Your program
* should create a separate child process for each given burst time in an array, then wait for all
* child processes to complete execution before the parent process exits.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <time.h>

/* function  prototypes */
int* convertCommandArguments( int size, int *pBurst, char *argv[]);
void scheduler(int count, int *pBurst);

int main(int argc, char *argv[]){ 
    /* local vars */ 
    int *pBurstTime = NULL;
    int size = 0; 

    /* At least one burst time is provided by command-line */
    if(argc < 2 ){
        printf("Usage: %s <burst_time1> <burst_time2> ...\n", argv[0]);
        return 1; 
    }

    /* size = number of command-line args*/
    size = argc - 1;
    /* allocate memory for the array */
    pBurstTime = malloc(size * sizeof(int));        
    /* 
    * Safety first!
    * check correct memory allocation
    */ 
    if (pBurstTime == NULL)
    {
        printf("Memory Allocation failed \n");
        return 1; 
    }
    
    /*
    calling functions
    *convert from string to int values from command-line
    */
    convertCommandArguments(size,pBurstTime,argv);

    /* Calling scheduler*/
    scheduler(size, pBurstTime);
    
    free(pBurstTime);



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
    * Function name     :   convertCommandArguments
    * Arguments         :   size        = Pointer to a string
    *                       *pBurst     = A pointer to an integer array
    *                       *argv[]     = command-line arguments (array of strings)
    * Return values/s   :   pBurst        
    * Remarks           :   takes command-line arguments (as strings), converts them into integers, and stores them in an integer array.               
    *                       If sscanf successfully reads one number, it returns 1 (meaning the string is a valid number).
    *                       If it fails (e.g., the string contains letters or invalid characters), it returns 0.
    */
   void scheduler(int size, int *pBurst){
    /*local vars*/
    pid_t pid;
    int status; 
    pid_t childPid; 
    double elapsedTime = 0.0; /*Track the current time in simulation*/

    time_t timeStart = time(NULL);

    printf("[Time %.2f] Scheduler starting execution \n", elapsedTime);
    
    /* First loop - create child process */
    for (int i = 0; i < size; i++)
    {
        /*Create a new child process*/
        pid = fork(); 
        
        if (pid < 0 )
        {
            /* code */
            perror("Fork of child process failed"); 
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            time_t processStart = time(NULL);
            printf("[Time %.2f] Child process: PID = %d, Burst Time = %d seconds \n", elapsedTime, getpid(), pBurst[i]);

            /* Simulate process execution using sleep*/
            sleep(pBurst[i]);

            /* Completetion time for this process */
            elapsedTime += pBurst[i];
            printf("[Time %.2f] Child process: PID = %d completed (Elapsed: %.2d seconds) \n",elapsedTime, getpid(), pBurst[i]);

            exit(EXIT_SUCCESS);
        }
        else {
            /* Parent process*/
            printf("[Time %.2f] Parent: Created child process with PID = %d\n", elapsedTime, pid);
        }
        
    }

    elapsedTime = 0.0;

    /*Second loop - wait child process*/
    for (int i = 0; i < size; i++)
    {
        
        childPid = wait(&status);
        if (childPid < 0)
        { 
            perror("Wait failed"); 
            exit(EXIT_FAILURE);
        }
        else{
            elapsedTime += pBurst[i];
            printf("[Time %.2f] Parent: Child process with PID = %d has finished \n", elapsedTime, childPid);
        }
        
    }
       /* Calculate and display total execution time*/
       time_t timeEnd = time(NULL);
       printf("[Time %.2f] Scheduler completed. Total execution time: %ld seconds\n",elapsedTime, (long)(timeEnd - timeStart));
       printf("Parent: All child processes have completed \n");
   }
