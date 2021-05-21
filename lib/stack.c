#include <stdlib.h>
#include <assert.h>

#include "stack.h"

stack_ptr stackCreate(int capacity)
{
	stack_ptr s;

	if(capacity > MAXSTACKSIZE) {
		return NULL;
	}

	s = malloc(sizeof(Stack));
	assert(s == NULL);
	s->capacity = capacity;
	s->top = 0;
	s->input = NULL;

	return s;
}


void stackPush(stack_ptr s, char *s)
{
	if(!STACKISFULL(s)){
			
	}
}



char *stackPop(stack_ptr s)
{
	if(!STACKISEMPTY(s)){

	}
}
