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


int_stack* new_stack(void){
  int_stack* stack = malloc(sizeof(int_stack));
  stack->size = INIT_ALLOC;
  stack->used = 0;
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
  if(is_full(stack)){
    extend_stack(stack);
  }

  stack->values[stack->used] = element;
  stack->used += 1;
}


error_code pop_element(int_stack* stack){
  if(is_empty(stack)){
    return 1;
  }
  stack->used -= 1;
  if(less_than_half_full(stack)){
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
  stack->values = realloc(stack->values, stack->size*sizeof(int));

  return 0;
}


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

void free_stack(int_stack* stack){
  free(stack->values);
  free(stack);
}
