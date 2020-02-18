#ifndef machine_H_
#define machine_H_


#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include "stack.h"
#include "locals.h"


    //basic ijvm properties
    
    struct
    {
        word_t file_header;
        word_t constant_pool_origin;
        word_t constant_pool_size;
        word_t *constants;
        word_t text_origin;
        word_t text_size;
        byte_t *text;
        bool executes;
        int instpointer; //program counter
    }ijvm;
    
    
    
    void branch()
{
    //function for all brancing instructions
    short_t offSet = (ijvm.text[ijvm.instpointer+1] & 0xff) << 8 | (ijvm.text[ijvm.instpointer+2] & 0xFF);
    ijvm.instpointer = ijvm.instpointer - 1;
    ijvm.instpointer += offSet;
    printf("offset is: %x\n", offSet);
    printf("jumped to: %x\n",ijvm.text[ijvm.instpointer]);
}

void push_constant()
{
    uint16_t index = ((ijvm.text[ijvm.instpointer+1] & 0xFF) << 8) | (ijvm.text[ijvm.instpointer+2] & 0xFF);
    push(ijvm.constants[index]);
    printf("the constant is: %x\n", ijvm.constants[index]);
    ijvm.instpointer+=2;
}

void iinc()
{
                int32_t localvar = ijvm.text[ijvm.instpointer+1];
                printf("my local var is %x\n", localvar);
                int32_t constantvar = (int8_t)ijvm.text[ijvm.instpointer+2];
                printf("my constantvar is %x\n", constantvar);
                locals.data[localvar] += constantvar;
                ijvm.instpointer+=2;
    
}

void iload()
{
    int32_t local_offset = ijvm.text[ijvm.instpointer+1];
                printf("%x\n",locals.data[local_offset]);
                push(locals.data[local_offset]);
                ijvm.instpointer++;
                locals.fp--;
    
}

void istore()
{
                int32_t c = pop();
                int32_t local_offset = ijvm.text[ijvm.instpointer+1];
                locals.data[local_offset] = c;
                printf("%x\n",locals.data[local_offset]);
                ijvm.instpointer++;
                locals.fp++;
}
void wide()
{
    byte_t next_instruction = ijvm.text[ijvm.instpointer+1];
    uint16_t index = ((ijvm.text[ijvm.instpointer+2] & 0xFF) << 8) | (ijvm.text[ijvm.instpointer+3] & 0xFF); 
    
    switch(next_instruction)
    {
       case OP_ILOAD:
                printf("ILOAD\n");
                push(locals.data[index]);
                ijvm.instpointer+=3;
                locals.fp--;
                break; 
            
        case OP_ISTORE:
                printf("ISTORE\n");
                int c = pop();
                locals.data[index] = c;
                ijvm.instpointer+=3;
                locals.fp++;
                break;
            
        case OP_IINC:
                printf("IINC\n");
                int32_t constantvar = (int8_t)ijvm.text[ijvm.instpointer+2];
                printf("my constantvar is %x\n", constantvar);
                locals.data[index] += constantvar;
                ijvm.instpointer+=7;
                break;
    }
}

void invoke()
{
                byte_t addr =  ijvm.text[ijvm.instpointer+2];
                printf("invoke argument is: %x\n", addr);
    
                int32_t methodaddr = ijvm.constants[addr];
                printf("address of method area is: %x\n", methodaddr);

                //short_t narg = (ijvm.text[methodaddr-1] << 8) | (ijvm.text[methodaddr] & 0xFF);
                uint16_t narg = ijvm.text[methodaddr] + (ijvm.text[methodaddr+1] << 8);
                printf("number of arguments is: %x\n", narg);
                //short_t lvarsize = (ijvm.text[methodaddr+1] <<8) | (ijvm.text[methodaddr+2] & 0xFF);
                uint16_t lvarsize = ijvm.text[methodaddr+2] + (ijvm.text[methodaddr+3] << 8);
                printf("lvarsize is: %x\n", lvarsize);

                byte_t instruction = ijvm.text[methodaddr+4];
                printf("method instruction is: %x\n", instruction);
                
                locals.fp=stack.sp;
               
                //new locals;
                for(int i=(narg/256)-1; i>=0;i--)
                {
                   locals.data[i] = stack.s[stack.sp];
                    locals.fp++;
                    stack.sp--;
                }
                
                push(ijvm.instpointer);
                ijvm.instpointer=methodaddr+3;
                printf("new pc-2 is at: %x\n", ijvm.text[ijvm.instpointer-2]);
                printf("new pc-1 is at: %x\n", ijvm.text[ijvm.instpointer-1]);
                printf("new pc is at: %x\n", ijvm.text[ijvm.instpointer]);
                printf("new pc+1 is at: %x\n",ijvm.text[ijvm.instpointer+1]);
                printf("new pc+2 is at %x\n", ijvm.text[ijvm.instpointer+2]);
}

void ret()
{
    word_t rval = pop();
                int oldpc = pop();
                push(rval);
                ijvm.instpointer = oldpc + 2;
                //locals_del();
                
                locals.fp = stack.sp+1;
                printf("in stack 0: %x\n", stack.s[0]);
                printf("in stack 1: %x\n", stack.s[1]);
                printf("in stack 2: %x\n", stack.s[2]);
}

#endif // machine_H_
