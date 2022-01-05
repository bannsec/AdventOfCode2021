#include<iostream>
#include<fstream>
#include<vector>
#include<string>

#define FILE_NAME "input"

int horizontal = 0;
int depth = 0;

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

// Function: handle_forward
// Description: Moves the turtle forward i times
// Parameters:
//      int i: The number of times to move forward
void handle_forward(int i) {
    horizontal += i;
}

// Function: handle_down
// Description: Moves the turtle down i times
// Parameters:
//      int i: The number of times to move down
void handle_down(int i) {
    depth += i;
}

// Function: handle_up
// Description: Moves the turtle up i times
// Parameters:
//      int i: The number of times to move up
void handle_up(int i) {
    depth -= i;
}


// Function: handle_line
// Description: Handles a line of the file
// Splits the line into a vector of strings by the space delimiter into [command, i]
// Calls handle_<command>(i)
// Valid commands are "forward", "down", "up"
// Parameters:
//      string line: The line to handle
void handle_line(string line) {
    // Split the line
    vector<string> * line_split = split_string(line, ' ');
    string command = line_split->at(0);
    string i_str = line_split->at(1);
    int i = stoi(i_str);
    delete line_split;

    // Call handle_<command>(i)
    if (command == "forward") {
        handle_forward(i);
    } else if (command == "down") {
        handle_down(i);
    } else if (command == "up") {
        handle_up(i);
    } else {
        cout << "Invalid command: " << command << endl;
    }
}


int main() {
    // Read the file
    vector<string> * lines = read_file(FILE_NAME);
    if (lines == nullptr) {
        return -1;
    }

    // Handle each line
    for (string line : *lines) {
        handle_line(line);
    }

    // Delete the lines
    delete lines;

    // Prints horizontal and depth
    cout << "Horizontal: " << horizontal << endl;
    cout << "Depth: " << depth << endl;

    // Prints horizontal*depth
    cout << "Horizontal*depth: " << horizontal * depth << endl;

    return 0;
}