#include <cmath>
#include <ctime>
#include <vector>
#include <stdexcept>

#include "PmergeMe.hpp"

using std::vector;

vector<int> PmergeMe::vector_mergeSort(char** begin, char** end) {
	vector<intm> vec(_size);
	for (size_t i = 0; begin != end; ++begin, ++i) {
		vec.at(i).v = atoi(*begin);
		vec.at(i).origPos = i;
		if (vec.at(i).v < 0) {
			throw std::invalid_argument("Negative numbers are not allowed");
		}
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

void swapBlocks(vector<int> &src, int start1, int start2, int blockSize) {
	for (int i = 0; i < blockSize; ++i) {
		std::swap(src.at(start1 + i), src.at(start2 + i));
	}
}

int binaryInsert(T &result, intm value, int left, int right) {
	if (left >= right) {
		typename T::iterator it = result.begin();
		advance(it, left);
		result.insert(it, value);
		return left;
	}
	int mid = (left + right) / 2;
	typename T::iterator it = result.begin();
	advance(it, mid);
	if (value < *it) {
		return binaryInsert(result, value, left, mid);
	} else {
		return binaryInsert(result, value, mid + 1, right);
	}
}

void insertBlock(
	vector<int>& src
	, size_t from
	, size_t blockSize
	, vector<int>& dest
	, vector<int>::iterator destI
) {
	for (; from < blockSize; from ++) {
		dest.insert(destI, src.at(from));
	}
}

void doSortMasha(vector<int> &src, size_t blockSize = 1) {
	// sort each block internally (reorder within 'pairs', whatever size, using starting element as key)
	for (size_t i = blockSize; i < src.size(); i += blockSize * 2) {
		if (src.at(i) > src.at(i - blockSize)) {
			swapBlocks(src, i - blockSize, i, blockSize);
		}
	}

	doSortMasha(src, blockSize * 2);

	vector<int> result;
	result.reserve(src.size());

	// insert B1 at start
	insertBlock(src, blockSize, blockSize, result, result.end());
	
	//insert all As
	insertBlock(src, 0, blockSize, result, result.end());
	// step is 2 * blockSize since we take only As, and the skipped ones are Bs
	for (size_t i = 2 * blockSize; i < src.size(); i += 2 * blockSize) {
		insertBlock(src, i, blockSize, result, result.end());
	}

	int blocksToInsert = result.size() - 2;

	fillJacobstahl(blocksToInsert);
	for (size_t i = 1; i < blocksToInsert; i ++) {
		int from = _jacobstahl.at(i);
		int where = binaryInsert(result, src, from * blockSize, (from + 1) * blockSize, )
	}

}
