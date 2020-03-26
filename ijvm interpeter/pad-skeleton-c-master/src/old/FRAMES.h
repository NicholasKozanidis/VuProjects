#ifndef FRAMES_H_
#define FRAMES_H_


#include "STACKUP.h"
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>


#define FRAMES_SIZE 2048


struct{
    int32_t fp;
    int32_t varsize;
    int32_t *variables;
}frame;




	void new_frame(int pc){
		stack.s = (int32_t *)malloc(sizeof(int32_t) * FRAMES_SIZE);
        stack.sp = -1;
		frame.variables= (int32_t *)malloc(sizeof(int32_t) * FRAMES_SIZE);
		frame.fp = pc;
		frame.varsize = 0;
	}
	
	void addVariable(int32_t newVar){
		frame.variables[frame.fp]= newVar;
		frame.fp+=1;
	}
	
	int32_t getVar(int index){
		return frame.variables[index]; 
	}
	
	void storeVariable(int index, int32_t newVar){
		frame.variables[index] = newVar;
	}



#endif // FRAMES_H_
