#ifndef __TOKEN__
#define __TOKEN__

#include <string>

enum TOKEN_TYPE {
	T_CONSTANT,
	T_SYMBOL,
	T_OPERATOR,
	T_LEFT_PAREN,
	T_RIGHT_PAREN,
	T_COMMA,
	T_FUNCTION,
};

enum FUNCTION_TYPE {
	F_1,
	F_2,
};

enum FUNCTION {
	SINH,
	COSH,
	TANH,
	SIN,
	COS,
	TAN,
	EXP,
	LN,
	SQRT,
	ABS,
	CEIL,
	FLOOR,

	LOG,
	MIN,
	MAX,
};

struct token {
	int pos;
	TOKEN_TYPE type;

	virtual std::string to_string() = 0;
	token(int p, TOKEN_TYPE t) : pos(p), type(t) {}
};

struct t_constant : token {
	double data;

	t_constant(int p, double d) : token(p, T_CONSTANT), data(d) {}
	virtual std::string to_string() { return std::to_string(data); }
};

struct t_symbol : token {
	t_symbol(int p) : token(p, T_SYMBOL) {}
	virtual std::string to_string() { return "x"; }
};

struct t_operator : token {
	char c;
	int priority;
	t_operator(int p, char _c) : token(p, T_OPERATOR), c(_c), priority() {
		if (_c == '+' || _c == '-') {
			priority = 0;
		}
		else if (_c == '*' || _c == '/') {
			priority = 1;
		}
		else if (_c == '^') {
			priority = 2;
		}
	}
	virtual std::string to_string() { return std::string() + c; }
};

struct t_left_paren : token {
	t_left_paren(int p) : token(p, T_LEFT_PAREN) {}
	virtual std::string to_string() { return "("; }
};

struct t_right_paren : token {
	t_right_paren(int p) : token(p, T_RIGHT_PAREN) {}
	virtual std::string to_string() { return ")"; }
};

struct t_comma : token {
	t_comma(int p) : token(p, T_COMMA) {}
	virtual std::string to_string() { return ","; }
};

struct t_function : token {
	FUNCTION_TYPE ftype;
	FUNCTION f;
	std::string name;
	t_function(int p, std::string n, FUNCTION f, FUNCTION_TYPE ft) : token(p, T_FUNCTION), ftype(ft), f(f), name(n) {}
	virtual std::string to_string() { return name; }
};

#endif