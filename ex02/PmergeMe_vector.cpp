#include <cmath>
#include <ctime>
#include <iomanip>
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
using std::setw;
using std::vector;

//#define DEBUG

static int binaryInsert(vector<intm> &result, intm value, int left, int right) {
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

vector<intm> PmergeMe::vector_doSort(vector<intm> src, int pad) {
	#ifdef DEBUG
	cout << setw(pad * 4) << " " << "sort called with: ";
	for (size_t i = 0; i < src.size(); i++ ) {
		cout << src.at(i) << " ";
	}
	cout << endl;
	#endif

	if (src.size() == 1) {
		return (src);
	}
	if (src.size() == 2) {
		if (src.at(1) < src.at(0)) {
			std::swap(src.at(0), src.at(1));
		}
		return (src);
	}

	vector<vector<intm> > pairs(src.size() / 2);
	for (size_t i = 0; i < src.size() - src.size() % 2; i += 2) {
		pairs.at(i/2) = vector<intm>(2);
		pairs.at(i/2).at(0) = max(src.at(i), src.at(i + 1));
		pairs.at(i/2).at(1) = min(src.at(i), src.at(i + 1));
	}

	// just top elements of those pairs for independent sorting
	// will fetch their order though to sort the pairs too
	// also this list will be used as result accumulator 
	// and returned in the end
	vector<intm> tops = vector<intm>(pairs.size());
	for (size_t i = 0; i < pairs.size(); ++i) {
		tops.at(i) = pairs.at(i).at(0);
		tops.at(i).origPos = i; // internal numbering for recursive call, will be reverted
	}

	tops = vector_doSort(tops, pad + 1);

	// reorganizing whole pairs
	vector<vector<intm> > sortedPairs(ceil(src.size() / 2.0));
	for (size_t i = 0; i < pairs.size(); ++i) {
		sortedPairs.at(i) = pairs.at(tops.at(i).origPos);
	}
	if (src.size() % 2 == 1) {
		sortedPairs.at(sortedPairs.size() - 1) = vector<intm>(2);
		// element 0 not used
		sortedPairs.at(sortedPairs.size() - 1).at(1) = src.at(src.size() - 1);
	}

	// we used the order from tops to sort pairs,
	// now restoring original numbering of elements for return
	// (this is embarrasing, I want single-linked lists)
	for (size_t i = 0; i < tops.size(); ++i) {
		tops.at(i).origPos = pairs.at(tops.at(i).origPos).at(0).origPos;
	}

	tops.insert(tops.begin(), sortedPairs.at(0).at(1)); // b1 smallest

	fillJacobstahl(sortedPairs.size());
	// other outsiders (b's)
	for (size_t i = 1; i < sortedPairs.size(); ++i) {
		int from = _jacobstahl.at(i);
		#ifdef DEBUG
		cout << setw(pad * 4) << " " << "Inserting " << i << "th outsider: idx " << from << endl;
		#endif
		intm who = sortedPairs.at(from).at(1);
		int where = binaryInsert(tops, who, 0, tops.size());
		#ifdef DEBUG
		cout << setw(pad * 4) << " " << "Inserting " << who << " at position " << where << endl;
		cout << setw(pad * 4) << " " << "Result vector after " << i << "th pass: ";
		for (size_t i = 0; i < tops.size(); ++i) {
			cout << tops.at(i) << " ";
		}
		cout << endl;
		#else
		(void)pad;
		(void)where;
		#endif
	}
	return (tops);
}

vector<int> PmergeMe::vector_mergeSort(char** begin, char** end) {
	vector<intm> vec(_size);
	for (size_t i = 0; begin != end; ++begin, ++i) {
		vec.at(i).v = atoi(*begin);
		vec.at(i).origPos = i;
	}
	clock_t startTime = clock();
	vector<intm> sorted = vector_doSort(vec, 0);
	clock_t endTime = clock();
	_sortingTime[VECTOR] = (double)(endTime - startTime) / CLOCKS_PER_SEC * 1000000.0;
	vector<int> ret(_size);
	for (size_t i = 0; i < _size; ++i) {
		ret.at(i) = sorted.at(i).v;
	}
	return (ret);
}
