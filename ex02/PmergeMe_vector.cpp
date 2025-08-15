#include <cmath>
#include <ctime>
#include <vector>

#include "PmergeMe.hpp"

using std::vector;

vector<int> PmergeMe::vector_mergeSort(char** begin, char** end) {
	vector<intm> vec(_size);
	for (size_t i = 0; begin != end; ++begin, ++i) {
		vec.at(i).v = atoi(*begin);
		vec.at(i).origPos = i;
	}
	clock_t startTime = clock();
	vector<intm> sorted = doSort(vec, 0);
	clock_t endTime = clock();
	_sortingTime[VECTOR] = (double)(endTime - startTime) / CLOCKS_PER_SEC * 1000000.0;
	vector<int> ret(_size);
	for (size_t i = 0; i < _size; ++i) {
		ret.at(i) = sorted.at(i).v;
	}
	return (ret);
}
