
#include "t_utils.h"

/*
 * ret
* 0 eq literating
 * 1 eq last
 * 2 not found
 * 3 index overflow
 */
int split(char separator, char *txt, int size, int return_index_of, char **dest)
{
    
    char *chartemp = malloc(sizeof(chartemp) * 200);
    int counter = 0;
    int index = 0;
    int split_loop = 0;
    int foundindex = 0;

    int a = 0;

    for(int i = 0; i < size; i++) {
        if (txt[i] == separator) {
            for(int c = (index - counter); c < (index); c++) {
                //printf("index %d on %c\n", a, txt[c + split_loop]);
                chartemp[a] = txt[c + split_loop];
                a = a + 1;
            }

            chartemp[a] = '\0'; 
            a = 0;
            counter = 0;
            split_loop = split_loop + 1;

            if (foundindex == return_index_of) {
                //printf("indexof %d: \"%s\"\n", return_index_of, chartemp);
                //strcpy(chartemp,  *dest);
                //strcpy(dest, chartemp);
                *dest = chartemp;
                return 0;
            }
            foundindex = foundindex + 1;

        } else if (txt[i+ 1] == '\0') {
            for(int c = (index - (counter)); c < (index + 1); c++) {
                chartemp[a] = txt[c + split_loop ];
                a = a + 1;
            }
            chartemp[a] = '\0'; 
            if (foundindex == return_index_of) {
                //printf("indexof %d: \"%s\"\n", return_index_of, chartemp);
                // strcpy(chartemp, **dest);
                *dest= chartemp;
                return 1;
            }
            if (return_index_of > foundindex)
            {
                return 3;
            }
            foundindex = foundindex + 1;
            
        } else {
            counter = counter + 1;
            index = index + 1;
        }
    }
    return 2;
}