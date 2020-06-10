#include "defs.h"

void
eval_arctanh(void)
{
	push(cadr(p1));
	eval();
	arctanh();
}

void
arctanh(void)
{
	save();
	arctanh_nib();
	restore();
}

void
arctanh_nib(void)
{
	p1 = pop();

	if (equaln(p1, 1) || equaln(p1, -1)) {
		push_symbol(ARCTANH);
		push(p1);
		list(2);
		return;
	}

	if (isdouble(p1)) {
		push_double(atanh(p1->u.d));
		return;
	}

	// arctanh(z) = 1/2 (log(1 + z) - log(1 - z))

	if (isdoublez(p1)) {
		push_double(1.0);
		push(p1);
		add();
		logarithm();
		push_double(1.0);
		push(p1);
		subtract();
		logarithm();
		subtract();
		push_double(0.5);
		multiply();
		return;
	}

	if (iszero(p1)) {
		push_integer(0);
		return;
	}

	// arctanh(-x) = -arctanh(x)

	if (isnegative(p1)) {
		push(p1);
		negate();
		arctanh();
		negate();
		return;
	}

	if (car(p1) == symbol(TANH)) {
		push(cadr(p1));
		return;
	}

	push_symbol(ARCTANH);
	push(p1);
	list(2);
}
