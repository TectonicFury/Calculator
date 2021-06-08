#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Expr.h"

int main(int argc, char const *argv[]) {
  printf("Running tests on Calculator\n");
  int passed, failed;
  passed = 0;
  failed = 0;

  if (!(63.0 == eval("7*9"))) {
    printf("Test Failed -- 7*9\n");
    ++failed;
  } else {
    printf("Test Passed -- 7*9\n");
    ++passed;
  }
  if (!(100 == eval("2*50"))) {
    printf("Test Failed -- 2*50\n");
    ++failed;
  } else {
    printf("Test Passed -- 2*50\n");
    ++passed;
  }
  if (!(63 == eval("(3+4)*(4+5)"))) {
    printf("Test Failed -- (3+4)*(4+5)\n");
    ++failed;
  } else {
    printf("Test Passed -- (3+4)*(4+5)\n");
    ++passed;
  }
  if (!(3628800.0 == eval("10*9*8*7*6*5*4*3*2*1"))) {
    printf("Test Failed -- 10*9*8*7*6*5*4*3*2*1\n");
    ++failed;
  } else {
    printf("Test Passed -- 10*9*8*7*6*5*4*3*2*1\n");
    ++passed;
  }
  if (!(4 == eval("((((((((((4))))))))))"))) {
    printf("Test Failed -- ((((((((((4))))))))))\n");
    ++failed;
  } else {
    printf("Test Passed -- ((((((((((4))))))))))\n");
    ++passed;
  }
  if (!(108 == eval("3*(4+5)*4"))) {
    printf("Test Failed -- 3*(4+5)*4\n");
    ++failed;
  } else {
    printf("Test Passed -- 3*(4+5)*4\n");
    ++passed;
  }
  printf("\n\n*************>>>...Results...<<<<***************\n\n");
  printf("Tests passed = %d\n", passed);
  printf("Tests failed = %d\n", failed);

  return 0;
}
