#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <string>
#include <vector>

using namespace std;

int load_prices_csv(const string& file_path, vector<string>& dates, vector<double>& prices);

double find_max_price(vector<double> prices, vector<string> dates, string& peak_date);

double find_max_drawdon(vector<double> prices, vector<string> dates, double max_price, string& trough_date);

void compute_returns(vector<double>& returns, const vector<double> prices, const vector<string> dates, 
    double& max_return, string& best_day, double& min_return, string& worst_day);

double find_mean(vector<double> returns);

double find_vol(vector<double> returns, int mean);

void print_the_report(string file_path, int quantity, double mean, double vol, string best_day, 
    double max_return, string worst_day, double min_return, double max_drawdown,
    string trough_date, double max_price, string peak_date);

#endif