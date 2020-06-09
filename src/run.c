#include "defs.h"

char *trace1;
char *trace2;

void
run(char *s)
{
	if (setjmp(stop_return))
		return;

	init(0);

	for (;;) {

		trace1 = s;

		if (pratt_flag) {
			s = scan_with_pratt(s);
		} else {
			s = scan(s);
		}

		if (s == NULL)
			break; // end of input

		trace2 = s;

		trace_input();

		eval_and_print_result(1);

		if (tos || tof)
			stop("internal error 1");

		if (clear_flag)
			init(1);
	}
}

void
stop(char *s)
{
	if (draw_flag == 2)
		longjmp(draw_stop_return, 1);

	if (s) {
		print_input_line();
		printbuf("Stop: ", RED);
		printbuf(s, RED);
		printbuf("\n", RED);
	}

	longjmp(stop_return, 1);
}

char *init_script[] = {
	"e=exp(1)",
	"i=sqrt(-1)",
	"trange=(-pi,pi)",
	"xrange=(-10,10)",
	"yrange=(-10,10)",
	"last=0",
	"trace=0",
	"tty=0",
	"cross(u,v)=(u[2]*v[3]-u[3]*v[2],u[3]*v[1]-u[1]*v[3],u[1]*v[2]-u[2]*v[1])",
	"curl(u)=(d(u[3],y)-d(u[2],z),d(u[1],z)-d(u[3],x),d(u[2],x)-d(u[1],y))",
	"div(u)=d(u[1],x)+d(u[2],y)+d(u[3],z)",
	"ln(x)=log(x)",
};

void
init(int level)
{
	int i, n;

	expanding = 1;
	interrupt = 0;
	draw_flag = 0;
	clear_flag = 0;

	tos = 0;
	tof = 0;

	p0 = symbol(NIL);
	p1 = symbol(NIL);
	p2 = symbol(NIL);
	p3 = symbol(NIL);
	p4 = symbol(NIL);
	p5 = symbol(NIL);
	p6 = symbol(NIL);
	p7 = symbol(NIL);
	p8 = symbol(NIL);
	p9 = symbol(NIL);

	if (symtab[0].u.printname && level < 1) {
		set_binding(symbol(TRACE), zero);
		return;
	}

	init_symbol_table();

	push_integer(0);
	zero = pop();

	push_integer(1);
	one = pop();

	push_integer(-1);
	minusone = pop();

	push_symbol(POWER);
	push_integer(-1);
	push_rational(1, 2);
	list(3);
	imaginaryunit = pop();

	n = sizeof init_script / sizeof (char *);

	for (i = 0; i < n; i++) {
		if (pratt_flag) {
			scan_with_pratt(init_script[i]);
		} else {
			scan(init_script[i]);
		}
		eval();
		pop();
	}

	gc();
}

void
print_status(void)
{
	outbuf_index = 0;

	sprintf(tbuf, "block_count %d\n", block_count);
	print_str(tbuf);

	sprintf(tbuf, "free_count %d\n", free_count);
	print_str(tbuf);

	sprintf(tbuf, "gc_count %d\n", gc_count);
	print_str(tbuf);

	sprintf(tbuf, "bignum_count %d\n", bignum_count);
	print_str(tbuf);

	sprintf(tbuf, "string_count %d\n", string_count);
	print_str(tbuf);

	sprintf(tbuf, "tensor_count %d\n", tensor_count);
	print_str(tbuf);

	sprintf(tbuf, "max_stack %d (%d%%)\n", max_stack, 100 * max_stack / STACKSIZE);
	print_str(tbuf);

	sprintf(tbuf, "max_frame %d (%d%%)\n", max_frame, 100 * max_frame / FRAMESIZE);
	print_str(tbuf);

	print_char('\0');

	printbuf(outbuf, BLACK);
}

void
eval_run(void)
{
	push(cadr(p1));
	eval();
	p1 = pop();

	if (!isstr(p1))
		stop("string expected");

	run_file(p1->u.str);

	push_symbol(NIL);
}

void
run_file(char *filename)
{
	int fd, n;
	char *buf, *s, *t1, *t2;

	fd = open(filename, O_RDONLY, 0);

	if (fd == -1)
		stop("cannot open file");

	// get file size

	n = (int) lseek(fd, 0, SEEK_END);

	if (n < 0) {
		close(fd);
		stop("lseek error");
	}

	lseek(fd, 0, SEEK_SET);

	buf = malloc(n + 1);

	if (buf == NULL) {
		close(fd);
		malloc_kaput();
	}

	push_string(buf); // popped below

	if (read(fd, buf, n) != n) {
		close(fd);
		stop("read error");
	}

	close(fd);

	buf[n] = 0;

	s = buf;

	t1 = trace1;
	t2 = trace2;

	while (1) {

		trace1 = s;

		if (pratt_flag) {
			s = scan_with_pratt(s);
		} else {
			s = scan(s);
		}

		if (s == NULL)
			break; // end of input

		trace2 = s;

		trace_input();

		eval_and_print_result(1);

		if (clear_flag)
			stop("clear not allowed in run file");
	}

	trace1 = t1;
	trace2 = t2;

	pop(); // pop buffer
}

void
trace_input(void)
{
	char c, *s;
	if (iszero(binding[TRACE]))
		return;
	c = 0;
	s = trace1;
	outbuf_index = 0;
	while (*s && s < trace2) {
		c = *s++;
		print_char(c);
	}
	if (c != '\n')
		print_char('\n');
	print_char('\0');
	printbuf(outbuf, BLUE);
}

// suppress blank lines

void
print_input_line(void)
{
	char c, *s;
	c = '\n';
	s = trace1;
	outbuf_index = 0;
	while (*s && s < trace2) {
		if (*s == '\n' && c == '\n') {
			s++;
			continue;
		}
		c = *s++;
		print_char(c);
	}
	if (c != '\n')
		print_char('\n');
	print_char('\0');
	printbuf(outbuf, RED);
}

void
print_scan_line(char *s)
{
	trace2 = s;
	print_input_line();
}

