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
	cout << "Before: ";
	for (char** it = begin; it != end; ++it) {
		cout << *it << " ";
	}
	cout << endl;
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
	vector<int> stl_sorted(_size);
	for (size_t i = 0; i < _size; ++i) {
		stl_sorted.at(i) = atoi(_source[i]);
	}
	if (tgt[0] != stl_sorted[0]) {
		throw std::runtime_error("The array's contents doesn't match the source");
	}
	std::sort(stl_sorted.begin(), stl_sorted.end());
	for (size_t i = 1; i < _size; ++i) {
		if (tgt[i] < tgt[i - 1]) {
			throw std::runtime_error("The array is not sorted");
		}
		if (tgt[i] != stl_sorted[i]) {
			throw std::runtime_error("The array's contents doesn't match the source");
		}
	}
}

size_t PmergeMe::tk(size_t k) {
	return ((pow(2, k + 1) + pow(-1, k)) / 3);
}

void PmergeMe::testTk() const {
	assert(tk(0) == 1);
	assert(tk(1) == 1);
	assert(tk(2) == 3);
	assert(tk(3) == 5);
	assert(tk(4) == 11);
	assert(tk(5) == 9);
	assert(tk(6) == 7);
}
