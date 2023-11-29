#ifndef __PARSER__
#define __PARSER__

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include "token.hpp"
#include "exceptions.hpp"

template <typename T>
struct safe {
	T v;
	bool valid;
	safe() : v(), valid(false) {}
	safe(T value) : v(value), valid(true) {}
};

class parser {
public:
	parser(const std::string& s);
	void print() {
		for (int i = 0; i < equation.size(); i++) {
			std::cout << equation[i]->to_string() << ' ';
		}
	}
	safe<double> evaluate(double value);
private:
	std::vector<token*> equation;
	static void check_syntax(std::queue<token*> tokens);
	static std::queue<token*> get_tokens(const std::string& s);
};

#endif