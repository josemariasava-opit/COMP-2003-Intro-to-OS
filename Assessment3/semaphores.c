/*2.
* Write a C program in Linux that demonstrates the use of
* semaphores for synchronizing three threads accessing a shared resource. The program
* should create three threads, all vying to modify a shared variable. Use semaphores to
* ensure that only one thread can access the variable at a time. Test your program by
* ensuring that each thread that accesses the shared variable adds a value to it
*/

/* 
Semaphore is a synchronization mechanism used to control access to a shared resource

Semaphore (Value = 2) → Allows 2 Threads in Critical Section at a Time

Thread 1               Thread 2               Thread 3
--------               --------               --------
     ┌───────────┐
     │ sem_wait() │     →   Thread 1 enters (Value = 1)
     └───────────┘
        ↓
     ┌───────────┐
     │ sem_wait() │     →   Thread 2 enters (Value = 0)
     └───────────┘
        ↓
     ┌────────────┐
     │ Critical Section │
     └────────────┘
        ↓
     ┌───────────┐
     │ sem_post() │     →   Thread 1 exits (Value = 1)
     └───────────┘
        ↓
     ┌───────────┐
     │ sem_wait() │     →   Thread 3 enters (Value = 0)
     └───────────┘
        ↓
     ───────────────────────────────────────────────────
                 Threads execute and synchronize using semaphore
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

/*
* Struct to pass data to threads
*/
typedef struct { 
    int *sharedVar; /* Pointer to the shared variable */
    sem_t *semaphore; /* Pointer to semaphore */ 
    int threadId; /* Thread ID */
}dataThread; 


/* Function prototypes */
void *semSharedResource(void *args); 

int main(){

    /* local vars */
    pthread_t threads[3]; 
    int sharedVariable = 0; 
    sem_t semaphore; 

    /* init semaphore, binary value */
    sem_init(&semaphore,0,1); 
    
    /* Create the threads */
    for (int i = 1; i <= 3; i++)
    {
        dataThread *data = malloc(sizeof(dataThread)); 
        if (!data)
        {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE); 
        }
        
        data->sharedVar = &sharedVariable; 
        data->semaphore = &semaphore; 
        data->threadId = i; 

        /* Create thread */
        if (pthread_create(&threads[i-1], NULL, semSharedResource, (void *)data)!= 0)
        {
            perror("Failed to create thread"); 
            return 1;
        }
    }

    /*  wait threads to complete execution */
    for (int i = 0; i < 3; i++)
    {
        pthread_join(threads[i], NULL); 
    }    
    
    /* Notify final value */
    printf("Final value of shared variable is: %d\n", sharedVariable); 

    /* Destroy semaphore */
    sem_destroy(&semaphore); 

    return 0; 
}

/*
* Function name     :   semSharedResource 
* Arguments         :   void* args     = generic pointer that allows any type of argument to be passed in
* Return value/s    :   NULL           = returns null because the function is void* 
* Remarks           :   
*/
void *semSharedResource(void *args){

    /* 
    * Casting args to dataThread
    * In this way is possible to access the dataThread structure
    */
   dataThread* data = (dataThread*) args; 
   
   /* Wait on semaphore */
   sem_wait(data->semaphore); 

    /* 
    *  Critical section 
    *  Operation to shared variable
    */
   *(data->sharedVar) += data->threadId; 

   /* Notify and display the result */
   printf("Thread added %d, Shared variable: %d \n", data->threadId, *(data->sharedVar)); 

   /* Unlock semaphore */
   sem_post(data->semaphore); 

   /* free data allocated prev. */
   free(data); 

   return NULL; 
}