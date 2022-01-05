#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<regex>

#define DEBUG 1

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

// Global variables
// Vector of vectors of ints, initialized
vector<vector<int>> * matrix = new vector<vector<int>>();

// The board is a two dimensional vector of ints
// The first dimension is the row
// The second dimension is the column
vector<vector<int>> * board = new vector<vector<int>>();

// Function: parse_line
// Description: Parses a line of the file
//            The line is a string of the form:
//            "x1,y1 -> x2,y2"
//            where x1,y1 is the start and x2,y2 is the end
//            of the line. Those four numbers will be stored as a vector of ints in the matrix
//            The function uses regex to parse the string
// Parameters:
//      string line: The line to parse
// Return:
//      bool: True if the line was parsed successfully, false otherwise
bool parse_line(string line) {
    // Regex to parse the line
    regex r("(\\d+),(\\d+) -> (\\d+),(\\d+)");

    // Match the line with the regex
    smatch m;
    if (regex_match(line, m, r)) {
        // Get the start and end coordinates
        int x1 = stoi(m[1]);
        int y1 = stoi(m[2]);
        int x2 = stoi(m[3]);
        int y2 = stoi(m[4]);

        // Create a vector of ints
        vector<int> * v = new vector<int>();
        v->push_back(x1);
        v->push_back(y1);
        v->push_back(x2);
        v->push_back(y2);

        // Print debug info
        if (DEBUG) {
            cout << "Line parsed: " << x1 << "," << y1 << " -> " << x2 << "," << y2 << endl;
            // print vector in format <x1,y1,x2,y2>
            cout << "<";
            for (int i = 0; i < v->size(); i++) {
                cout << v->at(i) << ",";
            }
            cout << ">" << endl;
        }

        // Add the vector to the matrix
        matrix->push_back(*v);

        return true;
    }

    return false;
}

// Function: init_board
// Description: Initializes the board with 0's
//              The board is a vector of vectors of ints
//              Board size is determined by max x2 and y2 integers in the matrix
//              Debug info is printed
// Parameters:
//       None
// Return:
//       bool: True if the board was initialized successfully, false otherwise
bool init_board() {
    // Get the max of all x and y coordinates
    int max_x = 0;
    int max_y = 0;
    for (int i = 0; i < matrix->size(); i++) {
        int x1 = matrix->at(i)[0];
        int y1 = matrix->at(i)[1];
        int x2 = matrix->at(i)[2];
        int y2 = matrix->at(i)[3];

        if (x1 > max_x) {
            max_x = x1;
        }
        if (y1 > max_y) {
            max_y = y1;
        }
        if (x2 > max_x) {
            max_x = x2;
        }
        if (y2 > max_y) {
            max_y = y2;
        }
    }

    // Create the board
    for (int i = 0; i <= max_y; i++) {
        vector<int> * v = new vector<int>();
        for (int j = 0; j <= max_x; j++) {
            v->push_back(0);
        }
        board->push_back(*v);
    }

    // Print debug info
    if (DEBUG) {
        cout << "Board initialized" << endl;
        cout << "Board size: " << max_x + 1 << "," << max_y + 1 << endl;
    }

    return true;
}

// Function: print_board
// Description: Prints the board representation as a grid
//              If the board entry is 0, prints a '.', otherwise prints the number
// Parameters:
//       None
// Return:
//       bool: True if the board was printed successfully, false otherwise
bool print_board() {
    // Print the board
    for (int i = 0; i < board->size(); i++) {
        for (int j = 0; j < board->at(i).size(); j++) {
            if (board->at(i)[j] == 0) {
                cout << ".";
            } else {
                cout << board->at(i)[j];
            }
        }
        cout << endl;
    }

    return true;
}

// Function: update_board
// Description: Updates the board with the values from the matrix
//              The matrix lines can be vertical or horizontal
//              The corresponding integer in the board is incremented
//              debug information about the line drawing is printed
// Parameters:
//       None
// Return:
//       bool: True if the board was updated successfully, false otherwise
bool update_board() {
    // Update the board
    for (int i = 0; i < matrix->size(); i++) {
        int x1 = matrix->at(i)[0];
        int y1 = matrix->at(i)[1];
        int x2 = matrix->at(i)[2];
        int y2 = matrix->at(i)[3];
        //int y1 = matrix->at(i)[0];
        //int x1 = matrix->at(i)[1];
        //int y2 = matrix->at(i)[2];
        //int x2 = matrix->at(i)[3];


        // Print debug info
        if (DEBUG) {
            cout << "Line drawn: " << y1 << "," << x1 << " -> " << y2 << "," << x2 << endl;
        }

        // Update the board
        // Taking into consideration that y1 may be greater than y2 and x1 may be greater than x2
        // Must find the min and max values of x and y prior to drawing
        int min_x = x1;
        int max_x = x2;
        int min_y = y1;
        int max_y = y2;
        if (x1 > x2) {
            min_x = x2;
            max_x = x1;
        }
        if (y1 > y2) {
            min_y = y2;
            max_y = y1;
        }

        // Draw the line
        // Only if the line is horizontal or vertical
        if (x1 == x2 || y1 == y2) {
            for (int j = min_x; j <= max_x; j++) {
                for (int k = min_y; k <= max_y; k++) {
                    board->at(k)[j]++;
                }
            }
        }

        // Print debug info
        if (DEBUG) {
            cout << "Board updated" << endl;
        }
    }

    return true;
}

// Function: count_cells
// Description: Counts the number of cells that are greater than 1
// Parameters:
//       None
// Return:
//       int: The number of cells that are greater than 1
int count_cells() {
    int count = 0;
    for (int i = 0; i < board->size(); i++) {
        for (int j = 0; j < board->at(i).size(); j++) {
            if (board->at(i)[j] > 1) {
                count++;
            }
        }
    }

    return count;
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

    // For each line in the file, parse it
    for (string line : *lines) {
        if (!parse_line(line)) {
            cout << "Error parsing line: " << line << endl;
            return -1;
        }
    }

    // Initialize the board
    if (!init_board()) {
        cout << "Error initializing board" << endl;
        return -1;
    }

    // Update the board
    if (!update_board()) {
        cout << "Error updating board" << endl;
        return -1;
    }

    // Print the board
    if (!print_board()) {
        cout << "Error printing board" << endl;
        return -1;
    }

    // Print solution: count the number of cells that are greater than 1
    cout << "Solution: " << count_cells() << endl;

}
