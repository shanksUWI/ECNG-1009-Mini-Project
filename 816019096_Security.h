#pragma once
#include "816019096_Commons.h"
#include "816019096_structures.h"
#include "816019096_filter.h"
#include "816019096_data_processing.h"


class Security
{ 
public:
	map<string, map<string, vector<daySec>>> m_timeline;
	map<string, map<string, vector<daySec>>>::iterator m_it_year;
	map<string, vector<daySec>>::iterator m_it_month;

	string m_sec_name;
	string m_tsymb;
	string m_GICS;

	unsigned int tmp = 0;
	map<string, map<string,long double>> m_std_devs; //std dev for each year

	Security(map<string, map<string, vector<daySec>>>& PSA_timeline, const string &tsymb);
	Security(map<string, map<string, vector<daySec>>>& PSA_timeline, const string &tsymb,const string &gics);
	Security();

	void late_init(map<string, map<string, vector<daySec>>>* PSA_timeline, string* tsymb, string* gics);

	//EXPERIMENTAL
	Security(map<string, map<string, vector<daySec>>>* PSA_timeline, string* tsymb, string* gics);

	unsigned long long int get_monthly_volume(const string &month, const string &year);

	unsigned long long int get_annual_volume(const string &year);

	unsigned long long int get_period_total(const string &pStart, const string &pEnd,char delim);

	map<string, map<string, unsigned long long int>> get_annual_volumes();
	map<string, unsigned long long int> get_monthly_volumes(const string &year);
	void plot_monthly_histogram(const string &year);


	void print_statistical_parameters(); //print a table showing the std dev for each month and each year available
};
