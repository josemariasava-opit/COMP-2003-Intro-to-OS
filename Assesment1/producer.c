#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/sem.h>

/* Define */
#define SHM_KEY 1122    
#define SEM_KEY 2211
#define SIZE 1024 
#define FLAG 0666 

/* Function prototypes */
void semaphore_operation(int semid, int operation);
int createSharedMemory(key_t key, size_t size, int flag);

int main(){

    /* local vars*/
    void *sharedMemory; 
    char buffer[100]; 

    int shmid, semid; 

    /* Call crateSharedMemory() */
    shmid = createSharedMemory((key_t) SHM_KEY, SIZE, FLAG); 

    /* Notify key shared memory */
    printf("Key of shared memory %d : \n", shmid);

    /* 
    *  Attach shared mempory segment to a process's space 
    *  with NULL arguments the OS chooses an address.
    */
    sharedMemory = shmat(shmid, NULL, 0); 
    if(sharedMemory == (void*) -1)
    {
        perror("shmat failed");
        exit(1); 
    }

    /* 
    *  create semaphore if does not exist with IPC_CREAT
    */
    semid = semget((key_t)SEM_KEY,1,FLAG|IPC_CREAT);

    if (semid == -1)
    {
        perror("semget failed"); 
        exit(1); 
    }
    
    /* 
    * Initialize semaphore to 0 (consumer must wait and cannot proceed yet) 
    */
    semctl(semid, 0, SETVAL, 0);
    
    /* Copy into buffer the string "777"*/
    strcpy(buffer,"777");

    /* Data writtern to shared memory */
    strcpy(sharedMemory,buffer); 

    /* 
    * Setting semaphore to 1 
    * Signal to the consumer (increment semaphore)
    */
    semaphore_operation(semid, 1);


    /* Detach from shared memory */ 
    shmdt(sharedMemory);

    return 0; 


}



/*
* Function name     :   semaphore_op
* Arguments         :   semid            = Semaphore identifier obtained from semget()
*                       operation        = Integer indicating the operation to perform on the semaphore
*                                          (-1 for wait/P operation, +1 for signal/V operation)
* Return values/s   :   0                = Operation succeeded
*                      -1                = Operation failed 
* Remarks           :   This function performs basic semaphore operations (wait/signal).
*                      The function uses a single sembuf structure to modify the first semaphore (sem_num = 0) in the semaphore set.
*                      No special flags are set (sem_flg = 0), meaning the operation will block if resources are unavailable during a wait operation.            
*/
void semaphore_operation(int semid, int operation){

    struct sembuf sb; 
    sb.sem_num = 0; /* Setting semaphore number to 0 */
    sb.sem_op = operation; /* Set operations to perform --> -1 for waint , 1 for signal */
    sb.sem_flg = 0; /* Set operation flag to 0 */
    /* Calling semop() system function to perform the operation */
    semop(semid, &sb, 1);
}

/*
* Function name     :   createSharedMemory
* Arguments         :   key            = key_t identifier to identify the shared memory segment
*                       size           = size in bytes of the requested shared memory segment 
*                       flag           = Integer value, flag that specify creation options and permissions 
*                                          (-1 for wait/P operation, +1 for signal/V operation)
* Return values/s   :   shmid          = A non-negative integer representing the shared memory segment ID on success
* Remarks           :   The function attempts to get a shared memory segment with the specified key, size and flag. 
*                       If successful, it returns the segment identifier (shmid), If fails notify user with perror() and exit the program. 
*                       This function applies always IPC_CREAT to whatever flag the user provides, so the function will always try to create the shared memory segment
*                       if it does not exist.           
*/
int createSharedMemory(key_t key, size_t size, int flag){

    int shmid; 

    shmid = shmget(key,size,flag|IPC_CREAT);
    if (shmid == -1)
    {
        perror("shmget failed");
        exit(1);
    }

    return shmid; 
}