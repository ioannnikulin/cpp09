#include <iostream>
#include <fstream>

#include "BitcoinExchange.hpp"

using std::cerr;
using std::cout;
using std::endl;
using std::getline;
using std::invalid_argument;
using std::ifstream;
using std::pair;
using std::string;

static void oneRequest(BitcoinExchange &exchange, const string &line) {
	pair<struct tm, float> parsed;
	try {
		parsed = BitcoinExchange::parseLine(line, " | ");
	} catch (const invalid_argument& e) {
		cerr << "Error: " << e.what() << endl;
		return;
	}
	float result;
	try {
		result = exchange.getValue(parsed.first, parsed.second);
	} catch (const invalid_argument& e) {
		cerr << "Error: " << e.what() << endl;
		return;
	}
	cout << line.substr(0, line.find(" | ")) << " => " << parsed.second << " = " << result << endl;
}

int main(int argc, char **argv) {
	if (argc == 1) {
		cerr << "Error: No input file provided." << endl;
		return 1;
	}
	if (argc > 2) {
		cerr << "Error: Too many arguments provided." << endl;
		return 1;
	}
	BitcoinExchange exchange("data.csv");
	ifstream input(argv[1]);
	if (!input.is_open()) {
		cerr << "Error: Could not open file " << argv[1] << endl;
		return 1;
	}
	string buffer;
	getline(input, buffer);
	while (getline(input, buffer)) {
		if (buffer.empty() || buffer[0] == '#') {
			continue;
		}
		oneRequest(exchange, buffer);
	}
	input.close();
	return 0;
}
