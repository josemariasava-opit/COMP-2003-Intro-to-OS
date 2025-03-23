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
#include <ctype.h>

/* DEFINE */
#define FILE_NAME "virtual_disk.bin"
#define RECORD_SIZE 256 /* each records can holds 256 bytes */
#define TOTAL_RECORDS 90 /* Virtual Disc contains 90 records max */
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

    printf("Mass Storage Simulation \n"
           "Init of the virtual disk...\n"
    );
    initDisk(&massStorage); 


    while (1)
    {
        printf("\nVirtual disk operations \n"
                "1 : Write data into disk \n"
                "2 : Read data from disk \n"
                "3 : Format disk and init values \n" 
                "4 : Exit \n"
                "Choose an option: \n"
            );
        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input! Please enter a number \n");
            while (getchar() != '\n') /* clean the buffer*/
            continue;
        }
        


        switch (choice)
        {
        case 1:
            printf("Enter the track number to write into disk (0-%d)",massStorage.totalRecords-1);
            scanf("%d", &track); 
            /* Clean the newline */
            getchar(); 
            /* Ask user for input data */
            printf("Enter data to write (max number of char: %d) ", massStorage.recordSize-1);
            fgets(dataRecord,massStorage.recordSize,stdin);
            /* Remove new line from fgets() */
            dataRecord[strcspn(dataRecord,"\n")] = 0; 
            writeRecord(&massStorage, track, dataRecord); 
            break;
        case 2:
            printf("Enter the track number to read from disk (0-%d)", massStorage.totalRecords-1); 
            scanf("%d", &track); 
            readRecord(&massStorage,track); 
            break;
        case 3:
            initDisk(&massStorage); 
            break;
        case 4: 
            printf("Exiting the program...\n"); 
            exit(0); 
        default:
            printf("\nSelect one of the possible operation from menù \n");
            break;
        }
    }
    
    return 0; 
}

/*
* Function name     : initDisk
* Arguments         : virtualDisk*   = Pointer to the virtual disk data structure
* Return value/s    : void           = does not return any value.
* Remarks           : Initializes the data structure of the virtual disk, creates the disk file, 
*                     and fills it with empty records. If the file creation fails, an error message is displayed. 
*/
void initDisk(virtualDisk* disk){

    /* Init disk structure */
    strcpy(disk->name, FILE_NAME); 
    disk->totalRecords = TOTAL_RECORDS; 
    disk->recordSize = RECORD_SIZE; 

    disk->pFile = fopen(disk->name,WRITE_BIN); 

    /* Check result from fopen() */
    if (!disk->pFile)
    {
        /* Notify ! */
        perror("Error creating the disk file \n");
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

/*
* Function name     : writeRecord
* Arguments         : virtualDisk*   = Pointer to the virtual disk data structure
*                     int            = Record index to write the data
*                     char*          = Pointer to the data to be written
* Return value/s    : void           = Does not return any value.
* Remarks           : Opens the disk file with "wb" options, validates the record index, moves to the correct position via fseek(), 
*                     writes the given data, and closes the file. Displays an error if the file cannot be opened.
*/
void writeRecord(virtualDisk* disk, int record, char *data){

    disk->pFile = fopen(disk->name, WRITE_BIN); 

    /* Check result from fopen() */
    if (!disk->pFile)
    {
        /* Notify ! */
        perror("Error opening the disk file \n"); 
        return; 
    }

    /* Check correct record/track from user */
    if (record<0 || record>= disk->totalRecords)
    {
        printf("Invalid track selected, you have to choose between 0 and %d \n", disk->totalRecords-1);
        /* Close file */
        fclose(disk->pFile); 
    }

    /* Move to track position = record* record_size from the beginning of the file */
    fseek(disk->pFile,record * disk->recordSize,SEEK_SET);
    
    /* Empty buffer */
    char buffer[RECORD_SIZE] = {0}; 
    /* 
    * Copy data into buffer 
    * strncpy(char *restrict dst, const char *restrict src, size_t dsize)
    */
    strncpy(buffer,data,disk->recordSize-1); 

    /* write buffer into file */
    fwrite(buffer,sizeof(char),disk->recordSize, disk->pFile); 
    /* Notify ! */
    printf("Data written to track %d \n", record); 
    
    /* Always close the file after operation */
    fclose(disk->pFile); 
}

/*
* Function name     : readRecord
* Arguments         : virtualDisk*   = Pointer to the virtual disk data structure
*                     int            = Record index to read the data from
* Return value/s    : void           = Does not return any value.
* Remarks           : Opens the disk file with "rb" option, validates the record index, moves to the correct position via fseek(), 
*                     reads the data into a buffer and displays it. Displays an error if the file cannot be opened.
*/
void readRecord(virtualDisk* disk, int record){

    disk->pFile = fopen(disk->name, READ_BIN); 

    /* Check result from fopen */
    if (!disk->pFile)
    {
        /* Notify! */ 
        perror("Error opening the disk file \n");
        return; 
    }

     /* Check correct record/track from user */
    if (record<0 || record>= disk->totalRecords)
    {
        printf("Invalid track selected, you have to choose between 0 and %d \n", disk->totalRecords-1);
         /* Close file */
        fclose(disk->pFile); 
    }

    /* Move to track position = record* record_size from the beginning of the file */
    fseek(disk->pFile,record * disk->recordSize,SEEK_SET);

    /* Empty buffer */
    char buffer[RECORD_SIZE] = {0}; 

    /* read from file and save into buffer var*/
    fread(buffer, sizeof(char), disk->recordSize, disk->pFile); 

    /* Notify ! */
    printf("Data read from track %d -> %s \n", record, buffer); 

    /* Always close the file after operation */
    fclose(disk->pFile);
}

