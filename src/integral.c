#include "defs.h"

char *itab[] = {

	"a",
	"a x",
	"1",

	"1 / x",
	"log(x)",
	"1",

	"x^a",			// integrand
	"x^(a + 1) / (a + 1)",	// answer
	"not(a = -1)",		// condition

	"exp(a x)",
	"exp(a x) / a",
	"1",

	"exp(a x + b)",
	"exp(a x + b) / a",
	"1",

	"x exp(a x^2)",
	"exp(a x^2) / (2 a)",
	"1",

	"x exp(a x^2 + b)",
	"exp(a x^2 + b) / (2 a)",
	"1",

	"log(a x)",
	"x log(a x) - x",
	"1",

	"a^x",
	"a^x / log(a)",
	"or(not(number(a)),a>0)",

	"1 / (a + x^2)",
	"arctan(x / sqrt(a)) / sqrt(a)",
	"or(not(number(a)),a>0)",

	"1 / (a - x^2)",
	"arctanh(x / sqrt(a)) / sqrt(a)",
	"1",

	"1 / sqrt(a - x^2)",
	"arcsin(x / sqrt(a))",
	"1",

	"1 / sqrt(a + x^2)",
	"log(x + sqrt(a + x^2))",
	"1",

	"1 / (a + b x)",
	"log(a + b x) / b",
	"1",

	"1 / ((a + b x)^2)",
	"-1 / (b (a + b x))",
	"1",

	"1 / ((a + b x)^3)",
	"-1 / ((2 b) ((a + b x)^2))",
	"1",

	"x / (a + b x)",
	"x / b - a log(a + b x) / (b b)",
	"1",

	"x / ((a + b x)^2)",
	"(log(a + b x) + a / (a + b x)) / (b^2)",
	"1",

	"x^2 / (a + b x)",
	"(1/2 (a + b x)^2 - 2 a (a + b x) + a^2 log(a + b x)) / (b^2)",
	"1",

	"x^2 / ((a + b x)^2)",
	"(a + b x - 2 a log(a + b x) - a^2 / (a + b x)) / (b^3)",
	"1",

	"x^2 / ((a + b x)^3)",
	"(log(a + b x) + 2 a / (a + b x) - a^2 / (2 ((a + b x)^2))) / (b^3)",
	"1",

	"1 / (x (a + b x))",
	"-log((a + b x) / x) / a",
	"1",

	"1 / (x ((a + b x)^2))",
	"1 / (a (a + b x)) - log((a + b x) / x) / (a^2)",
	"1",

	"1 / (x ((a + b x)^3))",
	"(1/2 ((2 a + b x) / (a + b x))^2 + log(x / (a + b x))) / (a^3)",
	"1",

	"1 / ((x^2) (a + b x))",
	"-1 / (a x) + b log((a + b x) / x) / (a^2)",
	"1",

	"1 / ((x^3) (a + b x))",
	"(2 b x - a) / (2 a^2 x^2) + b^2 log(x / (a + b x)) / (a^3)",
	"1",

	"1 / ((x^2) ((a + b x)^2))",
	"-(a + 2 b x) / (a^2 x (a + b x)) + 2 b log((a + b x) / x) / (a^3)",
	"1",

	"1 / (a + b x^2)",
	"arctan(x sqrt(a b) / a) / sqrt(a b)",
	"or(not(number(a b)),testgt(a b,0))",

	"1 / (a + b x^2)",
	"log((a + x sqrt(-a b)) / (a - x sqrt(-a b))) / (2 sqrt(-a b))",
	"or(not(number(a b)),testlt(a b,0))",

	"x / (a + b x^2)",
	"1 log(a + b x^2) / (2 b)",
	"1",

	"x^2 / (a + b x^2)",
	"x / b - a integral(1 / (a + b x^2),x) / b",
	"1",

	"1 / ((a + b x^2)^2)",
	"x / (2 a (a + b x^2)) + 1 integral(1 / (a + b x^2),x) / (2 a)",
	"1",

	"1 / (x (a + b x^2))",
	"1 log(x^2 / (a + b x^2)) / (2 a)",
	"1",

	"1 / ((x^2) (a + b x^2))",
	"-1 / (a x) - b integral(1 / (a + b x^2),x) / a",
	"1",

	"1 / (a + b x^3)",
	"1 (a / b)^(1/3) (1/2 log(((a / b)^(1/3) + x)^3 / (a + b x^3)) + sqrt(3) arctan((2 x - (a / b)^(1/3)) / ((a / b)^(1/3) sqrt(3)))) / (3 a)",
	"1",

	"x^2 / (a + b x^3)",
	"1 log(a + b x^3) / (3 b)",
	"1",

	"1 / (a + b x^4)",
	"1 (a 1/4 / b)^(1/4) (1/2 log((x^2 + 2 (a 1/4 / b)^(1/4) x + 2 (a 1/4 / b)^(1/2)) / (x^2 - 2 (a 1/4 / b)^(1/4) x + 2 (a 1/4 / b)^(1/2))) + arctan(2 (a 1/4 / b)^(1/4) x / (2 (a 1/4 / b)^(1/2) - x^2))) / (2 a)",
	"or(not(number(a b)),testgt(a b,0))",

	"1 / (a + b x^4)",
	"(-a / b)^(1/4) (1/2 log((x + (-a / b)^(1/4)) / (x - (-a / b)^(1/4))) + arctan(x / ((-a / b)^(1/4)))) / (2 a)",
	"or(not(number(a b)),testlt(a b,0))",

	"x / (a + b x^4)",
	"sqrt(b / a) arctan(x^2 sqrt(b / a)) / (2 b)",
	"or(not(number(a b)),testgt(a b,0))",

	"x / (a + b x^4)",
	"sqrt(-b / a) log((x^2 - sqrt(-a / b)) / (x^2 + sqrt(-a / b))) / (4 b)",
	"or(not(number(a b)),testlt(a b,0))",

	"x^2 / (a + b x^4)",
	"1 (1/2 log((x^2 - 2 (a 1/4 / b)^(1/4) x + 2 sqrt(a 1/4 / b)) / (x^2 + 2 (a 1/4 / b)^(1/4) x + 2 sqrt(a 1/4 / b))) + arctan(2 (a 1/4 / b)^(1/4) x / (2 sqrt(a 1/4 / b) - x^2))) / (4 b (a 1/4 / b)^(1/4))",
	"or(not(number(a b)),testgt(a b,0))",

	"x^2 / (a + b x^4)",
	"1 (log((x - (-a / b)^(1/4)) / (x + (-a / b)^(1/4))) + 2 arctan(x / ((-a / b)^(1/4)))) / (4 b (-a / b)^(1/4))",
	"or(not(number(a b)),testlt(a b,0))",

	"x^3 / (a + b x^4)",
	"1 log(a + b x^4) / (4 b)",
	"1",

	"sqrt(a + b x)",
	"2 1 sqrt((a + b x)^3) / (3 b)",
	"1",

	"x sqrt(a + b x)",
	"-2 (2 a - 3 b x) sqrt((a + b x)^3) 1/15 / (b^2)",
	"1",

	"x^2 sqrt(a + b x)",
	"2 (8 a^2 - 12 a b x + 15 b^2 x^2) sqrt((a + b x)^3) 1/105 / (b^3)",
	"1",

	"sqrt(a + b x) / x",
	"2 sqrt(a + b x) + a integral(1 / (x sqrt(a + b x)),x)",
	"1",

	"sqrt(a + b x) / (x^2)",
	"-sqrt(a + b x) / x + b 1/2 integral(1 / (x sqrt(a + b x)),x)",
	"1",

	"1 / sqrt(a + b x)",
	"2 sqrt(a + b x) / b",
	"1",

	"x / sqrt(a + b x)",
	"-2 (2 a - b x) sqrt(a + b x) / (3 (b^2))",
	"1",

	"x^2 / sqrt(a + b x)",
	"2 (8 a^2 - 4 a b x + 3 b^2 x^2) sqrt(a + b x) / (15 (b^3))",
	"1",

	"1 / (x sqrt(a + b x))",
	"log((sqrt(a + b x) - sqrt(a)) / (sqrt(a + b x) + sqrt(a))) / sqrt(a)",
	"or(not(number(a)),testgt(a,0))",

	"1 / (x sqrt(a + b x))",
	"2 arctan(sqrt(-(a + b x) / a)) / sqrt(-a)",
	"or(not(number(a)),testlt(a,0))",

	"1 / ((x^2) sqrt(a + b x))",
	"-sqrt(a + b x) / (a x) - b integral(1 / (x sqrt(a + b x)),x) / (2 a)",
	"1",

	"sqrt(x^2 + a)",
	"1/2 (x sqrt(x^2 + a) + a log(x + sqrt(x^2 + a)))",
	"1",

	"1 / sqrt(x^2 + a)",
	"log(x + sqrt(x^2 + a))",
	"1",

	"1 / (x sqrt(x^2 + a))",
	"arcsec(x / sqrt(-a)) / sqrt(-a)",
	"or(not(number(a)),testlt(a,0))",

	"1 / (x sqrt(x^2 + a))",
	"-log((sqrt(a) + sqrt(x^2 + a)) / x) / sqrt(a)",
	"or(not(number(a)),testgt(a,0))",

	"sqrt(x^2 + a) / x",
	"sqrt(x^2 + a) - sqrt(a) log((sqrt(a) + sqrt(x^2 + a)) / x)",
	"or(not(number(a)),testgt(a,0))",

	"sqrt(x^2 + a) / x",
	"sqrt(x^2 + a) - sqrt(-a) arcsec(x / sqrt(-a))",
	"or(not(number(a)),testlt(a,0))",

	"x / sqrt(x^2 + a)",
	"sqrt(x^2 + a)",
	"1",

	"x sqrt(x^2 + a)",
	"1/3 sqrt((x^2 + a)^3)",
	"1",

	"sqrt(a + x^6 + 3 a^(1/3) x^4 + 3 a^(2/3) x^2)",
	"1/4 (x sqrt((x^2 + a^(1/3))^3) + 3/2 a^(1/3) x sqrt(x^2 + a^(1/3)) + 3/2 a^(2/3) log(x + sqrt(x^2 + a^(1/3))))",
	"1",

	"sqrt(-a + x^6 - 3 a^(1/3) x^4 + 3 a^(2/3) x^2)",
	"1/4 (x sqrt((x^2 - a^(1/3))^3) - 3/2 a^(1/3) x sqrt(x^2 - a^(1/3)) + 3/2 a^(2/3) log(x + sqrt(x^2 - a^(1/3))))",
	"1",

	"1 / sqrt((x^2 + a)^3)",
	"x / (a sqrt(x^2 + a))",
	"and(number(a),testgt(a,0))",

	"1 / sqrt((x^2 + a)^3)",
	"-x / (a sqrt(x^2 + a))",
	"and(number(a),testlt(a,0))",

	"x / sqrt((x^2 + a)^3)",
	"-1 / sqrt(x^2 + a)",
	"1",

	"x sqrt(a + x^6 + 3 a^(1/3) x^4 + 3 a^(2/3) x^2)",
	"1/5 sqrt((x^2 + a^(1/3))^5)",
	"1",

	"x^2 sqrt(x^2 + a)",
	"1/4 x sqrt((x^2 + a)^3) - 1/8 a x sqrt(x^2 + a) - 1/8 a^2 log(x + sqrt(x^2 + a))",
	"1",

	"x^3 sqrt(x^2 + a)",
	"(1/5 x^2 - 2/15 a) sqrt((x^2 + a)^3)",
	"and(number(a),testgt(a,0))",

	"x^3 sqrt(x^2 + a)",
	"sqrt((x^2 + a)^5) 1/5 - a sqrt((x^2 + a)^3) 1/3",
	"and(number(a),testlt(a,0))",

	"x^2 / sqrt(x^2 + a)",
	"1/2 x sqrt(x^2 + a) - 1/2 a log(x + sqrt(x^2 + a))",
	"1",

	"x^3 / sqrt(x^2 + a)",
	"1/3 sqrt((x^2 + a)^3) - a sqrt(x^2 + a)",
	"1",

	"1 / ((x^2) sqrt(x^2 + a))",
	"-sqrt(x^2 + a) / (a x)",
	"1",

	"1 / ((x^3) sqrt(x^2 + a))",
	"-sqrt(x^2 + a) / (2 a (x^2)) + log((sqrt(a) + sqrt(x^2 + a)) / x) / (2 (a^(3/2)))",
	"or(not(number(a)),testgt(a,0))",

	"1 / ((x^3) sqrt(x^2 - a))",
	"sqrt(x^2 - a) / (2 a (x^2)) + 1 arcsec(x / (a^(1/2))) / (2 (a^(3/2)))",
	"or(not(number(a)),testgt(a,0))",

	"x^2 sqrt(a + x^6 + 3 a^(1/3) x^4 + 3 a^(2/3) x^2)",
	"1/6 x sqrt((x^2 + a^(1/3))^5) - 1/24 a^(1/3) x sqrt((x^2 + a^(1/3))^3) - 1/16 a^(2/3) x sqrt(x^2 + a^(1/3)) - 1/16 a log(x + sqrt(x^2 + a^(1/3)))",
	"or(not(number(a)),testgt(a,0))",

	"x^2 sqrt(-a - 3 a^(1/3) x^4 + 3 a^(2/3) x^2 + x^6)",
	"1/6 x sqrt((x^2 - a^(1/3))^5) + 1/24 a^(1/3) x sqrt((x^2 - a^(1/3))^3) - 1/16 a^(2/3) x sqrt(x^2 - a^(1/3)) + 1/16 a log(x + sqrt(x^2 - a^(1/3)))",
	"or(not(number(a)),testgt(a,0))",

	"x^3 sqrt(a + x^6 + 3 a^(1/3) x^4 + 3 a^(2/3) x^2)",
	"1/7 sqrt((x^2 + a^(1/3))^7) - 1/5 a^(1/3) sqrt((x^2 + a^(1/3))^5)",
	"or(not(number(a)),testgt(a,0))",

	"x^3 sqrt(-a - 3 a^(1/3) x^4 + 3 a^(2/3) x^2 + x^6)",
	"1/7 sqrt((x^2 - a^(1/3))^7) + 1/5 a^(1/3) sqrt((x^2 - a^(1/3))^5)",
	"or(not(number(a)),testgt(a,0))",

	"1 / ((x - a) sqrt(x^2 - a^2))",
	"-sqrt(x^2 - a^2) / (a (x - a))",
	"1",

	"1 / ((x + a) sqrt(x^2 - a^2))",
	"sqrt(x^2 - a^2) / (a (x + a))",
	"1",

	"sqrt(a - x^2)",
	"1/2 (x sqrt(a - x^2) + a arcsin(x / sqrt(abs(a))))",
	"1",

	"1 / (x sqrt(a - x^2))",
	"-log((sqrt(a) + sqrt(a - x^2)) / x) / sqrt(a)",
	"or(not(number(a)),testgt(a,0))",

	"sqrt(a - x^2) / x",
	"sqrt(a - x^2) - sqrt(a) log((sqrt(a) + sqrt(a - x^2)) / x)",
	"or(not(number(a)),testgt(a,0))",

	"x / sqrt(a - x^2)",
	"-sqrt(a - x^2)",
	"1",

	"x sqrt(a - x^2)",
	"-1/3 sqrt((a - x^2)^3)",
	"1",

	"x^2 sqrt(a - x^2)",
	"-x 1/4 sqrt((a - x^2)^3) + 1/8 a (x sqrt(a - x^2) + a arcsin(x / sqrt(a)))",
	"or(not(number(a)),testgt(a,0))",

	"x^3 sqrt(a - x^2)",
	"(-1/5 x^2 - 2/15 a) sqrt((a - x^2)^3)",
	"or(not(number(a)),testgt(a,0))",

	"x^2 / sqrt(a - x^2)",
	"-x 1/2 sqrt(a - x^2) + a 1/2 arcsin(x / sqrt(a))",
	"or(not(number(a)),testgt(a,0))",

	"1 / ((x^2) sqrt(a - x^2))",
	"-sqrt(a - x^2) / (a x)",
	"or(not(number(a)),testgt(a,0))",

	"sqrt(a - x^2) / (x^2)",
	"-sqrt(a - x^2) / x - arcsin(x / sqrt(a))",
	"or(not(number(a)),testgt(a,0))",

	"sqrt(a - x^2) / (x^3)",
	"-sqrt(a - x^2) / (2 (x^2)) + log((sqrt(a) + sqrt(a - x^2)) / x) / (2 sqrt(a))",
	"or(not(number(a)),testgt(a,0))",

	"sqrt(a - x^2) / (x^4)",
	"-sqrt((a - x^2)^3) / (3 a (x^3))",
	"or(not(number(a)),testgt(a,0))",

	"sqrt(a x^2 + b)",
	"x sqrt(a x^2 + b) 1/2 + b log(x sqrt(a) + sqrt(a x^2 + b)) 1/2 / sqrt(a)",
	"and(number(a),testgt(a,0))",

	"sqrt(a x^2 + b)",
	"x sqrt(a x^2 + b) 1/2 + b arcsin(x sqrt(-a / b)) 1/2 / sqrt(-a)",
	"and(number(a),testlt(a,0))",

	"sin(a x)",
	"-cos(a x) / a",
	"1",

	"cos(a x)",
	"sin(a x) / a",
	"1",

	"tan(a x)",
	"-log(cos(a x)) / a",
	"1",

	"1 / tan(a x)",
	"log(sin(a x)) / a",
	"1",

	"1 / cos(a x)",
	"log(tan(pi 1/4 + a x 1/2)) / a",
	"1",

	"1 / sin(a x)",
	"log(tan(a x 1/2)) / a",
	"1",

	"sin(a x)^2",
	"x 1/2 - sin(2 a x) / (4 a)",
	"1",

	"sin(a x)^3",
	"-cos(a x) (sin(a x)^2 + 2) / (3 a)",
	"1",

	"sin(a x)^4",
	"3/8 x - sin(2 a x) / (4 a) + sin(4 a x) / (32 a)",
	"1",

	"cos(a x)^2",
	"x 1/2 + sin(2 a x) / (4 a)",
	"1",

	"cos(a x)^3",
	"sin(a x) (cos(a x)^2 + 2) / (3 a)",
	"1",

	"cos(a x)^4",
	"3/8 x + sin(2 a x) / (4 a) + sin(4 a x) / (32 a)",
	"1",

	"1 / (sin(a x)^2)",
	"-1 / (a tan(a x))",
	"1",

	"1 / (cos(a x)^2)",
	"tan(a x) / a",
	"1",

	"sin(a x) cos(a x)",
	"sin(a x)^2 / (2 a)",
	"1",

	"sin(a x)^2 cos(a x)^2",
	"-sin(4 a x) / (32 a) + x 1/8",
	"1",

	"sin(a x) / (cos(a x)^2)",
	"1 / (a cos(a x))",
	"1",

	"sin(a x)^2 / cos(a x)",
	"(log(tan(pi 1/4 + a x 1/2)) - sin(a x)) / a",
	"1",

	"cos(a x) / (sin(a x)^2)",
	"-1 / (a sin(a x))",
	"1",

	"1 / (sin(a x) cos(a x))",
	"log(tan(a x)) / a",
	"1",

	"1 / (sin(a x) cos(a x)^2)",
	"(1 / cos(a x) + log(tan(a x 1/2))) / a",
	"1",

	"1 / (sin(a x)^2 cos(a x))",
	"(log(tan(pi 1/4 + a x 1/2)) - 1 / sin(a x)) / a",
	"1",

	"1 / (sin(a x)^2 cos(a x)^2)",
	"-2 / (a tan(2 a x))",
	"1",

	"sin(a + b x)",
	"-cos(a + b x) / b",
	"1",

	"cos(a + b x)",
	"sin(a + b x) / b",
	"1",

	"1 / (b + b sin(a x))",
	"-tan(pi 1/4 - a x 1/2) / (a b)",
	"1",

	"1 / (b - b sin(a x))",
	"tan(pi 1/4 + a x 1/2) / (a b)",
	"1",

	"1 / (b + b cos(a x))",
	"tan(a x 1/2) / (a b)",
	"1",

	"1 / (b - b cos(a x))",
	"-1 / (tan(a x 1/2) a b)",
	"1",

	"1 / (a + b sin(x))",
	"log((a tan(x 1/2) + b - sqrt(b^2 - a^2)) / (a tan(x 1/2) + b + sqrt(b^2 - a^2))) / sqrt(b^2 - a^2)",
	"b^2 - a^2",

	"1 / (a + b cos(x))",
	"log((sqrt(b^2 - a^2) tan(x 1/2) + a + b) / (sqrt(b^2 - a^2) tan(x 1/2) - a - b)) / sqrt(b^2 - a^2)",
	"b^2 - a^2",

	"x sin(a x)",
	"sin(a x) / (a^2) - x cos(a x) / a",
	"1",

	"x^2 sin(a x)",
	"2 x sin(a x) / (a^2) - (a^2 x^2 - 2) cos(a x) / (a^3)",
	"1",

	"x cos(a x)",
	"cos(a x) / (a^2) + x sin(a x) / a",
	"1",

	"x^2 cos(a x)",
	"2 x cos(a x) / (a^2) + (a^2 x^2 - 2) sin(a x) / (a^3)",
	"1",

	"arcsin(a x)",
	"x arcsin(a x) + sqrt(1 - a^2 x^2) / a",
	"1",

	"arccos(a x)",
	"x arccos(a x) + sqrt(1 - a^2 x^2) / a",
	"1",

	"arctan(a x)",
	"x arctan(a x) - log(1 + a^2 x^2) / (2 a)",
	"1",

	"log(a x)",
	"x log(a x) - x",
	"1",

	"x log(a x)",
	"x^2 log(a x) 1/2 - x^2 1/4",
	"1",

	"x^2 log(a x)",
	"x^3 log(a x) 1/3 - 1/9 x^3",
	"1",

	"log(x)^2",
	"x log(x)^2 - 2 x log(x) + 2 x",
	"1",

	"1 / (x (a + log(x)))",
	"log(a + log(x))",
	"1",

	"log(a x + b)",
	"(a x + b) log(a x + b) / a - x",
	"1",

	"log(a x + b) / (x^2)",
	"a log(x) / b - (a x + b) log(a x + b) / (b x)",
	"1",

	"sinh(x)",
	"cosh(x)",
	"1",

	"cosh(x)",
	"sinh(x)",
	"1",

	"tanh(x)",
	"log(cosh(x))",
	"1",

	"x sinh(x)",
	"x cosh(x) - sinh(x)",
	"1",

	"x cosh(x)",
	"x sinh(x) - cosh(x)",
	"1",

	"sinh(x)^2",
	"sinh(2 x) 1/4 - x 1/2",
	"1",

	"tanh(x)^2",
	"x - tanh(x)",
	"1",

	"cosh(x)^2",
	"sinh(2 x) 1/4 + x 1/2",
	"1",

	"x^3 exp(a x^2)",
	"exp(a x^2) (x^2 / a - 1 / (a^2)) 1/2",
	"1",

	"x^3 exp(a x^2 + b)",
	"exp(a x^2) exp(b) (x^2 / a - 1 / (a^2)) 1/2",
	"1",

	"exp(a x^2)",
	"-i sqrt(pi) erf(i sqrt(a) x) 1/2 / sqrt(a)",
	"1",

	"erf(a x)",
	"x erf(a x) + exp(-a^2 x^2) / (a sqrt(pi))",
	"1",

	"x^2 (1 - x^2)^(3/2)",
	"(x sqrt(1 - x^2) (-8 x^4 + 14 x^2 - 3) + 3 arcsin(x)) 1/48",
	"1",

	"x^2 (1 - x^2)^(5/2)",
	"(x sqrt(1 - x^2) (48 x^6 - 136 x^4 + 118 x^2 - 15) + 15 arcsin(x)) 1/384",
	"1",

	"x^4 (1 - x^2)^(3/2)",
	"(-x sqrt(1 - x^2) (16 x^6 - 24 x^4 + 2 x^2 + 3) + 3 arcsin(x)) 1/128",
	"1",

	"x exp(a x)",
	"exp(a x) (a x - 1) / (a^2)",
	"1",

	"x exp(a x + b)",
	"exp(a x + b) (a x - 1) / (a^2)",
	"1",

	"x^2 exp(a x)",
	"exp(a x) (a^2 x^2 - 2 a x + 2) / (a^3)",
	"1",

	"x^2 exp(a x + b)",
	"exp(a x + b) (a^2 x^2 - 2 a x + 2) / (a^3)",
	"1",

	"x^3 exp(a x)",
	"exp(a x) x^3 / a - 3 integral(x^2 exp(a x),x) / a",
	"1",

	"x^3 exp(a x + b)",
	"exp(a x + b) x^3 / a - 3 integral(x^2 exp(a x + b),x) / a",
	"1",

	"cos(x)^2 sin(x)",
	"-cos(x)^3 1/3",
	"1",

	NULL,
};

