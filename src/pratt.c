// This scanner uses the method of V. Pratt.
//
// The char pointers token_str and scan_str are pointers to the input string as
// in the following example.
//
//	| g | a | m | m | a |   | a | l | p | h | a |
//	  ^                   ^
//	  token_str           scan_str
//
// The char pointer token_buf points to a malloc buffer.
//
//	| g | a | m | m | a | \0 |
//	  ^
//	  token_buf

#include "defs.h"

#define T_INTEGER 1001
#define T_DOUBLE 1002
#define T_SYMBOL 1003
#define T_FUNCTION 1004
#define T_NEWLINE 1006
#define T_STRING 1007
#define T_GTEQ 1008
#define T_LTEQ 1009
#define T_EQ 1010
#define T_END 1011

int token;
int scan_mode;
int scan_level;
int pptoken, ptoken, ppnewline_flag, pnewline_flag, newline_flag, newline_at_top_level;

char *scan_str;
char *token_str;
char *token_buf;
char *ptoken_buf;

#define MIN_BP 0
#define MUL_BP 40
#define MAX_BP 10000

typedef void (*denotation)(void); // used for nud and led in the Pratt parser

struct token_info { // stores information about a token
	int token;
	int lbp;    // left binding power
	int nbp;    // next binding power
	denotation nud; // null denotation
	denotation led; // left denotation
};

// lbp and nbp are not used for nuds!
#define MAX_SYM 19
struct token_info sym_table[MAX_SYM + 1] = { // stores the nuds and leds
	// token   , lbp   , nbp       , nud      , led
	{T_INTEGER , MIN_BP, MIN_BP    , &nud_atom, &led_err}, // nilfix
	{T_DOUBLE  , MIN_BP, MIN_BP    , &nud_atom, &led_err}, // nilfix
	{T_STRING  , MIN_BP, MIN_BP    , &nud_atom, &led_err}, // nilfix
	{T_SYMBOL  , MIN_BP, MIN_BP    , &nud_atom, &led_err}, // nilfix
	{T_FUNCTION, MIN_BP, MIN_BP    , &nud_func, &led_err}, // nilfix rbp 0
	{'('       , MIN_BP, MIN_BP    , &nud_tens, &led_err}, // nilfix rbp 0
	{'='       , 10    , 10        , &nud_err , &led_rel}, // infixN rbp 10
	{T_EQ      , 20    , 20        , &nud_err , &led_rel}, // infixN rbp 20
	{T_GTEQ    , 20    , 20        , &nud_err , &led_rel}, // infixN rbp 20
	{T_LTEQ    , 20    , 20        , &nud_err , &led_rel}, // infixN rbp 20
	{'>'       , 20    , 20        , &nud_err , &led_rel}, // infixN rbp 20
	{'<'       , 20    , 20        , &nud_err , &led_rel}, // infixN rbp 20
	{'+'       , 30    , 31        , &nud_sign, &led_add}, // add, infixL rbp 30
	{'-'       , 30    , 31        , &nud_sign, &led_add}, // sub, infixL rbp 30
	{'*'       , MUL_BP, MUL_BP + 1, &nud_err , &led_mul}, // mul, infixL rbp 40
	{'/'       , MUL_BP, MUL_BP + 1, &nud_err , &led_mul}, // div, infixL rbp 40
	{'^'       , 50    , 51        , &nud_err , &led_pow}, // pow, infixR rbp 49
	{'!'       , 60    , 61        , &nud_err , &led_fac}, // suffix
	{'['       , 80    , 81        , &nud_err , &led_arr}, // array access infixL rbp 0
	{T_END     , MIN_BP, MIN_BP    , &nud_err , &led_err}  // dummy
};

char *
scan_with_pratt(char *s)
{
	scan_mode = 0;
	return scan_with_pratt_nib(s);
}

char *
scan_with_pratt1(char *s)
{
	scan_mode = 1; // mode for table of integrals
	return scan_with_pratt_nib(s);
}


char *
scan_with_pratt_nib(char * s)
{
	scan_str = s;
	scan_level = 0;
	get_token_skip_newlines2();
	if (token == T_END)
		return NULL;
	pratt(MIN_BP);
	if (newline_flag == 0 && token != T_END)
		scan_error2("expected newline");
	return scan_str;
}

void
nud_err(void)
{
	scan_str = token_str; // better error display
	scan_error2("syntax error");
}

void
led_err(void)
{
	scan_error2("no led found");
}

void
nud_atom(void)
{
	switch (ptoken) {
		case T_INTEGER :
			bignum_scan_integer(ptoken_buf);
			break;
		case T_DOUBLE :
			push_double(atof(ptoken_buf));
			break;
		case T_STRING :
			push_string(ptoken_buf);
			break;
		case T_SYMBOL :
			if (scan_mode && strlen(ptoken_buf) == 1)
				switch (ptoken_buf[0]) {
				case 'a':
					push(symbol(METAA));
					break;
				case 'b':
					push(symbol(METAB));
					break;
				case 'x':
					push(symbol(METAX));
					break;
				default:
					push(usr_symbol(ptoken_buf));
					break;
				}
			else
				push(usr_symbol(ptoken_buf));
			break;
	}
}

