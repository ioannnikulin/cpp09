#include <iostream>
#include <cassert>

#include "PmergeMe.hpp"

using std::cerr;
using std::cout;
using std::endl;
using std::invalid_argument;
using std::string;

static void tests() {
	PmergeMe sorter;
	sorter.testJacobstahl();
	char *args[] = { (char*)"3", (char*)"5", (char*)"9", (char*)"7", (char*)"4" };
	sorter.mergeSort(args, args + 5);
}

int main(int argc, char **argv) {
	if (argc == 1) {
		tests();
		return 0;
	}
	try {
		PmergeMe sorter;
		sorter.mergeSort(argv + 1, argv + argc);
	} catch (const invalid_argument& e) {
		cerr << "Error: " << e.what() << endl;
		return 1;
	}
	return 0;
}