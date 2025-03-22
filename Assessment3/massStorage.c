/*6. 
* Develop a C program that simulates basic disk I/O operations.
* Implement functions to read and write data to a simulated disk, where the disk is
* represented as a file in your program. Your program should manage basic operations like
* seeking to a specific position (track) in the file and performing read/write operations. You
* can create a file called virtual_disk.bin as a binary file to simulate a disk. Test your virtual
* disk by writing data to the disk and then reading data from the disk.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "virtual_disk.bin"
#define RECORD_SIZE 256 /* each records can holds 256 bytes */
#define TOTAL_RECORDS 120 /* Virtual Disc contains 120 records max */
#define WRITE_BIN "wb"
#define READ_BIN "rb"

/* Struc for representing the disk */
typedef struct
{
    FILE* pFile;
    char name[20];
    int totalRecords;
    int recordSize; 

} virtualDisk;


/* Function prototypes */
void initDisk(virtualDisk* disk); 
void writeRecord(virtualDisk* disk, int record, char *data); 
void readRecord(virtualDisk* disk, int record); 



int main(){

    /* Local vars */
    virtualDisk massStorage; 
    int choice, track; 
    char dataRecord[RECORD_SIZE]; 

    while (1)
    {
        printf("\n Virtual disk operations \n"
                "1 : Write data into disk \n"
                "2 : Read data from disk \n"
                "3 : Format disk and init values \n" 
                "4 : Exit \n"
                "Choose an option: "
            );
        scanf("%d", &choice); 

        switch (choice)
        {
        case 3:
            initDisk(&massStorage); 
            break;
        case 4: 
            printf("Exiting the program...\n"); 
            exit(0); 
        default:
            printf("Select one of the possible operation from menù \n");
            break;
        }
    }
    
    return 0; 
}


void initDisk(virtualDisk* disk){

    /* Init disk structure */
    strcpy(disk->name, FILE_NAME); 
    disk->totalRecords = TOTAL_RECORDS; 
    disk->recordSize = RECORD_SIZE; 

    disk->pFile = fopen(disk->name,WRITE_BIN); 

    /* Check correct fopen() */
    if (!disk->pFile)
    {
        /* Notify ! */
        perror("Error creating the disk file");
    }
    
    /* init the disk with empty records */
    char emptyData[RECORD_SIZE] = {0}; 
    /* loop and fill the disk reords*/
    for (int i = 0; i < disk->totalRecords; i++)
    {
        fwrite(emptyData, sizeof(char), RECORD_SIZE, disk->pFile); 
    }

    printf("Virtual Disk %s initialized \n", disk->name); 

    /* Always close the file after operation */
    fclose(disk->pFile); 
}

