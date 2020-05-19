#include "816019096_Commons.h"
#include "816019096_structures.h"
#include "816019096_Security.h"
#include "816019096_Timer.h"


//ORIGINAL
void find_totals(vector<GICS_Sec>& gics_list, vector<Security>& secs) {
	//Returns updates the annaual total volumes and highest trade volumes per GICS Sector
		//Timer timer;
	for (auto& sector : gics_list) {
		for (auto& sec : secs) {
			if (sec.m_GICS == sector.name) {
				map<string, map<string, unsigned long long int>> yearly_vols = sec.get_annual_volumes();
				for (auto& year : yearly_vols) {
					map<string, map<string, unsigned long long int>>::iterator yFound = sector.totals.find(year.first);
					map<string, map<string, unsigned long long int>>::iterator yFound_highestotals = sector.highest_trades.find(year.first);

					//Incrementing monthly total of the industry and by extension the year structure
					if (yFound == sector.totals.end()) {
						//make a new entry
						sector.totals.insert(pair<string, map<string, unsigned long long int>>(year.first, year.second));
					}
					else {
						for (auto& month : year.second) {
							map<string, unsigned long long int>::iterator mFound = yFound->second.find(month.first);
							if (mFound == yFound->second.end()) {
								//make a new month and then update
								yFound->second.insert(pair<string, unsigned long long int>(month.first, month.second));
							}
							else {
								//update
								mFound->second += month.second;
							}
						}
					}

					//finding the highest volume traded over the years and months of the sector
					if (yFound_highestotals == sector.highest_trades.end()) {
						//make a new entry
						sector.highest_trades.insert(pair<string, map<string, unsigned long long int>>(year.first, year.second));
					}
					else {
						for (auto& month : year.second) {
							map<string, unsigned long long int>::iterator mFound = yFound_highestotals->second.find(month.first);
							if (mFound == yFound_highestotals->second.end()) {
								//make a new month and then update
								yFound_highestotals->second.insert(pair<string, unsigned long long int>(month.first, month.second));
							}
							else {
								//update
								if (mFound->second < month.second) {
									mFound->second = month.second;
								}
							}
						}
					}

				}
			}
		}
	}
} 
void plot_annual_histogram(const vector<GICS_Sec>& gics_list, string year) {
	unsigned long long int buf = 0;
	unsigned long long int tmp = 0;
	map<string, unsigned long long int> annual_performance;

	for (auto& sector : gics_list) {
		for (auto& yr : sector.totals) {
			if (yr.first == year) {
				for (auto& month : yr.second) {
					buf = month.second;
					if (buf >= 0) {
						tmp += month.second;
					}
					else {
						continue;
					}
					//cout << "[XXX]" << tmp << endl;
				}
			}
		}

		annual_performance.insert(pair<string, unsigned long long int>(sector.name, tmp));
		tmp = 0;
	}


	//annual plot

	unsigned long long int scale = 0;
	for (auto& sec : annual_performance) {
		if (sec.second > scale) {
			scale = sec.second;
		}
	}

	if (scale >= 1) {
		scale /= 30;



		cout << setw(15) << "HISTOGRAM SHOWING PERFORMANCE OF GICS SECTORS" << " FOR THE YEAR " << year << endl << endl;
		for (auto& sec : annual_performance) {

			cout << std::setw(30) << std::left << sec.first;
			cout << "|" << string(sec.second / scale, '*') << endl;


		}


		cout << "\n[ ]Each '*' represents " << scale << " units traded" << endl;
	}
	else {
		cout << "[!]No histogram available for year:" << year << endl;
	}
}
void plot_annual_comparison_histogram(const vector<GICS_Sec>& gics_list, string year) {
	vector<unsigned long long int> totals;

	unsigned long long int tmp = 0;
	map<string, vector<unsigned long long int>> annual_performance;



	for (auto& sector :gics_list) {
		for (auto& yr : sector.totals) {
			if (yr.first == year) {
				for (auto& month : yr.second) {
					tmp += month.second;
				}
			}
		}
		totals.push_back(tmp);
		tmp = 0;
		totals.push_back(tmp);
		totals.shrink_to_fit();

		//assigning the monthly highs 
		for (auto& yr : sector.highest_trades) {
			if (yr.first == year) {
				for (auto& month : yr.second) {
					if (month.second > totals[1]) {
						totals[1] = month.second;
					}
				}
			}
		}

		annual_performance.insert(pair<string, vector<unsigned long long int>>(sector.name, totals));
		totals.clear();
	}


	//annual plot

	unsigned long long int scales[2] = { 0,0 };
	for (auto& sec : annual_performance) {
		if (sec.second[0] > scales[0]) {
			scales[0] = sec.second[0];
		}
		if (sec.second[1] > scales[1]) {
			scales[1] = sec.second[1];
		}
	}
	bool invalid = false;
	for (auto& x : scales) {
		x /= 30;
		if (x < 1) {
			invalid = true;
		}
	}

	if (!invalid) {

		cout << setw(15) << "HISTOGRAM SHOWING PERFORMANCE OF GICS SECTORS" << " FOR THE YEAR " << year << endl << endl;
		for (auto& sec : annual_performance) {

			cout << string(30, '-') << endl;
			cout << std::setw(30) << std::left << sec.first;
			cout << "|" << string(sec.second[0] / scales[0], '*') << endl;
			cout << std::setw(31) << std::right;
			cout << "|" << string(sec.second[1] / scales[1], 'x') << endl;


		}
		cout << string(30, '-') << endl;
		cout << "\n[ ]Each '*' represents " << scales[0] << " units traded for total volume traded" << endl;
		cout << "\n[ ]Each 'x' represents " << scales[1] << " units traded for the highest volume traded" << endl;
	}
	else{
		cout << "[!]No histogram available for year:" << year << endl;
	}
}



