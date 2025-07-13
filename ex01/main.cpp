#include <iostream>
#include <cassert>

#include "RPN.hpp"

using std::cerr;
using std::cout;
using std::endl;
using std::invalid_argument;
using std::string;

static void bad(string expr, string err) {
	try {
		RPN::calculate(expr);
		assert(false);
	} catch (const invalid_argument& e) {
		assert(string(e.what()) == err);
	}
}

static void tests() {
	assert(RPN::calculate("3") == 3);
	assert(RPN::calculate("  3   51 +     ") == 54);
	assert(RPN::calculate("1 2 -") == -1);
	assert(RPN::calculate("2 3 *") == 6);
	assert(RPN::calculate("8 2 /") == 4);
	assert(RPN::calculate("3 5 + 2 *") == 16);
	assert(RPN::calculate("3 5 2 * -") == -7);
	assert(RPN::calculate("8 9 * 9 - 9 - 9 - 4 - 1 +") == 42);
	assert(RPN::calculate("7 7 * 7 -") == 42);
	assert(RPN::calculate("1 2 * 2 / 2 * 2 4 - +") == 0);
	bad("1 2 + 3 4 +", "Invalid RPN expression");
	bad("1 2 + 3 4 + 5", "Invalid RPN expression");
	bad("1 + 1", "Invalid RPN expression");
	bad("(1 1 +)", "Invalid character in expression");
	bad("1 a +", "Invalid character in expression");
	bad("+ 1 1", "Invalid RPN expression");
	bad(" 1 1 1 - /", "Division by zero");
}

int main(int argc, char **argv) {
	if (argc == 1) {
		tests();
		return (0);
	} else if (argc > 2) {
		cerr << "Error: Too many arguments provided." << endl;
		return 1;
	} else if (argc == 2) {
		string expr(argv[1]);
		try {
			cout << RPN::calculate(expr) << endl;
		} catch (const invalid_argument& e) {
			cerr << "Error: " << e.what() << endl;
			return 1;
		}
		return 0;
	}
}