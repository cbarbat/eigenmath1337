struct display {
	int h, w, n;
	struct {
		int c, x, y;
	} a[1]; // a for array
};

struct text_metric {
	int ascent, descent, width;
};

/*
struct display {
	struct display *next;
	uint8_t type, attr;
	int h, w;
	int tot_h, tot_w;
	int len;
	uint8_t buf[0];
};

struct text_metric {
	float size;
	int ascent, descent, width, xheight, em;
};
*/