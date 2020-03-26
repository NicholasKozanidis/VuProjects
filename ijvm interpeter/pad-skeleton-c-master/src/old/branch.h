#ifndef BRANCH_H_
#define BRANCH_H_


#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>







short_t toSignedShort(char bytes[]) {
		return (bytes[0] << 8) | (bytes[1] & 0xFF);
	}



char* ShortToBytes(short_t val){
    int length = snprintf(NULL, 0, "%x", val);
    
    char* temp = malloc(length+1);
    
    snprintf(temp, length+1, "%x", val);
    
    return temp;
}




	







#endif // BRANCH_H_
