#include "parser.hpp"

// shunting yard algorithm
parser::parser(const std::string& s) {
	std::queue<p_token> tokens = get_tokens(s);
	check_syntax(tokens);
	std::stack<p_token> op;
	p_operator o1, o2;
	while (!tokens.empty()) {
		auto t = tokens.front(); tokens.pop();

		switch (t->type) {
		case T_CONSTANT:
		case T_SYMBOL:
			equation.push_back(t);
			break;
		case T_FUNCTION:
			op.push(t);
			break;
		case T_OPERATOR:
			o1 = std::dynamic_pointer_cast<t_operator>(t);
			while (!op.empty() && op.top()->type != T_LEFT_PAREN) {
				o2 = std::dynamic_pointer_cast<t_operator>(op.top());
				if (!(o2->priority > o1->priority || (o2->priority == o1->priority && o1->c == '^'))) break;
				equation.push_back(op.top());
				op.pop();
			}
			op.push(t);
			break;
		case T_LEFT_PAREN:
			op.push(t);
			break;
		case T_RIGHT_PAREN:
			while (op.top()->type != T_LEFT_PAREN) {
				equation.push_back(op.top()); op.pop();
			}
			op.pop(); // '('
			if (!op.empty() && op.top()->type == T_FUNCTION) {
				equation.push_back(op.top()); op.pop();
			}
			break;
		} // switch
	} // while

	while (!op.empty()) {
		equation.push_back(op.top()); op.pop();
	}
}