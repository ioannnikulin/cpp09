#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <cstddef>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <vector>
#include <ostream>

using std::advance;
using std::cout;
using std::endl;
using std::list;
using std::max;
using std::min;
using std::ostream;
using std::setw;
using std::vector;

struct myuint
{
	unsigned int v;
	bool operator<(const myuint &other) const;
	bool operator>(const myuint &other) const;
	bool operator==(const myuint &other) const;
	bool operator!=(const myuint &other) const;
	bool operator<=(const myuint &other) const;
	bool operator>=(const myuint &other) const;
	myuint(unsigned int value) : v(value) {}
};

extern unsigned int comparisonCounter;

class PmergeMe
{
private:
	PmergeMe(const PmergeMe &other);
	PmergeMe &operator=(const PmergeMe &other);

	std::vector<unsigned int> _jacobstahl;

	void fillJacobstahl(size_t sz);

public:
	PmergeMe();
	~PmergeMe();
	std::vector<myuint> sort(std::vector<myuint> src);
	void testJacobstahl() const;

private:
};

std::ostream &operator<<(std::ostream &os, const myuint &obj);

int runTestSuite();

bool parse(int argc, char **argv, std::vector<myuint> &vec);
std::vector<myuint> wrap(const std::vector<unsigned int> &vec);

#endif
