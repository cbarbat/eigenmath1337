#include "defs.h"

void
push_integer(int n)
{
	if (n < 0)
		push_rational_number(MMINUS, mint(-n), mint(1));
	else
		push_rational_number(MPLUS, mint(n), mint(1));
}

void
push_rational(int a, int b)
{
	if (a < 0)
		push_rational_number(MMINUS, mint(-a), mint(b));
	else
		push_rational_number(MPLUS, mint(a), mint(b));
}

void
push_rational_number(int sign, uint32_t *a, uint32_t *b)
{
	struct atom *p;
	if (MZERO(a)) {
		sign = MPLUS;
		if (!MEQUAL(b, 1)) {
			mfree(b);
			b = mint(1);
		}
	}
	p = alloc();
	p->k = RATIONAL;
	p->sign = sign;
	p->u.q.a = a;
	p->u.q.b = b;
	push(p);
}

int
pop_integer(void)
{
	int n = ERR;
	struct atom *p; // ok, no gc
	p = pop();
	if (isinteger(p) && MLENGTH(p->u.q.a) == 1 && p->u.q.a[0] < 0x80000000) {
		n = p->u.q.a[0];
		if (p->sign == MMINUS)
			n = -n;
	} else if (isdouble(p)) {
		n = (int) p->u.d;
		if ((double) n != p->u.d)
			n = ERR;
	}
	return n;
}

void
push_double(double d)
{
	struct atom *p; // ok, no gc before push
	p = alloc();
	p->k = DOUBLE;
	p->u.d = d;
	push(p);
}

double
pop_double(void)
{
	double d;
	struct atom *p; // ok, no gc
	p = pop();
	switch (p->k) {
	case RATIONAL:
		d = convert_rational_to_double(p);
		break;
	case DOUBLE:
		d = p->u.d;
		break;
	default:
		d = 0.0;
		break;
	}
	return d;
}

int
compare_numbers(struct atom *a, struct atom *b)
{
	double aa, bb;
	if (isrational(a) && isrational(b))
		return compare_rationals(a, b);
	if (isdouble(a))
		aa = a->u.d;
	else
		aa = convert_rational_to_double(a);
	if (isdouble(b))
		bb = b->u.d;
	else
		bb = convert_rational_to_double(b);
	if (aa < bb)
		return -1;
	if (aa > bb)
		return 1;
	return 0;
}

int
compare_rationals(struct atom *a, struct atom *b)
{
	int t;
	uint32_t *ab, *ba;
	if (a->sign == MMINUS && b->sign == MPLUS)
		return -1;
	if (a->sign == MPLUS && b->sign == MMINUS)
		return 1;
	ab = mmul(a->u.q.a, b->u.q.b);
	ba = mmul(a->u.q.b, b->u.q.a);
	if (a->sign == MMINUS)
		t = mcmp(ba, ab);
	else
		t = mcmp(ab, ba);
	mfree(ab);
	mfree(ba);
	return t;
}

double
convert_rational_to_double(struct atom *p)
{
	int i, n, na, nb;
	double a = 0.0, b = 0.0;

	na = MLENGTH(p->u.q.a);
	nb = MLENGTH(p->u.q.b);

	if (na < nb)
		n = na;
	else
		n = nb;

	for (i = 0; i < n; i++) {
		a = a / 4294967296.0 + p->u.q.a[i];
		b = b / 4294967296.0 + p->u.q.b[i];
	}

	if (na > nb)
		for (i = nb; i < na; i++) {
			a = a / 4294967296.0 + p->u.q.a[i];
			b = b / 4294967296.0;
		}

	if (na < nb)
		for (i = na; i < nb; i++) {
			a = a / 4294967296.0;
			b = b / 4294967296.0 + p->u.q.b[i];
		}

	if (p->sign == MMINUS)
		a = -a;

	return a / b;
}

