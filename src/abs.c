#include "defs.h"

void
eval_abs(void)
{
	push(cadr(p1));
	eval();
	absval();
}

void
absval(void)
{
	save();
	absval_nib();
	restore();
}

void
absval_nib(void)
{
	int h;

	p1 = pop();

	if (istensor(p1)) {
		absval_tensor();
		return;
	}

	if (isnum(p1)) {
		push(p1);
		if (isnegativenumber(p1))
			negate();
		return;
	}

	if (iscomplexnumber(p1)) {
		push(p1);
		push(p1);
		conjugate();
		multiply();
		push_rational(1, 2);
		power();
		return;
	}

	// abs(1/a) evaluates to 1/abs(a)

	if (car(p1) == symbol(POWER) && isnegativeterm(caddr(p1))) {
		push(p1);
		reciprocate();
		absval();
		reciprocate();
		return;
	}

	// abs(a*b) evaluates to abs(a)*abs(b)

	if (car(p1) == symbol(MULTIPLY)) {
		h = tos;
		p1 = cdr(p1);
		while (iscons(p1)) {
			push(car(p1));
			absval();
			p1 = cdr(p1);
		}
		multiply_factors(tos - h);
		return;
	}

	if (isnegativeterm(p1) || (car(p1) == symbol(ADD) && isnegativeterm(cadr(p1)))) {
		push(p1);
		negate();
		p1 = pop();
	}

	push_symbol(ABS);
	push(p1);
	list(2);
}

void
absval_tensor(void)
{
	if (p1->u.tensor->ndim != 1)
		stop("abs: tensor rank > 1");
	push(p1);
	push(p1);
	conjugate();
	inner();
	push_rational(1, 2);
	power();
	simplify();
}
