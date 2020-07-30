#include "defs.h"

void
eval_test(void)
{
	p1 = cdr(p1);
	while (iscons(p1)) {
		if (cdr(p1) == symbol(NIL)) {
			push(car(p1)); // default case
			eval();
			return;
		}
		push(car(p1));
		evalp();
		p2 = pop();
		if (!iszero(p2)) {
			push(cadr(p1));
			eval();
			return;
		}
		p1 = cddr(p1);
	}
	push_integer(0);
}

void
eval_check(void)
{
	push(cadr(p1));
	evalp();
	p1 = pop();
	if (iszero(p1))
		stop("check");
	push_symbol(NIL); // no result is printed
}

void
eval_testeq(void)
{
	int i, n;

	push(cadr(p1));
	eval();

	push(caddr(p1));
	eval();

	p2 = pop();
	p1 = pop();

	if (iszero(p1) && iszero(p2)) {
		push_integer(1);
		return;
	}

	if (iszero(p1) || iszero(p2)) {
		push_integer(0);
		return;
	}

	if (istensor(p1) && istensor(p2) && compatible_dimensions(p1, p2)) {
		n = p1->u.tensor->nelem;
		for (i = 0; i < n; i++) {
			if (testeq(p1->u.tensor->elem[i], p2->u.tensor->elem[i]))
				continue;
			push_integer(0);
			return;
		}
		push_integer(1);
		return;
	}

	if (istensor(p1) || istensor(p2)) {
		push_integer(0);
		return;
	}

	if (testeq(p1, p2))
		push_integer(1);
	else
		push_integer(0);
}

int
testeq(struct atom *q1, struct atom *q2)
{
	int t;
	save();
	p1 = q1;
	p2 = q2;
	t = testeq_nib();
	restore();
	return t;
}

int
testeq_nib(void)
{
	if (equal(p1, p2))
		return 1;

	while (cross_expr(p1)) {
		p0 = pop();
		push(p0);
		push(p1);
		cancel_factor();
		p1 = pop();
		push(p0);
		push(p2);
		cancel_factor();
		p2 = pop();
	}

	while (cross_expr(p2)) {
		p0 = pop();
		push(p0);
		push(p1);
		cancel_factor();
		p1 = pop();
		push(p0);
		push(p2);
		cancel_factor();
		p2 = pop();
	}

	push(p1);
	push(p2);
	subtract();

	p1 = pop();

	return iszero(p1);
}

int
cross_expr(struct atom *p)
{
	if (car(p) == symbol(ADD)) {
		p = cdr(p);
		while (iscons(p)) {
			if (cross_term(car(p)))
				return 1;
			p = cdr(p);
		}
		return 0;
	}

	return cross_term(p);
}

int
cross_term(struct atom *p)
{
	if (car(p) == symbol(MULTIPLY)) {
		p = cdr(p);
		while (iscons(p)) {
			if (cross_factor(car(p)))
				return 1;
			p = cdr(p);
		}
		return 0;
	}

	return cross_factor(p);
}

int
cross_factor(struct atom *p)
{
	if (isrational(p)) {
		if (MEQUAL(p->u.q.b, 1))
			return 0;
		push_rational_number(MPLUS, mcopy(p->u.q.b), mint(1));
		return 1;
	}

	if (car(p) == symbol(POWER) && !isminusone(cadr(p)) && isnegativeterm(caddr(p))) {
		if (isminusone(caddr(p)))
			push(cadr(p));
		else {
			push_symbol(POWER);
			push(cadr(p));
			push(caddr(p));
			negate();
			list(3);
		}
		return 1;
	}

	return 0;
}

void
cancel_factor(void)
{
	int h;

	save();

	p2 = pop();
	p1 = pop();

	if (car(p2) == symbol(ADD)) {
		h = tos;
		p2 = cdr(p2);
		while (iscons(p2)) {
			push(p1);
			push(car(p2));
			multiply();
			p2 = cdr(p2);
		}
		add_terms(tos - h);
	} else {
		push(p1);
		push(p2);
		multiply();
	}

	restore();
}

void
eval_testge(void)
{
	if (cmp_args() >= 0)
		push_integer(1);
	else
		push_integer(0);
}

void
eval_testgt(void)
{
	if (cmp_args() > 0)
		push_integer(1);
	else
		push_integer(0);
}

void
eval_testle(void)
{
	if (cmp_args() <= 0)
		push_integer(1);
	else
		push_integer(0);
}

void
eval_testlt(void)
{
	if (cmp_args() < 0)
		push_integer(1);
	else
		push_integer(0);
}

void
eval_not(void)
{
	push(cadr(p1));
	evalp();
	p1 = pop();
	if (iszero(p1))
		push_integer(1);
	else
		push_integer(0);
}

void
eval_and(void)
{
	p1 = cdr(p1);
	while (iscons(p1)) {
		push(car(p1));
		evalp();
		p2 = pop();
		if (iszero(p2)) {
			push_integer(0);
			return;
		}
		p1 = cdr(p1);
	}
	push_integer(1);
}

void
eval_or(void)
{
	p1 = cdr(p1);
	while (iscons(p1)) {
		push(car(p1));
		evalp();
		p2 = pop();
		if (!iszero(p2)) {
			push_integer(1);
			return;
		}
		p1 = cdr(p1);
	}
	push_integer(0);
}

int
cmp_args(void)
{
	int t;

	push(cadr(p1));
	eval();

	push(caddr(p1));
	eval();

	p2 = pop();
	p1 = pop();

	if (istensor(p1) || istensor(p2))
		stop("tensor comparison");

	push(p1);
	push(p2);
	subtract();
	p1 = pop();

	if (!isnum(p1)) {
		push(p1);
		sfloat(); // try converting pi and e
		p1 = pop();
		if (!isnum(p1))
			stop("non-numerical comparison");
	}

	if (iszero(p1))
		t = 0;
	else if (isrational(p1))
		t = (p1->sign == MMINUS) ? -1 : 1;
	else
		t = (p1->u.d < 0.0) ? -1 : 1;

	return t;
}

// like eval() except '=' is evaluated as '=='

void
evalp(void)
{
	save();
	p1 = pop();
	if (car(p1) == symbol(SETQ))
		eval_testeq();
	else {
		push(p1);
		eval();
	}
	restore();
}
