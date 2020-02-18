#include "../include/ijvm.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

uint8_t *heap;
int heapsize;

static uint32_t swap_uint32(uint32_t num){
  return ((num>>24)&0xff) | ((num<<8)&0xff0000) | ((num>>8)&0xff00) | ((num<<24)&0xff000000);
} 

/*Load the binary file and store it into computer memory*/
int init_ijvm(char *binary_file){
  /*Create some variables*/
  FILE *fp;
  struct stat filestat;
  
  if((fp = fopen(binary_file, "rb"))){
    /*If opening the file works, grab data and store in memory*/
    fstat(fileno(fp), &filestat);
    heap = (uint8_t *)malloc((unsigned)filestat.st_size);
    fread((void *)heap, 1, filestat.st_size, fp);
    heapsize = (unsigned)filestat.st_size;

    
  }else{
    /*If file cant be opened*/
    fprintf(stderr, "Couln't load binary %s\n", binary_file);
    return -1;
  }
  fclose(fp);
  return 0;
}

void destroy_ijvm(){
  /*Reset IJVM state*/
  free(heap);
  heapsize = 0;
  printf("Memory deallocated.\n");
}

void run(){
  /*Step while you can*/
  uint8_t hex_instruction;
  for(int i = 0; i < heapsize; i++){
    printf("%#02x\n", heap[i]);
    switch(hex_instruction = heap[i]){
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
  	
}

void set_input(FILE *fp){
  /*TODO: implement me*/
}

void set_output(FILE *fp){
  /*TODO: implement me*/
}
