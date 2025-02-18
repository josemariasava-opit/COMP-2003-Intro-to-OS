#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>

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
    void *shared_memory;    
    char buffer[100]; // data from user 

    int shmid,semid;

    /* Call crateSharedMemory() */
    shmid = createSharedMemory((key_t) SHM_KEY, SIZE, FLAG); 

    /* Notify key shared memory */
    printf("Key of shared mempory is %d \n", shmid); 

    /* 
    *  Attach shared mempory segment to a process's space 
    *  with NULL arguments the OS chooses an address.
    */
    shared_memory = shmat(shmid,NULL,0); 
    if (shared_memory == (void *) -1) {
        perror("shmat failed");
        exit(1);
    }

    /* 
    *  create semaphore if does not exist with IPC_CREAT
    */
    semid = semget((key_t)SEM_KEY, 1, 0666);
    if (semid == -1) {
          perror("semget failed");
          exit(1);
    }

    /* Wait for producer to signal */
    semaphore_operation(semid, -1);
    
    /* Notify the data read from shared memory */
    printf("Data read from shared memory is %s \n", (char *)shared_memory); 

    /* Detach from shared memory */
    shmdt(shared_memory);

    /* Remove shared memory */
    shmctl(shmid, IPC_RMID, NULL);

    /* Remove the semaphore set from system */
    semctl(semid, 0, IPC_RMID);
 

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
void semaphore_operation(int semid, int op) {
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = op;  // -1 for wait, +1 for signal
    sb.sem_flg = 0;
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