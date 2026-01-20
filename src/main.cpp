#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>
using namespace std;

int main() {

    vector<double> prices;

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

        prices.push_back(price); // Fill in the price vector
        quantity++;
    }

    if (prices.size() < 2) {
        cout << "Not enough data";
        return 1;
    }

    vector<double> returns;

    for (int i = 1; i < prices.size(); ++i) { // Compute returns from the prices data
        returns.push_back(prices[i]/prices[i-1] - 1);
    };

    double sum = 0;
    for (int i = 0; i < returns.size(); ++i) { // Compute total sum of the returns
        sum += returns[i];
    }
    double mean = sum/returns.size();

    double var = 0;

    for (int i = 0; i < returns.size(); ++i) { 
        var += pow(returns[i] - mean, 2);
    }
    var /= (returns.size()-1);
    var = sqrt(var);

    cout << fixed << setprecision(3);

    cout << "\n----- Analysis Summary -----\n\n";
    cout << quantity << " prices read; ";
    cout << quantity-1 << " returns computed.\n\n";
    
    cout << "Average daily return is " << mean*100 << "%\n";
    cout << "Daily volatility is " << var*100 << "%\n\n";

    cout << "Annual return is " << mean*252*100 << "%\n";
    cout << "Annual volatility is " << var*252*100 << "%\n\n";

    

    return 0;
}