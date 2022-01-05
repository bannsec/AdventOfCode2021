#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<map>
#include<bitset>

using namespace std;

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

// Function: calculate_most_common_value
// Description: Calculates the most common value in a vector of strings
// For each string in the vector, it counts the number of times it appears at index i
// Arguments:
//      vector<string> * lines: The vector of strings to calculate the most common value from
//      int index: The index to calculate the most common value from
//      string tie: The string to use if there is a tie
// Return:
//      string: The most common value at string[index]
string calculate_most_common_value(vector<string> * lines, int index, string tie) {
    // Map to store the number of times each value appears
    map<string, int> * values = new map<string, int>();

    // For each string in the vector
    for (string line : *lines) {
        // Get the value at index
        string value = line.substr(index, 1);

        // If the value is not in the map, add it
        if (values->find(value) == values->end()) {
            values->insert(pair<string, int>(value, 1));
        }
        // Otherwise, increment the value
        else {
            values->at(value)++;
        }
    }

    // Find the most common value
    int max = 0;
    string most_common_value = "";
    for (pair<string, int> value : *values) {
        if (value.second > max) {
            max = value.second;
            most_common_value = value.first;
        }
    }

    // print values map
    //for (pair<string, int> value : *values) {
    //    cout << value.first << ": " << value.second << endl;
    //}

    // If the number of times the most common value appears is equal to the number of times it does not appear, return tie
    if (values->at(most_common_value) == lines->size() - values->at(most_common_value)) {
        return tie;
    }
    
    // Otherwise, return the most common value
    return most_common_value;
}


// Function: convert_to_int
// Description: Inteprets a vector of strings as a binary number
// Arguments:
//      vector<string> * bits: The vector of strings to convert to an int
// Return:
//      int: The int value of the binary number
int convert_to_int(vector<string> * bits) {
    // Create a string to store the binary number
    string binary = "";

    // For each string in the vector
    for (int i = 0; i < bits->size(); i++) {
        // Add the string to the binary number
        binary += (*bits)[i];
    }

    // Convert the binary number to an int
    return stoi(binary, nullptr, 2);
}




// Function: downselect_lines
// Description: Downselects the lines of the vector of strings based on the given index and value
// Arguments:
//      vector<string> * lines: The vector of strings to downselect
//      int index: The index to downselect the lines with
//      string value: The value to downselect the lines with
// Return:
//      vector<string> *: The downselected lines
vector<string> * downselect_lines(vector<string> * lines, int index, string value) {
    // Create a new vector to store the downselected lines
    vector<string> * downselected_lines = new vector<string>();

    // For each line in the vector
    for (int i = 0; i < lines->size(); i++) {
        // Get the value at index i
        string line_value = (*lines)[i].substr(index, 1);

        // If the value is the same as the given value, add the line to the downselected lines
        if (line_value == value) {
            downselected_lines->push_back((*lines)[i]);
        }
    }

    return downselected_lines;
}


// Function: calculate_oxygen_generator_rating
// Description: Calculates the oxygen generator rating of the challenge
// Algorithm:
//      1. For each index value in len(lines[0]), calculate the most common value at that index
//      2. Downselect the lines to only include lines with the most common value at that index
//      3. Continue until only one line remains
//      4. Convert the remaining line to an int, this is the oxygen generator rating
// Arguments:
//      vector<string> * lines: The vector of strings to calculate the oxygen generator rating from
// Return:
//      int: The oxygen generator rating
int calculate_oxygen_generator_rating(vector<string> * lines) {
    // Create a vector to store the oxygen generator rating
    vector<int> * oxygen_generator_rating = new vector<int>();

    // While there are more than one line
    while (lines->size() > 1) {
        // For each index value in len(lines[0]), calculate the most common value at that index
        for (int i = 0; i < (*lines)[0].size(); i++) {
            // Calculate the most common value at index i
            string most_common_value = calculate_most_common_value(lines, i, "1");
            // cout << "Most common value at index " << i << ": " << most_common_value << endl;

            // Downselect the lines to only include lines with the most common value at that index
            lines = downselect_lines(lines, i, most_common_value);
            //cout << "Downselected lines: " << endl;
            //for (string line : *lines) {
            //    cout << line << endl;
            //}
        }
    }

    // Convert the remaining line to an int, this is the oxygen generator rating
    oxygen_generator_rating->push_back(convert_to_int(lines));

    return oxygen_generator_rating->at(0);
}

// Function: calculate_co2_scrubber_rating
// Description: Calculates the CO2 scrubber rating of the challenge
// Algorithm:
//      1. For each index value in len(lines[0]), calculate the least common value at that index, or "0" if there is a tie
//      2. Downselect the lines to only include lines with the least common value at that index
//      3. Continue until only one line remains
//      4. Convert the remaining line to an int, this is the co2 scrubber rating
// Arguments:
//      vector<string> * lines: The vector of strings to calculate the co2 scrubber rating from
// Return:
//      int: The co2 scrubber rating
int calculate_co2_scrubber_rating(vector<string> * lines) {
    // Create a vector to store the co2 scrubber rating
    vector<int> * co2_scrubber_rating = new vector<int>();

    // While there are more than one line
    while (lines->size() > 1) {
        // For each index value in len(lines[0]), calculate the least common value at that index, or "0" if there is a tie
        for (int i = 0; i < (*lines)[0].size(); i++) {
            // Calculate the least common value at index i
            string least_common_value = calculate_most_common_value(lines, i, "1");
            
            // If least_common_value is "1" then set least_common_value to "0", and vice versa
            if (least_common_value == "1") {
                least_common_value = "0";
            }
            else {
                least_common_value = "1";
            }
            cout << "Least common value at index " << i << ": " << least_common_value << endl;

            // Downselect the lines to only include lines with the least common value at that index
            lines = downselect_lines(lines, i, least_common_value);
            cout << "Downselected lines: " << endl;
            for (string line : *lines) {
                cout << line << endl;
            }

            // if len(lines) == 1, then break
            if (lines->size() == 1) {
                break;
            }
        }
    }

    // Convert the remaining line to an int, this is the co2 scrubber rating
    co2_scrubber_rating->push_back(convert_to_int(lines));

    return co2_scrubber_rating->at(0);
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

    // Print oxygen generator rating
    int oxygen_generator_rating = calculate_oxygen_generator_rating(lines);
    cout << "Oxygen generator rating: " << oxygen_generator_rating << endl;

    // Print co2 scrubber rating
    int co2_scrubber_rating = calculate_co2_scrubber_rating(lines);
    cout << "CO2 scrubber rating: " << co2_scrubber_rating << endl;

    cout << "Total rating: " << oxygen_generator_rating * co2_scrubber_rating << endl;

    // Free memory
    delete lines;

    return 0;

}
