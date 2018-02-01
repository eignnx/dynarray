#include <stdio.h>

#include "dynarray.h"

int main()
{
    int *v = dynarray_create(int);
    
    for (int i = 0; i < 10; i++) {
        dynarray_push(v, i);
        dynarray_push_rval(v, 100 - i);
    }

    for (int i = 0; i < dynarray_length(v); i++) {
        printf("v[%d] -> %d\n", i, v[i]);
    }

    dynarray_destroy(v);
}
