#include "RPN.hpp"

#include <stack>
#include <stdexcept>
#include <string>

using std::string;
using std::invalid_argument;
using std::stack;

RPN::RPN() {
}

RPN::RPN(const RPN& other) {
	(void)other;
}

RPN& RPN::operator=(const RPN& other) {
	(void)other;
	return (*this);
}

RPN::~RPN() {
}

bool RPN::isOperator(char c) {
	return c == '+' || c == '-' || c == '*' || c == '/';
}

static void processOperator(stack<int> &operands, char op) {
	if (operands.size() < 2) {
		throw invalid_argument("Invalid RPN expression");
	}
	int b = operands.top(); operands.pop();
	int a = operands.top(); operands.pop();
	switch (op) {
		case '+': operands.push(a + b); break;
		case '-': operands.push(a - b); break;
		case '*': operands.push(a * b); break;
		case '/':
			if (b == 0) {
				throw invalid_argument("Division by zero");
			}
			operands.push(a / b);
			break;
		default:
			throw invalid_argument("Invalid operator");
	}
}

int RPN::calculate(const string &expr) {
	stack<int> operands;

	for (size_t i = 0; i < expr.size(); ++i) {
		char c = expr[i];
		if (isspace(c)) {
			continue;
		}
		if (isdigit(c)) {
			int num = 0;
			while (i < expr.size() && isdigit(expr[i])) {
				num = num * 10 + (expr[i] - '0');
				++i;
			}
			--i;
			operands.push(num);
			continue;
		}
		if (isOperator(c)) {
			processOperator(operands, c);
			continue;
		}
		throw invalid_argument("Invalid character in expression");
	}
	if (operands.size() != 1) {
		throw invalid_argument("Invalid RPN expression");
	}
	return operands.top();
}