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
// Return:
//      string: The most common value at string[index]
string calculate_most_common_value(vector<string> * lines, int index) {
    // Create a map to store the number of times each value appears
    map<string, int> * values = new map<string, int>();

    // For each string in the vector
    for (int i = 0; i < lines->size(); i++) {
        // Get the value at index i
        string value = (*lines)[i].substr(index, 1);

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
    for (auto it = values->begin(); it != values->end(); it++) {
        if (it->second > max) {
            max = it->second;
            most_common_value = it->first;
        }
    }

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

// Function: invert_bits
// Description: Inverts the bits of a binary number
// Arguments:
//      int num: The binary number to invert
// Return:
//      int: The inverted binary number
int invert_bits(int num) {
    // Convert int num to a binary string, of size 12
    string binary = bitset<12>(num).to_string();

    // Invert the bits of the binary number
    for (int i = 0; i < binary.size(); i++) {
        if (binary[i] == '0') {
            binary[i] = '1';
        }
        else {
            binary[i] = '0';
        }
    }

    // Convert the inverted binary number to an int
    return stoi(binary, nullptr, 2);
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

    //
    // Gamma
    //

    // For i in range(0, len(lines[0])) calculate the most common value at index i, save it into a vector and print it
    vector<string> * values = new vector<string>();
    for (int i = 0; i < lines->at(0).length(); i++) {
        values->push_back(calculate_most_common_value(lines, i));
    }

    for (int i = 0; i < values->size(); i++) {
        cout << values->at(i) << endl;
    }

    // Print the int value of the binary number
    int gamma = convert_to_int(values);
    cout << gamma << endl;

    //
    // Epsilon
    //

    int epsilon = invert_bits(gamma);

    // Print epsilon
    cout << epsilon << endl;

    // Print epsilon * gamma
    cout << epsilon * gamma << endl;

    return 0;

}
