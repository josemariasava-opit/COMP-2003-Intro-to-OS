/*
* C supports the file-of-structures concept very cleanly. Once you open the file you can read a structure, write a structure, or seek to any structure in the file. 
* This file concept supports the concept of a file pointer. When the file is opened, the pointer points to record 0 (the first record in the file).
* Any read operation reads the currently pointed-to structure and moves the pointer down one structure. 
* Any write operation writes to the currently pointed-to structure and moves the pointer down one structure. Seek moves the pointer to the requested record. 
*/
#include <stdio.h>

/* Random record description - could be anything */
struct record {

    int x,y,z;
};

/* Writes and reads 10 arbitrary records */

int main(){ 

    int i,j; 
    FILE *pFile; 

    struct record r; // Init struct 

    //create the file of 10 records 
    pFile = fopen("junk","w"); 

    if (!pFile)
    {
        return 1; 
    }

    for (i = 0; i < 10; i++)
    {
        r.x = i; 
        fwrite(&r,sizeof(struct record),1,pFile);
    }

    fclose(pFile);
    

    // read the 10 record
    pFile = fopen("junk", "r"); 
    if (!pFile)
    {
        return 1; 
    }

    for (i = 0; i < 10; i++)
    {
        fread(&r,sizeof(struct record),1,pFile);
        printf("%d \n", r.x);
    }

    fclose(pFile);
    printf("\n"); 

     /* use fseek to read 4th record,
    change it, and write it back */
    pFile=fopen("junk","r+");
    if (!pFile)
        return 1;
    fseek(pFile,sizeof(struct record)*3,SEEK_SET);
    fread(&r,sizeof(struct record),1,pFile);
    r.x=100;
    fseek(pFile,sizeof(struct record)*3,SEEK_SET);
    fwrite(&r,sizeof(struct record),1,pFile);
    fclose(pFile);
    printf("\n");

    /* read the 10 records to insure
       4th record was changed */
    pFile=fopen("junk","r");
    if (!pFile)
        return 1;
    for (i=1;i<=10; i++)
    {
        fread(&r,sizeof(struct record),1,pFile);
        printf("%d\n",r.x);
    }
    fclose(pFile);
    return 0;
    
    
    




}