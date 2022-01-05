#include<iostream>
#include<fstream>
#include<vector>
#include<string>

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

// Function: count_increase
// Description: Inteprets each line of the file as an integer and counts the number of times the number increases from the previous line
// Parameters:
//      vector<string> * lines: The lines of the file
// Return:
//      int: The number of times the number increases

int count_increase(vector<string> * lines) {
    // The number of times the number increases
    int count = -1;

    // The previous number
    int prev = 0;

    // Iterate through the lines
    for (int i = 0; i < lines->size(); i++) {
        // Convert the line to an integer
        int num = stoi(lines->at(i));

        // If the number is greater than the previous number, increase the count
        if (num > prev) {
            count++;
        }

        // Update the previous number
        prev = num;
    }

    return count;
}

int main(int argc, char *argv[]) {
    vector<string> *lines = read_file(argv[1]);

    if (lines == nullptr) {
        return -1;
    }

    int count = count_increase(lines);

    cout << count << endl;

}