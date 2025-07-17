#include <algorithm>
#include <cmath>
#include <ctime>
#include <iostream>
#include <list>
#include <stdexcept>
#include <cassert>

#include "PmergeMe.hpp"

using std::ceil;
using std::cerr;
using std::cout;
using std::endl;
using std::invalid_argument;
using std::list;
using std::max;
using std::min;

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
	_sortingTime[0] = -1;
	_sortingTime[1] = -1;
}

void PmergeMe::mergeSort(char** begin, char** end) {
	if (begin >= end) {
		throw invalid_argument("Invalid range for merge sort");
	}
	_size = end - begin;
	_stl_sorted = vector<int>(_size);
	cout << "Before: ";
	for (int i = 0; begin + i != end; i ++) {
		cout << *(begin + i) << " ";
		_stl_sorted.at(i) = atoi(*(begin + i));
	}
	cout << endl;
	std::sort(_stl_sorted.begin(), _stl_sorted.end());
	fillJacobstahl();
	
	vector<int> sorted = mergeSort_vector(begin, end);
	assertSorted(sorted);
	sorted = mergeSort_list(begin, end);
	assertSorted(sorted);
	cout << "After: ";
	for (size_t i = 0; i < _size; i ++) {
		cout << sorted[i] << " ";
	}
	cout << endl;
	cout << "Time to process a range of " << _size << " elements with std::vector: "
	    << _sortingTime[VECTOR] << " microseconds" << endl;
	cout << "Time to process a range of " << _size << " elements with std::list:"
	    << _sortingTime[LIST] << " microseconds" << endl;
}

void PmergeMe::assertSorted(vector<int> tgt) const {
	for (size_t i = 1; i < _size; ++i) {
		if (tgt[i] < tgt[i - 1]) {
			throw std::runtime_error("The array is not sorted");
		}
		if (tgt[i] != _stl_sorted[i]) {
			throw std::runtime_error("The array's contents doesn't match the source");
		}
	}
}

void PmergeMe::fillJacobstahl() {
	_jacobstahl = vector<int>(2, 0);
	_jacobstahl.at(1) = 1;
	int pprev = 0;
	int prev = 0;
	int cur = 1;

	while (_jacobstahl.size() < _size) {
		pprev = prev;
		prev = cur;
		cur = prev + 2 * pprev;
		int ccur = cur;
		while (ccur > prev) {
			if (ccur < (int)_size) {
				_jacobstahl.push_back(ccur);
			}
			ccur --;
		}
	}
}

int PmergeMe::outsiderIdx(int idx, size_t size) {
	while(_jacobstahl.at(idx) >= (int)size) {
		idx ++;
	}
	return _jacobstahl.at(idx);
}

static vector<int> perfect22() {
	vector<int> result(22);
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
	test._size = 22;
	test.fillJacobstahl();
	vector<int> expected(perfect22());
	for (int i = 0; i < 22; i ++) {
		assert(test._jacobstahl.at(i) == expected.at(i));
	}
	test._size = 21;
	test.fillJacobstahl();
	expected.erase(expected.begin() + 12);
	for (int i = 0; i < 21; i ++) {
		assert(test._jacobstahl.at(i) == expected.at(i));
	}
}
