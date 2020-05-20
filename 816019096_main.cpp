#pragma once
#include "816019096_Commons.h"
#include "816019096_structures.h"
#include "816019096_data_processing.h"
#include "816019096_filter.h"
#include "816019096_gics_handling.h"
#include "816019096_Security.h"
#include "816019096_Timer.h"





struct sec_request {
	Security req_sec;
	bool req_status;
};

struct indiv_gics_request {
	vector<Security> secs;
	bool req_status;
};



sec_request get_security_record(string u_tsymb, map<string, map<string, vector<daySec>>>* psa_timeline, vector<security>* security_list) {
	//creating a structure for Security objects that encapsulates if they exist in the securities.csv file
	sec_request req;
	std::cout.setstate(std::ios_base::failbit);
	bool found = false;
	for (auto& x : *security_list) {
		if (x.tsymb == u_tsymb) {
			Security tmp(psa_timeline, &u_tsymb, &x.gicsInd);
			found = true;
			//__debugbreak();
			req.req_status = found;
			req.req_sec = tmp;
			std::cout.clear();
			return req;
			
		}
	}

	if (found != true) {
		req.req_status = false;
		req.req_sec = Security();
		std::cout.clear();
	}
	return req;
	
}


int main_menu(vector<Security>& main_sec_db,map<string, map<string, vector<daySec>>>& psa_timeline, vector<security>& security_list, vector<GICS_Sec>& gics_list)
{
	int option = 0;
	int option_tmp = 0;
	int generic_counter = 0;
	
	system("cls");
	cout << "===================================================" << endl;
	cout << "	New York Stock Exchange Analyzer	" << endl;
	cout << "===================================================\n\n\n"
		<< "1)Analyze a security\n"
		<< "2)Analyze GICS sectors' performances for a specific year\n"
		<< "3)Analyze GICS Sectors' performances over the available years\n"
		<< "4)Exit" << endl;

	cout << ">";
	cin >> option;
	
	switch (option)
	{
		case 1: {
			bool found = false;
			//finding ticker
			string ticker;
			system("cls");
			cout << "\n[!]Ensure the symbol entered is completely uppercase\n"
				<< "Please enter the ticker symbol for the security you wish to query >";
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			getline(cin, ticker);
			sec_request req = get_security_record(ticker, &psa_timeline, &security_list);
			found = req.req_status;

			if (!found) {
				cerr << "\n[!]Ticker not found, please enter a valid ticker symbol and ensure all letters are uppercase.";
				cout << "\nPress enter to continue."; cin.ignore();
				return 0;
			}
			else {
				system("cls");
				cout << "\nWhat type of query would you like to perform?\n"
					<< "1)Volume traded within a specific period(date-date)\n"
					<< "2)Volume traded within a specific month\n"
					<< "3)Volume traded within a Specific year\n" 
					<< "4)Table of standard deviations\n"
					<< "5)Plot histograms of a company's performance over their available years\n"
					<< "6)Return to main menu\n"
					<< endl << endl;
				cout << "[!]Dates are intepreted as MM/DD/YYYY\nEg. The 1st of February 2010 would be entered as 2/1/2010\n" << endl;
				cout << ">";
				cin >> option_tmp;
				try {
					switch (option_tmp) {
					case 1: {
						cout << "[!]Both the start and end dates must be an existing entry in the dataset, if they are not,\n"
							<< "please use dates close to valid entries by looking at the period in the dataset" << endl;
						string d1, d2;
						cout << "\nPlease enter the dates\n"
							<< "Start:";
						cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
						getline(cin, d1);
						cout << "\nEnd:";
						getline(cin, d2);
						cout << "\nThe total traded volume from " << d1 << " to " << d2 << " is " << req.req_sec.get_period_total(d1, d2, '/');
						cout << "\nPress enter to continue.";
						std::cin.clear();
						std::cin.ignore(32767, '\n');
						return 0;
					}break;
					case 2: {
						string month, year;
						cout << "\nPlease enter the month and year\n"
							<< "Month:";
						cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
						getline(cin, month);

						cout << "\nYear:";
						getline(cin, year);
						cout << "\nThe total volume traded for the month " << month << " in " << year << " is " << req.req_sec.get_monthly_volume(month, year);
						cout << "\nPress enter to continue.";
						std::cin.clear();
						std::cin.ignore(32767, '\n');
						return 0;
					}break;
					case 3: {
						string year;
						cout << "\nPlease enter the year\n"
							<< "Year:";
						cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
						getline(cin, year);
						cout << "\nTotal volume traded for " << year << " " << req.req_sec.get_annual_volume(year);
						cout << "\nPress enter to continue.";
						std::cin.clear();
						std::cin.ignore(32767, '\n');
						return 0;
					}break;
					case 4: {
						cout << "[!]If no entries exist, nan(ind) is printed instead" << endl;
						req.req_sec.print_statistical_parameters();
						cout << "\nPress enter to continue.";
						std::cin.clear();
						std::cin.ignore(32767, '\n');
						return 0;
					}break;
					case 5: {
						for (auto& yr : req.req_sec.m_timeline) {
							req.req_sec.plot_monthly_histogram(yr.first);
						}
						std::cin.clear();
						cout << "\nPress enter to continue.";
						std::cin.ignore(32767, '\n');
						return 0;
					}break;
					case 6: {return 0; }break;
					default: {
						cerr << "\n[!]Invalid selection\nPress enter to continue.";
						std::cin.clear();
						std::cin.ignore(32767, '\n');
						return 0;
					}break;
					}
				}
				catch(std::exception& e){
					throw e;
				}
				
			}
		}break;
		case 2: {
			string year;

			system("cls");
			cout << "\n[!]Ensure the entered year is in the YYYY format, Eg, 2020,2010,2016" << endl;
			cout << "\nPlease enter the year you want displayed:";
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			getline(cin, year);
			

			bool found_yr = false;
			for (auto& yr : psa_timeline) {
				if (yr.first == year) {
					found_yr = true;
					break;
				}
			}
			if (!found_yr) {
				cout << "\n[!]Invalid year entered.\nPress enter to continue." << endl;
				std::cin.clear();
				std::cin.ignore(32767, '\n');
				return 0;
			}

			cout << "\nWhat info do you want displayed on the histogram?\n1)GICS Sectors' totals\n2)GICS Sectors totals and highest trades per month" << endl;
			cout << ">";
			cin >> option_tmp;

			system("cls");
			switch (option_tmp) {
			case 1: {
				plot_annual_histogram(gics_list, year);
				cout << "\nPress enter to continue.";
				std::cin.clear();
				std::cin.ignore(32767, '\n');
				return 0;
			}; break;
			case 2: {plot_annual_comparison_histogram(gics_list, year);
				cout << "\nPress enter to continue."; 
				std::cin.clear();
				std::cin.ignore(32767, '\n');
				return 0;
			}; break;
			default: {cout << "\n[!]Invalid selection.\nPress enter to continue." << endl;
				std::cin.clear();
				std::cin.ignore(32767, '\n');
				return 0;
			}; break;
			}
			return 0;
		}break;
		case 3: {
			system("cls");
			for (auto& yr : psa_timeline) {
				plot_annual_comparison_histogram(gics_list, yr.first);
				cout << "--------------------------------" << endl;
			}
			cout << "\nPress enter to continue.";
			std::cin.clear();
			std::cin.ignore(32767, '\n');

		}break;
		case 4: {
			exit(0);
		}break;
		default: {
			cout << "\n[!]Invalid selection.\nPress enter to continue." << endl;
			std::cin.clear();
			std::cin.ignore(32767, '\n');
			return 0;
		}break;
	}

	return 0;
}




