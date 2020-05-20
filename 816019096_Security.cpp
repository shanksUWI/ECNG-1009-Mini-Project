#pragma once
#include "816019096_Security.h"
#include "816019096_Commons.h"
#include "816019096_data_processing.h"

Security::Security(map<string, map<string, vector<daySec>>>& PSA_timeline, const string &tsymb) {
	m_timeline = create_company_timeline(PSA_timeline, tsymb);
	m_tsymb = tsymb;

	map<string,map<string, vector<unsigned long long int>>> pop;



	//constructing population
	vector<unsigned long long int> month_totals;
	
	for (auto& year : m_timeline) {
		map<string, vector<unsigned long long int>> tmp_yr;
		for (auto& month : year.second) {
			for (auto& day : month.second) {
				month_totals.push_back(day.volume);
			}
			tmp_yr.insert(pair<string, vector<unsigned long long int>>(month.first, month_totals));
			month_totals.clear();
		}
		pop.insert(pair<string, map<string, vector<unsigned long long int>>>(year.first, tmp_yr));
	}

	//finding std devs for each pop
	for (auto& year : pop) {
		map<string, long double> tmp_yr;
		
		for (auto& month : year.second) {
			tmp_yr.insert(pair<string,long double>(month.first, get_std_dev(month.second)));
		}

		m_std_devs.insert(pair<string, map<string,long double>>(year.first, tmp_yr));
	}

}
Security::Security(map<string, map<string, vector<daySec>>>* PSA_timeline, string *tsymb, string *gics){
	m_timeline = create_company_timeline(*PSA_timeline, *tsymb);

	m_tsymb = *tsymb;
	m_GICS = *gics;

	map<string, map<string, vector<unsigned long long int>>> pop;

	//constructing pop
	vector<unsigned long long int> month_totals;
	for (auto& year : m_timeline) {
		map<string, vector<unsigned long long int>> tmp_yr;
		for (auto& month : year.second) {
			for (auto& day : month.second) {
				month_totals.push_back(day.volume);
			}

			tmp_yr.insert(pair<string, vector<unsigned long long int>>(month.first, month_totals));
			month_totals.clear();
		}
		pop.insert(pair<string, map<string, vector<unsigned long long int>>>(year.first, tmp_yr));
	}

	//finding std devs for each pop
	for (auto& year : pop) {
		map<string, long double> tmp_yr;
		
		for (auto& month : year.second) {
			tmp_yr.insert(pair<string, long double >(month.first, get_std_dev(month.second)));
		}

		m_std_devs.insert(pair<string, map<string, long double>>(year.first, tmp_yr));
	}
	}
Security::Security(map<string, map<string, vector<daySec>>>& PSA_timeline, const string &tsymb,const string &gics) {
	m_timeline = create_company_timeline(PSA_timeline, tsymb);
	m_tsymb = tsymb;
	m_GICS = gics;

	map<string, map<string, vector<unsigned long long int>>> pop;

	//append to pop
	//for each element in pop find the std dev and make a new entry in m_std_devs

	//constructing pop
	vector<unsigned long long int> month_totals;
	for (auto& year : m_timeline) {
		map<string, vector<unsigned long long int>> tmp_yr;
		for (auto& month : year.second) {
			for (auto& day : month.second) {
				month_totals.push_back(day.volume);
			}

			tmp_yr.insert(pair<string, vector<unsigned long long int>>(month.first, month_totals));
			month_totals.clear();
		}
		pop.insert(pair<string, map<string, vector<unsigned long long int>>>(year.first, tmp_yr));
	}

	//finding std devs for each pop
	for (auto& year : pop) {
		map<string, long double> tmp_yr;
		
		for (auto& month : year.second) {
			tmp_yr.insert(pair<string, long double>(month.first, get_std_dev(month.second)));
		}

		m_std_devs.insert(pair<string, map<string, long double>>(year.first, tmp_yr));
	}
}


Security::Security() {
	cout << "[+]Empty Security Object created" << endl;
}

//a function allowing for late intilization(this was because i was playing around with multithreading and was tryna speed up creation)
void Security::late_init(map<string, map<string, vector<daySec>>>* PSA_timeline, string* tsymb, string* gics) {
	m_timeline = create_company_timeline(PSA_timeline, tsymb);
	m_tsymb = *tsymb;
	m_GICS = *gics;

	map<string, map<string, vector<unsigned long long int>>> pop;

	//constructing pop
	vector<unsigned long long int> month_totals;
	for (auto& year : m_timeline) {
		map<string, vector<unsigned long long int>> tmp_yr;
		for (auto& month : year.second) {
			for (auto& day : month.second) {
				month_totals.push_back(day.volume);
			}

			tmp_yr.insert(pair<string, vector<unsigned long long int>>(month.first, month_totals));
			month_totals.clear();
		}
		pop.insert(pair<string, map<string, vector<unsigned long long int>>>(year.first, tmp_yr));
	}

	//finding std devs for each pop
	for (auto& year : pop) {
		map<string, long double > tmp_yr;
		//vector<float> mth_std_devs;
		for (auto& month : year.second) {
			tmp_yr.insert(pair<string, long double>(month.first, get_std_dev(month.second)));
		}

		m_std_devs.insert(pair<string, map<string, long double >>(year.first, tmp_yr));
	}
}

