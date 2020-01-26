#pragma once
void shipout(unsigned char *d, int w, int h);
void draw_main(void);
void check_for_parametric_draw(void);
void create_point_set(void);
void new_point(double t);
void eval_f(double t);
void setup_trange(void);
void setup_xrange(void);
void setup_yrange(void);
void setup_trange_f(void);
void setup_xrange_f(void);
void setup_yrange_f(void);
void emit_graph(void);
void fill(int i, int k, int level);
void get_xy(double t);
