#include "parser.hpp"
 
void parser::check_syntax(std::queue<token*> tokens) {
	token* back = tokens.back();
	std::stack<token*> s;
	while (!tokens.empty()) {
		bool function_needed = false;
		token* t = tokens.front(); tokens.pop();
		token* top = s.empty() ? NULL : s.top(), * tmp, *tmp2;
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
			tmp = s.top();  s.pop(); // value
			if (s.empty()) throw parser_exception("invalid syntax: parenthesis does not match", t->pos); // there is no '('
			if (s.top()->type == T_COMMA) {
				s.pop(); // comma
				s.pop(); // arg1
				function_needed = true;
			}
			if (s.top()->type == T_COMMA) throw parser_exception("invalid syntax : there can be at most two arguments", s.top()->pos);
			if (s.top()->type != T_LEFT_PAREN) throw parser_exception("invalid syntax : parenthesis does not match", t->pos); // there is no '('
			tmp2 = s.top();  s.pop();
			if (!s.empty() && s.top()->type == T_FUNCTION) {
				if (((t_function*)s.top())->ftype == F_2 && !function_needed) throw parser_exception("invalid syntax: two arguments expected", t->pos);
				s.pop(); // function
			}
			else if (function_needed) {
				throw parser_exception("invalid syntax: function expected", tmp2->pos);
			}
			s.push(tmp);
			break;
		case T_COMMA:
			if (top == NULL || !(top->type == T_CONSTANT || top->type == T_SYMBOL)) throw parser_exception("invalid syntax: invalid comma", t->pos);
			s.push(t);
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