void
eval_integral(void)
{
	int i, n;

	// 1st arg

	p1 = cdr(p1);
	push(car(p1));
	eval();

	// check for single arg

	p1 = cdr(p1);

	if (!iscons(p1)) {
		guess();
		integral();
		return;
	}

	while (iscons(p1)) {

		// next arg should be a symbol

		push(car(p1)); // have to eval in case of $METAX
		eval();
		p2 = pop();

		if (!issymbol(p2))
			stop("integral: symbol expected");

		p1 = cdr(p1);

		// if next arg is a number then use it

		n = 1;

		if (isnum(car(p1))) {
			push(car(p1));
			n = pop_integer();
			if (n < 1)
				stop("nth integral: check n");
			p1 = cdr(p1);
		}

		for (i = 0; i < n; i++) {
			push(p2);
			integral();
		}
	}
}

#undef F
#undef X
#undef I
#undef A
#undef C

#define F p3
#define X p4
#define I p5
#define A p6
#define C p7

void
integral(void)
{
	save();

	X = pop();
	F = pop();

	if (car(F) == symbol(ADD))
		integral_of_sum();
	else if (car(F) == symbol(MULTIPLY))
		integral_of_product();
	else
		integral_of_form();

	restore();
}

void
integral_of_sum(void)
{
	int h = tos;

	p1 = cdr(F);

	while (iscons(p1)) {
		push(car(p1));
		push(X);
		integral();
		p1 = cdr(p1);
	}

	add_terms(tos - h);
}

