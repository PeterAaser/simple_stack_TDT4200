#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stack.h"
#include <stdbool.h>
#include "util.h"

// Prototypes for methods that are not used outside stack
bool is_full(int_stack* stack);
bool is_empty(int_stack* stack);

void extend_stack(int_stack* stack);
error_code shrink_stack(int_stack* stack);
bool less_than_half_full(int_stack* stack);


void print_stack(int_stack* stack){
  printf("A stack of int. Contains %d elements, has allocated room for %d elements\n", stack->used, stack->size);
  printf("The stack looks like this:\n");
  printf("%d %d \n", stack->size, stack->used);
  for(int ii = 0; ii < stack->used; ii++){
    int what = 0;
    what = stack->values[ii];
    printf("[%d]\n", what);
  }
  printf("\n\n");
}


// Allocates memory for a new stack
int_stack* new_stack(void){

  // First we allocate memory for the stack object itself.
  // This object only consists of three fields, namely the size, used and pointer to values.
  // Memory for the actual values is allocated later
  int_stack* stack = malloc(sizeof(int_stack));

  // When we allocate memory in C we must keep track of where and how much. Attempting to get the size of a pointer will not tell you how
  // much memory is allocated, it will just give you the size of an address (4 or 8 bytes usually)
  // Because of this it is essential that we track both how much memory we have allocated, and how much of it is currently in use.
  stack->size = INIT_ALLOC;
  stack->used = 0;

  // Finally, after doing some book-keeping, we allocate the values on the stack.
  // Instead of malloc I've used calloc, which initializes all memory to 0.
  // Usually you want to use malloc, but here I've used calloc to show you what it is.
  stack->values = calloc(INIT_ALLOC, sizeof(int));
  return stack;
}


bool is_full(int_stack* stack){
  // Here I lacked a -1, a bug I found with valgrind
  // return stack->used == stack->size;
  return stack->used == stack->size - 1;
}


bool less_than_half_full(int_stack* stack){
  return (stack->used < (stack->size)/2);
}


bool is_empty(int_stack* stack){
  return stack->used == 0;
}


void push_element(int_stack* stack, int element){
  // When pushing an element we first check if it is full (based on our own book-keeping)
  if(is_full(stack)){
    // In the case that we have no more room, we first extend the memory allocated to the stack
    extend_stack(stack);
  }

  stack->values[stack->used] = element;
  stack->used += 1;
}


// The return value of this function is "error_code" which I have defined to be int.
// C will not tell you that you're using an int as an error_code, or vice versa, so
// don't be lulled into a false sense of security. I've added this very primitive
// way of checking if we got a failure or not which might be useful to you.
error_code pop_element(int_stack* stack){
  if(is_empty(stack)){
    return 1;
  }

  // When we pop an element we don't delete it, we just not that it is no longer in use.
  stack->used -= 1;
  if(less_than_half_full(stack)){
    // If our stack is less than half full we shrink it. This is generally a very bad idea performance wise,
    // and not something you should do unless you have a benchmark to measure performance gain/loss.
    shrink_stack(stack);
  }
  return 0;
}


error_code shrink_stack(int_stack* stack){

  d_printf("shrinking stack\n");

  if(stack->size == 0){
    return 1;
  }

  stack->size = stack->size/2;

  // Here I missed the sizeof which caused errors far downstream
  // stack->values = realloc(stack->values, stack->size);

  // Here we reallocate the memory, meaning that we ask the OS to either resize the buffer, or allocate
  // a new buffer and copy the memory in the current buffer to the new buffer. This is transparent to the programmer
  stack->values = realloc(stack->values, stack->size*sizeof(int));

  return 0;
}


// In this function I allocate a new block of memory and explicitly copy the contents from the old memory.
// This is usually not what you want, instead you want to use realloc which either resizes your memory-buffer,
// or in the case that there is no room to do so, automagically copies your data into a new buffer as I do here.
void extend_stack(int_stack* stack){

  d_printf("extending stack\n");

  int next_size = stack->size*2;
  int* next_values = malloc(sizeof(int)*next_size);

  for(int ii = 0; ii < stack->used; ii++){
    next_values[ii] = stack->values[ii];
  }

  free(stack->values);
  stack->values = next_values;
  stack->size = next_size;
}


int sum_stack(int_stack* stack){
  int sum = 0;
  for(int ii = 0; ii < stack->used; ii++){
    sum += stack->values[ii];
  }
  return sum;
}

// A pointer to a stack contains a pointer to the stacks values. We must make sure to free the memory used by the values
// before we free the stack. By only freeing the stack we lose the address of the allocated memory, but it will not be de-allocated.
// This means we not only fail to free the memory, but we've destroyed the necessary information to do so!
void free_stack(int_stack* stack){
  free(stack->values);
  free(stack);
}
