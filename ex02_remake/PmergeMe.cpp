#include <algorithm>
#include <cmath>
#include <ctime>
#include <iostream>
#include <list>
#include <stdexcept>
#include <cassert>

#include "PmergeMe.hpp"

unsigned int comparisonCounter = 0;

using std::ceil;
using std::cerr;
using std::cout;
using std::endl;
using std::invalid_argument;
using std::list;
using std::max;
using std::min;
using std::ostream;

PmergeMe::PmergeMe() {
}

PmergeMe::PmergeMe(const PmergeMe& other) {
	(void)other;
}

PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
	(void)other;
	return *this;
}

PmergeMe::~PmergeMe() {
}

/*
static vector<unsigned int> perfect22() {
	vector<unsigned int> result(22);
	result.at(0) = 0;
	result.at(1) = 1;
	result.at(2) = 3;
	result.at(3) = 2;
	result.at(4) = 5;
	result.at(5) = 4;
	result.at(6) = 11;
	result.at(7) = 10;
	result.at(8) = 9;
	result.at(9) = 8;
	result.at(10) = 7;
	result.at(11) = 6;
	result.at(12) = 21;
	result.at(13) = 20;
	result.at(14) = 19;
	result.at(15) = 18;
	result.at(16) = 17;
	result.at(17) = 16;
	result.at(18) = 15;
	result.at(19) = 14;
	result.at(20) = 13;
	result.at(21) = 12;
	return result;
}

void PmergeMe::testJacobstahl() const {
	PmergeMe test;
	test.fillJacobstahl(22);
	vector<unsigned int> expected(perfect22());
	for (int i = 0; i < 22; i ++) {
		assert(test._jacobstahl.at(i) == expected.at(i));
	}
	test.fillJacobstahl(21);
	expected.erase(expected.begin() + 12);
	for (int i = 0; i < 21; i ++) {
		assert(test._jacobstahl.at(i) == expected.at(i));
	}
}
*/

bool myuint::operator<(const myuint& other) const {
	comparisonCounter++;
	return v < other.v;
}

bool myuint::operator>(const myuint& other) const {
	comparisonCounter++;
	return v > other.v;
}

bool myuint::operator==(const myuint& other) const {
	comparisonCounter++;
	return v == other.v;
}

bool myuint::operator!=(const myuint& other) const {
	comparisonCounter++;
	return v != other.v;
}

bool myuint::operator<=(const myuint& other) const {
	comparisonCounter++;
	return v <= other.v;
}

bool myuint::operator>=(const myuint& other) const {
	comparisonCounter++;
	return v >= other.v;
}

ostream& operator<<(ostream& os, const myuint& obj) {
	os << obj.v;
	return os;
}

vector<myuint> wrap(const vector<unsigned int>& vec) {
	vector<myuint> result;
	result.reserve(vec.size());
	for (size_t i = 0; i < vec.size(); i ++) {
		result.push_back(myuint(vec.at(i)));
	}
	return result;
}

vector<myuint> PmergeMe::sort(vector<myuint> src) {
	comparisonCounter = 0;
	std::sort(src.begin(), src.end());
	return src;
}