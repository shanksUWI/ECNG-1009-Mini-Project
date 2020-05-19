#pragma once
#include "816019096_Commons.h"
#include "816019096_structures.h"
#include "816019096_Security.h"


void find_totals(vector<GICS_Sec>& gics_list, vector<Security>& secs); //finds the total volumes, highest volumes and lowest volumes traded per period and updates the sector struct
void plot_annual_histogram(const vector<GICS_Sec>& gics_list, string year); //plots the total volumes of each industry over the course of a year
void plot_annual_comparison_histogram(const vector<GICS_Sec>& gics_list, string year);//plots a histogram with each industry's total and higest trades, *-total volume, 'x'- highest volume


//EXPERIMENTAL
//void find_totals(vector<GICS_Sec>* gics_list, vector<Security>* secs); //finds the total volumes, highest volumes and lowest volumes traded per period and updates the sector struct
//void plot_annual_histogram(const vector<GICS_Sec>* gics_list, string *year); //plots the total volumes of each industry over the course of a year
//void plot_annual_comparison_histogram(const vector<GICS_Sec>* gics_list, string *year);//plots a histogram with each industry's total and higest trades, *-total volume, 'x'- highest volume
