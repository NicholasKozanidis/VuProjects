#include <ijvm.h>
#include <stdlib.h>

 //struct
    struct{
        char *file_header;
        char *constant_pool;//int
        char *text;
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
    unsigned long fileLen;
    
    //Open file
    file = fopen(binary_file, "rb");
    if(!file){
        fprintf(stderr, "Unable to open file %s", binary_file);
        return 1;
    }
    
    //Get file length
    fseeko(file, 0, SEEK_END);
    fileLen=ftello(file);
    fseeko(file, 0, SEEK_SET);
    
    //Allocate memory
    buffer=(char *)malloc(fileLen+1);
    if(!buffer){
        fprintf(stderr, "Out of Memory!");
        fclose(file);
        return 2;
    }
    
    //read file contents into buffer
    
    fread(buffer, fileLen, 1, file);
    fclose(file);
    
    
    
    //buffer break-down
    /*int j,g;
    
    
    for(j=0;j<4;j++)
        ijvm.file_header[j] = buffer[j];
    for(g=0;g>5 && g<20;g++)
        ijvm.constant_pool[g] = buffer[g];*/

    
    //test print buffer
    
    int i;
    for(i=0;i<fileLen;i++)
        printf( "%x\n", swap_uint32(((uint8_t *)buffer)[i]));

        
    
    //freebuffer
    free(buffer);
    return 0;
}



void destroy_ijvm()
{
  // Reset IJVM state
}

void run()
{
  // Step while you can
    
}

void set_input(FILE *fp)
{
  // TODO: implement me
}

void set_output(FILE *fp)
{
  // TODO: implement me
}
