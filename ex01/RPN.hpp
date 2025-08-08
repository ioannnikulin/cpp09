#ifndef RPN_HPP
#define RPN_HPP

#include <string>

using std::string;

class RPN {
private:
	RPN();
	RPN(const RPN& other);
	RPN &operator=(const RPN& other);
	static bool isOperator(char c);

public:
	static int calculate(const string &expr);
	~RPN();
};

#endif
