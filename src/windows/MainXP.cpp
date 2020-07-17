// Eigenmath by George Weigt

#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "comdlg32.lib")

#define _WIN32_WINNT 0x0501

#include <Windows.h>
#include <WinNT.h>
#include <WinDef.h>
#include <WinUser.h>
#include <WinGDI.h>
#include <WinBase.h>

//#include "stdafx.h"
#include "resource.h"

#include <commdlg.h>
#include <stdio.h>
#define _USE_MATH_DEFINES // for MS C++
#include <math.h>
#include <time.h>

#include "help.h"

extern "C" { 
#include "..\defs.h"
	int stop_flag;
	void run(char *);
	void eval_clear(void);
	void printstr(const char *);
}
extern void draw_display(void);
extern void update_cmd_history(const char *);
//extern void print_mem_info(void);
extern char *get_cmd_history(void);

void update_display(void);

int line_height = 20;
int left_margin = 5;
int right_margin = 5;
int main_client_width;
int main_client_height;
static int scroll_bar_width;
int display_x;
int display_y;
int display_width;
int display_height;
int max_x;
int max_y;
int total_w;
int total_h;
int update_display_request;

static int running;
static unsigned int timer;
static int edit_mode;

HINSTANCE hinst;
HINSTANCE hInst;
HWND main_window;
HWND input_window;
HWND edit_window;
HWND hscroll;
HWND vscroll;
#define NBUTTON 8
HWND button[NBUTTON];
HDC draw_hdc;
HDC run_hdc;
HMENU main_menu;

#define NFONT 11

HFONT display_font[NFONT];

struct text_metric {
	int ascent, descent, width;
} text_metric[NFONT];

#define SMALL_FONT 1
#define DEFAULT_FONT 2
#define TIMES_FONT 3
#define ITALIC_TIMES_FONT 4
#define SYMBOL_FONT 5
#define ITALIC_SYMBOL_FONT 6
#define SMALL_TIMES_FONT 7
#define SMALL_ITALIC_TIMES_FONT 8
#define SMALL_SYMBOL_FONT 9
#define SMALL_ITALIC_SYMBOL_FONT 10

#define DRAW_HRULE 20
#define DRAW_LEFT_BRACKET 21
#define DRAW_RIGHT_BRACKET 22
#define DRAW_LINE 23

static void MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void do_up_arrow(void);
void do_down_arrow(void);
void do_fonts(void);
void fill_font_metric_array(void);
void do_more_setup(void);

static void goto_edit_mode(void);
static void goto_calc_mode(void);
static void move_input_window(void);
static void move_edit_window(void);
static void move_buttons(void);
static void move_scroll_bars(void);
static void create_menu(void);
static void do_vscroll(int, int);
static void do_hscroll(int, int);
static void update_scroll_bars(void);
static void move_bits_up(int);
static void move_bits_down(int);
static void run_script(void);
static void do_new(void);
static void do_save(void);
static void do_saveas(void);
static void do_open(void);
static void save_file(void);
static void do_page_setup(void);
static void do_print(void);
static void do_button(const char *);
static void do_enter(void);
static void send_user_event(void);
static void activate_controls(void);
static void deactivate_controls(void);
static void process_user_event(void);
static void copy_all(void);
static void do_create_script(void);
void update_curr_cmd(const char *);

enum {
	ID_BASE = 40000,

	// events

	ID_USER_EVENT,
	ID_ESC,
	ID_ENTER,
	ID_UP_ARROW,
	ID_DOWN_ARROW,

	// application buttons

	ID_CLEAR,
	ID_DERIVATIVE,
	ID_DRAW,
	ID_FLOAT,
	ID_INTEGRAL,
	ID_SIMPLIFY,
	ID_EDIT_SCRIPT,
	ID_RUN_SCRIPT,

	// file menu

	ID_NEW,
	ID_OPEN,
	ID_SAVE,
	ID_SAVEAS,
	ID_PAGE_SETUP,
	ID_PRINT,

	// edit menu

	ID_UNDO,
	ID_CUT,
	ID_COPY,
	ID_PASTE,

	// asterisk menu

	ID_COPY_DISPLAY,
	ID_CREATE_SCRIPT,

	// help menu

	ID_HELP_ABS,
	ID_HELP_ADJ,
	ID_HELP_ARCCOS,
	ID_HELP_ARCCOSH,
	ID_HELP_ARCSIN,
	ID_HELP_ARCSINH,
	ID_HELP_ARCTAN,
	ID_HELP_ARCTANH,
	ID_HELP_ARG,
	ID_HELP_BESSELJ,
	ID_HELP_CHOOSE,
	ID_HELP_CIRCEXP,
	ID_HELP_COEFF,
	ID_HELP_COFACTOR,
	ID_HELP_CONJ,
	ID_HELP_CONTRACT,
	ID_HELP_COS,
	ID_HELP_COSH,
	ID_HELP_DEFINT,
	ID_HELP_DEG,
	ID_HELP_DENOMINATOR,
	ID_HELP_DERIVATIVE,
	ID_HELP_DET,
	ID_HELP_DOT,
	ID_HELP_ERF,
	ID_HELP_ERFC,
	ID_HELP_EVAL,
	ID_HELP_EXP,
	ID_HELP_EXPAND,
	ID_HELP_EXPCOS,
	ID_HELP_EXPSIN,
	ID_HELP_FACTOR,
	ID_HELP_FACTOR_POLY,
	ID_HELP_FACTORIAL,
	ID_HELP_FOR,
	ID_HELP_GRADIENT,
	ID_HELP_HERMITE,
	ID_HELP_IMAG,
	ID_HELP_INTEGRAL,
	ID_HELP_INV,
	ID_HELP_LAGUERRE,
	ID_HELP_LEADING,
	ID_HELP_LEGENDRE,
	ID_HELP_LOG,
	ID_HELP_MAG,
	ID_HELP_NROOTS,
	ID_HELP_NUMERATOR,
	ID_HELP_OUTER,
	ID_HELP_POLAR,
	ID_HELP_PRODUCT,
	ID_HELP_QUOTIENT,
	ID_HELP_REAL,
	ID_HELP_RECT,
	ID_HELP_ROOTS,
	ID_HELP_SIN,
	ID_HELP_SINH,
	ID_HELP_SQRT,
	ID_HELP_SUM,
	ID_HELP_TAN,
	ID_HELP_TANH,
	ID_HELP_TAYLOR,
	ID_HELP_TRANSPOSE,
	ID_HELP_UNIT,
	ID_HELP_ZERO,
};

#define NACCEL 7

ACCEL accel[NACCEL] = {
	{FCONTROL,	15,	ID_OPEN},	// ^O
	{FCONTROL,	19,	ID_SAVE},	// ^S
	{FCONTROL,	26,	ID_UNDO},	// ^Z
	{FCONTROL,	24,	ID_CUT},	// ^X
	{FCONTROL,	3,	ID_COPY},	// ^C
	{FCONTROL,	22,	ID_PASTE},	// ^V
	{FCONTROL,	27,	ID_ESC},	// ^[
};

extern "C"
void
eval_exit(void)
{
	// do nothing, so scripts don't exit the GUI
	push_symbol(NIL);
	/*
	PostMessage(
		main_window,
		WM_QUIT,
		0,
		0);
	*/
}

extern "C"
void
malloc_kaput(void)
{
	fprintf(stderr, "malloc kaput\n");
	PostMessage(
		main_window,
		WM_QUIT,
		0,
		0);
}

