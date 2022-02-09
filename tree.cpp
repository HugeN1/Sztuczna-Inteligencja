#include "iostream"
#include "vector"
#include "string"

using namespace std;

pair<string, bool> decide(vector<string> decisionToMake) {
    pair<string, bool> option;
    string day = decisionToMake.at(0);
    string outlook = decisionToMake.at(1);
    string temperature = decisionToMake.at(2);
    string humidity = decisionToMake.at(3);
    string wind = decisionToMake.at(4);

    option.first = day;
    if (outlook == "Overcast") {
        option.second = true;
        return option;
    }
    else if (outlook == "Sunny") {
        if (humidity == "High") {
            option.second = false;
            return option;
        }
        else {
            option.second = true;
            return option;
        }
    }
    else {
        if (wind == "Strong") {
            option.second = false;
            return option;
        }
        else {
            option.second = true;
            return option;
        }
    }
}

int main(int argc, char* argv[]) {
    vector<vector<string>> playTennis = {
            {"Day1", "Sunny", "Hot", "High", "Weak"},
            {"Day2", "Sunny", "Hot", "High", "Strong"},
            {"Day3", "Overcast", "Hot", "High", "Weak"},
            {"Day4", "Rain", "Mild", "High", "Weak"},
            {"Day5", "Rain", "Cool", "Normal", "Weak"},
            {"Day6", "Rain", "Cool", "Normal", "Strong"},
            {"Day7", "Overcast", "Cool", "Normal", "Strong"},
            {"Day8", "Sunny", "Mild","High", "Weak"},
            {"Day9", "Sunny", "Cool", "Normal", "Weak"},
            {"Day10", "Rain", "Mild", "Normal", "Weak"},
            {"Day11", "Sunny", "Mild", "Normal", "Strong"},
            {"Day12", "Overcast", "Mild", "High", "Strong"},
            {"Day13", "Overcast", "Hot","Normal", "Weak"},
            {"Day14", "Rain", "Mild", "High", "Strong"}
    };

    vector<bool> checkOutColumn = {
            false,
            false,
            true,
            true,
            true,
            false,
            true,
            false,
            true,
            true,
            true,
            true,
            true,
            false
    };

    pair<string, bool> weather;
    for (int i = 0; i < playTennis.size(); i++) {
        weather = decide(playTennis[i]);
        if (weather.second == checkOutColumn[i]) {
            cout << boolalpha;
            cout << weather.first << " : " << weather.second;
            cout << "\n";
        }
    }
    return 0;
}
