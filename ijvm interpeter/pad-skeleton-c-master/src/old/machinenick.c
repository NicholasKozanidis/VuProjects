#include "../include/ijvm.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
 
//struct
    struct{
        uint32_t file_header;
        uint32_t constant_pool_origin;//int
        uint32_t constant_pool_size;
        byte_t *constants;
        uint32_t text_origin;
        uint32_t text_size;
        byte_t *text;
    }ijvm;

//endianess
static uint32_t swap_uint32(uint32_t num)
{
return ((num>>24)&0xff) | ((num<<8)&0xff0000) |((num>>8)&0xff00) |((num<<24)&0xff000000); 
}

int init_ijvm(char *binary_file)
{
    
   
    
  // Implement loading of binary here
    FILE *file;
    char *buffer;
    
    //Open file
    file = fopen(binary_file, "rb");
    if(!file){
        fprintf(stderr, "Unable to open file %s", binary_file);
        return 1;
    }
    
    
    
    
    //read file contents into struct parts
    
    fread(&ijvm.file_header,sizeof(uint32_t), 1, file);
    fread(&ijvm.constant_pool_origin, sizeof(uint32_t), 1, file);
    fread(&ijvm.constant_pool_size, sizeof(uint32_t), 1, file);
    
    ijvm.constants = (byte_t *)malloc((unsigned)ijvm.constant_pool_size);
    fread(ijvm.constants,sizeof(ijvm.constant_pool_size),1,file);
    
    fread(&ijvm.text_origin,sizeof(uint32_t), 1, file);
    fread(&ijvm.text_size,sizeof(uint32_t), 1, file);
    
    ijvm.text = (byte_t *)malloc((unsigned)ijvm.text_size);
    fread(ijvm.text,sizeof(ijvm.text_size),1,file);




    
    
   
   

    fclose(file);

    
    //test print buffer
    
    int i;
    
        printf( "%x\n", swap_uint32((ijvm.file_header)));
        printf( "%x\n", swap_uint32((ijvm.constant_pool_origin)));
        printf( "%x\n", swap_uint32((ijvm.constant_pool_size)));
            for(i=0;i<sizeof(ijvm.constants);i++)
                printf( "%x\n", swap_uint32((ijvm.constants)[i]));
    
        printf( "%x\n", swap_uint32((ijvm.text_origin)));
        printf( "%x\n", swap_uint32((ijvm.text_size)));
            for(i=0;i<sizeof(ijvm.text);i++)
                printf( "%x\n", swap_uint32((ijvm.text)[i]));


        
    
    //freebuffer
    //free(ijvm.text);
    //free(ijvm.constants);
    return 0;
}



void destroy_ijvm()
{
  // Reset IJVM state
    free(ijvm.text);
    free(ijvm.constants);
}

void run()
{
}
  /*uint8_t hex_instruction;
  for(int i = 0; i < ijvm.text_size; i++){
    printf("%#02x\n", ijvm.text[i]);
    switch(hex_instruction = ijvm.text[i]){
      case OP_BIPUSH:
        printf("BIPUSH\n");
        break;
      case OP_DUP:
        printf("DUP\n");
        break;
      case OP_ERR:
        printf("ERR\n");
        break;
      case OP_GOTO:
        printf("GOTO\n");
        break;
      case OP_HALT:
        printf("HALT\n");
        break;
      case OP_IADD:
        printf("IADD\n");
        break;
      case OP_IAND:
        printf("IAND\n");
        break;
      case OP_IFEQ:
        printf("IFEQ\n");
        break;
      case OP_IFLT:
        printf("IFLT\n");
        break;
      case OP_ICMPEQ:
        printf("ICMPEQ\n");
        break;
      case OP_IINC:
        printf("IINC\n");
        break;
      case OP_ILOAD:
        printf("ILOAD\n");
        break;
      case OP_IN:
        printf("IN\n");
        break;
      case OP_INVOKEVIRTUAL:
        printf("INVOKEVIRTUAL\n");
        break;
      case OP_IOR:
        printf("IOR\n");
        break;
      case OP_IRETURN:
        printf("IRETURN\n");
        break;
      case OP_ISTORE:
        printf("ISTORE\n");
        break;
      case OP_ISUB:
        printf("ISUB\n");
        break;
      case OP_LDC_W:
        printf("LDC_W\n");
        break;
      case OP_NOP:
        printf("NOP\n");
        break;
      case OP_OUT:
        printf("OUT\n");
        break;
      case OP_POP:
        printf("POP\n");
        break;
      case OP_SWAP:
        printf("SWAP\n");
        break;
      case OP_WIDE:
        printf("\n");
        break;
      default:
        printf("NOTHING TO SEE\n");
        break;
    }





  }
  	
}*/
    


void set_input(FILE *fp)
{
  // TODO: implement me
}

void set_output(FILE *fp)
{
  // TODO: implement me
}
