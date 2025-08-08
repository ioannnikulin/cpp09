#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <cstddef>
#include <map>
#include <vector>
#include <ostream>

using std::ostream;
using std::vector;

struct intm {
	int v;
	size_t origPos;
	bool operator<(const intm& other) const;
};

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
	
	vector<int> vector_mergeSort(char** begin, char** end);
	vector<intm> vector_doSort(vector<intm> src, int pad);
	vector<int> list_mergeSort(char** begin, char** end);

	void fillJacobstahl(size_t sz);

public:
	PmergeMe();
	~PmergeMe();
	void mergeSort(char** begin, char** end);
	void assertSorted(vector<int> tgt) const;
	void testJacobstahl() const;
};

/*
struct pair { // because we used std::pair in btc, and you can't make vector<int[2]>
	int first;
	int second;
	bool operator<(const pair& other) const {
		return first < other.first;
	}
};
*/

std::ostream& operator<<(std::ostream& os, const intm& obj);

void test99();
void test3000();

#endif
