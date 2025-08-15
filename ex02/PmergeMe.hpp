#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <cstddef>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <vector>
#include <ostream>

using std::advance;
using std::cout;
using std::endl;
using std::list;
using std::max;
using std::min;
using std::ostream;
using std::setw;
using std::vector;

struct intm {
	int v;
	size_t origPos;
	bool operator<(const intm& other) const;
};

//#define DEBUG

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
	list<int> list_mergeSort(char** begin, char** end);

	void fillJacobstahl(size_t sz);

public:
	PmergeMe();
	~PmergeMe();
	void mergeSort(char** begin, char** end);
	void testJacobstahl() const;

private:
	template <typename T>
	vector<T> makePairs(T src) {
		vector<T> pairs;
		typename T::iterator i0 = src.begin();
		typename T::iterator i1 = src.begin();
		i1 ++;
		for (
			size_t i = 0;
			i < src.size() - src.size() % 2;
			i+=2, advance(i0, 2), advance(i1, 2)
		) {
			T pr;
			pr.push_back(max(*i0, *i1));
			pr.push_back(min(*i0, *i1));
			pairs.push_back(pr);
		}
		return pairs;
	}

	template <typename T>
	void onlyTwo(T& src) {
		typename T::iterator i0 = src.begin();
		typename T::iterator i1 = src.begin();
		i1 ++;
		if (*i1 < *i0) {
			std::iter_swap(i0, i1);
		}
	}

	template <typename T>
	T extractTops(const vector<T>& pairs) {
		T tops;
		int ii = 0;
		for (
			typename vector<T>::const_iterator i = pairs.begin();
			i < pairs.end();
			++i, ++ii
		) {
			intm item;
			item.v = (*((*i).begin())).v;
			item.origPos = ii; // internal numbering for recursive call, will be reverted
			tops.push_back(item);
		}
		return tops;
	}

	template <typename T>
	vector<T> reorganizePairsByTops(T& tops, vector<T>& pairs, T& src) {
		vector<T> sortedPairs;
		for (
			typename T::iterator i = tops.begin();
			i != tops.end(); 
			++i
		) {
			typename vector<T>::iterator p = pairs.begin();
			advance(p, i->origPos);
			sortedPairs.push_back(*p);
		}
		if (src.size() % 2 == 1) {
			T lastPair;
			intm dummy;
			lastPair.push_back(dummy);
			typename T::iterator last = src.end();
			--last;
			lastPair.push_back(*last);
			sortedPairs.push_back(lastPair);
		}
		return sortedPairs;
	}

	template <typename T>
	void restoreOriginalPositions(vector<T>& sortedPairs, T& tops) {
		for (
			typename T::iterator i = tops.begin(); 
			i != tops.end();
			++i
		) {
			typename vector<T>::iterator p = sortedPairs.begin();
			advance(p, i->origPos);
			(*i).origPos = p->begin()->origPos;
		}
	}

	template <typename T>
	void insertB1AtStart(vector<T>& sortedPairs, T& tops) {
		typename vector<T>::iterator b1 = sortedPairs.begin();
		typename T::iterator it = b1->begin();
		++it;
		tops.insert(tops.begin(), *it); // b1 smallest
	}

	template <typename T>
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

	template <typename T>
	T doSort(T src, int pad) {
		#ifdef DEBUG
		cout << setw(pad * 4) << " " << "sort called with: ";
		for (typename T::iterator i = src.begin(); i != src.end(); i++ ) {
			cout << *i << " ";
		}
		cout << endl;
		#endif

		if (src.size() == 1) {
			return (src);
		}
		if (src.size() == 2) {
			onlyTwo(src);
			return (src);
		}

		vector<T> pairs = makePairs(src);

		// this container will be used as result accumulator 
		// and returned in the end
		T tops = extractTops(pairs);

		tops = doSort(tops, pad + 1);

		#ifdef DEBUG
		cout << setw(pad * 4) << " " << "Result vector after recursive call: ";
		for (typename T::iterator i = tops.begin(); i != tops.end(); ++i) {
			cout << *i << " ";
		}
		cout << endl;
		#endif

		vector<T> sortedPairs = reorganizePairsByTops(tops, pairs, src);

		restoreOriginalPositions(sortedPairs, tops);

		insertB1AtStart(sortedPairs, tops);
		#ifdef DEBUG
		cout << setw(pad * 4) << " " << "Result vector after B1 insertion: ";
		for (typename T::iterator i = tops.begin(); i != tops.end(); ++i) {
			cout << *i << " ";
		}
		cout << endl;
		#endif

		fillJacobstahl(sortedPairs.size());
		// other outsiders (b's)
		for (size_t i = 1; i < sortedPairs.size(); ++i) {
			int from = _jacobstahl.at(i);
			#ifdef DEBUG
			cout << setw(pad * 4) << " " << "Inserting " << i << "th outsider: idx " << from << endl;
			#endif
			typename vector<T>::iterator it = sortedPairs.begin();
			advance(it, from);
			typename T::iterator itb = (*it).begin();
			++itb;
			intm who = *itb;
			int where = binaryInsert(tops, who, 0, tops.size());
			#ifdef DEBUG
			cout << setw(pad * 4) << " " << "Inserting " << who.v << " at position " << where << endl;
			cout << setw(pad * 4) << " " << "Result vector after " << i << "th pass: ";
			for (typename T::iterator i = tops.begin(); i != tops.end(); ++i) {
				cout << *i << " ";
			}
			cout << endl;
			#else
			(void)pad;
			(void)where;
			#endif
		}
		return (tops);
	}

	template <typename T>
	void assertSorted(T& tgt) const {
		typename T::iterator prev = tgt.begin();
		typename T::iterator cur = prev;
		++cur;
		for (
			size_t i = 1;
			i < _size;
			++i, ++cur, ++prev
		) {
			if (*cur < *prev) {
				throw std::runtime_error("The array is not sorted");
			}
			if (*cur != _stl_sorted[i]) {
				throw std::runtime_error("The array's contents doesn't match the source");
			}
		}
	}
};

std::ostream& operator<<(std::ostream& os, const intm& obj);

void test99();
void test3000();

#endif
