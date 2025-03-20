/*3.
* Write a C program in Linux that demonstrates preventing deadlocks
* using semaphores. A single-lane bridge connects two towns. Farmers in the two villages
* use this bridge to deliver their produce to the neighboring town. The bridge can become
* deadlocked if a northbound and a southbound farmer get on the bridge at the same time.
* Using semaphores, design an algorithm in pseudocode that prevents deadlock. Represent
* northbound and southbound farmers as separate threads. Once a farmer is on the bridge,
* the associated thread will sleep for a random period of time, representing traveling across
* the bridge.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#define BRIDGE_FREE -1 
#define NORTH_DIR true 
#define SOUTH_DIR false 
#define MAX_CROSSING 10

/*
 * Struct to represent the Bridge.
 * (Note: We now store the waiting counters only for reporting purposes.
 *  The key field is currentDirection, which will be reset to BRIDGE_FREE after
 *  every crossing. This lets the next waiting farmer, regardless of direction,
 *  claim the bridge.)
 */
typedef struct {
    sem_t *semBridge;       /* Semaphore to control access to the bridge */
    sem_t *mutex;           /* Semaphore to protect shared variables */
    int waitingNorthSide;   /* Count of northbound farmers waiting (for stats) */
    int waitingSouthSide;   /* Count of southbound farmers waiting (for stats) */
    int currentDirection;   /* -1: free, 0: south, 1: north */
} bridgeData;

/*
 * Struct to represent a Farmer.
 */
typedef struct {
    bridgeData *bridge;     /* Pointer to the shared bridge */
    bool directionFlag;     /* true for northbound, false for southbound */
    int farmerID;           /* Farmer’s ID */
} farmerData; 

/* Function prototypes */
void waitBridge(farmerData *data, int *waitedTime);
void crossBridge(farmerData *data, int waitedTime);
void updateBridgeData(farmerData *data);
void *farmerManager(void *args);
void printError(char *message);

int main(){ 
    /* Seed random number generator */
    srand(time(NULL)); 

    /* Time range 0.5 - 1.5 seconds for thread creation delays */
    int min_wait = 500000;   // 0.5 seconds in microseconds
    int max_wait = 1500000;  // 1.5 seconds in microseconds
    int range_time = max_wait - min_wait + 1; 

    sem_t mutex; 
    sem_t semBridge; 

    /* Initialize the bridge structure */
    bridgeData bridge;
    bridge.mutex = &mutex; 
    bridge.semBridge = &semBridge;
    bridge.waitingNorthSide = 0; 
    bridge.waitingSouthSide = 0; 
    bridge.currentDirection = BRIDGE_FREE; 

    /* Initialize semaphores */
    if (sem_init(bridge.mutex, 0, 1) != 0) {
        printError("sem_init mutex in main()");
    }
    if (sem_init(bridge.semBridge, 0, 1) != 0) {
        printError("sem_init semBridge in main()");
    }
    
    /* Create an array of farmer threads */
    pthread_t farmers[MAX_CROSSING]; 

    printf("-----------------------------------------------------\n"); 
    printf("Starting simulation of deadlocks.c with %d farmers\n", MAX_CROSSING); 
    printf("Odd-numbered farmers go to north, even-numbered farmers go to south\n\n");

    /* Create farmer threads with random delay between creations */
    for (int i = 0; i < MAX_CROSSING; i++) {
        farmerData *data = malloc(sizeof(farmerData)); 
        if (!data) {
            printf("Memory allocation failed for farmer id: %d\n", i+1); 
            continue; 
        }
        data->bridge = &bridge; 
        data->farmerID = i + 1; 
        data->directionFlag = ((i + 1) % 2 == 1); // Odd: NORTH, Even: SOUTH

        if (pthread_create(&farmers[i], NULL, farmerManager, data) != 0) {
            printf("Thread creation failed for farmer id: %d\n", i + 1); 
            free(data); 
            continue;
        }

        /* Random delay between thread creations */
        int wait_time = min_wait + rand() % range_time; 
        usleep(wait_time);
    }

    /* Wait for all farmer threads to finish */
    for (int i = 0; i < MAX_CROSSING; i++) {
        pthread_join(farmers[i], NULL); 
    }

    /* Destroy semaphores */
    sem_destroy(bridge.mutex);
    sem_destroy(bridge.semBridge); 

    printf("\nSimulation complete!\n"); 

    return 0; 
}
/*
* Function name     :   printError
* Arguments         :   char *message = error message to be printed to stderr.
* Return value/s    :   void           = does not return any value; exits the program.
* Remarks           :   Uses perror to print the error message and then exits the program.
*/
void printError(char *message){
    perror(message); 
    exit(1);
}

