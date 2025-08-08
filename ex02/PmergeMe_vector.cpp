#include <cmath>
#include <ctime>
#include <iostream>
#include <list>
#include <stdexcept>
#include <vector>

#include "PmergeMe.hpp"

using std::ceil;
using std::cerr;
using std::cout;
using std::endl;
using std::invalid_argument;
using std::max;
using std::min;
using std::vector;

#define DEBUG

static int binaryInsert(vector<int> &result, int value, int left, int right) {
	if (left >= right) {
		result.insert(result.begin() + left, value);
		return left;
	}
	int mid = (left + right) / 2;
	if (value < result[mid]) {
		return binaryInsert(result, value, left, mid);
	} else {
		return binaryInsert(result, value, mid + 1, right);
	}
}

void PmergeMe::vector_doSort(vector<intm>& src) {
	#ifdef DEBUG
	cout << "sort called with: ";
	for (size_t i = 0; i < src.size(); i++ ) {
		cout << src.at(i) << " ";
	}
	cout << endl;
	#endif

	if (src.size() == 1) {
		return ;
	}
	if (src.size() == 2) {
		if (src.at(1) < src.at(0)) {
			std::swap(src.at(0), src.at(1));
			std::swap(src.at(0).origPos, src.at(1).origPos);
		}
		return ;
	}
	vector<vector<intm> > pairs(src.size() / 2);
	for (size_t i = 0; i < src.size() - src.size() % 2; i += 2) {
		pairs.at(i/2) = vector<intm>(2);
		pairs.at(i/2).at(0) = max(src.at(i), src.at(i + 1));
		pairs.at(i/2).at(1) = min(src.at(i), src.at(i + 1));
	}
	vector<intm> tops(pairs.size());
	for (size_t i = 0; i < pairs.size(); ++i) {
		tops.at(i) = pairs.at(i).at(0);
	}
	vector_doSort(tops);

	tops.insert(tops.begin(), pairs.at(tops.at(0).origPos).at(1)); // b1 smallest
/*	for (size_t i = 0; i < order.size(); ++i) {
		origins.at(i) = order.at(i) * 2;
	}
	origins.insert(origins.begin(), order.at(0) * 2 + 1);

	#ifdef DEBUG
	cout << "Result vector after first pass: ";
	for (size_t i = 0; i < tops.size(); ++i) {
		cout << tops.at(i) << " ";
	}
	cout << endl;
	#endif

	if (src.size() % 2 == 1) {
		pairs.push_back(vector<int>(2, src.at(src.size() - 1)));
		order.push_back(pairs.size() - 1);
	} // top element will be ignored
	for (size_t i = 1; i < pairs.size(); ++i) {
		int from = outsiderIdx(i, pairs.size());
		#ifdef DEBUG
		cout << "Inserting " << i << "th outsider: idx " << from << endl;
		#endif
		int who = pairs.at(order.at(from)).at(1);
		int where = binaryInsert(tops, who, 0, i * 2);
		origins.insert(origins.begin() + where, order.at(from));
		#ifdef DEBUG
		cout << "Inserting " << who << " at position " << where << endl;
		cout << "Result vector after " << i << "th pass: ";
		for (size_t i = 0; i < tops.size(); ++i) {
			cout << tops.at(i) << " ";
		}
		cout << endl;
		#endif
	}

	src = tops;
	return (origins);*/
}

vector<int> PmergeMe::vector_mergeSort(char** begin, char** end) {
	vector<intm> vec(_size);
	for (size_t i = 0; begin != end; ++begin, ++i) {
		vec.at(i).v = atoi(*begin);
		vec.at(i).origPos = i;
	}
	vector_doSort(vec);
	vector<int> sorted(_size);
	for (size_t i = 0; i < _size; ++i) {
		sorted.at(i) = vec.at(i).v;
	}
	return (sorted);
}