unsigned long long int Security::get_monthly_volume(const string &month,const string &year) {
	unsigned long long res = 0;

	m_it_year = m_timeline.find(year);
	if (m_it_year == m_timeline.end()) {
		return res;
	}
	else {
		m_it_month = m_it_year->second.find(month);
		if (m_it_month == m_it_year->second.end()) {
			return res;
		}
		else {
			for (auto trade : m_it_month->second) {
				res += trade.volume;
			}
			return res;
		}

	}

}

unsigned long long int Security::get_annual_volume(const string &year) {
	unsigned long long res = 0;

	m_it_year = m_timeline.find(year);
	if (m_it_year == m_timeline.end()) {
		return res;
	}
	else {
		for (auto month : m_it_year->second) {
			res += get_monthly_volume(month.first,year);
		}
		return res;
	}

}


unsigned long long int Security::get_period_total(const string &pStart, const string &pEnd,char delim) {
	unsigned long long int res = 0;
	unsigned long long int diff = 0;
	
	date periodStart = break_date(pStart, delim);
	date periodEnd = break_date(pEnd, delim);  
	
	bool pStart_found = false;
	bool pEnd_found = false;


	for (auto& year : m_timeline) {
		for (auto& month : year.second) {
			for (auto& trade : month.second) {
				if (trade.date == pStart) {
					pStart_found = true;
				}
				else if (trade.date == pEnd) {
					pEnd_found = true;
				}
				if ((pStart_found == true) && (pEnd_found == true)) { break; }
			}
		}
	}

	if ((pStart_found == true) && (pEnd_found == true)) {

		//iterators for period year and month limits
		map<string, map<string, vector<daySec>>>::iterator pStart_itr = m_timeline.find(periodStart.year);
		map<string, vector<daySec>>::iterator pStart_subitr = pStart_itr->second.find(periodStart.month);
		map<string, map<string, vector<daySec>>>::iterator pEnd_itr = m_timeline.find(periodEnd.year);
		map<string, vector<daySec>>::iterator pEnd_subitr = pEnd_itr->second.find(periodEnd.month);


		if (periodStart.year <= periodEnd.year) {
			if (stoi(periodStart.year) < stoi(periodEnd.year)) {
				

				//finding the sum of all volumes before the start of the period in the period month and assigning to diff
				for (auto trade : pStart_subitr->second) {
					if (datecmp(trade.date, pStart, delim)) {
						diff += trade.volume;
						
					}
				}

				
				//finding the sum of all volumes after the end of the period in the period end month
				for (auto trade : pEnd_subitr->second) {
					if (trade.date == pEnd) {
					}
					else {
						if (!datecmp(trade.date, pEnd, delim) && (trade.date != pEnd)) {
							diff += trade.volume;
							
						}
					}
				}
				

				//iterating through each year and finding the sum of each month untill the end is reached, ignoring months before pstart month
				for (auto& year : m_timeline) {
					if (stoi(year.first) < stoi(pStart_itr->first)) {
						continue;
					}
					else if((stoi(year.first) > stoi(pEnd_itr->first))){
						continue;
					}
					else {
						//determine if is first
						for (auto& month : year.second) {
							if (year.first == periodStart.year) {
								if (month.first >= periodStart.month) {
									for (auto& trade : month.second) {
										cout << "[!|!]Res update :" << trade.date << " : " << trade.volume << endl;
										res += trade.volume;
									}
								}
							}
							else if(stoi(year.first) < stoi(periodEnd.year)){
								for (auto& trade : month.second) {
									cout << "[!|!]Res update :" << trade.date << " : " << trade.volume << endl;
									res += trade.volume;
								}
							}
							else if (year.first == periodEnd.year) {
								if (stoi(month.first) <= stoi(periodEnd.month)) {
									for (auto& trade : month.second) {
										cout << "[!|!]Res update :" << trade.date << " : " << trade.volume << endl;
										res += trade.volume;
									}
								}
							}
						}
					}
				}
				

				res -= diff;
			}
			else if(periodStart.year == periodEnd.year){
				if (periodStart.month <= periodEnd.month) {
					if (periodStart.month == periodEnd.month) {
						res = 0;
						for (auto& trade : pStart_subitr->second) {
							if ((datecmp(trade.date, pStart, '/')) && (pStart != trade.date)){
								continue;
							}
							else
							{
								if (pStart == trade.date) {
									res += trade.volume;
								}
								else if (trade.date == pEnd) {
									res += trade.volume;
								} 
								else if((datecmp(pStart, trade.date, '/')) && datecmp(trade.date,pEnd,'/')){
									res += trade.volume;
								}
							}
						}
					}
					else {
				
						//finding the sum of all volumes before the start of the period in the period month and assigning to diff
						for (auto& trade : pStart_subitr->second) {
							if (datecmp(trade.date, pStart, delim)) {
								diff += trade.volume;
							}
							else {
								continue;
							}
						}
						//finding the sum of all volumes after the end of the period in the period end month
						for (auto& trade : pEnd_subitr->second) {
							if (trade.date == pEnd) {
							}
							else {
								if (!datecmp(trade.date, pEnd, delim) && (trade.date != pEnd)) {
									diff += trade.volume;
								}
							}
							
						}
						
						//iterating through each month from the period start month to the period end(inclusive) and summing all values to res.
						for (auto& month : pStart_itr->second) {
							int mth_tmp = stoi(month.first);
							int pLower = stoi(periodStart.month);
							int pUpper = stoi(periodEnd.month);

							if (mth_tmp < pLower) {
								continue;
							}
							else if (mth_tmp > pUpper){
								continue;
							}
							else {
								for (auto& trade : month.second) {
									res += trade.volume;
								}
							}
						}
						res -= diff;
					}
				}
				else {
					throw std::domain_error("Please ensure the start of the period you are entering is less than or equal to the end ");
				}
			}
		}
		else {
			throw std::domain_error("Please ensure the start of the period you are entering is less than or equal to the end");
		}
	}
	else {
		throw std::domain_error("Invalid period entered, one of the period limits could not be found");
	}

	return res;
}