/*
* Function name     :   waitBridge
* Arguments         :   farmerData *data = pointer to the farmer's data structure,
*                       int *waitedTime = pointer to an integer that tracks how many seconds the farmer has waited.
* Return value/s    :   void           = does not return any value.
* Remarks           :   Waits until the bridge is free for the farmer's direction. Uses the mutex semaphore
*                     to protect shared variables, increments waiting counters, and updates waitedTime.
*/
void waitBridge(farmerData *data, int *waitedTime){
    bridgeData *pBridge = data->bridge; 

    /* Enter critical section to check the bridge status */
    if (sem_wait(pBridge->mutex) != 0) {
        printError("sem_wait mutex in waitBridge()");
    }
    
    /* (Optional) Update waiting counter for stats */
    if (data->directionFlag)
        pBridge->waitingNorthSide++;
    else
        pBridge->waitingSouthSide++;

    /* Wait until the bridge is free */
    while (pBridge->currentDirection != BRIDGE_FREE) {
        if (sem_post(pBridge->mutex) != 0) {
            printError("sem_post mutex in waitBridge()");
        }
        sleep(1);  // Sleep for a second to avoid tight busy waiting
        (*waitedTime)++; 
        if (sem_wait(pBridge->mutex) != 0) {
            printError("sem_wait mutex in waitBridge()");
        }
    }

    /* Claim the bridge by setting currentDirection to the farmer's direction */
    pBridge->currentDirection = data->directionFlag; 

    /* (Optional) Decrement waiting counter since the farmer is now crossing */
    if (data->directionFlag)
        pBridge->waitingNorthSide--;
    else
        pBridge->waitingSouthSide--;

    if (sem_post(pBridge->mutex) != 0) {
        printError("sem_post mutex in waitBridge()");
    }
}

/*
* Function name     :   crossBridge
* Arguments         :   farmerData *data = pointer to the farmer's data structure,
*                       int waitedTime = total seconds the farmer waited before accessing the bridge.
* Return value/s    :   void           = does not return any value.
* Remarks           :   Acquires the bridge semaphore, simulates the farmer crossing the bridge by sleeping for a random duration,
*                     prints a crossing message, and then releases the bridge semaphore.
*/
void crossBridge(farmerData *data, int waitedTime){
    bridgeData *pBridge = data->bridge; 

    /* Acquire the bridge semaphore */
    if (sem_wait(pBridge->semBridge) != 0) {
        printError("sem_wait semBridge in crossBridge()");
    }

    /* Print a message and simulate crossing time (1-3 seconds) */
    printf("Farmer %d (%sbound) is crossing the bridge. Waited approx. %d seconds\n",
           data->farmerID, data->directionFlag ? "North" : "South", waitedTime);
    
           sleep(1 + rand() % 3); 

    /* Release the bridge semaphore */
    if (sem_post(pBridge->semBridge) != 0) {
        printError("sem_post semBridge in crossBridge()");
    }
}

/*
* Function name     :   updateBridgeData
* Arguments         :   farmerData *data = pointer to the farmer's data structure.
* Return value/s    :   void           = does not return any value.
* Remarks           :   Resets the bridge's currentDirection to free after a farmer has crossed,
*                     allowing any waiting farmer, regardless of direction, to claim the bridge.
*/
void updateBridgeData(farmerData *data){
    bridgeData *pBridge = data->bridge; 

    if (sem_wait(pBridge->mutex) != 0) {
        printError("sem_wait mutex in updateBridgeData()");
    }

    pBridge->currentDirection = BRIDGE_FREE; 

    if (sem_post(pBridge->mutex) != 0) {
        printError("sem_post mutex in updateBridgeData()");
    }
}

/*
* Function name     :   farmerManager
* Arguments         :   void* args = generic pointer to a farmerData structure.
* Return value/s    :   void*          = returns NULL.
* Remarks           :   Manages a farmer's process: waiting to cross the bridge, crossing, and updating the bridge state.
*                     It calls waitBridge, crossBridge, and updateBridgeData in sequence.
*/
void *farmerManager(void *args){
    farmerData *data = (farmerData*) args;
    int waitedTime = 0; 

    waitBridge(data, &waitedTime); 
    crossBridge(data, waitedTime);
    updateBridgeData(data); 

    free(data); 
    return NULL;
}
