//#include "stdafx.h"
extern "C" {
#include "..\defs.h"
}

//	total_h		sum of the heights of all the items in the display queue
//
//	display_y	the display queue y coordinate that maps to the screen
//
//	maxy_y		the maximum value that display_y can have
//
//	total_w		the maximum width including right and left margins

extern int line_height;
extern int left_margin;
extern int right_margin;
extern int display_x;
extern int display_y;
extern int display_height;
extern int display_width;
extern int max_x;
extern int max_y;
extern int total_w;
extern int total_h;
extern int update_display_request;

extern int text_width(int, char *);

#define SMALL_FONT 1
#define DEFAULT_FONT 2

#define DRAW_HRULE 20
#define DRAW_LEFT_BRACKET 21
#define DRAW_RIGHT_BRACKET 22
#define DRAW_LINE 23
#define DRAW_POINT 24
#define DRAW_BOX 25
#define DRAW_LABEL 26

#define MAXHIST 10001

extern "C" {
	int test_flag;
}
int term_flag;

//	type
//
//	0	print string
//
//	1	display

struct yy {
	unsigned char type, attr;
	int w, h;
	union {
		char *s;
		unsigned char *d;
	} u;
} term[MAXHIST];

static int head, tail, len;
static char buf[1000];

#define OUTBUFLEN 10000
char out_buf[OUTBUFLEN + 1];
int out_count;

static void printcharf(int);
static void advance(void);

void
printchar_nowrap(int c)
{
	if (c == '\n' || len < 80)
		print_char(c);
}

static int
breaking_char(int c)
{
	switch (c) {
	case ' ':
	case '+':
	case '-':
	case '*':
	case '/':
	case ',':
	case '(':
		return 1;
	}
	return 0;
}

extern "C"
void
printchar(int c)
{
	if (test_flag && out_count < OUTBUFLEN)
		out_buf[out_count++] = c;
	printcharf(c);
	if (len >= 80)
		printcharf('\n');
}

static void
printcharf(int c)
{
	char *s;

	if (c == '\n') {
		s = (char *) malloc(len + 1);
		if (s == NULL)
			return;
		buf[len] = 0;
		strcpy(s, buf);
		len = 0;
		term[tail].type = 0;
		term[tail].attr = term_flag;
		term[tail].u.s = s;
		term[tail].w = text_width(DEFAULT_FONT, s);
		term[tail].h = line_height;
		total_h += line_height;
		if (total_h - max_y > display_height)
			max_y = total_h - display_height;
		display_y = max_y;
		advance();
		return;
	}

	if (len < (int) sizeof buf)
		buf[len++] = c;
}

extern "C"
void
printstr(char *s)
{
	while (*s)
		printchar(*s++);
}

extern "C"
void
clear_term(void)
{
	while (head != tail) {
		free(term[head].u.s);
		head = (head + 1) % MAXHIST;
	}
	head = 0;
	tail = 0;
	display_x = 0;
	display_y = 0;
	max_x = 0;
	max_y = 0;
	total_w = 0;
	total_h = 0;
	update_display_request = 1;
}

extern "C" void
eval_clear(void)
{
	if (test_flag == 0)
		clear_term();
	clear_flag = 1;
	push_symbol(NIL);
}

void
shipout(unsigned char *d, int w, int h)
{
	term[tail].type = 1;
	term[tail].attr = 0;
	term[tail].u.d = d;
	term[tail].w = w;
	term[tail].h = h + line_height;
	total_h += h + line_height;
	if (total_h - max_y > display_height)
		max_y = total_h - display_height;
	display_y = max_y;
	advance();
}

static void
advance(void)
{
	int h;
	tail = (tail + 1) % MAXHIST;
	if (tail == head) {
		h = term[head].h;
		free(term[head].u.s);
		head = (head + 1) % MAXHIST;
		total_h -= h;
		max_y -= h;
		display_y -= h;
		if (max_y < 0)
			max_y = 0;
		if (display_y < 0)
			display_y = 0;
	}
	update_display_request = 1;
}

extern void drawstring(int, int, char *, int);
extern void drawpixel(int, int, int, int);
extern void draw_left_bracket(int, int, int, int);
extern void draw_right_bracket(int, int, int, int);
extern void draw_line(int, int, int, int);

static int find_start_index(void);
static void draw(int);
static int yy;

void
draw_display(void)
{
	int i;
	update_display_request = 0;
	i = find_start_index();
	total_w = 0;
	while (i != tail && yy < display_height) {
		draw(i);
		i = (i + 1) % MAXHIST;
	}
	max_x = total_w - display_width;
	if (max_x < display_x)
		max_x = display_x;
}

// find the top of the term

static int
find_start_index(void)
{
	int indx, y;

	indx = tail;

	// back up until y <= display_y

	y = total_h;

	while (1) {

		if (y <= display_y)
			break;

		if (indx == head)
			break;

		indx = (indx + MAXHIST - 1) % MAXHIST;

		y -= term[indx].h;
	}

	// if too tall then negative start_y

	if (y - display_y < 0)
		yy = y - display_y;
	else
		yy = 0;

	return indx;
}

