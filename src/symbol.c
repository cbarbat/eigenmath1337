#include "defs.h"

// symbol lookup, create symbol if not found

struct atom *
lookup(char *s)
{
	int c, i, j;
	char *t;
	struct atom *p;

	c = tolower(*s) - 'a';

	if (c < 0 || c > 25)
		c = 26;

	j = NSYM * c;

	for (i = 0; i < NSYM; i++) {
		p = symtab[j];
		if (p == NULL)
			break;
		if (p->k == KSYM)
			t = p->u.ksym.name;
		else
			t = p->u.usym.name;
		if (strcmp(s, t) == 0)
			return p;
		j++;
	}

	if (i == NSYM)
		stop("symbol table full");

	p = alloc();
	s = strdup(s);

	if (s == NULL)
		malloc_kaput();

	p->k = USYM;
	p->u.usym.name = s;
	p->u.usym.index = j;

	symtab[j] = p;
	binding[j] = p;
	arglist[j] = symbol(NIL);

	usym_count++;

	return p;
}

char *
printname(struct atom *p)
{
	if (p->k == KSYM)
		return p->u.ksym.name;
	else if (p->k == USYM)
		return p->u.usym.name;
	else
		return "?";
}

void
set_binding(struct atom *p, struct atom *b)
{
	if (p->k != USYM || p == symbol(NIL) || p == symbol(PI))
		stop("reserved symbol");
	binding[p->u.usym.index] = b;
	arglist[p->u.usym.index] = symbol(NIL);
}

void
set_binding_and_arglist(struct atom *p, struct atom *b, struct atom *a)
{
	if (p->k != USYM || p == symbol(NIL) || p == symbol(PI))
		stop("reserved symbol");
	binding[p->u.usym.index] = b;
	arglist[p->u.usym.index] = a;
}

struct atom *
get_binding(struct atom *p)
{
	if (p->k != USYM)
		stop("symbol error");
	return binding[p->u.usym.index];
}

struct atom *
get_arglist(struct atom *p)
{
	if (p->k != USYM)
		stop("symbol error");
	return arglist[p->u.usym.index];
}

struct se {
	char *str;
	int index;
	void (*func)(void);
};

struct se stab[] = {

	{ "abs",		ABS,		eval_abs		},
	{ "adj",		ADJ,		eval_adj		},
	{ "and",		AND,		eval_and		},
	{ "arccos",		ARCCOS,		eval_arccos		},
	{ "arccosh",		ARCCOSH,	eval_arccosh		},
	{ "arcsin",		ARCSIN,		eval_arcsin		},
	{ "arcsinh",		ARCSINH,	eval_arcsinh		},
	{ "arctan",		ARCTAN,		eval_arctan		},
	{ "arctanh",		ARCTANH,	eval_arctanh		},
	{ "arg",		ARG,		eval_arg		},
	{ "atomize",		ATOMIZE,	eval_atomize		},

	{ "besselj",		BESSELJ,	eval_besselj		},
	{ "bessely",		BESSELY,	eval_bessely		},
	{ "binding",		BINDING,	eval_binding		},
	{ "binomial",		BINOMIAL,	eval_binomial		},

	{ "ceiling",		CEILING,	eval_ceiling		},
	{ "check",		CHECK,		eval_check		},
	{ "choose",		CHOOSE,		eval_choose		},
	{ "circexp",		CIRCEXP,	eval_circexp		},
	{ "clear",		CLEAR,		eval_clear		},
	{ "clock",		CLOCK,		eval_clock		},
	{ "coeff",		COEFF,		eval_coeff		},
	{ "cofactor",		COFACTOR,	eval_cofactor		},
	{ "conj",		CONJ,		eval_conj		},
	{ "contract",		CONTRACT,	eval_contract		},
	{ "cos",		COS,		eval_cos		},
	{ "cosh",		COSH,		eval_cosh		},

	{ "d",			SYMBOL_D,	NULL			},
	{ "defint",		DEFINT,		eval_defint		},
	{ "deg",		DEGREE,		eval_degree		},
	{ "denominator",	DENOMINATOR,	eval_denominator	},
	{ "derivative",		DERIVATIVE,	eval_derivative		},
	{ "det",		DET,		eval_det		},
	{ "dim",		DIM,		eval_dim		},
	{ "do",			DO,		eval_do			},
	{ "dot",		DOT,		eval_dot		},
	{ "draw",		DRAW,		eval_draw		},

	{ "eigen",		EIGEN,		eval_eigen		},
	{ "eigenval",		EIGENVAL,	eval_eigenval		},
	{ "eigenvec",		EIGENVEC,	eval_eigenvec		},
	{ "erf",		ERF,		eval_erf		},
	{ "erfc",		ERFC,		eval_erfc		},
	{ "eval",		EVAL,		eval_eval		},
	{ "exit",		EXIT,		eval_exit		},
	{ "exp",		EXP,		eval_exp		},
	{ "expand",		EXPAND,		eval_expand		},
	{ "expcos",		EXPCOS,		eval_expcos		},
	{ "expcosh",		EXPCOSH,	eval_expcosh		},
	{ "expsin",		EXPSIN,		eval_expsin		},
	{ "expsinh",		EXPSINH,	eval_expsinh		},
	{ "exptan",		EXPTAN,		eval_exptan		},
	{ "exptanh",		EXPTANH,	eval_exptanh		},

	{ "factor",		FACTOR,		eval_factor		},
	{ "factorial",		FACTORIAL,	eval_factorial		},
	{ "filter",		FILTER,		eval_filter		},
	{ "float",		FLOATF,		eval_float		},
	{ "floor",		FLOOR,		eval_floor		},
	{ "for",		FOR,		eval_for		},

