#ifndef locals_H_
#define locals_H_


#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include "stack.h"

#define LOCALS_SIZE 2048

struct{
    int32_t fp;
    int32_t *data;
}locals;


void locals_new()
{
    locals.fp =0;
    locals.data = (int32_t *)malloc(sizeof(int32_t) * LOCALS_SIZE);
}

void locals_del()
{
    free(locals.data);
}

void add_local(int32_t val)
{
    locals.data[locals.fp]=val;
    locals.fp++;
}


word_t get_local_variable(int i)
{
    return locals.data[i];
}



#endif // locals_H_