extern void draw_text(int, int, int, char *, int);
extern void draw_hrule(int, int, int);
extern void draw_point(int, int, int, int);
extern void draw_box(int, int, int, int);
extern void use_graph_pen(void);
extern void use_normal_pen(void);

#define N(x) (((int) d[k + x] << 24) | ((int) d[k + x + 1] << 16) | ((int) d[k + x + 2]) << 8 | ((int) d[k + x + 3]))

static void
draw(int i)
{
	int cmd, h, k, len, w, xx, x, x1, x2, y, y1, y2;
	unsigned char *d;

	w = term[i].w;

	switch (term[i].type) {

	case 0:
		if (term[i].u.s[0]) {
			xx = left_margin;
			if (xx + w + right_margin > total_w)
				total_w = xx + w + right_margin;
			xx -= display_x;
			len = (int) strlen(term[i].u.s);
			draw_text(DEFAULT_FONT, xx, yy, term[i].u.s, len);
		}
		yy += line_height;
		break;

	case 1:
		yy += line_height / 2;
		d = term[i].u.d;
		xx = (display_width - w) / 2; // center it
		if (xx < left_margin)
			xx = left_margin;
		if (xx + w + right_margin > total_w)
			total_w = xx + w + right_margin;
		xx -= display_x;
		k = 0;
		while (1) {
			cmd = d[k];
			if (cmd == 0)
				break;
			switch (cmd) {
			case 1:
				use_normal_pen();
				x = 256 * d[k + 1] + d[k + 2];
				y = 256 * d[k + 3] + d[k + 4];
				if (x > 32767)
					x -= 65536;
				if (y > 32767)
					y -= 65536;
				len = d[k + 5];
				draw_text(SMALL_FONT, xx + x, yy + y, (char *) d + k + 6, len); // small font
				k = k + 6 + len;
				break;
			case 2: // default font
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
				use_normal_pen();
				x = N(1);
				y = N(5);
				len = N(9);
				draw_text(cmd, xx + x, yy + y, (char *) d + k + 13, len);
				k = k + 13 + len;
				break;
			case DRAW_HRULE:
				use_normal_pen();
				x = N(1);
				y = N(5);
				w = N(9);
				h = N(13);
				draw_hrule(xx + x, yy + y, w);
				k += 17;
				break;
			case DRAW_LEFT_BRACKET:
				use_normal_pen();
				x = N(1);
				y = N(5);
				w = N(9);
				h = N(13);
				draw_left_bracket(xx + x, yy + y, w, h);
				k += 17;
				break;
			case DRAW_RIGHT_BRACKET:
				use_normal_pen();
				x = N(1);
				y = N(5);
				w = N(9);
				h = N(13);
				draw_right_bracket(xx + x, yy + y, w, h);
				k += 17;
				break;
			case DRAW_LINE:
				use_normal_pen();
				x1 = 256 * d[k + 1] + d[k + 2];
				y1 = 256 * d[k + 3] + d[k + 4];
				x2 = 256 * d[k + 5] + d[k + 6];
				y2 = 256 * d[k + 7] + d[k + 8];
				draw_line(xx + x1, yy + y1, xx + x2, yy + y2);
				k += 9;
				break;
			case DRAW_POINT:
				use_graph_pen();
				x = 256 * d[k + 1] + d[k + 2];
				y = 256 * d[k + 3] + d[k + 4];
				draw_point(xx, x, yy, y);
				k += 5;
				break;
			case DRAW_BOX:
				x1 = 256 * d[k + 1] + d[k + 2];
				y1 = 256 * d[k + 3] + d[k + 4];
				x2 = 256 * d[k + 5] + d[k + 6];
				y2 = 256 * d[k + 7] + d[k + 8];
				draw_box(xx + x1, yy + y1, xx + x2, yy + y2);
				k += 9;
				break;
			default:
				d[4] = 0; // error, force stop
				k = 4;
				break;
			}
		}

		// 1/2 line_height above and below display

		yy -= line_height / 2;
		yy += term[i].h;

		use_normal_pen();

		break;
	}
}

char *
get_tty_buf(void)
{
	int i, j, n;
	char *s;

	i = head;
	n = 0;

	while (i != tail) {
		if (term[i].type == 0)
			n += (int) strlen(term[i].u.s) + 1;
		i = (i + 1) % MAXHIST;
	}

	s = (char *) malloc(n + 1);

	if (s == NULL)
		return s;

	i = head;
	j = 0;

	while (i != tail) {
		if (term[i].type == 0) {
			strcpy(s + j, term[i].u.s);
			j += (int) strlen(term[i].u.s);
			s[j++] = '\n';
		}
		i = (i + 1) % MAXHIST;
	}

	s[j] = 0;

	return s;
}
