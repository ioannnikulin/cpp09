#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include "BitcoinExchange.hpp"

using std::atoi;
using std::clog;
using std::endl;
using std::ifstream;
using std::invalid_argument;
using std::map;
using std::string;

BitcoinExchange::BitcoinExchange(): 
	_rates(map<struct tm, float, BitcoinExchange::tm_less>())
{
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other) {
	if (this == &other)
		return;
	_rates.clear();
	_rates = other._rates;
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other) {
	if (this == &other)
		return *this;
	_rates.clear();
	_rates = other._rates;
	return *this;
}

BitcoinExchange::~BitcoinExchange() {
	_rates.clear();
}

BitcoinExchange::BitcoinExchange(string const& filename):
	_rates(map<struct tm, float, BitcoinExchange::tm_less>())
{
	ifstream file(filename.c_str(), ifstream::in);
	if (!file.is_open()) {
		throw invalid_argument("Could not open file: " + filename);
	}
	string line;
	getline(file, line);
	while (getline(file, line)) {
		pair<struct tm, float> parsed;
		try {
			parsed = BitcoinExchange::parseLine(line, ",");
		} catch (const invalid_argument &e) {
			throw invalid_argument("Error parsing database line: " + line + " - " + e.what());
		}
		_rates[parsed.first] = parsed.second;
	}
	file.close();
}

bool BitcoinExchange::tm_less::operator()(const struct tm &lhs, const struct tm &rhs) const {
	if (lhs.tm_year != rhs.tm_year)
		return lhs.tm_year < rhs.tm_year;
	if (lhs.tm_mon != rhs.tm_mon)
		return lhs.tm_mon < rhs.tm_mon;
	return lhs.tm_mday < rhs.tm_mday;
}

float BitcoinExchange::getValue(struct tm asOf, float btcAmount) const {
	if (btcAmount < 0) {
		throw invalid_argument("Negative BTC amount not allowed.");
	}
	if (btcAmount > BitcoinExchange::MAX_BTC_AMOUNT) {
		throw invalid_argument("Too large a number");
	}
	if (_rates.empty()) {
		throw invalid_argument("No rates available.");
	}
	map<struct tm, float, BitcoinExchange::tm_less>::const_iterator it = --_rates.end();
	if (BitcoinExchange::tm_less()(it->first, asOf)) {
		return it->second * btcAmount;
	}
	map<struct tm, float, BitcoinExchange::tm_less>::const_iterator it1 = _rates.lower_bound(asOf);
	if (BitcoinExchange::tm_less()(asOf, _rates.begin()->first)) {
		throw invalid_argument("No rate available for the given date.");
	}
	return it1->second * btcAmount;
}

struct tm BitcoinExchange::parseDate(const string& date) {
	struct tm tm = {};
	if (date.size() != 10 || date[4] != '-' || date[7] != '-') {
		throw invalid_argument("Invalid date format");
	}
	tm.tm_year = atoi(date.substr(0, 4).c_str()) - 1900;
	if (tm.tm_year < 0) {
		throw invalid_argument("Invalid year in date");
	}
	tm.tm_mon = atoi(date.substr(5, 2).c_str()) - 1;
	if (tm.tm_mon < 0 || tm.tm_mon > 11) {
		throw invalid_argument("Invalid month in date");
	}
	tm.tm_mday = atoi(date.substr(8, 2).c_str());
	if (
		tm.tm_mday < 1
		|| (tm.tm_mon == 1 && tm.tm_mday > 29)
		|| ((tm.tm_mon == 1 && tm.tm_mday == 29 && (tm.tm_year % 4 != 0 || (tm.tm_year % 100 == 0 && tm.tm_year % 400 != 0))))
		|| ((tm.tm_mon == 3 || tm.tm_mon == 5 || tm.tm_mon == 8 || tm.tm_mon == 10) && tm.tm_mday > 30)
		|| ((tm.tm_mon == 0 || tm.tm_mon == 2 || tm.tm_mon == 4 || tm.tm_mon == 6 || tm.tm_mon == 7 || tm.tm_mon == 9 || tm.tm_mon == 11) && tm.tm_mday > 31)
	) {
		throw invalid_argument("Invalid day in date");
	}
	tm.tm_hour = 0;
	tm.tm_min = 0;
	tm.tm_sec = 0;
	tm.tm_isdst = -1;
	mktime(&tm);
	return tm;
}

pair<struct tm, float> BitcoinExchange::parseLine(const string& line, const string& sep) {
	size_t pos = line.find(sep);
	if (pos == string::npos) {
		throw invalid_argument("Invalid input format.");
	}
	string dateStr = line.substr(0, pos);
	string valueStr = line.substr(pos + sep.length());
	struct tm date = BitcoinExchange::parseDate(dateStr);
	float value;
	try {
		value = std::atof(valueStr.c_str());
	} catch (const invalid_argument &) {
		throw invalid_argument("Invalid value format.");
	}
	if (value < 0) {
		throw invalid_argument("Negative value not allowed.");
	}
	return pair<struct tm, float>(date, value);
}
