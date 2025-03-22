/*4.
* Assume that a system has a 32-bit virtual address with a
* 4-KB page size. Write a C program that is passed a virtual address (in decimal) on the
* command line and have it output the page number and offset for the given address
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

/* 
*
* Page size -> 4KB (4096 bytes == 2^12 bytes)
* the page size is 12 bit, so we need al 12 bit to represent all possible offset within a page ( 0 - 4096 )
* for the page number, we need to remove the lower 12 bits from address wth shift operation >> 12 
*/

/* Function prototypes */
int isDecimal(const char *str); 
void procAddress(uint32_t virtuaAddress);

int main(int argc, char *argv[]){ 

    if (argc != 2)
    {
        printf("Usage: %s <virtual address> ( decimal value ) \n", argv[0]);
        return 1; 
    }

    /* Calling isDecimal to validate the input from user */
    if (!isDecimal(argv[1]))
    {
        printf("Error! Input must be a decimal value \n");
        return 1; 
    }

    // Parse the virtual address and process it
    uint32_t virtual_address = (uint32_t)strtoul(argv[1], NULL, 10);
    procAddress(virtual_address);

    return 0; 
}

/*
* Function name     : isDecimal
* Arguments         : const char *str = string to validate if it contains only decimal digits.
* Return value/s    : int = returns 1 if the string contains only decimal digits, 0 otherwise.
* Remarks           : Validates if a string contains only decimal digits by checking each character.
*                     It returns 0 if the string is NULL or empty, and checks if each character is a digit.
*/
int isDecimal(const char *str) {
    if (str == NULL || *str == '\0') {
        return 0;
    }
    
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            return 0;
        }
    }
    
    return 1;
}

/*
* Function name     : procAddress
* Arguments         : uint32_t virtualAddress = the 32-bit virtual address to process
* Return value/s    : void = returns nothing.
* Remarks           : Processes a virtual address to extract page number and offset.
*                     It calculates the page number by right-shifting the address by 12 bits,
*                     and the offset by applying a mask of 0xFFF (lower 12 bits).
*                     Then prints the virtual address, page number, and offset in decimal and hexadecimal.
*/
void procAddress(uint32_t virtualAddress){

    uint32_t pageNumber = virtualAddress >> 12; 
    uint32_t offset = virtualAddress & 0xFFF; 

    printf("Virtual Address: %u (0x%X)\n", virtualAddress, virtualAddress);
    printf("Page Number: %u (0x%X)\n", pageNumber, pageNumber);
    printf("Offset: %u (0x%X)\n", offset, offset);
}