	{ "gcd",		GCD,		eval_gcd		},

	{ "hermite",		HERMITE,	eval_hermite		},
	{ "hilbert",		HILBERT,	eval_hilbert		},

	{ "i",			SYMBOL_I,	NULL			},
	{ "imag",		IMAG,		eval_imag		},
	{ "inner",		INNER,		eval_inner		},
	{ "integral",		INTEGRAL,	eval_integral		},
	{ "inv",		INV,		eval_inv		},
	{ "isprime",		ISPRIME,	eval_isprime		},

	{ "j",			SYMBOL_J,	NULL			},

	{ "laguerre",		LAGUERRE,	eval_laguerre		},
	{ "last",		LAST,		NULL			},
	{ "latex",		LATEX,		eval_latex		},
	{ "lcm",		LCM,		eval_lcm		},
	{ "leading",		LEADING,	eval_leading		},
	{ "legendre",		LEGENDRE,	eval_legendre		},
	{ "lisp",		LISP,		eval_lisp		},
	{ "log",		LOG,		eval_log		},

	{ "mag",		MAG,		eval_mag		},
	{ "mathjax",		MATHJAX,	eval_mathjax		},
	{ "mathml",		MATHML,		eval_mathml		},
	{ "mod",		MOD,		eval_mod		},

	{ "nil",		NIL,		NULL			},
	{ "not",		NOT,		eval_not		},
	{ "nroots",		NROOTS,		eval_nroots		},
	{ "number",		NUMBER,		eval_number		},
	{ "numerator",		NUMERATOR,	eval_numerator		},

	{ "or",			OR,		eval_or			},
	{ "outer",		OUTER,		eval_outer		},

	{ "pi",			PI,		NULL			},
	{ "polar",		POLAR,		eval_polar		},
	{ "pratt",		PRATT,		eval_pratt		},
	{ "prime",		PRIME,		eval_prime		},
	{ "print",		PRINT,		eval_print		},
	{ "product",		PRODUCT,	eval_product		},

	{ "quote",		QUOTE,		eval_quote		},
	{ "quotient",		QUOTIENT,	eval_quotient		},

	{ "rank",		RANK,		eval_rank		},
	{ "rationalize",	RATIONALIZE,	eval_rationalize	},
	{ "real",		REAL,		eval_real		},
	{ "rect",		RECTF,		eval_rect		},
	{ "roots",		ROOTS,		eval_roots		},
	{ "run",		RUN,		eval_run		},

	{ "s",			SYMBOL_S,	NULL			},
	{ "sgn",		SGN,		eval_sgn		},
	{ "simplify",		SIMPLIFY,	eval_simplify		},
	{ "sin",		SIN,		eval_sin		},
	{ "sinh",		SINH,		eval_sinh		},
	{ "sqrt",		SQRT,		eval_sqrt		},
	{ "status",		STATUS,		eval_status		},
	{ "stop",		STOP,		eval_stop		},
	{ "string",		STRING,		eval_string		},
	{ "subst",		SUBST,		eval_subst		},
	{ "sum",		SUM,		eval_sum		},

	{ "t",			SYMBOL_T,	NULL			},
	{ "tan",		TAN,		eval_tan		},
	{ "tanh",		TANH,		eval_tanh		},
	{ "taylor",		TAYLOR,		eval_taylor		},
	{ "test",		TEST,		eval_test		},
	{ "testeq",		TESTEQ,		eval_testeq		},
	{ "testge",		TESTGE,		eval_testge		},
	{ "testgt",		TESTGT,		eval_testgt		},
	{ "testle",		TESTLE,		eval_testle		},
	{ "testlt",		TESTLT,		eval_testlt		},
	{ "trace",		TRACE,		NULL			},
	{ "transpose",		TRANSPOSE,	eval_transpose		},
	{ "tty",		TTY,		NULL			},

	{ "unit",		UNIT,		eval_unit		},

	{ "x",			SYMBOL_X,	NULL			},

	{ "y",			SYMBOL_Y,	NULL			},

	{ "z",			SYMBOL_Z,	NULL			},
	{ "zero",		ZERO,		eval_zero		},

	{ "+",			ADD,		eval_add		},
	{ "*",			MULTIPLY,	eval_multiply		},
	{ "^",			POWER,		eval_power		},
	{ "[",			INDEX,		eval_index		},
	{ "=",			SETQ,		eval_setq		},
	{ "(e)",		EXP1,		NULL			},
	{ "(a)",		METAA,		NULL			},
	{ "(b)",		METAB,		NULL			},
	{ "(x)",		METAX,		NULL			},
	{ "(X)",		SPECX,		NULL			},
};

void
init_symbol_table(void)
{
	int i, n;
	char *s;
	struct atom *p;

	memset(symtab, 0, 27 * NSYM * sizeof (struct atom *));
	memset(binding, 0, 27 * NSYM * sizeof (struct atom *));
	memset(arglist, 0, 27 * NSYM * sizeof (struct atom *));

	n = sizeof stab / sizeof (struct se);

	for (i = 0; i < n; i++) {
		p = alloc();
		s = strdup(stab[i].str);
		if (s == NULL)
			malloc_kaput();
		if (stab[i].func) {
			p->k = KSYM;
			p->u.ksym.name = s;
			p->u.ksym.func = stab[i].func;
			ksym_count++;
		} else {
			p->k = USYM;
			p->u.usym.name = s;
			p->u.usym.index = stab[i].index;
			usym_count++;
		}
		symtab[stab[i].index] = p;
		binding[stab[i].index] = p;
		arglist[stab[i].index] = p; // in case gc gets called
	}

	// do this last to ensure NIL is initialized

	for (i = 0; i < n; i++)
		arglist[stab[i].index] = symbol(NIL);
}