void
integral_of_product(void)
{
	push(F);
	push(X);
	partition_integrand();	// push const part then push var part
	F = pop();		// pop var part
	integral_of_form();
	multiply();		// multiply by const part
}

void
integral_of_form(void)
{
	int h;
	char **s;

	// save bindings in case eval(A) calls integral

	save_binding(symbol(METAA));
	save_binding(symbol(METAB));
	save_binding(symbol(METAX));

	set_binding(symbol(METAX), X);

	// put constants in F(X) on the stack

	h = tos;

	push(one); // 1 is a candidate for a or b

	push(F);
	push(X);
	collect_coeffs();
	push(X);
	decomp();

	s = itab;

	for (;;) {

		if (*s == NULL)
			stop("integral: could not find a solution");

		// integrand
		if (pratt_flag) {
			scan_with_pratt1(*s++);
		} else {
			scan1(*s++);
		}
		I = pop();

		// answer
		if (pratt_flag) {
			scan_with_pratt1(*s++);
		} else {
			scan1(*s++);
		}
		A = pop();

		// condition
		if (pratt_flag) {
			scan_with_pratt1(*s++);
		} else {
			scan1(*s++);
		}
		C = pop();

		if (find_integral(h))
			break;
	}

	tos = h; // pop all

	push(A); // answer
	eval();

	restore_binding(symbol(METAX));
	restore_binding(symbol(METAB));
	restore_binding(symbol(METAA));
}

