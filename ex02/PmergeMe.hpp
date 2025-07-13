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

	double _sortingTime[2]; // because we can't use map anymore
	static const size_t VECTOR = 0;
	static const size_t LIST = 1;
	
	vector<int> mergeSort_vector(char** begin, char** end);
	vector<int> mergeSort_list(char** begin, char** end);

	static size_t tk(size_t k);

public:
	PmergeMe();
	~PmergeMe();
	void mergeSort(char** begin, char** end);
	void assertSorted(vector<int> tgt) const;
	void testTk() const;
};

struct pair { // because we used std::pair in btc, and you can't make vector<int[2]>
	int first;
	int second;
};

#endif