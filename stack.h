#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

#define INIT_ALLOC 4
#define error_code int
#define ERROR 1


typedef struct {
  int* values;
  int size;
  int used;
} int_stack;

void print_stack(int_stack* stack);
int_stack* new_stack(void);

void push_element(int_stack* stack, int element);
error_code pop_element(int_stack* stack);
int sum_stack(int_stack* stack);

void free_stack(int_stack* stack);

#endif
