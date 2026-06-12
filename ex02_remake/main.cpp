#include <iostream>
#include <cassert>

#include "PmergeMe.hpp"

using std::cerr;
using std::cout;
using std::endl;
using std::invalid_argument;
using std::string;

int main(int argc, char **argv) {
	if (argc == 1) {
		runTestSuite();
		return 0;
	}
	try {
		PmergeMe sorter;
		vector<myuint> vec;
		if (!parse(argc, argv, vec)) {
			cerr << "Error: Invalid input. Please provide a sequence of positive integers." << endl;
			return 1;
		}
		sorter.sort(vec);
	} catch (const invalid_argument& e) {
		cerr << "Error: " << e.what() << endl;
		return 1;
	}
	return 0;
}
