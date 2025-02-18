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

    void *shared_memory;    
    char buffer[100]; // data from user 

    int shmid,semid;

    // we need to get the ID required by shmat, we need to attach this process to shared memory 
    shmid = shmget((key_t)1122, 1024, 0666);
    if (shmid == -1)
    {
        perror("shmget failed"); 
        exit(1); 
    }
    printf("Key of shared mempory is %d \n", shmid); 

    shared_memory = shmat(shmid,NULL,0); 
    if (shared_memory == (void *) -1) {
        perror("shmat failed");
        exit(1);
    }

    // Get semaphore
    semid = semget(2233, 1, 0666);
    if (semid == -1) {
          perror("semget failed");
          exit(1);
    }

    // Wait for producer to signal
    semaphore_operation(semid, -1);

      
    printf("Process attached at %p \n", shared_memory); 
    printf("Data read from shared memory is %s \n", (char *)shared_memory); 

    // Detach from shared memory
    shmdt(shared_memory);

    // Optionally remove shared memory (only if no other process needs it)
    shmctl(shmid, IPC_RMID, NULL);

    semctl(semid, 0, IPC_RMID);
 

    return 0; 
}
