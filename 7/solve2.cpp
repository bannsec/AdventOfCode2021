#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include <bits/stdc++.h>

using namespace std;

#define DEBUG 1

vector<string> * read_file(string file_name) {
    // String vector to store the lines of the file
    vector<string> * lines = new vector<string>();

    // Open file
    ifstream file(file_name, ios::in);
    if (!file.is_open()) {
        cout << "File not found" << endl;
        return nullptr;
    }

    // Read file line by line
    string line;
    while (getline(file, line)) {
        lines->push_back(line);
    }

    // Close file
    file.close();

    return lines;
}

// Function: split_string
// Description: Splits a string into a vector of strings
// Parameters:
//      string str: The string to split
//      char delimiter: The delimiter to split the string with
// Return:
//      vector<string> *: A vector of strings
vector<string> * split_string(string str, char delimiter) {
    // String vector to store the lines of the file
    vector<string> * lines = new vector<string>();

    // Split the string
    size_t pos = 0;
    string token;
    while ((pos = str.find(delimiter)) != string::npos) {
        token = str.substr(0, pos);
        lines->push_back(token);
        str.erase(0, pos + 1);
    }
    lines->push_back(str);

    return lines;
}

// Class crab
// Has one member variable: int position
class crab {
    public:
        int position;
        crab(int position) {
            this->position = position;
        }

        // Function: distance_to
        // Description: Calculates the distance to position by the following algorithm:
        //      1. Determine the absolute value of the difference between the current position and the position parameter
        //      2. For each digit in the absolute value, add 1 more than the previous digit. For instance, if the absolute value is 5, the distance is 1 + 2 + 3 + 4 + 5 = 15
        // Parameters:
        //      int position: The position to calculate the distance to
        // Return:
        //      int: The distance to position
        int distance_to(int position) {
            int distance = 0;
            int difference = abs(this->position - position);
            int i = 0;
            while (i < difference) {
                distance += i + 1;
                i++;
            }
            return distance;
        }
};

// Vector of crabs
vector<crab> *crabs = new vector<crab>();

// Function: parse_input
// Description: Parses the input and creates crabs
//            The input is a string of numbers separated by commas
// Parameters:
//      string input: The input to parse
// Return:
//      void
void parse_input(string input) {
    // Split the input
    vector<string> * lines = split_string(input, ',');

    // Create crabs
    for (int i = 0; i < lines->size(); i++) {
        crabs->push_back(crab(stoi(lines->at(i))));
    }

    // If DEBUG is enabled, print the crabs
    if (DEBUG) {
        cout << "Crabs: ";
        for (int i = 0; i < crabs->size(); i++) {
            cout << crabs->at(i).position << " ";
        }
        cout << endl;
    }

    // Delete lines
    delete lines;
}

// Function: calculate_cost
// Description: Calculates the cost of the solution by adding the distance to each crab
// Parameters:
//            int position: The position to calculate the cost from
// Return:
//            int: The cost of the solution
int calculate_cost(int position) {
    int cost = 0;
    for (int i = 0; i < crabs->size(); i++) {
        cost += crabs->at(i).distance_to(position);
    }
    return cost;
}

int main(int argc, char *argv[]) {

    // Check if the user has provided a file name
    // Help message uses argv[0]
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <file_name>" << endl;
        return 1;
    }

    vector<string> *lines = read_file(argv[1]);

    if (lines == nullptr) {
        return -1;
    }

    // Parse the input
    parse_input(lines->at(0));

    // Determine the maximum crab position
    int max_crab_position = 0;
    for (int i = 0; i < crabs->size(); i++) {
        if (crabs->at(i).position > max_crab_position) {
            max_crab_position = crabs->at(i).position;
        }
    }

    // If DEBUG is enabled, print the maximum crab position
    if (DEBUG) {
        cout << "Max crab position: " << max_crab_position << endl;
    }

    // Create a vector of ints to store the cost at each position
    //vector<int> *cost = new vector<int>();

    // Determine the position which results in the minimum cost
    int min_cost = INT_MAX;
    int min_cost_position = 0;
    for (int i = 0; i <= max_crab_position; i++) {
        int cost = calculate_cost(i);
        if (cost < min_cost) {
            min_cost = cost;
            min_cost_position = i;
        }
    }

    // Print the minimum cost and position
    cout << "Minimum cost: " << min_cost << endl;
    cout << "Minimum cost position: " << min_cost_position << endl;

    // Delete lines
    delete lines;

    // 104149091
}
