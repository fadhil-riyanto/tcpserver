#ifndef UTILS_H_
#define UTILS_H_

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int split(char separator, char *txt, int size, int return_index_of, char **dest);

#endif