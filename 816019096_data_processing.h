#pragma once
#include "816019096_structures.h"
#include "816019096_Commons.h"



void getTimeline(vector<Year>& years, std::map<string, day>& PSA);
bool datecmp(string d1, string d2, char delim);
void read_csv(string fname, vector<vector<string>>& data, string read_type);
void getSecInfoGICS(vector<vector<string>>& securities, vector<GICS_Sec>& gicsSecs, vector<security>& secList);

static void addDay(std::map<string, day> *PSA, string date, daySec sec);
daySec get_day_entry(const vector<string>& data);
void getDays(string filename, std::map<string, day>& PSA);

void printPSA(const std::map<string, day>& PSA);
unsigned long long int get_volume(string periodStart, string periodEnd, string tsymb, std::map<string, day>& PSA);
date break_date(string str_date, char delim);
void str_strip(string& str, char token);
long double get_std_dev(vector<unsigned long long int>& population); 