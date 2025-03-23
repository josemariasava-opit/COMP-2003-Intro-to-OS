/*5.
* Write a C program that simulates the process of demand
* paging. Create a scenario where a set number of page frames are initially empty. Randomly
* generate page numbers requested by a process, and simulate loading these pages into the
* available frames, tracking and displaying page faults.
*/

/* 
* pages are loaded into memory only when needed! 
*/

#include <stdio.h>
#include <stdlib.h> 
#include <stdbool.h>
#include <time.h>

/* Struct to represent memory system */
typedef struct {
    int* frames;
    int frameCount;
    int pageFaults;
    int* pageSequence;
    int sequenceLenght;

}memorySystem;

/* Function prototypes */
void initMemory(memorySystem* memory, int frameCount, int sequenceLength); 
bool pageInMemory(memorySystem* memory, int pageNumber);
int frameManager(memorySystem* memory, int* currentFrame); 
void generatePageSequence(memorySystem* memory, int maxPageNumber); 
void simulationManager(memorySystem* memory);
void destroyMemory(memorySystem* memory); 

int main(){ 

    srand((time(NULL)));

    /* Simulation */
    int frameCount = 3; 
    int maxPageNUmber = 9; 
    int sequenceLength = 20; 

    /* Init struct */
    memorySystem simMemory; 

    initMemory(&simMemory,frameCount,sequenceLength); 

    generatePageSequence(&simMemory, maxPageNUmber); 

    simulationManager(&simMemory); 

    destroyMemory(&simMemory); 

    return 0; 
}

void initMemory(memorySystem* memory, int frameCount, int sequenceLength){
    memory->frameCount = frameCount; 
    memory->frames = malloc(frameCount*sizeof(int)); 
    memory->pageFaults = 0; 
    memory->sequenceLenght = sequenceLength; 
    memory->pageSequence = malloc(sequenceLength*sizeof(int)); 
    
    for (int i = 0; i < frameCount; i++)
    {
        memory->frames[i] = -1; 
    }
}

bool pageInMemory(memorySystem* memory, int pageNumber){

    for (int i = 0; i < memory->frameCount; i++)
    {
        if (memory->frames[i] == pageNumber)
        {
            return true; 
        }
    }
    return false; 
}

int frameManager(memorySystem* memory, int* currentFrame){
    int frame = *currentFrame; 
    *currentFrame = (*currentFrame + 1) % memory->frameCount;
    return frame; 
}

void generatePageSequence(memorySystem* memory, int maxPageNumber){

    /* Page from 1 to maxPageNumber */
    for (int i = 0; i < memory->sequenceLenght; i++)
    {
        memory->pageSequence[i] = rand() % maxPageNumber + 1; 
    }
    
}

void simulationManager(memorySystem* memory){

    int currentFrame = 0; 

    for (int i = 0; i < memory->sequenceLenght; i++){
        int pageNumber = memory->pageSequence[i];
        printf("Requesting page: %d \n", pageNumber); 

        if (!pageInMemory(memory,pageNumber))
        {
            /* Update fault counts*/
            memory->pageFaults ++; 
            int frame = frameManager(memory, &currentFrame);
            printf("Page fault! Loading page %d into frame %d\n", pageNumber,frame);
            memory->frames[frame] = pageNumber;
        }
    }
    printf("Total page fault : %d \n", memory->pageFaults);
}

void destroyMemory(memorySystem* memory){ 

    free(memory->frames);
    free(memory->pageSequence); 
    memory->frames = NULL; 
    memory->pageSequence = NULL; 
}