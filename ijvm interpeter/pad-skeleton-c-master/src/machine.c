#include "../include/ijvm.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include "instructions.h"
#include "stack.h"
#include "locals.h"


//bytecode
FILE *file;
//out
FILE *out;
//in
FILE *in;

//endianess swap
    static uint32_t swap_uint32(uint32_t num)
    {
    return ((num>>24)&0xff) | ((num<<8)&0xff0000) |((num>>8)&0xff00) |((num<<24)&0xff000000); 
    }


int init_ijvm(char *binary_file)
{
    
    //basic initialization
    char *magic_num = "1deadfad";
    stack_new();
    locals_new();
    ijvm.instpointer=0;
    out = stdout;
    in = stdin;
    
    //Open file
    file = fopen(binary_file, "rb");
    if(!file)
    {
        fprintf(stderr, "Unable to open file %s", binary_file);
        return -1;
    }
 
    //read file contents into struct parts
    
    fread(&ijvm.file_header,sizeof(word_t), 1, file);
    ijvm.file_header = swap_uint32(ijvm.file_header);

    fread(&ijvm.constant_pool_origin, sizeof(word_t), 1, file);
    ijvm.constant_pool_origin = swap_uint32(ijvm.constant_pool_origin);

    fread(&ijvm.constant_pool_size, sizeof(word_t), 1, file);
    ijvm.constant_pool_size = swap_uint32(ijvm.constant_pool_size);
    
    
    ijvm.constants = malloc(ijvm.constant_pool_size);
    fread(ijvm.constants,sizeof(word_t),ijvm.constant_pool_size/4,file);
    for(int i=0; i<ijvm.constant_pool_size/4;i++)
    {
        ijvm.constants[i] = swap_uint32(ijvm.constants[i]);
    }

    
    fread(&ijvm.text_origin,sizeof(word_t), 1, file);
    ijvm.text_origin = swap_uint32(ijvm.text_origin);

    fread(&ijvm.text_size,sizeof(word_t), 1, file);
    ijvm.text_size = swap_uint32(ijvm.text_size);

    
    ijvm.text = malloc(ijvm.text_size);
    fread(ijvm.text, sizeof(byte_t), ijvm.text_size, file);

    fclose(file);
    
    //check if the file is .ijvm
    word_t mn;
    sscanf(magic_num, "%x", &mn);
        if(mn != ijvm.file_header)
        {
            fprintf(stderr, "Unable to open non-ijvm file %s", binary_file);
            return -1;
        }
    
    return 0;
}


void destroy_ijvm()
{
  // Reset IJVM state
    free(ijvm.text);
    free(ijvm.constants);
    stack_del();
}


void run()
{
    //Run until the end a halt instruction or an error
    ijvm.executes = true;
    
        while(ijvm.instpointer < ijvm.text_size && ijvm.executes)
        {
            step();
        }
}

bool step()
{
    
    uint8_t hex_instruction;
    int32_t a,b,c;
    
        switch(hex_instruction = ijvm.text[ijvm.instpointer]){

            case OP_BIPUSH:
                printf("BIPUSH\n");
                printf("Bipushed: %x\n", (int8_t)ijvm.text[ijvm.instpointer+1]);
                push((int8_t)ijvm.text[ijvm.instpointer+1]);
                ijvm.instpointer++;
                break;
            case OP_DUP:
                printf("DUP\n");
                push(stack.s[stack.sp]);
                break;
            case OP_ERR:
                printf("ERR\n");
                printf("Encountered error in the bytecode");
                ijvm.executes = false;
                break;
            case OP_GOTO:
                printf("GOTO\n");
                branch();
                break;
            case OP_HALT:
                ijvm.executes = false; 
                break;
            case OP_IADD:
                printf("IADD\n");
                a=pop();
                b=pop();
                push(a+b);
                break;
            case OP_IAND:
                printf("IAND\n");
                b = pop();
                a = pop();
                push(a&b);
                break;
            case OP_IFEQ:
                printf("IFEQ\n");
                c = pop();
                if(c==0)
                {
                    branch();
                }
                else
                {
                    ijvm.instpointer+=2;
                }
                break;
            case OP_IFLT:
                printf("IFLT\n");
                c = pop();
                if(c<0)
                {
                    branch();
                }
                else
                {
                    ijvm.instpointer+=2;
                }
                break;
            case OP_ICMPEQ:
                printf("ICMPEQ\n");
                b = pop();
                a = pop();
                if(b==a)
                {
                    branch();
                }
                else
                {
                    ijvm.instpointer+=2;
                }
                break;
            case OP_IINC:
                printf("IINC\n");
                iinc();
                break;
            case OP_ILOAD:
                printf("ILOAD\n");
                iload();
                break;
            case OP_IN:
                printf("IN\n");
                c=getc(in);
                if(c != EOF)
                {
                    push(c);
                }
                else
                {
                    push(0);
                }
                break;
            case OP_INVOKEVIRTUAL:
                printf("INVOKEVIRTUAL\n");
                invoke();
                break;
            case OP_IOR:
                printf("IOR\n");
                b = pop();
                a = pop();
                push(a|b);
                break;
            case OP_IRETURN:
                printf("IRETURN\n");
                ret();
                break;
            case OP_ISTORE:
                printf("ISTORE\n");
                istore();
                break;
            case OP_ISUB:
                printf("ISUB\n");
                b = pop();
                a = pop();
                push(a-b);
                break;
            case OP_LDC_W:
                printf("LDC_W\n");
                push_constant();
                break;
            case OP_NOP:
                printf("NOP\n");
                break;
            case OP_OUT:
                printf("OUT\n");
                putc(pop(), out);
                break;
            case OP_POP:
                printf("POP\n");
                pop();
                //stack.sp--;
                break;
            case OP_SWAP:
                printf("SWAP\n");
                c = stack.s[stack.sp];
                stack.s[stack.sp] = stack.s[stack.sp-1];
                stack.s[stack.sp-1] = c;
                break;
            case OP_WIDE:
                printf("WIDE\n");
                wide();
                break;
            default:
                
                break;
            }
            
            ijvm.instpointer++;
            printf("iteration is: %x\n", ijvm.instpointer);
    
        }
    
byte_t* get_text()
{
    return ijvm.text;
}

int text_size()
{
    return ijvm.text_size;
}

word_t get_constant(int i)
{
    return ijvm.constants[i];  
}
int get_program_counter()
{
    return ijvm.instpointer;
}

byte_t get_instruction()
{
    return ijvm.text[ijvm.instpointer];
}

void set_input(FILE *fp)
{
  in = fp;
}

void set_output(FILE *fp)
{

  out = fp;
}
