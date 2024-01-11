#include "header/test_function.h"

#include <stdio.h>

void check_line_ending(char *search)
{
    while(*search != '\0') {
        /* Seach for a newline */
        if(*search == '\n') {
            printf("\nnewline Found\n");
            search++;
        }

        /* Search for a CR or a CRLF */
        if(*search == '\r') {
            // OK, we found a CR, is it followed by a LF?
            if(*(search + 1) == '\n') {
                // Yes, it is, thus, it is a CRLF
                printf("\nCRLF Found\n");
                search += 2; // Note the 2! CRLF is 2 characters!
            }
            else {
                // No, just a lonely CR, forever alone.
                printf("\nCarriage return found\n");
                search++;
            }
        }
    }

}