#include "parser.hpp"
 
void parser::check_syntax(std::queue<std::shared_ptr<token>> tokens) {
	std::shared_ptr<token> back = tokens.back();
	std::stack<std::shared_ptr<token>> s;

	while (!tokens.empty()) {
		std::shared_ptr<token> t = tokens.front(); tokens.pop();
		std::shared_ptr<token> top = s.empty() ? NULL : s.top(), tmp;
		switch (t->type) {
		case T_CONSTANT:
		case T_SYMBOL:
			if (top != NULL && (top->type == T_CONSTANT || top->type == T_SYMBOL)) throw parser_exception("invalid syntax: operator expected", t->pos);
			if (top != NULL && top->type == T_FUNCTION) throw parser_exception("invalid syntax: \'(\' expected", t->pos);
			s.push(t);
			break;

		case T_OPERATOR:
			if (top == NULL || !(top->type == T_CONSTANT || top->type == T_SYMBOL)) throw parser_exception("invalid syntax: invalid operator", t->pos);
			s.pop();
			break;

		case T_LEFT_PAREN:
			if (top != NULL && (top->type == T_CONSTANT || top->type == T_SYMBOL)) throw parser_exception("invalid syntax: invalid parenthesis", t->pos);
			s.push(t);
			break;

		case T_RIGHT_PAREN:
			if (top == NULL || top->type == T_LEFT_PAREN) throw parser_exception("invalid syntax: value expected", t->pos);
			if (top == NULL || !(top->type == T_CONSTANT || top->type == T_SYMBOL)) throw parser_exception("invalid syntax: invalid parenthesis", t->pos);
			s.pop(); // value
			if (s.empty()) throw parser_exception("invalid syntax: parenthesis does not match", t->pos); // there is no '('
			if (s.top()->type != T_LEFT_PAREN) throw parser_exception("invalid syntax : parenthesis does not match", t->pos); // there is no '('
			s.pop(); // '('
			if (!s.empty() && s.top()->type == T_FUNCTION) {
				s.pop(); // function
			}
			s.push(std::make_shared<t_constant>(0, 0)); // dummy constant
			break;

		case T_FUNCTION:
			if (top != NULL && (top->type == T_CONSTANT || top->type == T_SYMBOL)) throw parser_exception("invalid syntax : operator expected", t->pos);
			s.push(t);
			break;
			
		} // switch
	} // while

	if (s.size() == 0) throw parser_exception("invalid syntax : value expected", back->pos+1);
	if (s.size() != 1 || !(s.top()->type == T_CONSTANT || s.top()->type == T_SYMBOL)) throw parser_exception("invalid syntax : equation is not ended", back->pos+1);
}