int main() {


	//Below code makes a database of companies from the PSA file and securities file


	//data processing containers
	vector<vector<string>> secs_file;
	secs_file.reserve(1000000);
	vector<GICS_Sec> gics_list;
	vector<security> security_list;
	map<string, day> PSA;
	map<string, map<string, vector<daySec>>> psa_timeline;

	//Vectors for storing security objectss
	vector<Security> main_sec_db; //for reading one year alone
	main_sec_db.reserve(600);




	string sec_file_name = "securities.csv";
	string psa_file_name = "prices-split-adjusted.csv";

	char exit_ch;
	int gc = 1;
	
	try {

		cout << "[*]Reading securities data from :" << sec_file_name << endl;
		read_csv(sec_file_name, secs_file, "nh");

		cout << "[*]Processing securities file" << endl;
		getSecInfoGICS(secs_file, gics_list, security_list);

		
		cout << "[*]Reading prices-split-adjusted data from :" << psa_file_name << endl;
		getDays(psa_file_name, PSA);
		
		cout << "[*]Processing prices-split-adjusted data into a timeline" << endl;
		create_PSA_timeline(PSA, psa_timeline);

		cout << "[*]Creating Security objects" << endl;
		for (auto& sec : security_list) {
			cout << "\n[+]Adding " << sec.tsymb << ", " << gc << " of " << security_list.size();
			main_sec_db.emplace_back(Security(&psa_timeline, &sec.tsymb, &sec.gicsInd));
			gc++;

		}

		cout << "[*]Calculating GICS Sector statistics" << endl;
		find_totals(gics_list, main_sec_db);

		
		int run_state = 0;
		while (true) {
			run_state = main_menu(main_sec_db, psa_timeline, security_list, gics_list);
			cin.ignore();
		}

	}
	catch (std::runtime_error e) {
		cerr << e.what() << endl;
		cerr << "\nUnable to recover\nExiting...." << endl;
	}
	catch (std::domain_error e) {
		cerr << "\n"<<e.what() << endl;
		cout << "Press enter to continue." << endl;
		cin.ignore();

		int run_state = 0;
		while (run_state == 0)
		{
			run_state = main_menu(main_sec_db, psa_timeline, security_list, gics_list);
			cin.ignore();
		}
		
	}
	catch (std::invalid_argument e) {
		cerr << "\n[!]Invalid argument entered, please ensure you enter numbers where"
			<< " required and not letters as this will be considered invalid input"
			<< "\nPress enter to continue" << endl;
		std::cin.clear();
		std::cin.ignore(32767, '\n');
		int run_state = 0;
		while (run_state == 0)
		{
			run_state = main_menu(main_sec_db, psa_timeline, security_list, gics_list);
			cin.ignore();
		}

	}
	catch (std::exception & e) {
		cerr << "\n"<<e.what() << endl;
		cout << "Press enter to continue." << endl;
		cin.ignore();

		int run_state = 0;
		while (run_state == 0)
		{
			run_state = main_menu(main_sec_db, psa_timeline, security_list, gics_list);
			cin.ignore();
		}
	}

		
	return 0;
}
