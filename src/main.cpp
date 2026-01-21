#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>
#include <limits>
using namespace std;

void print_the_report(string file_path, int quantity, double mean, double var, string best_day, 
    double max_return, string worst_day, double min_return, double max_drawdown,
    string trough_date, double max_price, string peak_date) {

    cout << "\nData source: " << file_path << "\n";

    cout << "\n----- Analysis Summary -----\n\n";
    cout << quantity << " prices read; ";
    cout << quantity-1 << " returns computed.\n\n";

    cout << "Average daily return is " << mean*100 << "%\n";
    cout << "Daily volatility is " << var*100 << "%\n\n";

    cout << "Annual return is " << mean*252*100 << "%\n";
    cout << "Annual volatility is " << var*252*100 << "%\n\n";

    cout << "The best day was " << best_day << " with the return of " << max_return*100 << "%\n";
    cout << "The worst day was " << worst_day << " with the return of " << min_return*100 << "%\n\n";

    cout << "The max drawdown was " << max_drawdown*100 << "% on the day " << trough_date << "\n";
    cout << "The peak price was " << max_price << " on the day " << peak_date << "\n\n";
}

double find_mean(vector<double> returns) {
    double sum = 0;
    for (int i = 0; i < returns.size(); ++i) { 
        sum += returns[i];
    }
    return sum/returns.size();
}

double find_var(vector<double> returns, int mean) {

    double var = 0;
    for (int i = 0; i < returns.size(); ++i) { 
        var += pow(returns[i] - mean, 2);
    }
    var /= (returns.size()-1);
    var = sqrt(var);

    return var;
}


int main(int argc, char* argv[]) {

    string file_path = "data/prices.csv";  

    if (argc > 1) {
        file_path = argv[1];
    }

    vector<double> prices;
    vector<string> dates;

    ifstream file(file_path); // Opens the file with the financial data

    if (!file.is_open()) { 
        cout << "Error: could not open" << file_path << "\n"; // File failed to open, outputs the error
        return 1;
    }


    string line;
    int quantity = 0;
    double max_price = 0, max_drawdown = 0;
    string peak_date, trough_date;

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

        if (peak_date == "") {
            peak_date = trough_date = date;
        }

        dates.push_back(date); // Fill in the date vector
        prices.push_back(price); // Fill in the price vector

        if (price/max_price - 1 < max_drawdown) {
            max_drawdown =  price/max_price - 1;
            trough_date = date;
        }
        if (price > max_price) {
            max_price =  price;
            peak_date = date;
        }
        quantity++;
    }

    if (prices.size() < 2) {
        cout << "Not enough data";
        return 1;
    }

    vector<double> returns;
    string best_day, worst_day;

    double min_return = numeric_limits<double>::infinity();
    double max_return = -numeric_limits<double>::infinity();

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
    };

    double mean = find_mean(returns);

    double var = find_var(returns, mean);

    cout << fixed << setprecision(2);
    print_the_report(file_path, quantity, mean, var, best_day, max_return, worst_day, min_return, max_drawdown, 
    trough_date, max_price, peak_date);
    

    return 0;
}