map<string, unsigned long long int> Security::get_monthly_volumes(const string &year) {

	map<string, unsigned long long int> monthly_volumes;
	

	//initializing vector to all zeros
	for (int x = 0; x < 12; x++) {
		monthly_volumes.insert(pair<string, unsigned long long int>(std::to_string(x + 1),0));
	}

	m_it_year = m_timeline.find(year);


	if (m_it_year == m_timeline.end()) {
		return monthly_volumes;
		cout << "Ended early" << endl;
	}
	else {

		for (auto &i : monthly_volumes) {
			i.second = get_monthly_volume(i.first, year);
		}


		return monthly_volumes;
	}
}

map<string, map<string, unsigned long long int>> Security::get_annual_volumes() {
	map<string, map<string, unsigned long long int>> res;

	for (auto& year : m_timeline) {
		map<string, unsigned long long int> tmp = get_monthly_volumes(year.first);
		res.insert(pair<string, map<string, unsigned long long int>>(year.first, tmp));
	}

	return res;
}

void Security::plot_monthly_histogram(const string &year) {
	map<string, unsigned long long int> performance = get_monthly_volumes(year);
	
	unsigned long long int scale = 0;
	for (auto& mth : performance) {
		if (mth.second > scale) {
			scale = mth.second;
		}
	}

	if (scale >= 1) {
		scale /= 30;
		string tmp;


		cout << setw(15) << "HISTOGRAM SHOWING PERFORMANCE OF " << m_tsymb << " for the year " << year << endl << endl;

		for (int x = 1; x < 13; x++) {
			tmp = std::to_string(x);
			auto itr = performance.find(tmp);
			tmp = "Month " + itr->first;
			cout << std::setw(10) << std::left << tmp;
			cout << "|" << string(itr->second / scale, '*') << endl;
		}

		cout << "\n[ ]Each '*' represents " << scale << " units traded" << endl;

	}
	else {
		cout << "[!]No histogram available for year:" << year << endl;
	}
	
}

void Security::print_statistical_parameters() {
	string tmp_sub;
	//Calculate a table of std deviations for each year or month(that is available)
	string tmp;
	for (auto yr : m_std_devs) {
		cout << string(30, '-') << endl;
		tmp = "YEAR " + yr.first;
		cout << setw(20)<<right<< tmp<< endl;
		cout << string(30, '-') << endl;

		for (int x = 1; x < 13; x++) {
			tmp_sub = std::to_string(x);
			auto itr = yr.second.find(tmp_sub);
			if (itr == yr.second.end()) {
				continue;
			}
			else {
				tmp_sub = "Month " + itr->first;
				cout << std::setw(10) << std::left << tmp_sub;
				cout << setw(15) << right << itr->second << endl;
			}
		}
		cout << string(30, '-') << endl;
		cout << "\n" << endl;
	}
	
}