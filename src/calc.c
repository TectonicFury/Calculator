#include <stdio.h>
#include <stdlib.h>
#include "Expr.h"

int main(int argc, char const *argv[]) {
  char input[50];

  printf(">>> ");
  get_user_expr(input, 50);
  int i = 0;
  expr e = create_expr(input, &i, 100);
  printf("value of expression = %.2lf\n", eval_expr(e));
  printf("value of expression new = %.2lf\n", eval("3*49"));
  // free_expr(e);
  return 0;
}