void
convert_double_to_rational(double d)
{
	double e, x;
	if (d == 0.0) {
		push(zero);
		return;
	}
	if (!isnormal(d))
		stop("floating point value is nan or inf, cannot convert to rational number");
	x = fabs(d);
	e = floor(log10(x)) + 1.0;
	best_rational_approximation(x / pow(10.0, e));
	push_integer(10);
	push_integer((int) e);
	power();
	multiply();
	if (d < 0.0)
		negate();
}

#undef N
#define N 1000

void
best_rational_approximation(double x)
{
	int a = 0, b = 1, c = 1, d = 1;
	double m;
	for (;;) {
		m = (double) (a + c) / (double) (b + d);
		if (m == x)
			break;
		if (x < m) {
			c += a;
			d += b;
			if (d > N) {
				push_rational(a, b);
				return;
			}
		} else {
			a += c;
			b += d;
			if (b > N) {
				push_rational(c, d);
				return;
			}
		}
	}
	if (b + d <= N)
		push_rational(a + c, b + d);
	else if (d > b)
		push_rational(c, d); // largest denominator is most accurate
	else
		push_rational(a, b);
}

void
bignum_scan_integer(char *s)
{
	int sign;
	uint32_t *a;
	if (*s == '-')
		sign = MMINUS;
	else
		sign = MPLUS;
	if (*s == '+' || *s == '-')
		s++;
	a = mscan(s);
	if (a == NULL)
		stop("parse error");
	push_rational_number(sign, a, mint(1));
}

void
bignum_float(void)
{
	double d;
	struct atom *p; // ok, no gc
	p = pop();
	d = convert_rational_to_double(p);
	push_double(d);
}

void
bignum_factorial(int n)
{
	push_rational_number(MPLUS, bignum_factorial_nib(n), mint(1));
}

uint32_t *
bignum_factorial_nib(int n)
{
	int i;
	uint32_t *a, *b, *t;
	if (n == 0 || n == 1)
		return mint(1);
	a = mint(2);
	b = mint(0);
	for (i = 3; i <= n; i++) {
		b[0] = (uint32_t) i;
		t = mmul(a, b);
		mfree(a);
		a = t;
	}
	mfree(b);
	return a;
}

void
msetbit(uint32_t *x, uint32_t k)
{
	x[k / 32] |= 1 << (k % 32);
}

void
mclrbit(uint32_t *x, uint32_t k)
{
	x[k / 32] &= ~(1 << (k % 32));
}

void
mshiftright(uint32_t *a)
{
	int c, i, n;
	n = MLENGTH(a);
	c = 0;
	for (i = n - 1; i >= 0; i--)
		if (a[i] & 1) {
			a[i] = (a[i] >> 1) | c;
			c = 0x80000000;
		} else {
			a[i] = (a[i] >> 1) | c;
			c = 0;
		}
	if (n > 1 && a[n - 1] == 0)
		MLENGTH(a) = n - 1;
}

uint32_t *
mscan(char *s)
{
	uint32_t *a, *b, *t;
	a = mint(0);
	t = mint(0);
	while (*s) {
		if (*s < '0' || *s > '9') {
			mfree(a);
			a = NULL;
			break;
		}
		t[0] = 10;
		b = mmul(a, t);
		mfree(a);
		t[0] = *s - '0';
		a = madd(b, t);
		mfree(b);
		s++;
	}
	mfree(t);
	return a;
}

// convert bignum to string

char *
mstr(uint32_t *a)
{
	int k, n, r;
	char c;

	static char *str;
	static int len;

	if (str == NULL) {
		str = (char *) malloc(1000);
		if (str == NULL)
			malloc_kaput();
		len = 1000;
	}

	// estimate string size

	n = 10 * MLENGTH(a) + 1;

	if (n > len) {
		free(str);
		str = (char *) malloc(n);
		if (str == NULL)
			malloc_kaput();
		len = n;
	}

	a = mcopy(a);

	k = len - 1;

	str[k] = 0;

	for (;;) {
		k -= 9;
		r = mdivby1billion(a);
		c = str[k + 9];
		sprintf(str + k, "%09d", r);
		str[k + 9] = c; // restore char clobbered by sprintf
		if (MZERO(a))
			break;
	}

	// remove leading zeroes

	while (str[k] == '0')
		k++;

	if (str[k] == 0)
		k--; // leave one leading zero

	mfree(a);

	return str + k;
}

