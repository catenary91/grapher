#include <memory>
#include "parser.hpp"

struct f_data {
	std::string name;
	FUNCTION f;

	f_data(const std::string n, FUNCTION f) : name(n), f(f) {}
	bool match(const std::string& s, int pos) const {
		return name.compare(0, name.length(), s, pos, name.length()) == 0;
	}
};

const f_data FUNCTIONS[] = {
	f_data("sinh", SINH),
	f_data("cosh", COSH),
	f_data("tanh", TANH),
	f_data("sin", SIN),
	f_data("cos", COS),
	f_data("tan", TAN),
	f_data("asinh", ASINH),
	f_data("acosh", ACOSH),
	f_data("atanh", ATANH),
	f_data("asin", ASIN),
	f_data("acos", ACOS),
	f_data("atan", ATAN),
	f_data("exp", EXP),
	f_data("log", LOG),
	f_data("sqrt", SQRT),
	f_data("abs", ABS),
	f_data("ceil", CEIL),
	f_data("floor", FLOOR),
};
const int F_CNT = sizeof(FUNCTIONS) / sizeof(f_data);

p_token parse_function(const std::string& s, int pos) {
	for (int i = 0; i < F_CNT; i++) {
		if (s.length() - pos >= FUNCTIONS[i].name.length() && FUNCTIONS[i].match(s, pos)) return std::make_shared<t_function>(pos, FUNCTIONS[i].name, FUNCTIONS[i].f);
	}
	return p_token();
}

const double T_PI = 3.141592653589793;
const double T_E  = 2.718281828459045;


std::queue<p_token> parser::get_tokens(const std::string& s) {
	if (s.empty()) throw parser_exception("error : function is empty", 0);
	std::queue<p_token> tokens;
	int pos = 0, len = s.length();
	while (pos < len) {

		// space is ignored
		if (s[pos] == ' ') {
			pos++;
			continue;
		}

		// symbol
		if (s[pos] == 'x') {
			tokens.push(std::make_shared<t_symbol>(pos));
			pos++;
			continue;
		}

		// constant
		if (isdigit(s[pos])) {
			size_t cnt;
			double d = std::stod(s.substr(pos), &cnt);
			tokens.push(std::make_shared<t_constant>(pos, d));
			pos += cnt;
			continue;
		}

		// negative constant
		if (s.length() - pos >= 2 && s[pos] == '-' && isdigit(s[pos+1]) && (tokens.empty() || tokens.back()->type == T_LEFT_PAREN)) {
			size_t cnt;
			double d = std::stod(s.substr(pos), &cnt);
			tokens.push(std::make_shared<t_constant>(pos, d));
			pos += cnt;
			continue;
		}

		// operator
		if (s[pos] == '+' || s[pos] == '-' || s[pos] == '*' || s[pos] == '/' || s[pos] == '^') {
			tokens.push(std::make_shared<t_operator>(pos, s[pos]));
			pos++;
			continue;
		}

		// parenthesis
		if (s[pos] == '(') {
			tokens.push(std::make_shared<t_left_paren>(pos));
			pos++;
			continue;
		}
		if (s[pos] == ')') {
			tokens.push(std::make_shared<t_right_paren>(pos));
			pos++;
			continue;
		}

		// function
		p_token t = parse_function(s, pos);
		if (t) {
			tokens.push(t);
			pos += std::dynamic_pointer_cast<t_function>(t)->name.length();
			continue;
		}

		// e
		if (s[pos] == 'e') {
			tokens.push(std::make_shared<t_constant>(pos, T_E));
			pos++;
			continue;
		}

		// pi
		if (s.length() - pos >= 2 && s[pos] == 'p' && s[pos + 1] == 'i') {
			tokens.push(std::make_shared<t_constant>(pos, T_PI));
			pos += 2;
			continue;
		}

		// unknown token
		throw parser_exception("invalid token", pos);

	}

	return tokens;
}