void
nud_func(void)
{
	int n = 1;
	struct atom * p;
	p = usr_symbol(token_buf);
	push(p);
	get_token2(); // after left paren
	if (token != ')') {
		pratt(MIN_BP);
		n++;
		while (token == ',') {
			get_token2(); // after comma
			pratt(MIN_BP);
			n++;
		}
	}
	if (token != ')')
		scan_error2(") expected");
	// after right paren
	get_token2();
	list(n);
}

void
nud_tens(void)
{
	int n;
	if (ptoken != '(')
		scan_error2("( expected");
	pratt(MIN_BP);
	if (token == ',') {
		n = 1;
		while (token == ',') {
			get_token2();
			pratt(MIN_BP);
			n++;
		}
		build_tensor(tos - n);
	}
	if (token != ')')
		scan_error2(") expected");
	get_token2();
}

void
nud_sign(void)
{
	if (ptoken == '+') {
		pratt(30); // get right on the stack
	} else if (ptoken == '-') {
		pratt(30); // get right on the stack
		static_negate();
	}
}

void
led_rel(void)
{
	// left is on stack
	switch (ptoken) {
	case T_EQ:
		push_symbol(TESTEQ);
		swap();
		pratt(20);
		list(3);
		break;
	case T_GTEQ:
		push_symbol(TESTGE);
		swap();
		pratt(20);
		list(3);
		break;
	case T_LTEQ:
		push_symbol(TESTLE);
		swap();
		pratt(20);
		list(3);
		break;
	case '>':
		push_symbol(TESTGT);
		swap();
		pratt(20);
		list(3);
		break;
	case '<':
		push_symbol(TESTLT);
		swap();
		pratt(20);
		list(3);
		break;
	case '=':
		push_symbol(SETQ);
		swap();
		pratt(10);
		list(3);
		break;
	default:
		break;
	}
}

void
led_add(void)
{
	int h = tos - 1;

	// left is on stack
	// put right on stack
	while (ptoken == '+' || ptoken == '-')
	{
		if (ptoken == '+') {
			pratt(30);
		}
		else if (ptoken == '-') {
			pratt(30);
			static_negate();
		}
	}
	list(tos - h);
	push_symbol(ADD);
	swap();
	cons();
}

int
factor_pending(void) {
	switch (token) {
	case '*':
	case '/':
		get_token_skip_newlines2();
	case '(':
	case T_SYMBOL:
	case T_FUNCTION:
	case T_INTEGER:
	case T_DOUBLE:
	case T_STRING:
		return 1;
	default:
		break;
	}
	return 0;
}

void
led_mul(void)
{
	int h = tos - 1;

	// left is on stack
	// put right on stack
	while (factor_pending())
	{
		newline_at_top_level = (newline_flag == 1 && scan_level == 0);
		if (ptoken == '/')
		{
			pratt(MUL_BP);
			static_reciprocate();
		}
		else if (ptoken == '*')
		{
			pratt(MUL_BP);
		}
		else if (!newline_at_top_level)
		{
			pratt(MUL_BP);
		}
		else break;
	}
	if (tos - h > 0)
	{
		list(tos - h);
		push_symbol(MULTIPLY);
		swap();
		cons();
	}
	else
		scan_error2("expected operand");
}

void
led_pow(void)
{
	// left is on stack
	if (ptoken == '^') {
		push_symbol(POWER);
		swap();
		pratt(49);
		list(3);
	}
}

void
led_fac(void)
{
	// left is on stack
	push_symbol(FACTORIAL);
	swap();
	list(2);
}

void
led_arr(void)
{
	int h = tos;

	// left is on stack
	if (ptoken == '[') {
		push_symbol(INDEX);
		swap();
		pratt(MIN_BP);
		while (token == ',') {
			get_token2();
			pratt(MIN_BP);
		}
		if (token != ']')
			scan_error2("] expected");
		get_token2();
		list(tos - h + 1);
	}
}

int
find_token(int token) {
	int i;
	for (i=0; i < MAX_SYM; i++)
		if (token == sym_table[i].token)
			break;
	return i;
}

