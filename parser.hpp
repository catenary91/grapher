#ifndef __PARSER__
#define __PARSER__

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <memory>
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
private:
	std::vector<std::shared_ptr<token>> equation;
	static void check_syntax(std::queue<std::shared_ptr<token>> tokens); // in syntax_checker.cpp
	static std::queue<std::shared_ptr<token>> get_tokens(const std::string& s); // in tokenizer.cpp
};

#endif