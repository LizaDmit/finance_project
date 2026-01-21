#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>
#include <limits>

#include "functions.h"

using namespace std;


int main(int argc, char* argv[]) {

    string file_path = "data/prices.csv";  

    if (argc > 1) {
        file_path = argv[1];
    }

    vector<double> prices;
    vector<string> dates;
    string peak_date, trough_date;

    int quantity = load_prices_csv(file_path, dates, prices);

    double max_price = find_max_price(prices, dates, peak_date);
    double max_drawdown = find_max_drawdon(prices, dates, max_price, trough_date);

    if (prices.size() < 2) {
        cout << "Not enough data for further calculations";
        return 1;
    }

    vector<double> returns;
    string best_day, worst_day;
    double min_return = numeric_limits<double>::infinity();
    double max_return = -numeric_limits<double>::infinity();

    compute_returns(returns, prices, dates, max_return, best_day,  
        min_return, worst_day);

    double mean = find_mean(returns);
    double vol = find_vol(returns, mean);

    cout << fixed << setprecision(2);
    print_the_report(file_path, quantity, mean, vol, best_day, max_return, worst_day, min_return, max_drawdown, 
    trough_date, max_price, peak_date);
    

    return 0;
}