#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>
#include <limits>
using namespace std;

void print_the_report(int quantity, double mean, double var, string best_day, double max_return, string worst_day, double min_return) {
    cout << "\n----- Analysis Summary -----\n\n";
    cout << quantity << " prices read; ";
    cout << quantity-1 << " returns computed.\n\n";

    cout << "Average daily return is " << mean*100 << "%\n";
    cout << "Daily volatility is " << var*100 << "%\n\n";

    cout << "Annual return is " << mean*252*100 << "%\n";
    cout << "Annual volatility is " << var*252*100 << "%\n\n";

    cout << "The best day was " << best_day << " with the return of " << max_return*100 << "%\n";
    cout << "The worst day was " << worst_day << " with the return of " << min_return*100 << "%\n";
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


int main() {

    vector<double> prices;
    vector<string> dates;

    ifstream file("data/prices.csv"); // Opens the file with the financial data

    if (!file.is_open()) { 
        cout << "Error: could not open data/prices.csv\n"; // File failed to open, outputs the error
        return 1;
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

    cout << fixed << setprecision(3);
    print_the_report(quantity, mean, var, best_day, max_return, worst_day, min_return);
    

    return 0;
}