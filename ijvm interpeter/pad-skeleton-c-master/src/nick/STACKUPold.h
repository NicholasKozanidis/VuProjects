#ifndef STACKUP_H_
#define STACKUP_H_

#include "FRAMES.h"
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>


#define STACK_SIZE 2048



struct{
    int32_t sp;
    int32_t *s;
}stack;


void stack_new()
{
    
    stack.sp =-1;
    stack.s = (int32_t *)malloc(sizeof(int32_t) * STACK_SIZE);
}

void stack_del()
{
    free(stack.s);
}



int32_t* get_stack(){
    
    int32_t* temp = (int32_t *)malloc(sizeof(int32_t) * stack.sp);
        
        for(int i=0; i<stack.sp; i++)
        {
            temp[i]= stack.s[i];
        }
    
        return temp;
        //free(temp);
    
}


int stack_size()
{
    return stack.sp+1;
}

int32_t pop()
{
  //assert(stack.sp > 0);
    if (stack.sp == -1)
        fprintf(stderr, "Error: stack empty\n");
    else
        return stack.s[stack.sp--];

}


int32_t tos()
{
   if (stack.sp == -1) {
        fprintf(stderr, "Error: stack empty\n");
        return -1;
    }
    
  return stack.s[stack.sp];
}

void push(int32_t n)
{
  //assert(stack.sp < STACK_SIZE -1);
        stack.s[++stack.sp] = n;
}












#endif // STACKUP_H_