// find constants such that F = I

int
find_integral(int h)
{
	int i, j;
	for (i = h; i < tos; i++) {
		set_binding(symbol(METAA), stack[i]);
		for (j = h; j < tos; j++) {
			set_binding(symbol(METAB), stack[j]);
			push(C);			// condition ok?
			eval();
			p1 = pop();
			if (iszero(p1))
				continue;		// no, go to next j
			push(F);			// F = I?
			push(I);
			eval();
			subtract();
			p1 = pop();
			if (iszero(p1))
				return 1;		// yes
		}
	}
	return 0;					// no
}

// returns constant expresions on the stack

void
decomp(void)
{
	save();
	decomp_nib();
	restore();
}

void
decomp_nib(void)
{
	p2 = pop(); // x
	p1 = pop(); // expr

	// is the entire expression constant?

	if (!find(p1, p2)) {
		push(p1);
		return;
	}

	// sum?

	if (car(p1) == symbol(ADD)) {
		decomp_sum();
		return;
	}

	// product?

	if (car(p1) == symbol(MULTIPLY)) {
		decomp_product();
		return;
	}

	// naive decomp if not sum or product

	p3 = cdr(p1);
	while (iscons(p3)) {
		push(car(p3));
		push(p2);
		decomp();
		p3 = cdr(p3);
	}
}

