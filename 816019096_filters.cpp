#include "816019096_Commons.h"
#include "816019096_structures.h"
#include "816019096_Security.h"
#include "816019096_data_processing.h"
#include "816019096_Timer.h"

//creates a unique timeline with each years and months trade stored sequentially
void create_PSA_timeline(const map<string, day>& PSA_map, map <string, map<string, vector<daySec>>> &res) {
	Timer timer ;
	for (auto &record : PSA_map) {

		date d1 = break_date(record.first,'/');
		for (auto &trade : record.second.secTraded) {
			if (res.empty()) {
				//creating map to store month and data
				map<string, vector<daySec>> tmp;
				vector<daySec> tmpVec= { trade };
				tmp.insert(pair <string, vector<daySec>>(d1.month,tmpVec));
				res.insert(pair < string, map<string, vector<daySec>>>(d1.year,tmp));
				
			}
			else {
				//determine if the year exists
				if (res.find(d1.year) == res.end()) {
					map<string, vector<daySec>> tmp;
					vector<daySec> tmpVec = { trade };
					tmp.insert(pair <string, vector<daySec>>(d1.month, tmpVec)); //adding the created month
					res.insert(pair < string, map<string, vector<daySec>>>(d1.year, tmp)); //adding a new year with the created month
					
				}
				else {
					
					//get location of the year
					map<string, map<string, vector<daySec>>>::iterator it_yr = res.find(d1.year);
					//checking to see if month exists
					if (it_yr->second.find(d1.month) == it_yr->second.end()) {
						//make a new month entry because it doesn't exist
						map<string, vector<daySec>> tmp;
						vector<daySec> tmpVec = { trade };
						it_yr->second.insert(pair <string, vector<daySec>>(d1.month, tmpVec)); //adding a new month to the current year
						
					}
					else {
						map<string, vector<daySec>>::iterator it_mth = it_yr->second.find(d1.month);
						it_mth->second.push_back(trade);
					}
				}
			}
		}
	}
	
}

//creates timelines for individual Security objects
map<string, map<string, vector<daySec>>> create_company_timeline(map <string, map<string, vector<daySec>>>* PSA_timeline, string* tsymb) {
	map<string, map<string, vector<daySec>>> res;
	for (auto& year : *PSA_timeline) {
		map<string, vector<daySec>> comp;
		for (auto& month : year.second) {
			vector<daySec> monthly_trades;
			for (auto& record : month.second) {
				if (record.tsymb == *tsymb) {
					monthly_trades.push_back(record);
				}
			}
			comp.insert(pair<string, vector<daySec>>(month.first, monthly_trades));
		}
		res.insert(pair<string, map<string, vector<daySec>>>(year.first, comp));
	}

	return res;
}

map<string, map<string, vector<daySec>>> create_company_timeline(const map <string, map<string, vector<daySec>>> &PSA_timeline,const string &tsymb) {
	map<string, map<string, vector<daySec>>> res;
	for (auto& year : PSA_timeline) {
		map<string, vector<daySec>> comp;
		for (auto& month : year.second) {
			vector<daySec> monthly_trades;
			for (auto& record : month.second) {
				if (record.tsymb == tsymb) {
					monthly_trades.push_back(record);
				}
			}
			comp.insert(pair<string, vector<daySec>>(month.first, monthly_trades));
		}
		res.insert(pair<string, map<string, vector<daySec>>>(year.first, comp));
	}

	return res;

}
