#include "parser.hpp"
#include <cmath>

inline double evaluate_f(FUNCTION f, double a) {
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
	case ASINH:
		return asinh(a);
	case ACOSH:
		return acosh(a);
	case ATANH:
		return atanh(a);
	case ASIN:
		return asin(a);
	case ACOS:
		return acos(a);
	case ATAN:
		return atan(a);
	case EXP:
		return exp(a);
	case LOG:
		return log(a);
	case SQRT:
		return sqrt(a);
	case ABS:
		return fabs(a);
	case CEIL:
		return ceil(a);
	case FLOOR:
		return floor(a);
	
	default:
		return std::nan("");
	}
}

double parser::evaluate(double value) {
	std::stack<double> s;
	double a, b, tmp;
	p_function f;
	for (auto t : equation) {
		switch (t->type) {
		case T_CONSTANT:
			s.push(std::dynamic_pointer_cast<t_constant>(t)->data);
			break;
		case T_SYMBOL:
			s.push(value);
			break;
		case T_OPERATOR:
			b = s.top(); s.pop();
			a = s.top(); s.pop();
			switch (std::dynamic_pointer_cast<t_operator>(t)->c) {
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
				s.push(a / b);
				break;
			case '^':
				tmp = pow(a, b);
				if (std::isnan(tmp)) return tmp;
				s.push(tmp);
				break;
			}
			break;
		case T_FUNCTION:
			f = std::dynamic_pointer_cast<t_function>(t);
			a = s.top(); s.pop();
			tmp = evaluate_f(f->f, a);
			if (std::isnan(tmp)) return tmp;
			s.push(tmp);
			break;
		default:
			return std::nan("");
		}
	}
	return s.top();
}