void
pratt(int rbp)
{
	// rbp = right binding power

	int i, nbp, lbp, token_has_nud, token_has_led;
	denotation nud, led;

	nbp = MAX_BP;
	i = find_token(token);
	token_has_nud = (sym_table[i].nud != &nud_err);
	nud = sym_table[i].nud;
	if (!token_has_nud) {
		scan_error2("expected operand");
	}
	if (token == '+' || token == '-') {
		if (ptoken == '+' || ptoken == '-' || ptoken == '*' || ptoken == '/' || ptoken == '^')
			scan_error2("expected operand");
		else
			get_token_skip_newlines2(); // skip after + or - sign
	} else
		get_token2();
	nud();
	while (1) {
		i = find_token(token);
		token_has_nud = (sym_table[i].nud != &nud_err);
		token_has_led = (sym_table[i].led != &led_err);
		if (token_has_led) {
			lbp = sym_table[i].lbp;
		} else if (token_has_nud) {
			lbp = MUL_BP; // implicit multiplication
		} else {
			lbp = MIN_BP;
		}
		if (!(rbp < lbp && lbp < nbp))
			break;
		newline_at_top_level = (newline_flag == 1 && scan_level == 0);
		if ((!newline_at_top_level || !token_has_nud) && token_has_led) {
			nbp = sym_table[i].nbp;
			led = sym_table[i].led;
			get_token_skip_newlines2(); // after binary operator
			led();
		} else if (!newline_at_top_level && token_has_nud && !token_has_led) {
			nbp = MAX_BP;
			led = &led_mul; // implicit multiplication
			led();
		} else
			break;
	}
}

void
get_token_skip_newlines2(void)
{
	scan_level++;
	get_token2();
	scan_level--;
}

void
get_token2(void)
{
	if (token_buf) {
		if (ptoken_buf)
			free(ptoken_buf);
		ptoken_buf = strdup(token_buf);
	}
	pptoken = ptoken;
	ptoken = token;
	ppnewline_flag = pnewline_flag;
	pnewline_flag = newline_flag;
	newline_flag = 0;
	get_token_nib2();
	if (token == T_NEWLINE)
		newline_flag = 1;
	if (scan_level)
		while (token == T_NEWLINE)
			get_token_nib2();
}

void
get_token_nib2(void)
{
	// skip spaces

	while (isspace(*scan_str) && *scan_str != '\n')
		scan_str++;

	token_str = scan_str;

	// end of string?

	if (*scan_str == '\0') {
		token = T_END;
		return;
	}

	// newline?

	if (*scan_str == '\n') {
		scan_str++;
		token = T_NEWLINE;
		return;
	}

	// comment?

	if (*scan_str == '#' || (scan_str[0] == '-' && scan_str[1] == '-')) {
		while (*scan_str && *scan_str != '\n')
			scan_str++;
		if (*scan_str)
			scan_str++;
		token = T_NEWLINE;
		return;
	}

	// number?

	if (isdigit( * scan_str) ||  * scan_str == '.') {
		while (isdigit( * scan_str))
			scan_str++;
		if ( * scan_str == '.') {
			scan_str++;
			while (isdigit( * scan_str))
				scan_str++;
			if (token_str + 1 == scan_str)
				scan_error2("expected decimal digit"); // only a decimal point
			token = T_DOUBLE;
		} else
			token = T_INTEGER;
		update_token_buf2(token_str, scan_str);
		return;
	}

	// symbol?

	if (isalpha( * scan_str)) {
		while (isalnum( * scan_str))
			scan_str++;
		if ( * scan_str == '(')
			token = T_FUNCTION;
		else
			token = T_SYMBOL;
		update_token_buf2(token_str, scan_str);
		return;
	}

	// string ?

	if ( * scan_str == '"') {
		scan_str++;
		while ( * scan_str != '"') {
			if (*scan_str == '\0' || *scan_str == '\n') {
				token_str = scan_str;
				scan_error2("runaway string");
			}
			scan_str++;
		}
		scan_str++;
		token = T_STRING;
		update_token_buf2(token_str + 1, scan_str - 1); // don't include quote chars
		return;
	}

	// relational operator?

	if ( * scan_str == '=' && scan_str[1] == '=') {
		scan_str += 2;
		token = T_EQ;
		return;
	}

	if ( * scan_str == '<' && scan_str[1] == '=') {
		scan_str += 2;
		token = T_LTEQ;
		return;
	}

	if ( * scan_str == '>' && scan_str[1] == '=') {
		scan_str += 2;
		token = T_GTEQ;
		return;
	}

	// single char token

	token =  * scan_str++;

	// ignore newlines in parentheses and brackets

	if (token == '(' || token == '[')
		scan_level++;

	if (token == ')' || token == ']')
		scan_level--;
}

void
update_token_buf2(char *a, char *b)
{
	int n;

	if (token_buf)
		free(token_buf);

	n = (int) (b - a);

	token_buf = (char *) malloc(n + 1);

	if (token_buf == NULL)
		malloc_kaput();

	strncpy(token_buf, a, n);

	token_buf[n] = '\0';
}

void
scan_error2(char *errmsg)
{
	print_scan_line(scan_str);
	outbuf_index = 0;
	print_str("Stop: Syntax error, ");
	print_str(errmsg);
	if (token_str < scan_str) {
		print_str(" instead of '");
		while (*token_str && token_str < scan_str)
			print_char(*token_str++);
		print_str("'");
	}
	print_char('\n');
	print_char('\0');
	printbuf(outbuf, RED);
	stop(NULL);
}
