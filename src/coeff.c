#include "defs.h"

// get the coefficient of x^n in polynomial p(x)

#undef P
#undef X
#undef N

#define P p1
#define X p2
#define N p3

void
eval_coeff(void)
{
	push(cadr(p1));			// 1st arg, p
	eval();

	push(caddr(p1));		// 2nd arg, x
	eval();

	push(cadddr(p1));		// 3rd arg, n
	eval();

	N = pop();
	X = pop();
	P = pop();

	if (N == symbol(NIL)) {		// only 2 args?
		N = X;
		X = symbol(SYMBOL_X);
	}

	push(P);			// divide p by x^n
	push(X);
	push(N);
	power();
	divide();

	push(X);			// keep the constant part
	filter();
}

//	Put polynomial coefficients on the stack
//
//	Input:		tos-2		p(x)
//
//			tos-1		x
//
//	Output:		Returns number of coefficients on stack
//
//			tos-n		Coefficient of x^0
//
//			tos-1		Coefficient of x^(n-1)

int
coeff(void)
{
	int h, n;

	save();

	p2 = pop();
	p1 = pop();

	h = tos;

	for (;;) {

		push(p1);
		push(p2);
		push_integer(0);
		subst();
		eval();

		p3 = pop();
		push(p3);

		push(p1);
		push(p3);
		subtract();

		p1 = pop();

		if (iszero(p1)) {
			n = tos - h;
			restore();
			return n;
		}

		push(p1);
		push(p2);
		divide();
		p1 = pop();
	}
}
