// Find the least common multiple of two expressions.

#include "defs.h"

void
eval_lcm(void)
{
	p1 = cdr(p1);
	push(car(p1));
	eval();
	p1 = cdr(p1);
	while (iscons(p1)) {
		push(car(p1));
		eval();
		lcm();
		p1 = cdr(p1);
	}
}

void
lcm(void)
{
	expanding++;
	save();
	lcm_nib();
	restore();
	expanding--;
}

void
lcm_nib(void)
{
	p2 = pop();
	p1 = pop();

	push(p1);
	push(p2);
	gcd();

	push(p1);
	divide();

	push(p2);
	divide();

	reciprocate();
}
