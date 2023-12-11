#ifndef __PARSER__
#define __PARSER__

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include "token.hpp"

struct parser_exception {
	const char* msg;
	int pos;
	parser_exception(const char* m, int p) : msg(m), pos(p) {}
};

class parser {
public:
	parser(const std::string& s);  // in parser.hpp
	void print() {
		for (auto t : equation) {
			std::cout << t->to_string() << ' ';
		}
	}
	double evaluate(double value); // in evaluator.cpp
	~parser() {
		int len = equation.size();
		for (int i=0; i<len; i++) {
			delete equation[i];
		}
	}
private:
	std::vector<token*> equation;
	static void check_syntax(std::queue<token*> tokens); // in syntax_checker.cpp
	static std::queue<token*> get_tokens(const std::string& s); // in tokenizer.cpp
};

#endif