void
decomp_sum(void)
{
	int h;

	// decomp terms involving x

	p3 = cdr(p1);
	while (iscons(p3)) {
		if (find(car(p3), p2)) {
			push(car(p3));
			push(p2);
			decomp();
		}
		p3 = cdr(p3);
	}

	// add together all constant terms

	h = tos;
	p3 = cdr(p1);
	while (iscons(p3)) {
		if (!find(car(p3), p2))
			push(car(p3));
		p3 = cdr(p3);
	}

	if (tos - h) {
		add_terms(tos - h);
		p3 = pop();
		push(p3);
		push(p3);
		negate(); // need both +a, -a for some integrals
	}
}

void
decomp_product(void)
{
	int h;

	// decomp factors involving x

	p3 = cdr(p1);
	while (iscons(p3)) {
		if (find(car(p3), p2)) {
			push(car(p3));
			push(p2);
			decomp();
		}
		p3 = cdr(p3);
	}

	// multiply together all constant factors

	h = tos;
	p3 = cdr(p1);
	while (iscons(p3)) {
		if (!find(car(p3), p2))
			push(car(p3));
		p3 = cdr(p3);
	}

	if (tos - h)
		multiply_factors(tos - h);
}

// for example, exp(a x + b x) -> exp((a + b) x)

