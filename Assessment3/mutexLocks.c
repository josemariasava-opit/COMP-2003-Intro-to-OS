/*1. 
* Write a C program in Linux that demonstrates the use of mutex
* locks for synchronizing three threads accessing a shared resource. The program should
* create three threads, all vying to modify a shared variable. Use a mutex lock to ensure that
* only one thread can access the variable at a time. Test your program by ensuring that each
* thread that accesses the shared variable adds a value to it
*/

/*
Prevents multiple threads from accessing a shared resource simultaneously
    -----------------------
1   pthread_mutex_lock()   ->  Thread acquire the mutex
    -----------------------
    -----------------------
2   Critical Section       ->  Access the shared resources 
    -----------------------
    -----------------------
3   pthread_mutex_unlock() ->  Thread release the mutex 
    -----------------------
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/*
* Struct to pass data to threads
*/
typedef struct { 
    int *sharedVar; /* Pointer to the shared variable */
    pthread_mutex_t *lock; /* Pointer to the mutex */
    int threadId; /* Thread ID */
}dataThread; 

/* Function prototypes */
void *sharedResource(void *args);

int main(){

    /* local vars */
    pthread_t threads[3]; 
    int sharedVariable = 0; 
    pthread_mutex_t lock; 

    /* init mutex */
    pthread_mutex_init(&lock,NULL); 

    /* Create the threads */
    for (int i = 1; i <= 3; i++)
    {
        dataThread *data = malloc(sizeof(dataThread)); 
        data->sharedVar = &sharedVariable; 
        data->lock = &lock; 
        data->threadId = i; 

        /* Create thread */
        if (pthread_create(&threads[i-1], NULL, sharedResource, (void *)data)!= 0)
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
    printf("Final value of shared variable is: %d ", sharedVariable); 
    
    /* Destroy mutex */
    pthread_mutex_destroy(&lock); 

    return 0; 
}

/*
* Function name     :   sharedResource 
* Arguments         :   void* args     = generic pointer that allows any type of argument to be passed in
* Return value/s    :   NULL           = returns null because the function is void* 
* Remarks           :   
*/
void *sharedResource(void *args){

    /* 
    * Casting args to dataThread
    * In this way is possible to access the dataThread structure
    */
   dataThread* data = (dataThread*) args; 

   pthread_mutex_lock(data->lock); // Lock mutex 

   // change shared variable 
   *(data->sharedVar) += data->threadId; 

   // notify and display the result 
   printf("Thread added %d, Shared variable: %d \n", data->threadId, *(data->sharedVar)); 

   // unlock mutex 
   pthread_mutex_unlock(data->lock); 

   // free data allocated prev.
   free(data); 

   return NULL; 
}