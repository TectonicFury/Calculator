#include <stdio.h>
#include <stdlib.h>
#include "Expr.h"

struct Expr {
  void *value;
  char type_tag;
  expr next;
};
double eval(char *s) {
  int i = 0;
  expr e = create_expr(s, &i, 100);
  double val = eval_expr(e);
  free_expr(e); //need to free the space before returning
  return val;
}
double eval_expr(expr e) {
  if (is_number(e)) {
    return number(e);
  } else if (is_sum(e)) {
    double adnd = eval_expr(addend(e));
    double agnd = eval_expr(augend(e));
    return adnd + agnd;
  } else if (is_product(e)) {
    double mlplr = eval_expr(multiplier(e));
    double mlpcnd = eval_expr(multiplicand(e));
    return mlplr * mlpcnd;
  } else if (is_expr_paren(e)) {//extract the expression in case of extraneous parenthesis; this case was not evaluatable in SICP symbolic diff
    return eval_expr(e->value);
  } else {
    printf("bad syntax -- EVAL\n");
    return -1000000000;
  }
}

expr create_expr(char *s, int *start_index, int lim) {//creates the expr using the user input
  expr begin_expr = (expr)malloc(sizeof(struct Expr));
  expr t = begin_expr;
  expr u;
  while (s[*start_index] != '\0' && s[*start_index] != ')' && *start_index < lim) {
    if (s[*start_index] == ' ') {
      *start_index = *start_index + 1;
      continue;
    } else {
      if (s[*start_index] == '(') {
          *start_index = *start_index + 1;
          expr p = create_expr(s, start_index, lim);
          if (!p) {
            printf("Bad synatx\n");
            return NULL;
          }
          t->type_tag = '(';
          t->value = p;
          t->next = (expr)malloc(sizeof(struct Expr));
          u = t;
          t = t->next;
      } else if (is_numeric(s[*start_index])) {
        int check_n = *start_index;
        double num =  get_number(s, start_index, lim);
        if ((*start_index - check_n) <= 0) {
          printf("bad stuff\n");
        }
        double *np = (double*)malloc(sizeof(double));
        *np = num;
        t->value = np;
        t->type_tag = 'n';
        t->next = (expr)malloc(sizeof(struct Expr));
        u = t;
        t = t->next;
      } else if (is_operator(s[*start_index])) {
        char *cp = (char*)malloc(sizeof(char));
        *cp = s[*start_index];
        t->value = cp;
        t->type_tag = 'o';
        t->next = (expr)malloc(sizeof(struct Expr));
        u = t;
        t = t->next;
        *start_index = *start_index + 1;
      } else {
        printf("Bad syntax\n");
        return NULL;
      }
    }
  }

    u->next = NULL;
    if (s[*start_index] == ')') {
      *start_index = *start_index + 1;
    }

  return begin_expr;
}

expr copy_expr(expr e) { //recursively copies the whole list
  if (!e) {
    return NULL;
  }
  expr e_copy = (expr)malloc(sizeof(struct Expr));
  if (e->type_tag == '(') {
    e_copy->type_tag = '(';
    e_copy->value  = copy_expr(e->value);
    e_copy->next = copy_expr(e->next);
  } else if (e->type_tag == 'o') {
    e_copy->type_tag = 'o';
    char *cp = (char*)malloc(sizeof(char));
    *cp = *((char*)e->value);
    e_copy->value = cp;
    e_copy->next = copy_expr(e->next);
  } else {
    e_copy->type_tag = 'n';
    double *dp = (double*)malloc(sizeof(double));
    *dp = *((double*)e->value);
    e_copy->value = dp;
    e_copy->next = copy_expr(e->next);
  }
  return e_copy;
}

expr copy_single_expr(expr e) { //copies only the first expr, doesn't copy the trailing exprs in the list
  if (!e) {
    return NULL;
  }
  expr e_single_copy = (expr)malloc(sizeof(struct Expr));
  if (e->type_tag == '(') {
    //type_tag is '(' for parenthesised expression
    e_single_copy->type_tag = '(';
    e_single_copy->value  = copy_expr(e->value); //if its a list, then we copy the whole underlying list
  } else if (e->type_tag == 'o') {
    //type_tag is 'o' for operator
    e_single_copy->type_tag = 'o';
    char *cp = (char*)malloc(sizeof(char));
    *cp = *((char*)e->value);
    e_single_copy->value = cp;
  } else {
    // the type_tag is 'n' for numeric
    e_single_copy->type_tag = 'n';
    double *dp = (double*)malloc(sizeof(double));
    *dp = *((double*)e->value);
    e_single_copy->value = dp;
  }
  return e_single_copy;
}

