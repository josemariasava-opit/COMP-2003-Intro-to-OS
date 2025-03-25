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
#define WRITE_BIN "r+b" /* reads and writes without truncating the file */
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
void initDisk(virtualDisk* disk, int flagInit); 
void writeRecord(virtualDisk* disk, int record, char *data); 
void readRecord(virtualDisk* disk, int record); 
int isValidRecord(virtualDisk* disk, int record); 
int isTrackEmpty(virtualDisk* disk, int record);
void checkDiskInfo(virtualDisk* disk);

int main(){
    /* Local vars */
    virtualDisk massStorage; 
    int choice, track; 
    char dataRecord[RECORD_SIZE]; 

    printf("Mass Storage Simulation \n");
    /* call initDisk() with flag 0 and do not force formatting */
    initDisk(&massStorage, 0); 


    while (1)
    {
        printf("\nVirtual disk operations \n"
                "1 : Write data into disk \n"
                "2 : Read data from disk \n"
                "3 : Format disk and init values \n" 
                "4 : Check disk info \n"
                "5 : Exit \n"
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
        /* validate record with isValidRecord() and then calling writeRecord() to avoid error with file handling */ 
        case 1:
            do
            {
                printf("Enter the track number to write into disk (0-%d) ",massStorage.totalRecords-1);
                scanf("%d", &track); 
                getchar(); /* Clean the newline */
            } while (!isValidRecord(&massStorage, track));

            /* Ask user for input data */
            printf("Enter data to write (max number of char: %d) ", massStorage.recordSize-1);
            fgets(dataRecord,massStorage.recordSize,stdin);
            /* Remove new line from fgets() */
            dataRecord[strcspn(dataRecord,"\n")] = 0; 
            writeRecord(&massStorage, track, dataRecord); 
            break;
        /* validate record with isValidRecord() and then calling readRecord() to avoid error with file handling */ 
        case 2:
            do
            {
                printf("Enter the track number to read from disk (0-%d) ", massStorage.totalRecords-1); 
                scanf("%d", &track); 
                getchar();  /* Clean the newline */
            } while (!isValidRecord(&massStorage, track));
            readRecord(&massStorage,track); 
            break;
        /* Calling initDisk() with flag 1 and format all disk */
        case 3:
            printf("Reinitializing virtual disk...\n");
            initDisk(&massStorage,1);
            break;
        /* Calling checkDiskInfo() */
        case 4: 
            checkDiskInfo(&massStorage);
            break; 
        case 5: 
            printf("Exiting the program...\n"); 
            exit(0); 
            break;
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
*                     flagInit       = Integer values, used to manage the init condition 
* Return value/s    : void           = does not return any value.
* Remarks           : Initializes the data structure of the virtual disk, creates the disk file, 
*                     and fills it with empty records. If the file creation fails, an error message is displayed. 
*                     With flagInit the init of the file is handle deeply, if the file already exist and is not empty we keep the data inside. 
*                     if is empty we initialize it with 0 
*/
void initDisk(virtualDisk* disk, int flagInit){

    /* Init disk structure */
    strcpy(disk->name, FILE_NAME); 
    disk->totalRecords = TOTAL_RECORDS; 
    disk->recordSize = RECORD_SIZE; 
    /* Check if virtual_disk.bin exists in the folder */
    disk->pFile = fopen(disk->name, READ_BIN); 

    if (disk->pFile && !flagInit)
    {
        /* Move to the end of file to check the size */
        fseek(disk->pFile,0,SEEK_END);
        long fileSize = ftell(disk->pFile); 
        /* Always close the file after operation  */
        fclose(disk->pFile); 

        if (fileSize>0)
        {
            printf("Virtual Disk %s already exist and contains data \n", disk->name); 
            return; 
        }
        else
        {
            printf("Virtual disk %s exists but is empty. Reinitializing... \n", disk->name); 
        }
    } 
    /* Format disk from user choice */
    else if (disk->pFile && flagInit)
    {
        fclose(disk->pFile); 
        printf("Formatting the virtual disk %s...\n", disk->name); 
    }
    /* Cannot read the file -> the file does not exist */
    else
    {
        printf("Virtual disk %s does not exist. Creating a new one...\n", disk->name);
    }


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
    /* Notify !*/
    printf("Virtual Disk %s initialized with %d empty tracks\n", disk->name, disk->totalRecords); 
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

    /* Check if the track is empty */
    if (!isTrackEmpty(disk, record)) {
        char choice;
        printf("Track %d already contains data. Overwrite? (y/n): ", record);
        scanf(" %c", &choice);
        getchar(); /* Clear newline */

        if (tolower(choice) != 'y') {
            printf("Write operation canceled.\n");
            return;
        }
    }

    disk->pFile = fopen(disk->name, WRITE_BIN); 

    /* Check result from fopen() */
    if (!disk->pFile)
    {
        /* Notify ! */
        perror("Error opening the disk file \n"); 
        return; 
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

/*
* Function name     : isValidRecord
* Arguments         : virtualDisk*   = Pointer to the virtual disk data structure
*                     int            = Record index to read the data from
* Return value/s    : int            = Returns 1 if the record index is valid, 0 if it is out of range.
* Remarks           : Checks if the given record index falls within the valid range (0 to totalRecords - 1).
*                     If the index is out of range, an error message is displayed, and 0 is returned.
*/
int isValidRecord(virtualDisk* disk,int record){

    if (record <0 || record>disk->totalRecords-1)
    {
        printf("Wrong track requested. Virtual disk goes from track 0 to track %d\n ", disk->totalRecords-1);
        return 0; 
    }
    return 1; 
}

/*
* Function name     : isTrackEmpty
* Arguments         : virtualDisk*   = Pointer to the virtual disk data structure
*                     int            = Record index to read the data from
* Return value/s    : int            = Returns 1 if the track is empty, 0 if it contains data, and -1 on file access error.
* Remarks           : Opens the disk file with "r+b" option, moves to the specified track using fseek(), 
*                     reads the data into a buffer, and checks if all bytes are zero.
*                     If a track/record contains data the function return 0
*/
int isTrackEmpty(virtualDisk* disk, int record) {
    disk->pFile = fopen(disk->name, READ_BIN);
    if (!disk->pFile) {
        perror("Error opening disk file \n");
        return -1; 
    }

    /* Move to the correct track */
    fseek(disk->pFile, record * disk->recordSize, SEEK_SET);
    /* Read the data */
    char buffer[RECORD_SIZE] = {0};
    fread(buffer, sizeof(char), disk->recordSize, disk->pFile);
    /* Always close the file after operation */
    fclose(disk->pFile);

    /* Check if all bytes are zero */
    for (int i = 0; i < disk->recordSize; i++) {
        if (buffer[i] != 0) {
            return 0; // Not empty
        }
    }
    return 1; // Empty track
}

/*
* Function name     : checkDiskInfo
* Arguments         : virtualDisk*   = Pointer to the virtual disk data structure
* Return value/s    : void           = Does not return any value.
* Remarks           : This function retrieves and displays the information about the virtual disk file.
*                     - The function displays the following disk information:
*                       - Disk file size (in bytes).
*                       - Total number of records (tracks).
*                       - Number of occupied tracks.
*                       - Number of free tracks.
*/
void checkDiskInfo(virtualDisk* disk){
    disk->pFile = fopen(disk->name,READ_BIN); 

    if (!disk->pFile)
    {
        perror("Error opening disk file \n");
        return; 
    }

    /* Get file size */
    fseek(disk->pFile,0,SEEK_END); 
    long fileSize = ftell(disk->pFile); 
    /* Always close the file after operation */
    fclose(disk->pFile); 

    /* Check occupied tracks */
    int occupiedTracks = 0; 
    for (int i = 0; i < disk->totalRecords; i++)
    {
        if (!isTrackEmpty(disk,i))
        {
            occupiedTracks ++; 
        }
    }
    
    /* Notify ! */
    printf("\nDisk info:\n"
            "File disk size: %ld bytes \n"
            "Total tracks: %d\n"
            "Occupied tracks on disk: %d \n"
            "Free tracks on disk: %d\n",fileSize,disk->totalRecords,occupiedTracks,disk->totalRecords-occupiedTracks
    );
}