void
echo_input(const char *s)
{
	printstr(s);
	printstr("\n");
}

extern "C"
void
printbuf(char *s, int color)
{
	printstr(s);
}


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
 	int tmp;
	MSG msg;
	HACCEL haccel;

	hinst = hInstance;

	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	do_fonts();

	fill_font_metric_array();

	create_menu();

	do_more_setup();

	haccel = CreateAcceleratorTable(accel, NACCEL);

	SetTimer(main_window, 0, 1000, NULL);

	while (1) {

		tmp = GetMessage(&msg, NULL, 0, 0);

		if (tmp < 1)
			break;

		tmp = TranslateAccelerator(main_window, haccel, &msg);

		if (tmp == 0) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.hwnd == input_window && msg.message == WM_KEYDOWN) {
			switch (msg.wParam) {
			case 13:
				PostMessage(main_window, ID_ENTER, 0, 0);
				break;
			case VK_UP:
				PostMessage(main_window, ID_UP_ARROW, 0, 0);
				break;
			case VK_DOWN:
				PostMessage(main_window, ID_DOWN_ARROW, 0, 0);
				break;
			case VK_PRIOR:
				PostMessage(
					main_window,
					WM_VSCROLL,
					SB_PAGEUP,
					(LPARAM) vscroll);
				break;
			case VK_NEXT:
				PostMessage(
					main_window,
					WM_VSCROLL,
					SB_PAGEDOWN,
					(LPARAM) vscroll);
				break;
			case VK_HOME:
				PostMessage(
					main_window,
					WM_VSCROLL,
					SB_TOP,
					(LPARAM) vscroll);
				break;
			case VK_END:
				PostMessage(
					main_window,
					WM_VSCROLL,
					SB_BOTTOM,
					(LPARAM) vscroll);
				break;
			default:
				break;
			}
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//

static void
MyRegisterClass(HINSTANCE hInstance)
{
	static WNDCLASSEX wcex;

	if (zero == NULL) {
		init();
		prep();
		binding[TRACE] = zero;
	}

	eval_clear();

	wcex.cbSize			= sizeof(WNDCLASSEX); 
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon		= LoadIcon(hInstance, (LPCSTR) IDI_ICON1);
	//wcex.hIcon		= CreateIcon(hInstance, 0, 0, 0, 0, NULL, NULL);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= "Eigenmath";
	wcex.hIconSm		= NULL;

	RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	main_window = CreateWindow(
		"Eigenmath",
		"Eigenmath 1337",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0,
//		CW_USEDEFAULT, 0,
		800, 540,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(main_window, nCmdShow);
	UpdateWindow(main_window);

	return TRUE;
}

static struct {
	char *s;
	long long id;
} main_button[NBUTTON] = {
	{"Clear",		ID_CLEAR},
	{"Draw",		ID_DRAW},
	{"Simplify",		ID_SIMPLIFY},
	{"Float",		ID_FLOAT},
	{"Derivative",		ID_DERIVATIVE},
	{"Integral",		ID_INTEGRAL},
	{"Edit Script",		ID_EDIT_SCRIPT},
	{"Run Script",		ID_RUN_SCRIPT},
};

void
do_more_setup(void)
{
	int h, i, j, k, w, x, y;
	RECT r;

	GetClientRect(main_window, &r);

	main_client_width = r.right;
	main_client_height = r.bottom;

	vscroll = CreateWindow(
		"SCROLLBAR",
		NULL,
		WS_CHILD | WS_VISIBLE | SBS_VERT | SBS_RIGHTALIGN,
		0, 0,
		main_client_width, main_client_height - 3 * line_height,
		main_window,
		NULL,
		hinst,
		NULL);

	GetWindowRect(vscroll, &r);

	scroll_bar_width = r.right - r.left;

	display_width = main_client_width - scroll_bar_width;
	display_height = main_client_height - 3 * line_height - scroll_bar_width;

	// move the scroll bar to adjust for its width

	x = display_width;
	y = 0;
	w = scroll_bar_width;
	h = display_height;

	MoveWindow(vscroll, x, y, w, h, TRUE);

	x = 0;
	y = display_height;
	w = display_width;
	h = scroll_bar_width;

	hscroll = CreateWindow(
		"SCROLLBAR",
		NULL,
		WS_CHILD | WS_VISIBLE | SBS_HORZ,
		x, y,
		w, h,
		main_window,
		NULL,
		hinst,
		NULL);

	x = 0;
	y = main_client_height - 3 * line_height;
	w = main_client_width;
	h = line_height;

	input_window = CreateWindow(
		"EDIT",
		NULL,
		WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
		x, y,
		w, h,
		main_window,
		NULL,
		hinst,
		NULL);

	SendMessage(input_window, WM_SETFONT, (WPARAM) display_font[DEFAULT_FONT], 0);

	SetFocus(input_window);

	edit_window = CreateWindow(
		"EDIT",
		NULL,
		WS_CHILD | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN,
		0, 0,
		main_client_width, main_client_height - 2 * line_height,
		main_window,
		NULL,
		hinst,
		NULL);

	SendMessage(edit_window, WM_SETFONT, (WPARAM) display_font[DEFAULT_FONT], 0);

	// create buttons

	for (i = 0; i < NBUTTON; i++) {
		j = i * main_client_width / NBUTTON;
		k = (i + 1) * main_client_width / NBUTTON;
		button[i] = CreateWindow(
			"BUTTON",
			main_button[i].s,
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			j, main_client_height - 2 * line_height,
			k - j, 2 * line_height,
			main_window,
			(HMENU) main_button[i].id,
			hinst,
			NULL);
		SendMessage(
			button[i],
			WM_SETFONT,
			(WPARAM) GetStockObject(ANSI_VAR_FONT),
			0);
#if 0
		button[2 * i + 1] = CreateWindow(
			"BUTTON",
			main_button[2 * i + 1].s,
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			j, main_client_height - line_height,
			k - j, line_height,
			main_window,
			(HMENU) main_button[2 * i + 1].id,
			hinst,
			NULL);
		SendMessage(
			button[2 * i + 1],
			WM_SETFONT,
			(WPARAM) GetStockObject(ANSI_VAR_FONT),
			0);
#endif
	}

	update_scroll_bars();
}

static struct {
	char *s;
	int id;
} menu[] = {

	{"File",		0},
	{"New",			ID_NEW},
	{"Open\tCtrl+O",	ID_OPEN},
	{"Save\tCtrl+S",	ID_SAVE},
	{"Save As",		ID_SAVEAS},
	//{"-",			0},
	//{"Page Setup",		ID_PAGE_SETUP},
	//{"Print",		ID_PRINT},
	{0,			0},

	{"Edit",		0},
	{"Undo\tCtrl+Z",	ID_UNDO},
	{"Cut\tCtrl+X",		ID_CUT},
	{"Copy\tCtrl+C",	ID_COPY},
	{"Paste\tCtrl+V",	ID_PASTE},
	{0,			0},

	{"*",					0},
	{"Copy display to clipboard",		ID_COPY_DISPLAY},
	{"Create script from command history",	ID_CREATE_SCRIPT},
	{0,					0},

	{"Help",				0},

	{"Complex number functions",		0},
	{"arg",					ID_HELP_ARG},
	{"conj",				ID_HELP_CONJ},
	{"imag",				ID_HELP_IMAG},
	{"mag",					ID_HELP_MAG},
	{"polar",				ID_HELP_POLAR},
	{"real",				ID_HELP_REAL},
	{"rect",				ID_HELP_RECT},
	{0,					0},

	{"Polynomial functions",		0},
	{"coeff",				ID_HELP_COEFF},
	{"deg",					ID_HELP_DEG},
	{"expand",				ID_HELP_EXPAND},
	{"factor",				ID_HELP_FACTOR_POLY},
	{"leading",				ID_HELP_LEADING},
	{"nroots",				ID_HELP_NROOTS},
	{"quotient",				ID_HELP_QUOTIENT},
	{"roots",				ID_HELP_ROOTS},
	{0,					0},

	{"Linear algebra functions",		0},
	{"adj",					ID_HELP_ADJ},
	{"cofactor",				ID_HELP_COFACTOR},
	{"contract",				ID_HELP_CONTRACT},
	{"det",					ID_HELP_DET},
	{"dot",					ID_HELP_DOT},
	{"inv",					ID_HELP_INV},
	{"outer",				ID_HELP_OUTER},
	{"transpose",				ID_HELP_TRANSPOSE},
	{"unit",				ID_HELP_UNIT},
	{"zero",				ID_HELP_ZERO},
	{0,					0},

	{"Calculus functions",			0},
	{"defint",				ID_HELP_DEFINT},
	{"derivative",				ID_HELP_DERIVATIVE},
	{"gradient",				ID_HELP_GRADIENT},
	{"integral",				ID_HELP_INTEGRAL},
	{"taylor",				ID_HELP_TAYLOR},
	{0,					0},

	{"Exponential functions",		0},
	{"circexp",				ID_HELP_CIRCEXP},
	{"exp",					ID_HELP_EXP},
	{"expcos",				ID_HELP_EXPCOS},
	{"expsin",				ID_HELP_EXPSIN},
	{"log",					ID_HELP_LOG},
	{0,					0},

	{"Circular functions",			0},
	{"arccos",				ID_HELP_ARCCOS},
	{"arcsin",				ID_HELP_ARCSIN},
	{"arctan",				ID_HELP_ARCTAN},
	{"cos",					ID_HELP_COS},
	{"sin",					ID_HELP_SIN},
	{"tan",					ID_HELP_TAN},
	{0,					0},

	{"Hyperbolic functions",		0},
	{"arccosh",				ID_HELP_ARCCOSH},
	{"arcsinh",				ID_HELP_ARCSINH},
	{"arctanh",				ID_HELP_ARCTANH},
	{"cosh",				ID_HELP_COSH},
	{"sinh",				ID_HELP_SINH},
	{"tanh",				ID_HELP_TANH},
	{0,					0},

	{"Special functions",			0},
	{"besselj",				ID_HELP_BESSELJ},
	{"hermite",				ID_HELP_HERMITE},
	{"laguerre",				ID_HELP_LAGUERRE},
	{"legendre",				ID_HELP_LEGENDRE},
	{0,					0},

	{"Other functions",			0},
	{"abs",					ID_HELP_ABS},
	{"choose",				ID_HELP_CHOOSE},
	{"denominator",				ID_HELP_DENOMINATOR},
	{"erf",					ID_HELP_ERF},
	{"erfc",				ID_HELP_ERFC},
	{"eval",				ID_HELP_EVAL},
	{"factor",				ID_HELP_FACTOR},
	{"factorial",				ID_HELP_FACTORIAL},
	{"for",					ID_HELP_FOR},
	{"numerator",				ID_HELP_NUMERATOR},
	{"product",				ID_HELP_PRODUCT},
	{"sqrt",				ID_HELP_SQRT},
	{"sum",					ID_HELP_SUM},
	{0,					0},

	{0,					0},	// end of 'examples' menu

	{0,					0},	// end
};
 
static void
create_menu(void)
{
	int k = 0, level = 0;
	char *s[2];
	HMENU hmenu[3];

	hmenu[0] = CreateMenu();

	while (1) {
		if (menu[k].s == NULL) {
			if (level) {
				AppendMenu(
					hmenu[level - 1],
					MF_POPUP | MF_ENABLED | MF_STRING,
					(UINT_PTR) hmenu[level],
					s[level - 1]);
				level--;
				k++;
				continue;
			} else
				break;
		}

		if (menu[k].s[0] == '-') {
			AppendMenu(
				hmenu[level],
				MF_SEPARATOR,
				0,
				0);
			k++;
			continue;
		}

		if (menu[k].id == 0) {
			s[level] = menu[k].s;
			level++;
			hmenu[level] = CreatePopupMenu();
			k++;
			continue;
		}

		AppendMenu(
			hmenu[level],
			MF_ENABLED | MF_STRING,
			menu[k].id,
			menu[k].s);

		k++;
	}

	SetMenu(main_window, hmenu[0]);

	main_menu = hmenu[0];
}

//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int hi, lo;
	PAINTSTRUCT ps;
	RECT r;

	switch (message) {
	case ID_USER_EVENT:
		process_user_event();
		break;
	case ID_ENTER:
		if (running)
			break;
		do_enter();
		break;
	case ID_UP_ARROW:
		if (running)
			break;
		do_up_arrow();
		break;
	case ID_DOWN_ARROW:
		if (running)
			break;
		do_down_arrow();
		break;
	case WM_TIMER:
		if (running == 1)
			send_user_event();
		break;
	case WM_ACTIVATE:
		if (edit_mode == 0)
			SetFocus(input_window);
		else
			return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	case WM_SIZE:
		GetClientRect(main_window, &r);
		main_client_width = r.right;
		main_client_height = r.bottom;
		display_width = main_client_width - scroll_bar_width;
		display_height = main_client_height - 3 * line_height - scroll_bar_width;
		move_input_window();
		move_edit_window();
		move_buttons();
		move_scroll_bars();
		max_y = total_h - display_height;
		if (max_y < 0)
			max_y = 0;
		if (display_y > max_y)
			display_y = max_y;
		update_scroll_bars();
		PostMessage(main_window, WM_PAINT, 0, 0);
		break;
	case WM_COMMAND:
		hi = HIWORD(wParam);
		lo = LOWORD(wParam);
		switch (lo) {
		case ID_ESC:
			stop_flag = 1;
			break;
		case ID_NEW:
			if (running)
				break;
			do_new();
			break;
		case ID_OPEN:
			if (running)
				break;
			do_open();
			break;
		case ID_SAVE:
			do_save();
			break;
		case ID_SAVEAS:
			do_saveas();
			break;
//		case ID_PAGE_SETUP:
//			do_page_setup();
//			break;
//		case ID_PRINT:
//			do_print();
//			break;

		case ID_UNDO:
			if (running)
				break;
			if (edit_mode == 0)
				SendMessage(input_window, WM_UNDO, 0, 0);
			else
				SendMessage(edit_window, WM_UNDO, 0, 0);
			break;
		case ID_CUT:
			if (running)
				break;
			if (edit_mode == 0)
				SendMessage(input_window, WM_CUT, 0, 0);
			else
				SendMessage(edit_window, WM_CUT, 0, 0);
			break;
		case ID_COPY:
			if (running)
				break;
			if (edit_mode == 0)
				SendMessage(input_window, WM_COPY, 0, 0);
			else
				SendMessage(edit_window, WM_COPY, 0, 0);
			break;
		case ID_PASTE:
			if (running)
				break;
			if (edit_mode == 0)
				SendMessage(input_window, WM_PASTE, 0, 0);
			else
				SendMessage(edit_window, WM_PASTE, 0, 0);
			break;

		// "asterisk" pull-down menu

		case ID_COPY_DISPLAY:
			copy_all();
			break;
		case ID_CREATE_SCRIPT:
			if (running)
				break;
			do_create_script();
			break;

		// linear algebra

		case ID_HELP_ADJ:
			HELP(help_adj);
			break;
		case ID_HELP_COFACTOR:
			HELP(help_cofactor);
			break;
		case ID_HELP_CONTRACT:
			HELP(help_contract);
			break;
		case ID_HELP_DET:
			HELP(help_det);
			break;
		case ID_HELP_DOT:
			HELP(help_dot);
			break;
		case ID_HELP_INV:
			HELP(help_inv);
			break;
		case ID_HELP_OUTER:
			HELP(help_outer);
			break;
		case ID_HELP_TRANSPOSE:
			HELP(help_transpose);
			break;
		case ID_HELP_UNIT:
			HELP(help_unit);
			break;
		case ID_HELP_ZERO:
			HELP(help_zero);
			break;

		// calculus

		case ID_HELP_DERIVATIVE:
			HELP(help_derivative);
			break;
		case ID_HELP_GRADIENT:
			HELP(help_gradient);
			break;
		case ID_HELP_INTEGRAL:
			HELP(help_integral);
			break;

		// complex number functions

		case ID_HELP_ARG:
			HELP(help_arg);
			break;
		case ID_HELP_CONJ:
			HELP(help_conj);
			break;
		case ID_HELP_IMAG:
			HELP(help_imag);
			break;
		case ID_HELP_MAG:
			HELP(help_mag);
			break;
		case ID_HELP_POLAR:
			HELP(help_polar);
			break;
		case ID_HELP_REAL:
			HELP(help_real);
			break;
		case ID_HELP_RECT:
			HELP(help_rect);
			break;

		// circular functions

		case ID_HELP_ARCCOS:
			HELP(help_arccos);
			break;
		case ID_HELP_ARCSIN:
			HELP(help_arcsin);
			break;
		case ID_HELP_ARCTAN:
			HELP(help_arctan);
			break;
		case ID_HELP_COS:
			HELP(help_cos);
			break;
		case ID_HELP_SIN:
			HELP(help_sin);
			break;
		case ID_HELP_TAN:
			HELP(help_tan);
			break;

		// hyperbolic functions

		case ID_HELP_ARCCOSH:
			HELP(help_arccosh);
			break;
		case ID_HELP_ARCSINH:
			HELP(help_arcsinh);
			break;
		case ID_HELP_ARCTANH:
			HELP(help_arctanh);
			break;
		case ID_HELP_COSH:
			HELP(help_cosh);
			break;
		case ID_HELP_SINH:
			HELP(help_sinh);
			break;
		case ID_HELP_TANH:
			HELP(help_tanh);
			break;

		// special functions

		case ID_HELP_BESSELJ:
			HELP(help_besselj);
			break;
		case ID_HELP_HERMITE:
			HELP(help_hermite);
			break;
		case ID_HELP_LAGUERRE:
			HELP(help_laguerre);
			break;
		case ID_HELP_LEGENDRE:
			HELP(help_legendre);
			break;

		// polynomial functions

		case ID_HELP_COEFF:
			HELP(help_coeff);
			break;
		case ID_HELP_DEG:
			HELP(help_deg);
			break;
		case ID_HELP_QUOTIENT:
			HELP(help_quotient);
			break;

		// other functions

		case ID_HELP_ABS:
			HELP(help_abs);
			break;
		case ID_HELP_CHOOSE:
			HELP(help_choose);
			break;
		case ID_HELP_CIRCEXP:
			HELP(help_circexp);
			break;
		case ID_HELP_DEFINT:
			HELP(help_defint);
			break;
		case ID_HELP_DENOMINATOR:
			HELP(help_denominator);
			break;
		case ID_HELP_ERF:
			HELP(help_erf);
			break;
		case ID_HELP_ERFC:
			HELP(help_erfc);
			break;
		case ID_HELP_EVAL:
			HELP(help_eval);
			break;
		case ID_HELP_EXP:
			HELP(help_exp);
			break;
		case ID_HELP_EXPAND:
			HELP(help_expand);
			break;
		case ID_HELP_EXPCOS:
			HELP(help_expcos);
			break;
		case ID_HELP_EXPSIN:
			HELP(help_expsin);
			break;
		case ID_HELP_FACTOR:
			HELP(help_factor);
			break;
		case ID_HELP_FACTOR_POLY:
			HELP(help_factor_polynomial);
			break;
		case ID_HELP_FACTORIAL:
			HELP(help_factorial);
			break;
		case ID_HELP_FOR:
			HELP(help_for);
			break;
		case ID_HELP_LEADING:
			HELP(help_leading);
			break;
		case ID_HELP_LOG:
			HELP(help_log);
			break;
		case ID_HELP_NROOTS:
			HELP(help_nroots);
			break;
		case ID_HELP_NUMERATOR:
			HELP(help_numerator);
			break;
		case ID_HELP_PRODUCT:
			HELP(help_product);
			break;
		case ID_HELP_SQRT:
			HELP(help_sqrt);
			break;
		case ID_HELP_ROOTS:
			HELP(help_roots);
			break;
		case ID_HELP_SUM:
			HELP(help_sum);
			break;
		case ID_HELP_TAYLOR:
			HELP(help_taylor);
			break;

		// window buttons, clear, draw, etc.

		case ID_CLEAR:
			do_button("clear");
			break;
		case ID_DRAW:
			do_button("draw");
			break;
		case ID_SIMPLIFY:
			do_button("simplify");
			break;
		case ID_FLOAT:
			do_button("float");
			break;
		case ID_DERIVATIVE:
			do_button("derivative");
			break;
		case ID_INTEGRAL:
			do_button("integral");
			break;
		case ID_EDIT_SCRIPT:
			if (running)
				break;
			if (edit_mode == 0)
				goto_edit_mode();
			else
				goto_calc_mode();
			break;
		case ID_RUN_SCRIPT:
			run_script();
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		draw_hdc = BeginPaint(hWnd, &ps);
		if (edit_mode == 0) {
			SetBkMode(draw_hdc, TRANSPARENT);
			draw_display();
			update_scroll_bars();
		}
		// paint the little square between the scroll bars
		r.left = display_width;
		r.top = display_height;
		r.right = r.left + scroll_bar_width + 1;
		r.bottom = r.top + scroll_bar_width + 1;
		FillRect(draw_hdc, &r, (HBRUSH) (COLOR_BTNFACE + 1));
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_VSCROLL:
		if ((HWND) lParam == vscroll)
			do_vscroll(LOWORD(wParam), HIWORD(wParam));
		else
			return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	case WM_HSCROLL:
		if ((HWND) lParam == hscroll)
			do_hscroll(LOWORD(wParam), HIWORD(wParam));
		else
			return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	case WM_KEYDOWN:
		if (edit_mode == 0)
			switch (wParam) {
			case VK_PRIOR:
				SendMessage(
					main_window,
					WM_VSCROLL,
					MAKELONG(SB_PAGEUP, 0),
					(LPARAM) vscroll);
				break;
			case VK_NEXT:
				SendMessage(
					main_window,
					WM_VSCROLL,
					MAKELONG(SB_PAGEDOWN, 0),
					(LPARAM) vscroll);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		else
			return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

char *
get_curr_cmd(void)
{
	int i, len;
	char *s;

	len = GetWindowTextLength(input_window);
	s = (char *) malloc(len + 1);
	GetWindowText(input_window, s, len + 1);

	// trim trailing spaces

	for (i = len - 1; i >= 0; i--) {
		if (isspace(s[i]))
			s[i] = 0;
		else
			break;
	}

	return s;
}

void
update_curr_cmd(const char *s)
{
	SetWindowText(input_window, s);
}

void
do_fonts(void)
{
	static LOGFONT lf;

	lf.lfQuality = ANTIALIASED_QUALITY;
	lf.lfPitchAndFamily = FF_ROMAN + VARIABLE_PITCH;

	lf.lfHeight = 36;

	lf.lfItalic = FALSE;
	lf.lfCharSet = ANSI_CHARSET;
	strcpy(lf.lfFaceName, "Times");
	display_font[TIMES_FONT] = CreateFontIndirect(&lf);

	lf.lfItalic = TRUE;
	lf.lfCharSet = ANSI_CHARSET;
	strcpy(lf.lfFaceName, "Times");
	display_font[ITALIC_TIMES_FONT] = CreateFontIndirect(&lf);

	lf.lfItalic = FALSE;
	lf.lfCharSet = SYMBOL_CHARSET;
	strcpy(lf.lfFaceName, "Symbol");
	display_font[SYMBOL_FONT] = CreateFontIndirect(&lf);

	lf.lfItalic = TRUE;
	lf.lfCharSet = SYMBOL_CHARSET;
	strcpy(lf.lfFaceName, "Symbol");
	display_font[ITALIC_SYMBOL_FONT] = CreateFontIndirect(&lf);

	lf.lfHeight = 24;

	lf.lfItalic = FALSE;
	lf.lfCharSet = ANSI_CHARSET;
	strcpy(lf.lfFaceName, "Times");
	display_font[SMALL_TIMES_FONT] = CreateFontIndirect(&lf);

	lf.lfItalic = TRUE;
	lf.lfCharSet = ANSI_CHARSET;
	strcpy(lf.lfFaceName, "Times");
	display_font[SMALL_ITALIC_TIMES_FONT] = CreateFontIndirect(&lf);

	lf.lfItalic = FALSE;
	lf.lfCharSet = SYMBOL_CHARSET;
	strcpy(lf.lfFaceName, "Symbol");
	display_font[SMALL_SYMBOL_FONT] = CreateFontIndirect(&lf);

	lf.lfItalic = TRUE;
	lf.lfCharSet = SYMBOL_CHARSET;
	strcpy(lf.lfFaceName, "Symbol");
	display_font[SMALL_ITALIC_SYMBOL_FONT] = CreateFontIndirect(&lf);

	// DEFAULT_FONT

	lf.lfHeight = 20;
	lf.lfItalic = FALSE;
	lf.lfWeight = FW_BOLD;
	lf.lfCharSet = ANSI_CHARSET;
	lf.lfQuality = ANTIALIASED_QUALITY;
	lf.lfPitchAndFamily = FF_MODERN;
	strcpy(lf.lfFaceName, "");
	display_font[DEFAULT_FONT] = CreateFontIndirect(&lf);

	// SMALL_FONT

	display_font[SMALL_FONT] = (HFONT) GetStockObject(ANSI_FIXED_FONT);
}

void
fill_font_metric_array(void)
{
	int i;
	HDC hdc;
	TEXTMETRIC tm;

	hdc = GetDC(main_window);

	for (i = 1; i < NFONT; i++) {

		SelectObject(hdc, display_font[i]);
		GetTextMetrics(hdc, &tm);

		text_metric[i].ascent = tm.tmAscent;
		text_metric[i].descent = tm.tmDescent;
		text_metric[i].width = tm.tmAveCharWidth;
	}

	ReleaseDC(main_window, hdc);
}

static void
goto_edit_mode(void)
{
	int i;

	if (edit_mode == 1)
		return;

	edit_mode = 1;

	// dim buttons

	for (i = 0; i < NBUTTON - 2; i++)
		EnableWindow(button[i], FALSE);

	SendMessage(button[NBUTTON - 2], WM_SETTEXT, 0, (LPARAM) "<<");

	ShowWindow(input_window, SW_HIDE);
	ShowWindow(vscroll, SW_HIDE);
	ShowWindow(hscroll, SW_HIDE);
	ShowWindow(edit_window, SW_SHOW);
	SetFocus(edit_window);
}

static void
goto_calc_mode(void)
{
	int i;

	if (edit_mode == 0)
		return;

	edit_mode = 0;

	// light buttons

	for (i = 0; i < NBUTTON - 2; i++)
		EnableWindow(button[i], TRUE);

	SendMessage(button[NBUTTON - 2], WM_SETTEXT, 0, (LPARAM) "Edit Script");

	ShowWindow(edit_window, SW_HIDE);
	ShowWindow(input_window, SW_SHOW);
	ShowWindow(vscroll, SW_SHOW);
	ShowWindow(hscroll, SW_SHOW);
	SetFocus(input_window);
}

// TextOut can only handle 4,000 or so chars at a time

void
draw_text(int font, int x, int y, char *s, int len)
{
	SelectObject(draw_hdc, display_font[font]);
	TextOut(draw_hdc, x, y, s, len);
#if 0
	SIZE size;
	SelectObject(draw_hdc, display_font[font]);
	while (len > 4000) {
		TextOut(hdc, x, y, s, 4000);
		GetTextExtentPoint32(hdc, s, 4000, &size);
		x += size.cx;
		s += 4000;
		len -= 4000;
	}
	if (len)
		TextOut(draw_hdc, x, y, (char *) s, len);
#endif
}

int
text_width(int font, char *s)
{
	SIZE size;
	SelectObject(run_hdc, display_font[font]);
	GetTextExtentPoint32(run_hdc, s, (int) strlen(s), &size);
	return (int) size.cx;
}

void
get_height_width(int *h, int *w, int font, char *s)
{
	SIZE size;
	SelectObject(run_hdc, display_font[font]);
	GetTextExtentPoint32(run_hdc, s, (int) strlen(s), &size);
	*h = size.cy;
	*w = size.cx;
}

void
draw_hrule(int x, int y, int w)
{
	MoveToEx(draw_hdc, x, y, NULL);
	LineTo(draw_hdc, x + w, y);
}

// Example: w = 5, h = 7
//
//	  0 1 2 3 4
//	0 B x x x A
//	1 x
//	2 x
//	3 x
//	4 x
//	5 x
//	6 C x x x D
//
//	A = (x + w - 1, y)
//
//	B = (x, y)
//
//	C = (x, y + h - 1)
//
//	D = (x + w - 1, y + h - 1)
//
//	Since the last pixel is not drawn, use D = (x + w, y + h - 1).

void
draw_left_bracket(int x, int y, int w, int h)
{
	MoveToEx(draw_hdc, x + w - 1, y, NULL);
	LineTo(draw_hdc, x, y);
	LineTo(draw_hdc, x, y + h - 1);
	LineTo(draw_hdc, x + w, y + h - 1);

	// extra stem width

	MoveToEx(draw_hdc, x + 1, y, NULL);
	LineTo(draw_hdc, x + 1, y + h - 1);
}

// Example: w = 5, h = 7
//
//	  0 1 2 3 4
//	0 A x x x B
//	1         x
//	2         x
//	3         x
//	4         x
//	5         x
//	6 D x x x C
//
//	A = (x, y)
//
//	B = (x + w - 1, y)
//
//	C = (x + w - 1, y + h - 1)
//
//	D = (x, y + h - 1)
//
//	Since the last pixel is not drawn, use D = (x - 1, y + h - 1).

void
draw_right_bracket(int x, int y, int w, int h)
{
	MoveToEx(draw_hdc, x, y, NULL);
	LineTo(draw_hdc, x + w - 1, y);
	LineTo(draw_hdc, x + w - 1, y + h - 1);
	LineTo(draw_hdc, x - 1, y + h - 1);

	// extra stem width

	MoveToEx(draw_hdc, x + w - 2, y, NULL);
	LineTo(draw_hdc, x + w - 2, y + h - 1);
}

void
draw_line(int x1, int y1, int x2, int y2)
{
	MoveToEx(draw_hdc, x1, y1, NULL);
	LineTo(draw_hdc, x2, y2);
}

static int draw_mode;
static HPEN original_pen;
static HBRUSH original_brush;

void
use_graph_pen(void)
{
	HPEN pen;
	HBRUSH brush;
	LOGBRUSH tmp;
	if (draw_mode == 1)
		return;
	draw_mode = 1;
	pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	tmp.lbStyle = BS_SOLID;
	tmp.lbColor = RGB(0, 0, 0);
	tmp.lbHatch = 0;
	brush = CreateBrushIndirect(&tmp);
	original_pen = (HPEN) SelectObject(draw_hdc, pen);
	original_brush = (HBRUSH) SelectObject(draw_hdc, brush);
}

void
use_normal_pen(void)
{
	HPEN pen;
	HBRUSH brush;
	if (draw_mode == 0)
		return;
	draw_mode = 0;
	pen = (HPEN) SelectObject(draw_hdc, original_pen);
	brush = (HBRUSH) SelectObject(draw_hdc, original_brush);
	DeleteObject(pen);
	DeleteObject(brush);
}

void
draw_box(int x1, int y1, int x2, int y2)
{
#if 1
	HBRUSH brush;
	LOGBRUSH tmp;
	use_normal_pen();
	tmp.lbStyle = BS_SOLID;
	tmp.lbColor = RGB(255, 255, 239);
	tmp.lbHatch = 0;
	brush = CreateBrushIndirect(&tmp);
	SelectObject(draw_hdc, brush);
	Rectangle(draw_hdc, x1, y1, x2 + 1, y2 + 1);
	brush = (HBRUSH) GetStockObject(WHITE_BRUSH);
	brush = (HBRUSH) SelectObject(draw_hdc, brush);
	DeleteObject(brush);
#else
	MoveToEx(draw_hdc, x1, y1, NULL);
	LineTo(draw_hdc, x2, y1);
	LineTo(draw_hdc, x2, y2);
	LineTo(draw_hdc, x1, y2);
	LineTo(draw_hdc, x1, y1);
#endif
}

void
draw_point(int x, int dx, int y, int dy)
{
	//Rectangle(draw_hdc, x - 1, y - 1, x + 2, y + 2);
	RECT r;
	r.left = x + dx - 1;
	r.top = y + dy - 1;
	r.right = x + dx + 2;
	r.bottom = y + dy + 2;
	if (dx == 0)
		r.left++;
	if (dy == 0)
		r.top++;
	if (dx == 300)
		r.right--;
	if (dy == 300)
		r.bottom--;
	FillRect(draw_hdc, &r, (HBRUSH) GetStockObject(BLACK_BRUSH));
}

static void
move_input_window(void)
{
	int h, w, x, y;

	x = 0;
	y = main_client_height - 3 * line_height;
	w = main_client_width;
	h = line_height;

	MoveWindow(input_window, x, y, w, h, TRUE);
}

static void
move_edit_window(void)
{
	int h, w, x, y;

	x = 0;
	y = 0;
	w = main_client_width;
	h = main_client_height - 2 * line_height;

	MoveWindow(edit_window, x, y, w, h, TRUE);
}

static void
move_buttons(void)
{
	int i, j, k;
	for (i = 0; i < NBUTTON; i++) {
		j = i * main_client_width / NBUTTON;
		k = (i + 1) * main_client_width / NBUTTON;
		MoveWindow(
			button[i],
			j, main_client_height - 2 * line_height,
			k - j, 2 * line_height,
			TRUE);
	}
}

static void
move_scroll_bars(void)
{
	int h, w, x, y;

	x = display_width;
	y = 0;
	w = scroll_bar_width,
	h = display_height;

	MoveWindow(vscroll, x, y, w, h, TRUE);

	x = 0;
	y = display_height;
	w = display_width;
	h = scroll_bar_width;

	MoveWindow(hscroll, x, y, w, h, TRUE);
}

// Cannot use "pos" because it is too small (16 bits). Use GetScrollInfo instead.

static void
do_vscroll(int cmd, int pos)
{
	int y;
	SCROLLINFO info;

	y = display_y;

	switch (cmd) {

	case SB_TOP:
		display_y = 0;
		break;

	case SB_BOTTOM:
		display_y = max_y;
		break;

	case SB_LINEUP:
		display_y -= line_height;
		break;

	case SB_LINEDOWN:
		display_y += line_height;
		break;

	case SB_PAGEUP:
		display_y -= display_height;
		break;

	case SB_PAGEDOWN:
		display_y += display_height;
		break;

	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		info.cbSize = sizeof (SCROLLINFO);
		info.fMask = SIF_TRACKPOS;
		GetScrollInfo(vscroll, SB_CTL, &info);
		display_y = info.nTrackPos;
		break;

	default:
		break;
	}

	if (display_y < 0)
		display_y = 0;

	if (display_y > max_y)
		display_y = max_y;

	if (display_y < y)
		move_bits_down(y - display_y);

	if (display_y > y)
		move_bits_up(display_y - y);

	update_scroll_bars();
}

static void
move_bits_down(int dy)
{
	RECT r;
	HRGN hrgn;

	//ScrollWindowEx(main_window, 0, dy, NULL, NULL, NULL, NULL, SW_ERASE | SW_INVALIDATE);
	//return;

	draw_hdc = GetDC(main_window);
	
	if (dy < display_height)
		BitBlt(
			draw_hdc,
			0, dy,
			display_width, display_height - dy,
			draw_hdc,
			0, 0,
			SRCCOPY);
	else
		dy = display_height;

	r.left = 0;
	r.top = 0;
	r.right = display_width;
	r.bottom = dy;

	// have to draw instead of update to stay ahead of next scroll event

	// display bitmap has to be valid before next scroll event

	FillRect(draw_hdc, &r, (HBRUSH) GetStockObject(WHITE_BRUSH));
	hrgn = CreateRectRgnIndirect(&r);
	SelectClipRgn(draw_hdc, hrgn);
	SetBkMode(draw_hdc, TRANSPARENT);
	draw_display();
	DeleteObject(hrgn);

	ReleaseDC(main_window, draw_hdc);
}

static void
move_bits_up(int dy)
{
	RECT r;
	HRGN hrgn;

	//ScrollWindowEx(main_window, 0, -dy, NULL, NULL, NULL, NULL, SW_ERASE | SW_INVALIDATE);
	//return;

	draw_hdc = GetDC(main_window);

	if (dy < display_height)
		BitBlt(
			draw_hdc,
			0, 0,
			display_width, display_height - dy,
			draw_hdc,
			0, dy,
			SRCCOPY);
	else
		dy = display_height;

	r.left = 0;
	r.top = display_height - dy;
	r.right = display_width;
	r.bottom = display_height;

	// have to draw instead of update to stay ahead of next scroll event

	// display bitmap has to be valid before next scroll event

	FillRect(draw_hdc, &r, (HBRUSH) GetStockObject(WHITE_BRUSH));
	hrgn = CreateRectRgnIndirect(&r);
	SelectClipRgn(draw_hdc, hrgn);
	SetBkMode(draw_hdc, TRANSPARENT);
	draw_display();
	DeleteObject(hrgn);

	ReleaseDC(main_window, draw_hdc);
}

// According to MS documentation...
//
//	MaxScrollPos = MaxRangeValue - (PageSize - 1)
//
// Since we want MaxScrollPos = max_y, we set nMax = max_y + display_height - 1.
//
//	nMax = max_y + display_height - 1
//
//	nPage = display_height
//
//	MaxScrollPos = max_y + display_height - 1 - (display_height - 1) = max_y

static void
update_scroll_bars(void)
{
	SCROLLINFO info;

	info.cbSize = sizeof (SCROLLINFO);
	info.fMask = SIF_ALL;
	info.nMin = 0;
	info.nMax = max_y + display_height - 1;
	info.nPage = display_height;
	info.nPos = display_y;
	SetScrollInfo(vscroll, SB_CTL, &info, TRUE);

	info.cbSize = sizeof (SCROLLINFO);
	info.fMask = SIF_ALL;
	info.nMin = 0;
	info.nMax = max_x + display_width - 1;
	info.nPage = display_width;
	info.nPos = display_x;
	SetScrollInfo(hscroll, SB_CTL, &info, TRUE);
}

// Cannot use "pos" because it is too small (16 bits). Use GetScrollInfo instead.

static void move_bits_left(int);
static void move_bits_right(int);

void
do_hscroll(int cmd, int pos)
{
	int x;
	SCROLLINFO info;

	x = display_x;

	switch (cmd) {

	case SB_LEFT:
		display_x = 0;
		break;

	case SB_RIGHT:
		display_x = max_x;
		break;

	case SB_LINELEFT:
		display_x -= line_height;
		break;

	case SB_LINERIGHT:
		display_x += line_height;
		break;

	case SB_PAGELEFT:
		display_x -= display_width;
		break;

	case SB_PAGERIGHT:
		display_x += display_width;
		break;

	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		info.cbSize = sizeof (SCROLLINFO);
		info.fMask = SIF_TRACKPOS;
		GetScrollInfo(hscroll, SB_CTL, &info);
		display_x = info.nTrackPos;
		break;

	default:
		break;
	}

	if (display_x < 0)
		display_x = 0;

	if (display_x > max_x)
		display_x = max_x;

	if (display_x < x)
		move_bits_right(x - display_x);

	if (display_x > x)
		move_bits_left(display_x - x);

	update_scroll_bars();
}

static void
move_bits_right(int dx)
{
	int h, w, x, y;
	RECT r;
	HRGN hrgn;

	draw_hdc = GetDC(main_window);
	
	if (dx < display_width) {

		//	Example: dx = 2
		//
		//	x x x x x x x x x x
		//	x             x   x
		//	x             x   x
		//	x             x   x
		//	x             x   x
		//	x x x x x x x x x x
		//	\              \
		//	 \              \
		//        \              \
		//	   \              \
		//	x x x x x x x x x x
		//	x   x             x
		//	x   x             x
		//	x   x             x
		//	x   x             x
		//	x x x x x x x x x x

		// dimensions of dst rectangle

		x = dx;
		y = 0;
		w = display_width - dx;
		h = display_height;

		BitBlt(
			draw_hdc,
			x, y,
			w, h,
			draw_hdc,
			0, 0, // src
			SRCCOPY);
	} else
		dx = display_width;

	// set update rectangle

	if (dx == 1) dx = 2; // bug in windows 2000

	r.left = 0;
	r.top = 0;
	r.right = dx;
	r.bottom = display_height;

	// have to draw instead of update to stay ahead of next scroll event

	// display bitmap has to be valid before next scroll event

	FillRect(draw_hdc, &r, (HBRUSH) GetStockObject(WHITE_BRUSH));
	hrgn = CreateRectRgnIndirect(&r);
	SelectClipRgn(draw_hdc, hrgn);
	SetBkMode(draw_hdc, TRANSPARENT);
	draw_display();
	DeleteObject(hrgn);

	ReleaseDC(main_window, draw_hdc);
}

static void
move_bits_left(int dx)
{
	int h, w, x, y;
	RECT r;
	HRGN hrgn;

	draw_hdc = GetDC(main_window);

	if (dx < display_width) {

		//	Example: dx = 2
		//
		//	x x x x x x x x x x
		//	x   x             x
		//	x   x             x
		//	x   x             x
		//	x   x             x
		//	x x x x x x x x x x
		//	   /              /
		//	  /              /
		//       /              /
		//	/              /
		//	x x x x x x x x x x
		//	x             x   x
		//	x             x   x
		//	x             x   x
		//	x             x   x
		//	x x x x x x x x x x

		// dimensions of dst rectangle

		x = 0;
		y = 0;
		w = display_width - dx;
		h = display_height;

		BitBlt(
			draw_hdc,
			x, y,
			w, h,
			draw_hdc,
			dx, 0, // src
			SRCCOPY);
	} else
		dx = display_width;

	// set update rectangle

	if (dx == 1) dx = 2; // bug in windows 2000

	r.left = display_width - dx;
	r.top = 0;
	r.right = display_width;
	r.bottom = display_height;

	// have to draw instead of update to stay ahead of next scroll event

	// display bitmap has to be valid before next scroll event

	FillRect(draw_hdc, &r, (HBRUSH) GetStockObject(WHITE_BRUSH));
	hrgn = CreateRectRgnIndirect(&r);
	SelectClipRgn(draw_hdc, hrgn);
	SetBkMode(draw_hdc, TRANSPARENT);
	draw_display();
	DeleteObject(hrgn);

	ReleaseDC(main_window, draw_hdc);
}

#if 0

void
redraw_window_now(void)
{
	RECT r;
	HRGN hrgn;

	draw_hdc = GetDC(main_window);

	r.left = 0;
	r.top = 0;
	r.right = display_width;
	r.bottom = display_height;

	FillRect(draw_hdc, &r, (HBRUSH) GetStockObject(WHITE_BRUSH));
	hrgn = CreateRectRgnIndirect(&r);
	SelectClipRgn(draw_hdc, hrgn);
	SetBkMode(draw_hdc, TRANSPARENT);
	draw_display();
	DeleteObject(hrgn);

	ReleaseDC(main_window, draw_hdc);

	update_scroll_bars();

	SetFocus(input_window);
}

#endif

void
update_display(void)
{
	RECT r;
	if (update_display_request == 0)
		return;
	r.left = 0;
	r.top = 0;
	r.right = display_width;
	r.bottom = display_height;
	RedrawWindow(main_window, &r, NULL, RDW_ERASE | RDW_INVALIDATE);
}

OPENFILENAME ofn;
char filename[1000];

// f = fopen(s, "rb"); // must do rb for msdos files else \r changes to \n

static void
do_create_script(void)
{
	char *s;
	*filename = 0;
	s = get_cmd_history();
	SetWindowText(edit_window, s);
	free(s);
	goto_edit_mode();
}

static void
do_new(void)
{
	char buf[1];
	*filename = 0;
	buf[0] = 0;
	SetWindowText(edit_window, buf);
	goto_edit_mode();
}

static void
do_open(void)
{
	int len;
	FILE *f;
	char *buf;
	ofn.lStructSize = sizeof (OPENFILENAME);
	ofn.hwndOwner = main_window;
	ofn.lpstrFile = filename;
	ofn.nMaxFile = 1000;
	if (GetOpenFileName(&ofn) == 0)
		return;
	buf = (char *) malloc(100001);
	if (buf == 0)
		return;
	f = fopen(filename, "rb"); // must do rb for msdos files else \r changes to \n
	if (f == 0) {
		free(buf);
		return;
	}
	len = (int) fread(buf, 1, 100000, f);
	fclose(f);
	buf[len] = 0;
	SetWindowText(edit_window, buf);
	free(buf);
	goto_edit_mode();
}

static void
do_save(void)
{
	if (*filename)
		save_file();
	else
		do_saveas();
}

static void
do_saveas(void)
{
	ofn.lStructSize = sizeof (OPENFILENAME);
	ofn.hwndOwner = main_window;
	ofn.lpstrFile = filename;
	ofn.nMaxFile = 1000;
	if (GetSaveFileName(&ofn) == 0)
		return;
	save_file();
}

static void
save_file(void)
{
	int len;
	FILE *f;
	char *buf;
	len = GetWindowTextLength(edit_window);
	buf = (char *) malloc(len + 1);
	if (buf == 0) {
		printstr("Error writing file.\n");
		update_display();
		return;
	}
	GetWindowText(edit_window, buf, len + 1);
	f = fopen(filename, "wb");
	if (f == NULL) {
		free(buf);
		printstr("Error writing file.\n");
		update_display();
		return;
	}
	if (fwrite(buf, 1, len, f) != len) {
		printstr("Error writing file.\n");
		update_display();
	}
	fclose(f);
	free(buf);
}

PAGESETUPDLG page;

static void
do_page_setup(void)
{
	page.lStructSize = sizeof page;
	page.hwndOwner = main_window;

	PageSetupDlg(&page);
}

PRINTDLG pd;

static void
do_print(void)
{
	if (pd.lStructSize == 0) {
		pd.lStructSize = sizeof pd;
		pd.hwndOwner = main_window;
		pd.hDevMode = NULL;
		pd.hDevNames = NULL;
		pd.Flags = PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC;
		pd.nCopies = 1;
		pd.nFromPage = 1;
		pd.nToPage = 1;
		pd.nMinPage = 1;
		pd.nMaxPage = 0xffff;
	}

	if (PrintDlg(&pd) == TRUE) {


		DeleteDC(pd.hDC);
	}
}

static char *inp;
static HANDLE thread;

static DWORD WINAPI
task(LPVOID p)
{
	run_hdc = GetDC(main_window);
	run(inp);
	ReleaseDC(main_window, run_hdc);
	running = 2;
	send_user_event();
	return 0;
}

static void
create_task(void)
{
	DWORD id;
	timer = (unsigned int) time(NULL);
	running = 1;
	thread = CreateThread(
		NULL,
		1024 * 1024,
		task,
		NULL,
		0,
		&id);
}

static void
send_user_event(void)
{
	PostMessage(main_window, ID_USER_EVENT, 0, 0);
}

static void
process_user_event(void)
{
	unsigned int dt;
	static char buf[1000];

	if (running == 0)
		return;

	if (running == 2) {
		CloseHandle(thread);
		update_curr_cmd("");
		update_display();
		activate_controls();
		running = 0;
		return;
	}

	dt = (unsigned int) time(NULL) - timer;

	if (dt > 1) {
		deactivate_controls();
		sprintf(buf, "Esc key to stop (%d)", dt);
		update_curr_cmd(buf);
		update_display();
	}
}

static int shunted;

static void
deactivate_controls(void)
{
	int i;
	if (shunted == 1)
		return;
	shunted = 1;
	for (i = 0; i < NBUTTON; i++)
		EnableWindow(button[i], FALSE);
	EnableWindow(input_window, FALSE);
}

static void
activate_controls(void)
{
	int i;
	if (shunted == 0)
		return;
	shunted = 0;
	for (i = 0; i < NBUTTON; i++)
		EnableWindow(button[i], TRUE);
	EnableWindow(input_window, TRUE);
	SetFocus(input_window);
}

static void
do_enter(void)
{
	if (running || edit_mode)
		return;
	if (inp)
		free(inp);
	inp = get_curr_cmd();
	update_cmd_history(inp); // reset history pointer no matter what
	if (*inp == 0)
		return;
	run_hdc = GetDC(main_window);
	echo_input(inp);
	ReleaseDC(main_window, run_hdc);
	update_curr_cmd("");
	create_task();
}

static void
do_button(const char *s)
{
	char *tmp;
	if (edit_mode == 0)
		SetFocus(input_window);
	if (running || edit_mode)
		return;
	if (inp)
		free(inp);
	inp = get_curr_cmd();
	update_cmd_history((const char *)inp); // reset history pointer no matter what
	if (*inp == 0) {
		free(inp);
		inp = (char *) malloc(strlen(s) + 7);
		strcpy(inp, s);
	} else {
		tmp = (char *) malloc(strlen(s) + strlen(inp) + 3);
		if (strcmp(s, "derivative") == 0)
			strcpy(tmp, "d");
		else
			strcpy(tmp, s);
		strcat(tmp, "(");
		strcat(tmp, inp);
		strcat(tmp, ")");
		free(inp);
		inp = tmp;
	}
	update_cmd_history((const char *)inp);
	run_hdc = GetDC(main_window);
	echo_input(inp);
	ReleaseDC(main_window, run_hdc);
	update_curr_cmd("");
	create_task();
}

static void
run_script(void)
{
	int len;
	if (edit_mode == 0)
		SetFocus(input_window); // move focus from run button
	if (running)
		return;
	if (inp)
		free(inp);
	inp = get_curr_cmd();
	update_cmd_history(inp);
	free(inp);
	len = GetWindowTextLength(edit_window);
	inp = (char *) malloc(len + 1);
	GetWindowText(edit_window, inp, len + 1);
	goto_calc_mode();
	deactivate_controls();
	eval_clear();
	update_display();
	create_task();
	update_curr_cmd("");
}

static void
copy_all(void)
{
	HDC tmp;
	RECT r;
	HBITMAP bitmap;
	if (total_h == 0)
		return;
	tmp = GetDC(main_window);
	draw_hdc = CreateCompatibleDC(tmp);
	r.left = 0;
	r.top = 0;
	r.right = display_width;
	r.bottom = total_h - display_y;
	if (r.bottom > display_height)
		r.bottom = display_height;
	bitmap = CreateCompatibleBitmap(tmp, r.right, r.bottom);
	SelectObject(draw_hdc, bitmap);
	FillRect(draw_hdc, &r, (HBRUSH) GetStockObject(WHITE_BRUSH));
	SetBkMode(draw_hdc, TRANSPARENT);
	draw_display();
	OpenClipboard(main_window);
	EmptyClipboard();
	SetClipboardData(CF_BITMAP, bitmap);
	CloseClipboard();
	DeleteObject(bitmap);
	DeleteDC(draw_hdc);
	DeleteDC(tmp);
}

static void
do_special(char *s)
{
	if (inp)
		free(inp);
	inp = strdup(s);
	update_cmd_history(inp);
	echo_input(inp);
	update_curr_cmd("");
	run(inp);
}

static void
do_help(char **s, int n)
{
	int i;
	if (running)
		return;
	goto_calc_mode();
	run_hdc = GetDC(main_window);
	do_special("clear");
	for (i = 0; i < n; i++)
		do_special(s[i]);
	ReleaseDC(main_window, run_hdc);
	update_display();
}
