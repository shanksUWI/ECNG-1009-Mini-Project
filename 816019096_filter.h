#include "816019096_Commons.h"
#include "816019096_structures.h"
#include "816019096_data_processing.h"
#include "816019096_Security.h"



//The below function takes any map defined by a date string key and returns a new map defined by yearly and monthly keys
void create_PSA_timeline(const map<string, day>& PSA_map, map <string, map<string, vector<daySec>>> &res); 
map<string, map<string, vector<daySec>>> create_company_timeline(const map <string, map<string, vector<daySec>>>& PSA_timeline, const string& tsymb);

map<string, map<string, vector<daySec>>> create_company_timeline(map <string, map<string, vector<daySec>>>* PSA_timeline, string* tsymb);
