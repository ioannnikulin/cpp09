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

int getInt(const int& v) {
	return v;
}

int getFirst(const pair& p) {
	return p.first;
}

vector<int> PmergeMe::sortAndReportOrder (vector<int>& src) {
	#ifdef DEBUG
	cout << "sortAndReportOrder called with: ";
	for (size_t i = 0; i < src.size(); i++ ) {
		cout << src.at(i) << " ";
	}
	cout << endl;
	#endif

	vector<int> resultOrder(src.size(), 0);
	if (src.size() == 1) {
		return (resultOrder);
	}
	if (src.size() == 2) {
		resultOrder.at(1) = 1;
		if (src.at(0) > src.at(1)) {
			std::swap(resultOrder.at(0), resultOrder.at(1));
		}
		return (resultOrder);
	}
	vector<vector<int> > pairs(src.size() / 2);
	for (size_t i = 0; i < src.size() - src.size() % 2; i += 2) {
		pairs.at(i/2) = vector<int>(2);
		pairs.at(i/2).at(0) = max(src.at(i), src.at(i + 1));
		pairs.at(i/2).at(1) = min(src.at(i), src.at(i + 1));
	}
	vector<int> tops(pairs.size());
	for (size_t i = 0; i < pairs.size(); ++i) {
		tops.at(i) = pairs.at(i).at(0);
	}
	vector<int> order = sortAndReportOrder(tops);

	tops.insert(tops.begin(), pairs.at(order.at(0)).at(1)); // b1 smallest
	resultOrder.insert(resultOrder.begin(), order.at(0) * 2 + 1);

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
		resultOrder.insert(resultOrder.begin() + where, order.at(from));
		#ifdef DEBUG
		cout << "Inserting " << who << " at position " << where << endl;
		#endif
	}

	src = tops;
	return (resultOrder);
}

vector<int> PmergeMe::mergeSort_vector(char** begin, char** end) {
	vector<int> vec(_size);
	for (size_t i = 0; begin != end; ++begin, ++i) {
		vec.at(i) = atoi(*begin);
	}
	sortAndReportOrder(vec);
	return (vec);
}