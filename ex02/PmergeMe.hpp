#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <cstddef>
#include <map>
#include <vector>

using std::map;
using std::vector;

class PmergeMe
{
private:
	PmergeMe(const PmergeMe& other);
	PmergeMe &operator=(const PmergeMe& other);

	char** _source;
	size_t _size;

	vector<int> _jacobstahl;
	vector<int> _stl_sorted;

	double _sortingTime[2]; // because we can't use map anymore
	static const size_t VECTOR = 0;
	static const size_t LIST = 1;
	
	vector<int> mergeSort_vector(char** begin, char** end);
	vector<int> sortAndReportOrder (vector<int>& src);
	vector<int> mergeSort_list(char** begin, char** end);

	void fillJacobstahl();
	int outsiderIdx(int idx, size_t size);

public:
	PmergeMe();
	~PmergeMe();
	void mergeSort(char** begin, char** end);
	void assertSorted(vector<int> tgt) const;
	void testJacobstahl() const;
};

struct pair { // because we used std::pair in btc, and you can't make vector<int[2]>
	int first;
	int second;
	bool operator<(const pair& other) const {
		return first < other.first;
	}
};

#endif