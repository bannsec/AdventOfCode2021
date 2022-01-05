#include<iostream>
#include<fstream>
#include<vector>
#include<string>

#define FILE_NAME "input"

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

// Function: rolling_sum_3
// Description: Creates vector of 3-line sums of vector of integers
// Parameters:
//      vector<int> * numbers: The vector of integers to calculate the sum of
// Return:
//      vector<int> *: A vector of 3-line sums
vector<int> * rolling_sum_3(vector<int> * numbers) {
    // Vector to store the 3-line sums
    vector<int> * sums = new vector<int>();

    // Loop through the vector of integers
    for (int i = 0; i < numbers->size() - 2; i++) {
        // Calculate the sum of the 3 lines
        int sum = 0;
        for (int j = i; j < i + 3; j++) {
            sum += numbers->at(j);
        }

        // Add the sum to the vector
        sums->push_back(sum);
    }

    return sums;
}

// Function: vector_strings_to_ints
// Description: Converts a vector of strings to a vector of integers
// Parameters:
//      vector<string> * strings: The vector of strings to convert
// Return:
//      vector<int> *: A vector of integers
vector<int> * vector_strings_to_ints(vector<string> * strings) {
    // Vector to store the integers
    vector<int> * numbers = new vector<int>();

    // Loop through the vector of strings
    for (int i = 0; i < strings->size(); i++) {
        // Convert the string to an integer
        int number = stoi(strings->at(i));

        // Add the integer to the vector
        numbers->push_back(number);
    }

    return numbers;
}


int main() {
    vector<string> *lines = read_file(FILE_NAME);

    if (lines == nullptr) {
        return -1;
    }

    // Create rolling sum 3 vector
    vector<int> * numbers = rolling_sum_3(vector_strings_to_ints(lines));

    // Count number of times the number is greater than the previous number
    int count = 0;
    for (int i = 0; i < numbers->size() - 1; i++) {
        if (numbers->at(i) < numbers->at(i + 1)) {
            count++;
        }
    }

    // Print numbers vector
    for (int i = 0; i < numbers->size(); i++) {
        cout << numbers->at(i) << " ";
    }

    cout << count << endl;
}
