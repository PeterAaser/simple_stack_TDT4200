#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "test.h"
#include "util.h"

// Prototypes for methods that are not used outside test
bool strings_equal(char* a, char* b);
error_code perform_test_op(int_stack* stack, char* argv[], int* arg_index_pointer);


// This method gets, in addition to the stack that is being tested, the program arguments
// and a pointer to an int. This arg_index_pointer lets the perform_test_op method signal how many arguments
// it consumed. When it sees "push" it must consume both "push" and the next argument, while "pop" only
// consumes one argument.
error_code perform_test_op(int_stack* stack, char* argv[], int* arg_index_pointer){

  d_printf("arg_index_pointer: %d, arg: %s\n", *arg_index_pointer, argv[*arg_index_pointer]);

  if(strings_equal(argv[(*arg_index_pointer)], "push")){

    int integer_to_push = atoi(argv[(*arg_index_pointer)+1]);
    int next_arg_index_position = (*arg_index_pointer) + 2;

    push_element(stack, integer_to_push);
    *arg_index_pointer = next_arg_index_position;

    return 0;
  }

  if(strings_equal(argv[(*arg_index_pointer)], "pop")){
    pop_element(stack);
    *arg_index_pointer = (*arg_index_pointer) + 1;

    return 0;
  }

  d_printf("INVALID ARG: %s\n", argv[*arg_index_pointer]);
  return ERROR;
}


int test(int argc, char* argv[]){
  d_printf("--- running tests ---\n");
  d_printf("got %d args\n", argc);
  d_printf("\n");

  int_stack* test_stack = new_stack();

  // In the methods above you saw the pointer *arg_index_pointer.
  // Two things are important to note. First, putting "pointer" in the name of a pointer is
  // poor form, let the type system handle type information, it's just there for pedagogical purposes
  // Secondly, in the functions it's an int pointer, yet here it's declared as an int.
  // The reason this is OK is that we do not pass arg_pointer directly, we instead pass &arg_pointer,
  // which means that we pass the address of the variable arg_pointer, not arg_pointer itself.
  // The result of this is that functions can change the value of arg_pointer as a side effect instead
  // of explicitly returning the next argument index.
  int arg_pointer = 1;
  error_code my_error;

  do {
    my_error = perform_test_op(test_stack, argv, &arg_pointer);
  }
  while( (arg_pointer < argc) && (my_error != ERROR) );

  d_printf("Done with args stuff\n");

  if(my_error == ERROR){
    printf("PARSE ERROR\n");
    free_stack(test_stack);
    return ERROR;
  }

  int test_sum = sum_stack(test_stack);

  d_printf("test sum was %d\n", test_sum);

  free_stack(test_stack);

  printf("%d",test_sum);
  return 0;
}


// strcmp returns the difference in the value of the first mismatch found. Yes, seriously.
// if(strcmp(a, b)){
//   printf("fail\n");
// }
// will thus only happen when a and b are similar.
// see: https://youtu.be/PbPa3MGlVS8?t=44s
bool strings_equal(char* a, char* b){
  return (strcmp(a, b) == 0);
}
