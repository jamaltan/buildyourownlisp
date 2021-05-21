#ifndef __STACK_H_
#define __STACK_H_

#define MAXSTACKSIZE (1<<10)
#define STACKISFULL(s) ( (s)->capacity == (s)->top + 1 )
#define STACKISEMPTY(s) ( (s)->top == 0 )

typedef stack struct {
	int capacity;
	int top;
	char **input;
} Stack;

typedef Stack *stack_ptr;

stack_ptr stackCreate(int capacity);
void stackPush(stack_ptr s, char *s);
char *stackPop(stack_ptr s);

#endif /* __STACK_H_ */
