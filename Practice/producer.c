/*
shmget() --> used to create the shared memory segment 

shmat() --> used to attach the shared memory segment with the adress space of the process 

#include <sys/ipc.h>
#include <sys/shm.h> 

int shmget(key_t key, size_t size, int shmflg)
         key    : identifies shared segment 
         size   : size of the shared segment 
         shmflg : specify permission on the shared segment 

         will return a valid identifies used in shmat() to attach the shared segment 


#include <sys/types.h>
#includes <sys/shm.h>
void* shmat(int shmid, const* void shmaddr, int shmflg) 

        shmid   : value returned by shmget() 
        shmaddr : address where to attach the shared segment, NULL automatically assign an address 
        shmflg  : 0 if previous is NULL
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>

void semaphore_operation(int semid, int op) {
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = op;  // -1 for wait, +1 for signal
    sb.sem_flg = 0;
    semop(semid, &sb, 1);
}

int main(){

    void *shared_memory; // store address where shared segment attach to process 
    char buffer[100]; // data from user 

    int shmid, semid; // 

    /*
    * creates shared memory. 
    * key could be any number 
    * size = 1024 bytes 
    * shmflag = 0666 read and write permission 
    * IPC_CREAT 
    * create a shared segment with ID 1122, 1024 bytes of memory, wirh read/write permission 
    */
    shmid = shmget((key_t)1122, 1024, 0666|IPC_CREAT);
    if (shmid == -1)
    {
        perror("shmget failed"); 
        exit(1); 
    }
    printf("Key of shared memory is %d \n", shmid);

    /*
    * shmat attach the shared segment to the address space of this process 
    */
    shared_memory = shmat(shmid,NULL,0);
    if (shared_memory == (void *)-1)
    {
        perror("shmat failed"); 
        exit(1); 
    }

    semid = semget(2233,1,0666| IPC_CREAT);

    if(semid == -1){
            perror("semget failed");
            exit(1); 
    }
        
    

    // Initialize semaphore to 0 (consumer must wait)
    semctl(semid, 0, SETVAL, 0);
    
    printf("Process attached at %p \n", shared_memory);  

    strcpy(buffer,"777");
    
    strcpy(shared_memory,buffer); // Data writtern to shared memory 

    // Signal the consumer (increment semaphore)
    semaphore_operation(semid, 1);


    // Detach from shared memory 
    shmdt(shared_memory);

    return 0; 
}
