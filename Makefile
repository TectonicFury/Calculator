CFLAGS=-Wall	-g	-std=c11

all:	calculator test

calculator:	calc.o Expr.o
	clang	-o	calculator.app	calc.o	Expr.o $(CFLAGS)

calc.o:	calc.c Expr.h

Expr.o: Expr.c Expr.h

test:	test_calc.o Expr.o Expr.h
	clang	-o	calc.tests	test_calc.o	Expr.o	$(CFLAGS)
clean:
	rm	calculator.app	*.o	*.tests
