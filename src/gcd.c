// Greatest common denominator

#include "defs.h"

void
eval_gcd(void)
{
	p1 = cdr(p1);
	push(car(p1));
	eval();
	p1 = cdr(p1);
	while (iscons(p1)) {
		push(car(p1));
		eval();
		gcd();
		p1 = cdr(p1);
	}
}

void
gcd(void)
{
	int x = expanding;
	save();
	gcd_main();
	restore();
	expanding = x;
}

void
gcd_main(void)
{
	expanding = 1;

	p2 = pop();
	p1 = pop();

	if (equal(p1, p2)) {
		push(p1);
		return;
	}

	if (isrational(p1) && isrational(p2)) {
		push(p1);
		push(p2);
		gcd_rational_numbers();
		return;
	}

	if (car(p1) == symbol(ADD) && car(p2) == symbol(ADD)) {
		gcd_expr_expr();
		return;
	}

	if (car(p1) == symbol(ADD)) {
		gcd_expr(p1);
		p1 = pop();
	}

	if (car(p2) == symbol(ADD)) {
		gcd_expr(p2);
		p2 = pop();
	}

	if (car(p1) == symbol(MULTIPLY) && car(p2) == symbol(MULTIPLY)) {
		gcd_term_term();
		return;
	}

	if (car(p1) == symbol(MULTIPLY)) {
		gcd_term_factor();
		return;
	}

	if (car(p2) == symbol(MULTIPLY)) {
		gcd_factor_term();
		return;
	}

	// gcd of factors

	if (car(p1) == symbol(POWER)) {
		p3 = caddr(p1);
		p1 = cadr(p1);
	} else
		p3 = one;

	if (car(p2) == symbol(POWER)) {
		p4 = caddr(p2);
		p2 = cadr(p2);
	} else
		p4 = one;

	if (!equal(p1, p2)) {
		push_integer(1);
		return;
	}

	// are both exponents numerical?

	if (isnum(p3) && isnum(p4)) {
		push(p1);
		if (lessp(p3, p4))
			push(p3);
		else
			push(p4);
		power();
		return;
	}

	// are the exponents multiples of eah other?

	push(p3);
	push(p4);
	divide();

	p5 = pop();

	if (isnum(p5)) {

		push(p1);

		// choose the smallest exponent

		if (car(p3) == symbol(MULTIPLY) && isnum(cadr(p3)))
			p5 = cadr(p3);
		else
			p5 = one;

		if (car(p4) == symbol(MULTIPLY) && isnum(cadr(p4)))
			p6 = cadr(p4);
		else
			p6 = one;

		if (lessp(p5, p6))
			push(p3);
		else
			push(p4);

		power();
		return;
	}

	push(p3);
	push(p4);
	subtract();

	p5 = pop();

	if (!isnum(p5)) {
		push_integer(1);
		return;
	}

	// can't be equal because of test near beginning

	push(p1);

	if (isnegativenumber(p5))
		push(p3);
	else
		push(p4);

	power();
}

// in this case gcd is used as a composite function, i.e. gcd(gcd(gcd...

void
gcd_expr_expr(void)
{
	if (length(p1) != length(p2)) {
		push_integer(1);
		return;
	}

	p3 = cdr(p1);
	push(car(p3));
	p3 = cdr(p3);
	while (iscons(p3)) {
		push(car(p3));
		gcd();
		p3 = cdr(p3);
	}
	p3 = pop();

	p4 = cdr(p2);
	push(car(p4));
	p4 = cdr(p4);
	while (iscons(p4)) {
		push(car(p4));
		gcd();
		p4 = cdr(p4);
	}
	p4 = pop();

	push(p1);
	push(p3);
	divide();
	p5 = pop();

	push(p2);
	push(p4);
	divide();
	p6 = pop();

	if (equal(p5, p6)) {
		push(p5);
		push(p3);
		push(p4);
		gcd();
		multiply();
	} else
		push_integer(1);
}

void
gcd_expr(struct atom *p)
{
	p = cdr(p);
	push(car(p));
	p = cdr(p);
	while (iscons(p)) {
		push(car(p));
		gcd();
		p = cdr(p);
	}
}

void
gcd_term_term(void)
{
	push_integer(1);
	p3 = cdr(p1);
	while (iscons(p3)) {
		p4 = cdr(p2);
		while (iscons(p4)) {
			push(car(p3));
			push(car(p4));
			gcd();
			multiply();
			p4 = cdr(p4);
		}
		p3 = cdr(p3);
	}
}

void
gcd_term_factor(void)
{
	push_integer(1);
	p3 = cdr(p1);
	while (iscons(p3)) {
		push(car(p3));
		push(p2);
		gcd();
		multiply();
		p3 = cdr(p3);
	}
}

void
gcd_factor_term(void)
{
	push_integer(1);
	p4 = cdr(p2);
	while (iscons(p4)) {
		push(p1);
		push(car(p4));
		gcd();
		multiply();
		p4 = cdr(p4);
	}
}

void
gcd_rational_numbers(void)
{
	uint32_t *a, *b;
	save();
	p2 = pop();
	p1 = pop();
	a = mgcd(p1->u.q.a, p2->u.q.a);
	b = mgcd(p1->u.q.b, p2->u.q.b);
	push_rational_number(MPLUS, a, b);
	restore();
}
