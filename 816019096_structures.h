#pragma once
#include "816019096_Commons.h"


using std::string;
using std::vector;
using std::pair;
using std::map;


struct daySec {
	string date;
	string tsymb;//ticker symbol
	string gicsInd;
	int volume = 0;
};

struct day {
	vector<daySec> secTraded;
};

struct Month {
	string mth;
	vector<day> entries;
};

struct Year {
	string year;
	vector<Month> months;
};


struct security {
	string name;
	string tsymb;//ticker symbol
	string gicsInd;
	vector<Year> years;
};


struct date {
	string day;
	string month;
	string year;
};

struct GICS_Sec {
	string name;
	vector<Year> timeline;
	map<string, map<string, unsigned long long int>> totals; //first - year, second-month
	map<string, map<string, unsigned long long int>> highest_trades; //first - year, second-month, this field shows the highest volumes from this industry per month and year
	string best_securities[5];
	string worst_securities[5];

};
