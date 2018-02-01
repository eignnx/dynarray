#include <stdio.h>

#include "dynarray.h"

int main()
{
    int *v = dynarray_create(int);
    
    for (int i = 0; i < 10; i++) {
        int temp = 10 - i;
        dynarray_push(v, &temp);
    }

    for (int i = 0; i < dynarray_length(v); i++) {
        printf("v[%d] -> %d\n", i, v[i]);
    }

    dynarray_destroy(v);
}