// returns remainder as function value, quotient returned in a

int
mdivby1billion(uint32_t *a)
{
	int i;
	uint64_t kk = 0;
	for (i = MLENGTH(a) - 1; i >= 0; i--) {
		kk = kk << 32 | a[i];
		a[i] = (uint32_t) (kk / 1000000000); // compiler warns w/o cast
		kk -= (uint64_t) 1000000000 * a[i];
	}
	mnorm(a);
	return (int) kk; // compiler warns w/o cast
}

// returns u + v

uint32_t *
madd(uint32_t *u, uint32_t *v)
{
	int i, nu, nv, nw;
	uint64_t t;
	uint32_t *w;
	nu = MLENGTH(u);
	nv = MLENGTH(v);
	if (nu > nv)
		nw = nu + 1;
	else
		nw = nv + 1;
	w = mnew(nw);
	for (i = 0; i < nu; i++)
		w[i] = u[i];
	for (i = nu; i < nw; i++)
		w[i] = 0;
	t = 0;
	for (i = 0; i < nv; i++) {
		t += (uint64_t) w[i] + v[i];
		w[i] = (uint32_t) t;
		t >>= 32;
	}
	for (i = nv; i < nw; i++) {
		t += w[i];
		w[i] = (uint32_t) t;
		t >>= 32;
	}
	mnorm(w);
	return w;
}

// returns u - v

uint32_t *
msub(uint32_t *u, uint32_t *v)
{
	int i, nu, nv, nw;
	uint64_t t;
	uint32_t *w;
	nu = MLENGTH(u);
	nv = MLENGTH(v);
	if (nu > nv)
		nw = nu;
	else
		nw = nv;
	w = mnew(nw);
	for (i = 0; i < nu; i++)
		w[i] = u[i];
	for (i = nu; i < nw; i++)
		w[i] = 0;
	t = 0;
	for (i = 0; i < nv; i++) {
		t += (uint64_t) w[i] - v[i];
		w[i] = (uint32_t) t;
		t = (int64_t) t >> 32; // cast to extend sign
	}
	for (i = nv; i < nw; i++) {
		t += w[i];
		w[i] = (uint32_t) t;
		t = (int64_t) t >> 32; // cast to extend sign
	}
	mnorm(w);
	return w;
}

// returns u * v

uint32_t *
mmul(uint32_t *u, uint32_t *v)
{
	int i, j, nu, nv, nw;
	uint64_t t;
	uint32_t *w;
	nu = MLENGTH(u);
	nv = MLENGTH(v);
	nw = nu + nv;
	w = mnew(nw);
	for (i = 0; i < nu; i++)
		w[i] = 0;
	for (j = 0; j < nv; j++) {
		t = 0;
		for (i = 0; i < nu; i++) {
			t += (uint64_t) u[i] * v[j] + w[i + j];
			w[i + j] = (uint32_t) t;
			t >>= 32;
		}
		w[i + j] = (uint32_t) t;
	}
	mnorm(w);
	return w;
}

// returns floor(u / v)