void free_expr(expr e) { //recursively frees the memory allocated for expression
  if (!e) return;
  free_expr(e->next);
  e->next = NULL;
  if (e->type_tag == '(') {
    free_expr(e->value);
    e->value = NULL;
  } else {
    free(e->value);
    e->value = NULL;
    free(e);
  }
}

void print_expr(expr e) {//prints the expr
  expr t = e;
  while (t != NULL) {
    if (t->type_tag == '(') {
      printf("(");
      print_expr(t->value);
      printf(") ");
      t = t->next;
    } else if (t->type_tag == 'n') {
      printf(" %.2lf ", *((double*) t->value));
      t = t->next;
    } else if (t->type_tag == 'o') {
      printf("%c ", *((char*)t->value));
      t = t->next;
    }
  }
}

int is_number(expr e) {
  return (e->type_tag == 'n' && !e->next);
}

double number(expr e) {
  return *((double*)e->value);
}

int is_sum(expr e) {//checks if the expression is a sum
  while (e) {
    if (e->type_tag == 'o' && *((char*)e->value) == '+') {
      return 1;
    }
    e = e->next;
  }
  return 0;
}

int is_product(expr e) {//checks if the expression is a product
  while (e) {
    if (e->type_tag == 'o' && *((char*)e->value) == '*') {
      return 1;
    }
    e = e->next;
  }
  return 0;
}

expr addend(expr e) {//get all the terms before the '+' sign
  expr e_addend  = copy_single_expr(e);
  expr t = e_addend;
  e = e->next;
  while (e && !(e->type_tag == 'o' && *((char*)e->value) == '+')) {
    t->next = copy_single_expr(e);
    t = t->next;
    e = e->next;
  }
  t->next = NULL;
  return e_addend;
}

expr augend(expr e) {//get all the terms after the '+' sign
  int flag = 0;

  while (!flag) {
    if (e->type_tag == 'o' && *((char*)e->value) == '+') {
      flag = 1;
      break;
    }
    e = e->next;
  }
  e = e->next;
  expr e_augend = copy_single_expr(e);
  expr t = e_augend;
  e = e->next;
  while (e) {
    t->next = copy_single_expr(e);
    t = t->next;
    e = e->next;
  }
  t->next = NULL;
  return e_augend;
}

expr multiplier(expr e) {//get all the terms before the '*' sign
  expr e_multiplier  = copy_single_expr(e);
  expr t = e_multiplier;
  e = e->next;
  while (e && !(e->type_tag == 'o' && *((char*)e->value) == '*')) {
    t->next = copy_single_expr(e);
    t = t->next;
    e = e->next;
  }
  t->next = NULL;
  return e_multiplier;
}

expr multiplicand(expr e) {//get all the terms after the '*' sign
  int flag = 0;

  while (!flag) {
    if (e->type_tag == 'o' && *((char*)e->value) == '*') {
      flag = 1;
      break;
    }
    e = e->next;
  }
  e = e->next;
  expr e_multiplicand = copy_single_expr(e);
  expr t = e_multiplicand;
  e = e->next;
  while (e) {
    t->next = copy_single_expr(e);
    t = t->next;
    e = e->next;
  }
  t->next = NULL;
  return e_multiplicand;
}

int is_expr_paren(expr e) { //does it have extraneous parenthesis like ((3+4))
  return (e->type_tag == '(');
}
int get_user_expr(char s[], int lim) {//get input string from user
  int c, i;
  for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; i++) {
    s[i] = c;
  }

  if (c == '\n') {
    s[i] = '\0';
  }
  return i; //returns the length of the whole string
}

int is_numeric(char c) {
  return ((c >= '0' && c <= '9') || c == '.');
}
int is_operator(char c) {
  return (c == '+' || c == '*');
}
double get_number(char* expr, int* start, int lim) {
  while (expr[*start] == ' ') {
    *start = *start + 1;
  }
  double val = 0;
  int flag = 0;
  double decimal_part = 0;
  int frac_digits = 0;
  double x_digit = 0;
  while((expr[*start] >= '0' && expr[*start] <= '9') || expr[*start] == '.') {
    if (expr[*start] == '.') {
      flag = 1;
      *start = *start + 1;
      continue;
    }
    if (!flag) {
      val = val * 10 + (expr[*start] - '0');
    } else {
      x_digit = (expr[*start] - '0');
      for (size_t k = 0; k <= frac_digits; k++) {
        x_digit = x_digit/10.0;
      }
      decimal_part = decimal_part + x_digit;
      frac_digits++;
    }
    *start = *start + 1;
  }
  return val + decimal_part;
}