void
collect_coeffs(void)
{
	save();
	collect_coeffs_nib();
	restore();
}

void
collect_coeffs_nib(void)
{
	int h, i, j, n;
	struct atom **s;

	p2 = pop(); // x
	p1 = pop(); // expr

	if (!iscons(p1)) {
		push(p1);
		return;
	}

	h = tos;
	s = stack + tos;

	// depth first

	push(car(p1));
	p1 = cdr(p1);
	while (iscons(p1)) {
		push(car(p1));
		push(p2);
		collect_coeffs();
		p1 = cdr(p1);
	}
	list(tos - h);
	p1 = pop();

	if (car(p1) != symbol(ADD)) {
		push(p1);
		return;
	}

	// partition terms

	p1 = cdr(p1);

	while (iscons(p1)) {
		p3 = car(p1);
		if (car(p3) == symbol(MULTIPLY)) {
			push(p3);
			push(p2);
			partition_integrand();	// push const part then push var part
		} else if (find(p3, p2)) {
			push(one);		// const part
			push(p3);		// var part
		} else {
			push(p3);		// const part
			push(one);		// var part
		}
		p1 = cdr(p1);
	}

	// sort by var part

	n = tos - h;

	qsort(s, n / 2, 2 * sizeof (struct atom *), collect_coeffs_sort_func);

	// combine const parts of matching var parts

	for (i = 0; i < n - 2; i += 2) {
		if (equal(s[i + 1], s[i + 3])) {
			push(s[0]);
			push(s[2]);
			add();
			s[0] = pop();
			for (j = i + 2; j < n; j++)
				s[j] = s[j + 2];
			n -= 2;
			tos -= 2;
			i -= 2; // use the same index again
		}
	}

	// combine all the parts without expanding

	expanding = 0;

	n = tos - h;

	for (i = 0; i < n; i += 2) {
		push(s[i]);		// const part
		push(s[i + 1]);		// var part
		multiply();
		s[i / 2] = pop();
	}

	tos -= n / 2;

	add_terms(tos - h);

	expanding = 1;
}

int
collect_coeffs_sort_func(const void *q1, const void *q2)
{
	return cmp_terms(((struct atom **) q1)[1], ((struct atom **) q2)[1]);
}

void
partition_integrand(void)
{
	int h;

	save();

	p2 = pop(); // x
	p1 = pop(); // expr

	// push const part

	h = tos;
	p3 = cdr(p1);
	while (iscons(p3)) {
		if (!find(car(p3), p2))
			push(car(p3));
		p3 = cdr(p3);
	}

	if (h == tos)
		push(one);
	else
		multiply_factors(tos - h);

	// push var part

	h = tos;
	p3 = cdr(p1);
	while (iscons(p3)) {
		if (find(car(p3), p2))
			push(car(p3));
		p3 = cdr(p3);
	}

	if (h == tos)
		push(one);
	else
		multiply_factors(tos - h);

	restore();
}
