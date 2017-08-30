#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "test.h"

// When a C program is invoked with arguments these can be accessed in argv[]
// argc tells us the count of args. Usually argv[0] is the name of the invoked program
// For instance, with the makefile, printf("%s",argv[0]); would yield >myProgram
int main(int argc, char* argv[]){

  // Sometimes we don't want to run fancy tests, but debugging in main is usuall poor form

  /* printf("hello world\n"); */

  /* int_stack* my_stack = new_stack(); */
  /* print_stack(my_stack); */


  /* for(int ii = 0; ii < 10; ii++){ */
  /*   push_element(my_stack, ii); */
  /* } */
  /* print_stack(my_stack); */


  /* for(int ii = my_stack->used; ii > 0; ii--){ */
  /*   pop_element(my_stack); */
  /* } */
  /* print_stack(my_stack); */

  // Instead I all my tests in my test method, as if that's any better...
  return test(argc, argv);
}
