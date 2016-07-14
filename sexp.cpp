#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

struct _value;
typedef struct _value value_t;

typedef enum _tag { tNUMBER, tSTRING, tSYMBOL, tPAIR } tag_t;
typedef struct _pair {
	value_t *car;
	value_t *cdr;
} pair_t;
struct _value {
	tag_t tag;
	union {
		long num;
		const char *str;
		const char *sym;
		pair_t pair;
	};
};
void fatal (const char *msg)
{
	fprintf(stderr, "%s\n", msg);
	exit(1);
}
/*=================================================================*/
/* object maker */
/*=================================================================*/
#define balloc(type) (type*)malloc(sizeof(type))
value_t *mk_number (long num)
{
	value_t *val = balloc(value_t);
	val->tag = tNUMBER;
	val->num = num;
	return val;
}
value_t *mk_string (const char *s)
{
	value_t *val = balloc(value_t);
	val->tag = tSTRING;
	val->str = s;
	return val;
}
value_t *mk_symbol (const char *s)
{
	value_t *val = balloc(value_t);
	val->tag = tSYMBOL;
	val->sym = s;
	return val;
}
value_t *mk_pair (value_t *car, value_t *cdr)
{
	value_t *val = balloc(value_t);
	val->tag = tPAIR;
	val->pair.car = car;
	val->pair.cdr = cdr;
	return val;
}
/*=================================================================*/
/* typeof */ /* extracter */
/*=================================================================*/
bool is_null   (value_t *val) { return val == NULL; }
bool is_number (value_t *val) { return val->tag == tNUMBER; }
bool is_string (value_t *val) { return val->tag == tSTRING; }
bool is_symbol (value_t *val) { return val->tag == tSYMBOL; }
bool is_pair   (value_t *val) { return val->tag == tPAIR; }

long to_number (value_t *val) { return val->num; }
const char *to_string (value_t *val) { return val->str; }
const char *to_symbol (value_t *val) { return val->sym; }
pair_t *to_pair (value_t *val) { return &(val->pair); }
/*=================================================================*/
/* basic list OPs */
/*=================================================================*/
value_t *car (value_t *val) { return val->pair.car; }
value_t *cdr (value_t *val) { return val->pair.cdr; }
value_t *cons (value_t *car, value_t *cdr) { return mk_pair(car, cdr); }

value_t *set_car (value_t *val, value_t *car) { return val ? (val->pair.car=car) : car; }
value_t *set_cdr (value_t *val, value_t *cdr) { return val ? (val->pair.cdr=cdr) : cdr; }
/*=================================================================*/
/* display */
/*=================================================================*/
void display (value_t *val);
void show_list (value_t *val)
{
	display(car(val));
	if (!is_null(cdr(val))) {
		printf(" ");
		show_list(cdr(val));
	}
}
void display (value_t *val)
{
	if (is_null(val)) { printf("null"); return; }
	switch (val->tag) {
	case tNUMBER: printf("%ld",		to_number(val)); break;
	case tSTRING: printf("\"%s\"",	to_string(val)); break;
	case tSYMBOL: printf("%s",		to_symbol(val)); break;
	case tPAIR:   printf("(");show_list(val);printf(")");break;
				  do {
					  printf("(");
					  display(car(val)); printf(" "); display(cdr(val));
					  printf(")");
				  } while(0); break;
	default:      printf("?%p", val); break;
	}
}
/*=================================================================*/
/* reader */
/*=================================================================*/
#define BUFSZ 64
typedef value_t *(*Reader) (int c, FILE *in);
Reader readers[256] = {NULL};

value_t *R (FILE *in);
value_t *read_string (int c, FILE *in);
value_t *read_alpha (int c, FILE *in);
value_t *read_sign (int c, FILE *in);
value_t *read_blank (int c, FILE *in) { return NULL; }
value_t *read_digit (int c, FILE *in)
{
	int idx = 0;
	char buf[BUFSZ] = {'\0'};
	buf[idx++] = c;
	while ((c = getc(in)) > 0 &&
		   (read_digit == readers[c] || read_alpha == readers[c])) {
		buf[idx++] = c;
	}
	ungetc(c, in);
	return mk_number(atoi(buf));
}
value_t *read_alpha (int c, FILE *in)
{
	int idx = 0;
	char buf[BUFSZ] = {'\0'};
	buf[idx++] = c;
	while ((c = getc(in)) > 0 &&
		   (read_alpha == readers[c] || read_digit == readers[c] || read_sign == readers[c])) {
		buf[idx++] = c;
	}
	ungetc(c, in);
	return mk_symbol(strdup(buf));
}
value_t *read_sign (int c, FILE *in)
{
	int d = getc(in);
	ungetc(d, in);
	return (d > 0 && read_digit == readers[c]) ? read_digit(d, in) : read_alpha(d, in);
}
value_t *read_string (int c, FILE *in)
{
	int idx = 0;
	char buf[BUFSZ] = {'\0'};
	while ((c = getc(in)) > 0 && c != '"') {
		if ('\\' == (buf[idx++]=c))
			buf[idx++] = getc(in);
	}
	return mk_string(strdup(buf));
}
value_t *read_list (int d, FILE *in)
{
	int c;
	value_t *car = NULL, *cdr = NULL;
	while (isspace(c = getc(in)))
		/* nop */;
	if (c == ')') {
		return NULL;
	}
	ungetc(c, in); car = R(in);
	ungetc(d, in); cdr = R(in);
	return mk_pair(car, cdr);
}
value_t *read_semi (int c, FILE *in)
{
	while ((c = getc(in)) > 0 && c != '\n' && c != '\r')
		/* nop */;
	return NULL;
}
/*=================================================================*/
/* repl */
/*=================================================================*/
value_t *R (FILE *in)
{
	int c;
	value_t *cell;
    do {
		while (isspace((c = getc(in))))
			/* nop */;
		cell = readers[c](c, in);
	} while (0);
	return cell;
}
value_t *E (value_t *exp) { return exp; }
void P (value_t *val) { display(val); printf("\n"); }
void repl (FILE *in)
{
loop:
	printf("> "); fflush(stdout);
	P(E(R(in)));  fflush(stdin);
	goto loop;
}
/*=================================================================*/
/* main */
/*=================================================================*/
void init_readers (Reader r, const char *s)
{
	while (*s) {
		readers[*(s++)] = r;
	}
}
int main (int argc, char **argv)
{
	init_readers(read_blank, " \n\t\v\f\r");
	init_readers(read_digit, "0123456789");
	init_readers(read_alpha, "abcdefghijklmnopqrstuvwxyz");
	init_readers(read_alpha, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	init_readers(read_list,  "(");

	init_readers(read_string,"\"");
	init_readers(read_semi,  ";");

	repl(stdin);
	return 0;
}
