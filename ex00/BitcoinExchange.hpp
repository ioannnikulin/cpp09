#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <ctime>
#include <map>
#include <string>

using std::map;
using std::pair;
using std::string;

class BitcoinExchange {
private:
	BitcoinExchange();
	BitcoinExchange(const BitcoinExchange& other);
	BitcoinExchange &operator=(const BitcoinExchange& other);
	struct tm_less {
		bool operator()(const struct tm &lhs, const struct tm &rhs) const;
	};
	map<struct tm, float, tm_less> _rates;
	static const int MAX_BTC_AMOUNT = 1000;

public:
	~BitcoinExchange();
	BitcoinExchange(std::string const& filename);
	float getValue(struct tm asOf, float btcAmount) const;
	static struct tm parseDate(const std::string& date);
	static pair<struct tm, float> parseLine(const string &line, const string& sep);
};

#endif
