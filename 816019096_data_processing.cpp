#include "816019096_structures.h"
#include "816019096_Commons.h"
#include "816019096_Security.h"
#include "816019096_Timer.h"



void str_strip(string& str, char token) {
	str.erase(remove(str.begin(), str.end(), token), str.end());
}

//This is a function used for reading any csv file into a vector of string vectors with the option of headings or no headings specified by rtype
void read_csv(string filename, vector<vector<string>>& data, string rtype) {
	std::ifstream file;
	std::string str;
	std::stringstream ss;
	string token;
	file.open(filename);
	vector<string> fields;
	
	if (!file) {
		throw(std::runtime_error("[!]"+ filename+" could not be found"));
	}

	if (rtype == "h") {
		while (getline(file, str)) {
			ss << str;
			while (getline(ss, token, ',')) { //the 3rd argument must be a character in getline
				str_strip(token, '"');
				fields.push_back(token);
			}
			data.push_back(fields);
			fields.clear();
			ss.clear();
		}
	}
	else if(rtype == "nh"){
		getline(file, str); //throwing away the first line that contais headers
		while (getline(file, str)) {
			ss << str;
			while (getline(ss, token, ',')) { //the 3rd argument must be a character in getline
				str_strip(token, '"');
				fields.push_back(token);
			}
			data.push_back(fields);
			fields.clear();
			ss.clear();
		}

		file.close();
	}

	file.close();
}

date break_date(string str_date, char delim) {
	date d1;
	std::stringstream ss(str_date);
	getline(ss, d1.month, delim);
	getline(ss, d1.day, delim);
	getline(ss, d1.year, delim);

	return d1;

}




//function for adding data entries to unique dates or updating a date with an entry if another occurance of the same date is found in the prices file
static void addDay(std::map<string, day>* PSA, string date, daySec *sec) {
	
	
	
	if (PSA->find(date) == PSA->end()) {
		day* tmp = new day;
		tmp->secTraded.push_back(*sec);
		PSA->insert(std::pair<string, day>(date, *tmp));
	}
	else {
		std::map<string, day>::iterator loc = PSA->find(date);
		loc->second.secTraded.push_back(*sec);
	}
	
}

bool datecmp(string s, string s2, char delim) {
	//tests if the date in s is less than the date in s2

	date d1 = break_date(s,delim);
	date d2 = break_date(s2, delim);

	//converting date fields to numbers
	int d1_yr = stoi(d1.year);		int d2_yr = stoi(d2.year);
	int d1_mth = stoi(d1.month);	int d2_mth = stoi(d2.month);
	int d1_day = stoi(d1.day);		int d2_day = stoi(d2.day);

	if ((d1.day == d2.day) && (d1.month == d2.month) && (d1.year == d2.year)) {
		return false;
	}

	if (d1_yr > d2_yr) {
		return false;
	}
	else {
		if (d1_mth > d2_mth) {
			return false;
		}
		else {
			if (d1_day > d2_day) {
				return false;
			}
			else {
				return true;
			}
		}
	}
}

//printPSA was used for debugging purposes and may be reused later on if this program requires further development
void printPSA(const std::map<string, day>& PSA) {
	for (auto i : PSA) {
		for (auto x : i.second.secTraded) {
			cout << i.first << " : " << x.tsymb << " : " << x.volume << endl;
		}
	}
}

//constucts an entry givnen the vector string from the read_csv function on the prices file
daySec get_day_entry(const vector<string>& data) {
	daySec res;
	res.gicsInd = "NaN";
	res.date = *data.begin();
	res.tsymb = *(data.begin() + 1);
	res.volume = stoi(*(data.begin() + 6));

	return res;
}


//constructs a timeline of all trades
void getDays(string filename, std::map<string, day>& PSA) {
	Timer timer;
	vector<vector<string>> psavec;
	read_csv(filename, psavec,"nh");
	//int sz = psavec.size();
	for (auto &i : psavec) {
		addDay(&PSA, i[0], &get_day_entry(i));
	}
}

