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
		return binaryInsert(result, value, mid + 1, right) + mid + 1;
	}
}

static vector<pair> doSort(vector<int>& src) {
	vector<pair> pairs(ceil(src.size() / 2.0));
	for (size_t i = 0; i < src.size(); i += 2) {
		pairs.at(i).first = max(src.at(i), src.at(i + 1));
		pairs.at(i).second = min(src.at(i), src.at(i + 1));
	}
	doSort(pairs.begin(), pairs.end());
	vector<int> result(pairs.size() + 1);
	result.at(0) = pairs[0].second;
	for (size_t i = 1; i <= pairs.size(); ++i) {
		result.at(i) = pairs[i - 1].first;
	}
	for (size_t i = 1; i <= ceil(_size / 2); ++i) {
		int j = binaryInsert(result, pairs[tk(i)].second, 0, i);
		#ifdef DEBUG
		cout << "Inserting " << pairs[tk(i)].second << " at position " << j << endl;
		#endif
		j = binaryInsert(result, pairs[tk(i) - 1].second, 0, i);
		#ifdef DEBUG
		cout << "Inserting " << pairs[tk(i)].second << " at position " << j << endl;
		#endif
	}
	
}

vector<int> PmergeMe::mergeSort_vector(char** begin, char** end) {
	vector<int> vec(_size);
	for (size_t i = 0; begin != end; ++begin, ++i) {
		vec.at(i) = atoi(*begin);
	}
	doSort(vec.begin(), vec.end(), );
}