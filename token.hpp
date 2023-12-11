#ifndef __TOKEN__
#define __TOKEN__

#include <string>
#include <memory>

enum TOKEN_TYPE {
	T_CONSTANT,
	T_SYMBOL,
	T_OPERATOR,
	T_LEFT_PAREN,  // left parenthesis
	T_RIGHT_PAREN, // right parenthesis
	T_FUNCTION,
};

enum FUNCTION {
	SINH,
	COSH,
	TANH,
	SIN,
	COS,
	TAN,
	ASINH,
	ACOSH,
	ATANH,
	ASIN,
	ACOS,
	ATAN,
	EXP,
	LOG,
	SQRT,
	ABS,
	CEIL,
	FLOOR,
};

struct token {
	int pos;
	TOKEN_TYPE type;

	virtual std::string to_string() = 0;
	token(int p, TOKEN_TYPE t) : pos(p), type(t) {}
	virtual ~token() {}
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

struct t_function : token {
	FUNCTION f;
	std::string name;
	t_function(int p, std::string n, FUNCTION f) : token(p, T_FUNCTION), f(f), name(n) {}
	virtual std::string to_string() { return name; }
};

using p_token = std::shared_ptr<token>;
using p_constant = std::shared_ptr<t_constant>;
using p_symbol = std::shared_ptr<t_symbol>;
using p_operator = std::shared_ptr<t_operator>;
using p_left_paren = std::shared_ptr<t_left_paren>;
using p_right_paren = std::shared_ptr<t_right_paren>;
using p_function = std::shared_ptr<t_function>;

#endif