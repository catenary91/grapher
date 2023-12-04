#include "parser.hpp"

struct f_data {
	std::string name;
	FUNCTION_TYPE ftype;
	FUNCTION f;

	f_data(const std::string n, FUNCTION f, FUNCTION_TYPE ft) : name(n), ftype(ft), f(f) {}
	bool match(const std::string& s, int pos) const {
		return name.compare(0, name.length(), s, pos, name.length()) == 0;
	}
};

const f_data FUNCTIONS[] = {
	f_data("sinh", SINH, F_1),
	f_data("cosh", COSH, F_1),
	f_data("tanh", TANH, F_1),
	f_data("sin", SIN, F_1),
	f_data("cos", COS, F_1),
	f_data("tan", TAN, F_1),
	f_data("asinh", ASINH, F_1),
	f_data("acosh", ACOSH, F_1),
	f_data("atanh", ATANH, F_1),
	f_data("asin", ASIN, F_1),
	f_data("acos", ACOS, F_1),
	f_data("atan", ATAN, F_1),
	f_data("exp", EXP, F_1),
	f_data("ln", LN, F_1),
	f_data("sqrt", SQRT, F_1),
	f_data("abs", ABS, F_1),
	f_data("ceil", CEIL, F_1),
	f_data("floor", FLOOR, F_1),

	f_data("log", LOG, F_2),
	f_data("max", MAX, F_2),
	f_data("min", MIN, F_2),
};
const int F_CNT = sizeof(FUNCTIONS) / sizeof(f_data);

token* parse_function(const std::string& s, int pos) {
	for (int i = 0; i < F_CNT; i++) {
		if (s.length() - pos >= FUNCTIONS[i].name.length() && FUNCTIONS[i].match(s, pos)) return new t_function(pos, FUNCTIONS[i].name, FUNCTIONS[i].f, FUNCTIONS[i].ftype);
	}
	return NULL;
}

const double T_PI = 3.141592653589793;
const double T_E  = 2.718281828459045;


std::queue<token*> parser::get_tokens(const std::string& s) {
	if (s.empty()) throw parser_exception("error : function is empty", 0);
	std::queue<token*> tokens;
	token* before, * front;
	int pos = 0, len = s.length();
	while (pos < len) {

		// space is ignored
		if (s[pos] == ' ') {
			pos++;
			continue;
		}

		// symbol
		if (s[pos] == 'x') {
			tokens.push(new t_symbol(pos));
			pos++;
			continue;
		}

		// constant
		if (isdigit(s[pos])) {
			size_t cnt;
			double d = std::stod(s.substr(pos), &cnt);
			tokens.push(new t_constant(pos, d));
			pos += cnt;
			continue;
		}

		// -x
		if (s.length() - pos >= 2 && s[pos] == '-' && s[pos + 1] == 'x' && (tokens.empty() || tokens.back()->type == T_LEFT_PAREN || tokens.back()->type == T_COMMA)) {
			tokens.push(new t_constant(pos, -1)); // -1
			tokens.push(new t_operator(pos + 1, '*')); // *
			tokens.push(new t_symbol(pos + 1)); // x
			pos += 2;
			continue;
		}

		// negative constant
		if (s.length() - pos >= 2 && s[pos] == '-' && isdigit(s[pos+1]) && (tokens.empty() || tokens.back()->type == T_LEFT_PAREN || tokens.back()->type == T_COMMA)) {
			size_t cnt;
			double d = std::stod(s.substr(pos), &cnt);
			tokens.push(new t_constant(pos, d));
			pos += cnt;
			continue;
		}

		// operator
		if (s[pos] == '+' || s[pos] == '-' || s[pos] == '*' || s[pos] == '/' || s[pos] == '^') {
			tokens.push(new t_operator(pos, s[pos]));
			pos++;
			continue;
		}

		// parenthesis
		if (s[pos] == '(') {
			tokens.push(new t_left_paren(pos));
			pos++;
			continue;
		}
		if (s[pos] == ')') {
			tokens.push(new t_right_paren(pos));
			pos++;
			continue;
		}

		// comma
		if (s[pos] == ',') {
			tokens.push(new t_comma(pos));
			pos++;
			continue;
		}

		// function
		token* t = parse_function(s, pos);
		if (t != NULL) {
			tokens.push(t);
			pos += ((t_function*)t)->name.length();
			continue;
		}

		if (s[pos] == 'e') {
			tokens.push(new t_constant(pos, T_E));
			pos++;
			continue;
		}

		if (s.length() - pos >= 2 && s[pos] == 'p' && s[pos + 1] == 'i') {
			tokens.push(new t_constant(pos, T_PI));
			pos += 2;
			continue;
		}

		// unknown token
		throw parser_exception("invalid token", pos);

	}


	// put hidden multiplies
	// 1. constant + symbol
	// 2. constant + left_paren
	// 3. symbol + left_paren
	// 4. right_paren + left_paren
	// 5. constant + function
	// 6. symbol + function
	len = tokens.size();
	before = tokens.front();
	tokens.push(before); tokens.pop();
	front = tokens.front();
	for (int i=1; i<len; i++) {
		if ((before->type == T_CONSTANT && front->type == T_SYMBOL) || 
			(before->type == T_CONSTANT && front->type == T_LEFT_PAREN) ||
			(before->type == T_SYMBOL && front->type == T_LEFT_PAREN) ||
			(before->type == T_RIGHT_PAREN && front->type == T_LEFT_PAREN) ||
			(before->type == T_CONSTANT && front->type == T_FUNCTION) ||
			(before->type == T_SYMBOL && front->type == T_FUNCTION)
		) {
			tokens.push(new t_operator(front->pos, '*'));
		}
		tokens.push(front); tokens.pop();
		before = front;
		front = tokens.front();
	}	

	return tokens;
}