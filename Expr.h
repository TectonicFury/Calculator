#ifndef EXPR_H_
#define EXPR_H_
typedef struct Expr *expr;
int get_user_expr(char *s, int lim); //gets a string expression from terminal
void print_expr(expr e); //prints the user expression
int is_numeric(char c);
double get_number(char* expr, int* start, int lim); //gets a number from the user string
int is_operator(char c); //checks if the character is an operator (+ *)

expr create_expr(char *s, int *start_index, int lim); //creates a linked list on the lines of a LISP list to enable quick processing of the user expression string
void free_expr(expr e); //recursively frees the Expression linked list
expr copy_expr(expr e); //deep-copy the expression
expr copy_single_expr(expr e);
double eval(char* s);//the outermost eval function
double eval_expr(expr e);//the most crucial function
expr addend(expr e);//gets the first term in a sum
expr augend(expr e);//gets the second term in a sum
expr multiplier(expr e);//gets the first term in a product
expr multiplicand(expr e);//gets the second term in a product
int is_number(expr e);
int is_sum(expr e);
int is_product(expr e);
int is_expr_paren(expr e);
double number(expr e);
#endif