uint32_t *
mdiv(uint32_t *u, uint32_t *v)
{
	int i, k, nu, nv;
	uint32_t *q, qhat, *w;
	uint64_t a, b, t;
	mnorm(u);
	mnorm(v);
	if (MLENGTH(v) == 1 && v[0] == 0)
		stop("divide by zero"); // v = 0
	nu = MLENGTH(u);
	nv = MLENGTH(v);
	k = nu - nv;
	if (k < 0) {
		q = mnew(1);
		q[0] = 0;
		return q; // u < v, return zero
	}
	u = mcopy(u);
	q = mnew(k + 1);
	w = mnew(nv + 1);
	b = v[nv - 1];
	do {
		q[k] = 0;
		while (nu >= nv + k) {
			// estimate 32-bit partial quotient
			a = u[nu - 1];
			if (nu > nv + k)
				a = a << 32 | u[nu - 2];
			if (a < b)
				break;
			qhat = (uint32_t) (a / (b + 1));
			if (qhat == 0)
				qhat = 1;
			// w = qhat * v
			t = 0;
			for (i = 0; i < nv; i++) {
				t += (uint64_t) qhat * v[i];
				w[i] = (uint32_t) t;
				t >>= 32;
			}
			w[nv] = (uint32_t) t;
			// u = u - w
			t = 0;
			for (i = k; i < nu; i++) {
				t += (uint64_t) u[i] - w[i - k];
				u[i] = (uint32_t) t;
				t = (int64_t) t >> 32; // cast to extend sign
			}
			if (t) {
				// u is negative, restore u
				t = 0;
				for (i = k; i < nu; i++) {
					t += (uint64_t) u[i] + w[i - k];
					u[i] = (uint32_t) t;
					t >>= 32;
				}
				break;
			}
			q[k] += qhat;
			mnorm(u);
			nu = MLENGTH(u);
		}
	} while (--k >= 0);
	mnorm(q);
	mfree(u);
	mfree(w);
	return q;
}

// returns u mod v

uint32_t *
mmod(uint32_t *u, uint32_t *v)
{
	int i, k, nu, nv;
	uint32_t qhat, *w;
	uint64_t a, b, t;
	mnorm(u);
	mnorm(v);
	if (MLENGTH(v) == 1 && v[0] == 0)
		stop("divide by zero"); // v = 0
	u = mcopy(u);
	nu = MLENGTH(u);
	nv = MLENGTH(v);
	k = nu - nv;
	if (k < 0)
		return u; // u < v
	w = mnew(nv + 1);
	b = v[nv - 1];
	do {
		while (nu >= nv + k) {
			// estimate 32-bit partial quotient
			a = u[nu - 1];
			if (nu > nv + k)
				a = a << 32 | u[nu - 2];
			if (a < b)
				break;
			qhat = (uint32_t) (a / (b + 1));
			if (qhat == 0)
				qhat = 1;
			// w = qhat * v
			t = 0;
			for (i = 0; i < nv; i++) {
				t += (uint64_t) qhat * v[i];
				w[i] = (uint32_t) t;
				t >>= 32;
			}
			w[nv] = (uint32_t) t;
			// u = u - w
			t = 0;
			for (i = k; i < nu; i++) {
				t += (uint64_t) u[i] - w[i - k];
				u[i] = (uint32_t) t;
				t = (int64_t) t >> 32; // cast to extend sign
			}
			if (t) {
				// u is negative, restore u
				t = 0;
				for (i = k; i < nu; i++) {
					t += (uint64_t) u[i] + w[i - k];
					u[i] = (uint32_t) t;
					t >>= 32;
				}
				break;
			}
			mnorm(u);
			nu = MLENGTH(u);
		}
	} while (--k >= 0);
	mfree(w);
	return u;
}

// returns u ** v

uint32_t *
mpow(uint32_t *u, uint32_t *v)
{
	uint32_t *t, *w;
	u = mcopy(u);
	v = mcopy(v);
	// w = 1
	w = mnew(1);
	w[0] = 1;
	for (;;) {
		if (v[0] & 1) {
			// w = w * u
			t = mmul(w, u);
			mfree(w);
			w = t;
		}
		// v = v >> 1
		mshr(v);
		// v = 0?
		if (MLENGTH(v) == 1 && v[0] == 0)
			break;
		// u = u * u
		t = mmul(u, u);
		mfree(u);
		u = t;
	}
	mfree(u);
	mfree(v);
	return w;
}

