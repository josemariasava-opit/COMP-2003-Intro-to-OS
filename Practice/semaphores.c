/*
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
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t semaphore;  // Declare a semaphore

void* thread_function(void* arg) {
    sem_wait(&semaphore);  // Decrement semaphore (Enter Critical Section)
    
    printf("Thread %ld is in the critical section.\n", (long)arg);
    sleep(1);  // Simulate some work
    printf("Thread %ld is leaving the critical section.\n", (long)arg);
    
    sem_post(&semaphore);  // Increment semaphore (Exit Critical Section)
    return NULL;
}

int main() {
    pthread_t t1, t2, t3;
    
    sem_init(&semaphore, 0, 2); // Initialize semaphore (allows 2 threads at a time)
    
    pthread_create(&t1, NULL, thread_function, (void*)1);
    pthread_create(&t2, NULL, thread_function, (void*)2);
    pthread_create(&t3, NULL, thread_function, (void*)3);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    
    sem_destroy(&semaphore); // Destroy semaphore
    return 0;
}
