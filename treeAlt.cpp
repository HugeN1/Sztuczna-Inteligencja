#include "iostream"
#include "vector"
#include "string"

using namespace std;

pair<string, bool> decide(vector<string> decisionToMake) {
    pair<string, bool> option;
    string day = decisionToMake.at(0);
    string temperature = decisionToMake.at(1);
    string humidity = decisionToMake.at(2);
    string wind = decisionToMake.at(3);
    string outlook = decisionToMake.at(4);

    option.first = day;
    if (temperature == "Hot") {
        if (humidity == "High") {
            option.second = true;
            return option;
        }
        else {
            if (wind == "Strong") {
                option.second = false;
                return option;
            }
            else {
                if (outlook == "Overcast") {
                    option.second = true;
                    return option;
                }
                else {
                    option.second = false;
                    return option;
                }
            }
        }
    }
    else if (temperature == "Mild") {
        if (humidity == "High") {
            option.second = false;
            return option;
        }
        else {
            if (wind == "Strong") {
                option.second = false;
                return option;
            }
            else {
                if (outlook == "Overcast") {
                    option.second = true;
                    return option;
                }
                else {
                    option.second = false;
                    return option;
                }
            }
        }
    }
    else {
        if (humidity == "High") {
            option.second = false;
            return option;
        }
        else {
            if (wind == "Strong") {
                option.second = false;
                return option;
            }
            else {
                if (outlook == "Overcast") {
                    option.second = false;
                    return option;
                }
                else {
                    option.second = true;
                    return option;
                }
            }
        }
    }
}

int main(int argc, char* argv[]) {
    vector<vector<string>> playTennis = {
            {"Day1", "Hot", "Normal", "Mild", "Sunny"},
            {"Day2", "Hot", "Normal", "Strong","Sunny"},
            {"Day3", "Hot", "Normal", "Mild", "Overcast"},
            {"Day4", "Mild", "High", "Strong", "Overcast"},
            {"Day5", "Cool", "High", "Mild", "Sunny"},
            {"Day6", "Cool", "High", "Strong", "Overcast"},
            {"Day7", "Cool", "High", "Mild", "Sunny"},
            {"Day8", "Mild", "High", "Mild", "Overcast"},
            {"Day9", "Cool", "High", "Mild", "Overcast"},
            {"Day10", "Mild", "Normal", "Strong", "Overcast"},
            {"Day11", "Mild", "High", "Mild", "Overcast"},
            {"Day12", "Mild", "High", "Strong", "Sunny"},
            {"Day13", "Hot", "High","Mild","Overcast"},
            {"Day14", "Mild", "Normal", "Mild", "Overcast"}
    };

    vector<bool> checkOutColumn = {
            false,
            false,
            true,
            false,
            false,
            false,
            false,
            false,
            false,
            false,
            false,
            false,
            true,
            true
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
