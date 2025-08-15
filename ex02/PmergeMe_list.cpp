#include <cmath>
#include <ctime>
#include <list>

#include "PmergeMe.hpp"

using std::list;

list<int> PmergeMe::list_mergeSort(char** begin, char** end) {
	list<intm> vec;
	for (size_t i = 0; begin != end; ++begin, ++i) {
		intm item;
		item.v = atoi(*begin);
		item.origPos = i;
		vec.push_back(item);
	}
	clock_t startTime = clock();
	list<intm> sorted = doSort(vec, 0);
	clock_t endTime = clock();
	_sortingTime[LIST] = (double)(endTime - startTime) / CLOCKS_PER_SEC * 1000000.0;
	list<int> ret;
	for (size_t i = 0; i < _size; ++i) {
		int v = sorted.front().v;
		sorted.pop_front();
		ret.push_back(v);
	}
	return (ret);
}
