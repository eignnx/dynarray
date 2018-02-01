#include <stdio.h>

#include "dynarray.h"

int main()
{
    int *ints = dynarray_create(int);
    
    for (int i = 0; i < 10; i++) {
        int temp = 10 - i;
        ints = dynarray_push(ints, &temp);
    }

    for (int i = 0; i < 10; i++) {
        printf("element %d:\t%d\n", i, ints[i]);
    }
}