// u = u >> 1

void
mshr(uint32_t *u)
{
	int i;
	for (i = 0; i < MLENGTH(u) - 1; i++) {
		u[i] >>= 1;
		if (u[i + 1] & 1)
			u[i] |= 0x80000000;
	}
	u[i] >>= 1;
	mnorm(u);
}

// compare u and v

int
mcmp(uint32_t *u, uint32_t *v)
{
	int i;
	mnorm(u);
	mnorm(v);
	if (MLENGTH(u) < MLENGTH(v))
		return -1;
	if (MLENGTH(u) > MLENGTH(v))
		return 1;
	for (i = MLENGTH(u) - 1; i >= 0; i--) {
		if (u[i] < v[i])
			return -1;
		if (u[i] > v[i])
			return 1;
	}
	return 0; // u = v
}

// convert unsigned to bignum

uint32_t *
mint(uint32_t n)
{
	uint32_t *p;
	p = mnew(1);
	p[0] = n;
	return p;
}

uint32_t *
mnew(int n)
{
	uint32_t *u;
	u = (uint32_t *) malloc((n + 1) * sizeof (uint32_t));
	if (u == NULL)
		malloc_kaput();
	bignum_count++;
	*u = n;
	return u + 1;
}

void
mfree(uint32_t *u)
{
	free(u - 1);
	bignum_count--;
}

uint32_t *
mcopy(uint32_t *u)
{
	int i;
	uint32_t *v;
	v = mnew(MLENGTH(u));
	for (i = 0; i < MLENGTH(u); i++)
		v[i] = u[i];
	return v;
}

// remove leading zeroes

void
mnorm(uint32_t *u)
{
	while (MLENGTH(u) > 1 && u[MLENGTH(u) - 1] == 0)
		MLENGTH(u)--;
}

// Bignum GCD
// Uses the binary GCD algorithm.
// See "The Art of Computer Programming" p. 338.
// mgcd always returns a positive value
// mgcd(0, 0) = 0
// mgcd(u, 0) = |u|
// mgcd(0, v) = |v|

uint32_t *
mgcd(uint32_t *u, uint32_t *v)
{
	int i, k, n, sign;
	uint32_t *t;

	if (MZERO(u)) {
		t = mcopy(v);
		return t;
	}

	if (MZERO(v)) {
		t = mcopy(u);
		return t;
	}

	u = mcopy(u);
	v = mcopy(v);

	k = 0;

	while ((u[0] & 1) == 0 && (v[0] & 1) == 0) {
		mshiftright(u);
		mshiftright(v);
		k++;
	}

	if (u[0] & 1) {
		t = mcopy(v);
		sign = -1;
	} else {
		t = mcopy(u);
		sign = 1;
	}

	while (1) {

		while ((t[0] & 1) == 0)
			mshiftright(t);

		if (sign == 1) {
			mfree(u);
			u = mcopy(t);
		} else {
			mfree(v);
			v = mcopy(t);
		}

		mfree(t);

		if (mcmp(u, v) < 0) {
			t = msub(v, u);
			sign = -1;
		} else {
			t = msub(u, v);
			sign = 1;
		}

		if (MZERO(t)) {
			mfree(t);
			mfree(v);
			n = (k / 32) + 1;
			v = mnew(n);
			for (i = 0; i < n; i++)
				v[i] = 0;
			msetbit(v, k);
			t = mmul(u, v);
			mfree(u);
			mfree(v);
			return t;
		}
	}
}

// returns x^n mod m

