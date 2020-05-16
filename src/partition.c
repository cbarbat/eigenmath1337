/* Partition a term

	Input stack:

		term (factor or product of factors)

		free variable

	Output stack:

		constant expression

		variable expression
*/

#include "defs.h"

void
partition(void)
{
	save();

	p2 = pop();
	p1 = pop();

	p3 = one;
	p4 = one;

	p1 = cdr(p1);

	while (iscons(p1)) {
		if (find(car(p1), p2)) {
			push(p4);
			push(car(p1));
			multiply();
			p4 = pop();
		} else {
			push(p3);
			push(car(p1));
			multiply();
			p3 = pop();
		}
		p1 = cdr(p1);
	}

	push(p3);
	push(p4);

	restore();
}
