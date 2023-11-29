#include "parser.hpp"
#include <cmath>

double evaluate1(FUNCTION f, double a) {
	switch (f) {
	case SINH:
		return sinh(a);
	case COSH:
		return cosh(a);
	case TANH:
		return tanh(a);
	case SIN:
		return sin(a);
	case COS:
		return cos(a);
	case TAN:
		return tan(a);
	case EXP:
		return exp(a);
	case LN:
		if (a<0) return double();
		return log(a);
	case SQRT:
		if (a < 0) return double();
		return sqrt(a);
	case ABS:
		return fabs(a);
	default:
		return std::nan("");
	}
}

double evaluate2(FUNCTION f, double a, double b) {
	switch (f) {
	case LOG:
		if (a < 0 || a == 1 || b < 0) return double();
		return log(b) / log(a);
	case MAX:
		return a > b ? a : b;
	case MIN:
		return a > b ? b : a;
	default:
		return std::nan("");
	}
}

double parser::evaluate(double value) {
	std::stack<double> s;
	int len = equation.size();
	double a, b;
	double tmp;
	for (int i = 0; i < len; i++) {
		switch (equation[i]->type) {
		case T_CONSTANT:
			s.push(((t_constant*)equation[i])->data);
			break;
		case T_SYMBOL:
			s.push(value);
			break;
		case T_OPERATOR:
			b = s.top(); s.pop();
			a = s.top(); s.pop();
			switch (((t_operator*)equation[i])->c) {
			case '+':
				s.push(a + b);
				break;
			case '-':
				s.push(a - b);
				break;
			case '*':
				s.push(a * b);
				break;
			case '/':
				if (b == 0) return double();
				s.push(a / b);
				break;
			case '^':
				double result = pow(a, b);
				if (std::isnan(result)) return double();
				s.push(result);
				break;
			}
			break;
		case T_FUNCTION:
			t_function* f = (t_function*)equation[i];
			if (f->ftype == F_1) {
				a = s.top(); s.pop();
				tmp = evaluate1(f->f, a);
			}
			else {
				b = s.top(); s.pop();
				a = s.top(); s.pop();
				tmp = evaluate2(f->f, a, b);
			}
			s.push(tmp);
			break;
		}
	}
	return s.top();
}