#include <stdio.h>
#include <stdlib.h>
#define FILE_NAME "binaryFile.bin"
#define BINARY_WRITE "wb"
#define BINARY_READ "rb"

/* Functions prototype */
void writeFile(FILE* myFile);
void readFile(FILE* myFile);

int main() {
    // Open file for writing
    FILE* binFile = fopen(FILE_NAME, BINARY_WRITE);
    if (!binFile) {
        printf("Could not create file for writing\n");
        exit(-1);
    }
    
    // Write to the file using the file pointer
    writeFile(binFile);
    
    // Close the file after writing
    fclose(binFile);
    printf("File closed after writing\n");
    
    // Re-open for reading
    binFile = fopen(FILE_NAME, BINARY_READ);
    if (!binFile) {
        printf("Could not open file for reading\n");
        exit(-1);
    }
    
    // Read from the file using the file pointer
    readFile(binFile);
    
    // Close the file after reading
    fclose(binFile);
    
    return 0;
}

void writeFile(FILE* myFile) {
    if (myFile == NULL) {
        printf("Invalid file pointer\n");
        return;
    }
    
    int number = 32;
    size_t written = fwrite(&number, sizeof(int), 1, myFile);
    printf("Wrote %zu integer(s)\n", written);
    
    double myDouble = 3.1415;
    written = fwrite(&myDouble, sizeof(double), 1, myFile);
    printf("Wrote %zu double(s)\n", written);
}

void readFile(FILE* myFile) {
    if (myFile == NULL) {
        printf("Invalid file pointer\n");
        return;
    }
    
    int number = 0;
    size_t read = fread(&number, sizeof(int), 1, myFile);
    printf("Read %zu integer(s)\n", read);
    printf("Number is : %d \n", number);
    
    double myDouble = 0.0;
    read = fread(&myDouble, sizeof(double), 1, myFile);
    printf("Read %zu double(s)\n", read);
    printf("Double is : %lf \n", myDouble);
}