//constructs unique GICS structures
void getSecInfoGICS(vector<vector<string>>& securities, vector<GICS_Sec>& gicsSecs, vector<security>& secList) {
	int x = 0; int y = 0;

	for (x = 1; x < securities.size(); x++) {
		security s1;
		s1.tsymb = securities[x][0]; s1.name = securities[x][1]; s1.gicsInd = securities[x][3];
		secList.push_back(s1);

		string curr = securities[x][3];
		if (gicsSecs.empty()) {
			GICS_Sec tmp;
			tmp.name = curr;
			gicsSecs.push_back(tmp);
		}
		else {
			int duplicate = 0;
			for (y = 0; y < gicsSecs.size(); y++) {
				if (curr == gicsSecs[y].name) {
					duplicate = 1;
					break;
				}
			}

			if (duplicate == 0) {
				GICS_Sec tmp;
				tmp.name = curr;
				gicsSecs.push_back(tmp);
			}
		}
	}
}

//determines the volume within a period
unsigned long long int get_volume(string periodStart, string periodEnd, string tsymb, std::map<string, day>& PSA) {
	unsigned long long int tot_vol = 0;

	std::map<string, day>::iterator itStart = PSA.find(periodStart);
	std::map<string, day>::iterator itEnd = PSA.find(periodEnd);
	itEnd++;

	for (itStart; itStart != itEnd; itStart++) {
		for (auto a : itStart->second.secTraded) {
			std::map<string, day>::iterator tmp_it = std::prev(itStart);
			if (datecmp(tmp_it->first, itStart->first, '/')) {
				if (a.tsymb == tsymb) {
					cout << itStart->first << ":" << a.volume << endl;
					tot_vol += a.volume;
				}
			}
			else {
				continue;
			}
		}
	}
	return tot_vol;
}

//GROUPS PSA file into respective years and months
void getTimeline(vector<Year>& years, std::map<string, day>& PSA) {
	string month, year;
	int yrFound; int mthFound;
	for (auto i : PSA) {
		month = break_date(i.first, '/').month; //breaking up the date so it can be used more easily
		year = break_date(i.first, '/').year;

		yrFound = 0; mthFound = 0;

		//checkig if vector is empty
		if (years.empty()) {
			vector<day> entTMP = { i.second };
			Month mthTMP = { month,entTMP };
			vector<Month> mthTMPVec = { mthTMP };
			Year yrTMP = { year,mthTMPVec };
			years.push_back(yrTMP);
		}
		else {
			//checking if year already exists
			for (auto x : years) {
				if (x.year == year) {
					yrFound = 1;
					//checking if month exists
					for (auto y : x.months) {
						if (y.mth == month) {
							mthFound = 1;
							y.entries.push_back(i.second);
							break;
						}
					}
					if (mthFound == 0) {
						vector<day> entTMP = { i.second };
						Month mthTMP = { month,entTMP };
						x.months.push_back(mthTMP);
						break;
					}
				}
			}
			if (yrFound == 0) {
				vector<day> entTMP = { i.second };
				Month mthTMP = { month,entTMP };
				vector<Month> mthTMPVec = { mthTMP };
				Year yrTMP = { year,mthTMPVec };
				years.push_back(yrTMP);
			}
		}


	}
}

//determines the std deviation of a population of ints stored in a vector
long double get_std_dev(vector<unsigned long long int>& population) {
	

	//finding mean
	long double mean = 0;
	for (auto x : population) {
		mean += x;
	}

	int n = (int)population.size();
	mean /= n;

	//finding std dev.
	vector<long double> mean_diff;

	for (auto x : population) {

		mean_diff.push_back(std::pow((x - mean), 2));
	}
	long double std_dev = 0;
	for (auto x : mean_diff) {
		std_dev += x;
	}
	std_dev /= n;
	std_dev = std::sqrt(std_dev);

	return std_dev;
}