uint32_t *
mmodpow(uint32_t *x, uint32_t *n, uint32_t *m)
{
	uint32_t *y, *z;
	x = mcopy(x);
	n = mcopy(n);
	y = mint(1);
	while (1) {
		if (n[0] & 1) {
			z = mmul(y, x);
			mfree(y);
			y = mmod(z, m);
			mfree(z);
		}
		mshiftright(n);
		if (MZERO(n))
			break;
		z = mmul(x, x);
		mfree(x);
		x = mmod(z, m);
		mfree(z);
	}
	mfree(x);
	mfree(n);
	return y;
}

// returns NULL if not perfect root, otherwise returns a^(1/n)

uint32_t *
mroot(uint32_t *a, uint32_t *n)
{
	int i, j, k;
	uint32_t *b, *c, m;

	if (MLENGTH(n) > 1 || n[0] == 0)
		return NULL;

	// k is bit length of a

	k = 32 * (MLENGTH(a) - 1);

	m = a[MLENGTH(a) - 1];

	while (m) {
		m >>= 1;
		k++;
	}

	if (k == 0)
		return mint(0);

	// initial guess of index of ms bit in result

	k = (k - 1) / n[0];

	j = k / 32 + 1; // k is bit index, not number of bits

	b = mnew(j);

	for (i = 0; i < j; i++)
		b[i] = 0;

	while (k >= 0) {
		msetbit(b, k);
		mnorm(b);
		c = mpow(b, n);
		switch (mcmp(c, a)) {
		case -1:
			break;
		case 0:
			mfree(c);
			return b;
		case 1:
			mclrbit(b, k);
			break;
		}
		mfree(c);
		k--;
	}

	mfree(b);

	return NULL;
}

// Bignum prime test (returns 1 if prime, 0 if not)
// Uses Algorithm P (probabilistic primality test) from p. 395 of
// "The Art of Computer Programming, Volume 2" by Donald E. Knuth.

int
mprime(uint32_t *n)
{
	int i, k;
	uint32_t *q;

	// 1?

	if (MLENGTH(n) == 1 && n[0] == 1)
		return 0;

	// 2?

	if (MLENGTH(n) == 1 && n[0] == 2)
		return 1;

	// even?

	if ((n[0] & 1) == 0)
		return 0;

	// n = 1 + (2 ^ k) q

	q = mcopy(n);

	k = 0;
	do {
		mshiftright(q);
		k++;
	} while ((q[0] & 1) == 0);

	// try 25 times

	for (i = 0; i < 25; i++)
		if (mprimef(n, q, k) == 0)
			break;

	mfree(q);

	if (i < 25)
		return 0;
	else
		return 1;
}

//	This is the actual implementation of Algorithm P.
//
//	Input:		n		The number in question.
//
//			q		n = 1 + (2 ^ k) q
//
//			k
//
//	Output:		1		when n is probably prime
//
//			0		when n is definitely not prime

int
mprimef(uint32_t *n, uint32_t *q, int k)
{
	int i, j;
	uint32_t *t, *x, *y;

	// generate x

	t = mcopy(n);

	while (1) {
		for (i = 0; i < MLENGTH(t); i++)
			t[i] = rand();
		x = mmod(t, n);
		if (!MZERO(x) && !MEQUAL(x, 1))
			break;
		mfree(x);
	}

	mfree(t);

	// exponentiate

	y = mmodpow(x, q, n);

	// done?

	if (MEQUAL(y, 1)) {
		mfree(x);
		mfree(y);
		return 1;
	}

	j = 0;

	while (1) {

		// y = n - 1?

		t = msub(n, y);

		if (MEQUAL(t, 1)) {
			mfree(t);
			mfree(x);
			mfree(y);
			return 1;
		}

		mfree(t);

		if (++j == k) {
			mfree(x);
			mfree(y);
			return 0;
		}

		// y = (y ^ 2) mod n

		t = mmul(y, y);
		mfree(y);
		y = mmod(t, n);
		mfree(t);

		// y = 1?

		if (MEQUAL(y, 1)) {
			mfree(x);
			mfree(y);
			return 0;
		}
	}
}
