#include "functions.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

void print_the_report(string file_path, int quantity, double mean, double vol, string best_day, 
    double max_return, string worst_day, double min_return, double max_drawdown,
    string trough_date, double max_price, string peak_date, double max_20d_vol, string max_20d_vol_date) {

    cout << "\nData source: " << file_path << "\n";

    cout << "\n----- Analysis Summary -----\n\n";
    cout << quantity << " prices read; ";
    cout << quantity-1 << " returns computed\n\n";

    cout << "Average daily return is " << mean*100 << "%\n";
    cout << "Daily volatility is " << vol*100 << "%\n\n";

    cout << "Annual return is " << mean*252*100 << "%\n";
    cout << "Annual volatility is " << vol*252*100 << "%\n\n";

    cout << "The best day was " << best_day << " with the return of " << max_return*100 << "%\n";
    cout << "The worst day was " << worst_day << " with the return of " << min_return*100 << "%\n\n";

    cout << "The max drawdown was " << max_drawdown*100 << "% on the day " << trough_date << "\n";
    cout << "The peak price was " << max_price << " on the day " << peak_date << "\n\n";

    if (quantity > 20) {
        cout << "Max 20-day volatility: " << max_20d_vol*100 << "% (on " << max_20d_vol_date << ")\n\n";
    }
    else {
        cout << "Not enough data to compute 20-days rolling volatility\n\n";
    }
}


int load_prices_csv(const string& file_path, vector<string>& dates, vector<double>& prices) {

    ifstream file(file_path); // Opens the file with the financial data

    if (!file.is_open()) { 
        cout << "Error: could not open" << file_path << "\n"; // File failed to open, outputs the error
        return 0;
    }
    string line;
    int quantity = 0;

    getline(file, line); // Skip the header

    while(getline(file, line)) { 

        string date;
        double price;

        int i;
        for (i = 0; i < line.size() && line[i] != ','; ++i) {;} // Find the index of the comma

        if (i == line.size() || i + 1 >= line.size()) {   // Skip a "bad" line   
            continue;}

        date = line.substr(0,i);
        price = stod(line.substr(i+1));

        dates.push_back(date); // Fill in the date vector
        prices.push_back(price); // Fill in the price vector

        quantity++;
    }
    return quantity;
}

double find_max_price(const vector<double>& prices, const vector<string> &dates, string& peak_date) {
    double max_price = 0;
    
    for (int i = 0; i < prices.size(); ++i) {
        if (prices[i] > max_price) {
            max_price =  prices[i];
            peak_date = dates[i];
        }
    }
    return max_price;
}

double find_max_drawdon(const vector<double>& prices, const vector<string>& dates, double max_price, string& trough_date) {
    double max_drawdown = 0;
    
    for (int i = 0; i < prices.size(); ++i) {
        if (prices[i]/max_price - 1 < max_drawdown) {
            max_drawdown =  prices[i]/max_price - 1;
            trough_date = dates[i];
        }
    }
    return max_drawdown;
}

void compute_returns(vector<double>& returns, const vector<double>& prices, const vector<string>& dates, 
    double& max_return, string& best_day, double& min_return, string& worst_day) {

    for (int i = 1; i < prices.size(); ++i) { // Compute returns from the prices data
    double curr_return = prices[i]/prices[i-1] - 1;

    if (max_return < curr_return) {
        max_return = curr_return;
        best_day = dates[i];
    }
    if (min_return > curr_return) {
        min_return = curr_return;
        worst_day = dates[i];
    }
    returns.push_back(curr_return);
        }
    }


double find_mean(const vector<double>& returns) {
    double sum = 0;
    for (int i = 0; i < returns.size(); ++i) { 
        sum += returns[i];
    }
    return sum/returns.size();
}

double find_vol(const vector<double>& returns, double mean) {
    double vol = 0;
    for (int i = 0; i < returns.size(); ++i) { 
        vol += pow(returns[i] - mean, 2);
    }
    vol /= (returns.size()-1);
    vol = sqrt(vol);

    return vol;
}

void find_vol_20d(vector<double>& vol_20d, const vector<double>& returns) {

    vol_20d.clear();
    if (returns.size() < 20) return;

    vector<double> sub_returns(20, 0.0);
        
    for (int i = 0; i < returns.size()-19; ++i) {

                for (int j = 0; j < 20; ++j) {
                    sub_returns[j] = returns[i+j];
                }

            double mean = find_mean(sub_returns);
            double vol = find_vol(sub_returns, mean);

            vol_20d.push_back(vol);
        }
    }

 void report_vol_20d(const vector<double>& vol_20d, const vector<string>& dates, double& max_20d_vol, string& max_20d_vol_date) {
    
    for (int i = 0; i < vol_20d.size(); ++i) {
        if (max_20d_vol < vol_20d[i]) {
            max_20d_vol = vol_20d[i];
            max_20d_vol_date = dates[i+19];
        }
    }
 }