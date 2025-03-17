/*
Thread 1                Thread 2
--------                --------
     ┌───────────────┐
     │ pthread_mutex_lock()  │      →   Thread acquires the mutex
     └───────────────┘
           ↓
     ┌───────────────┐
     │   Critical Section    │      →   Accesses and modifies shared resource
     └───────────────┘
           ↓
     ┌───────────────┐
     │ pthread_mutex_unlock() │      →   Thread releases the mutex
     └───────────────┘
            ↓
     ──────────────────────────────────────
                     ↓
       Next thread acquires the mutex and executes

*/

#include <stdio.h>
#include <pthread.h>

pthread_mutex_t lock;  // Declare a mutex

int shared_resource = 0; // Shared resource

void* thread_function(void* arg) {
    pthread_mutex_lock(&lock); // Acquire the mutex (Critical Section Start)
    
    shared_resource++;  // Modify shared resource
    printf("Thread %ld updated shared resource: %d\n", (long)arg, shared_resource);
    
    pthread_mutex_unlock(&lock); // Release the mutex (Critical Section End)
    return NULL;
}

int main() {
    pthread_t t1, t2;
    
    pthread_mutex_init(&lock, NULL); // Initialize the mutex
    
    pthread_create(&t1, NULL, thread_function, (void*)1);
    pthread_create(&t2, NULL, thread_function, (void*)2);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    pthread_mutex_destroy(&lock); // Destroy the mutex
    return 0;
}

