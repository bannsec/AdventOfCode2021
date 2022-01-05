#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<map>
#include<set>
#include<algorithm>
#include<assert.h>
#include<regex>

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

// class: Paper
// Represents oragami paper.
// Members:
//      vector<vector<bool>> * paper: The paper. Boolean represents if dot exists at that coordinate.
class Paper {
public:
    vector<vector<bool>> * paper;

    // Function: Paper
    // Description: Constructor
    // Parameters:
    //      vector<vector<bool>> * paper: The paper. Boolean represents if dot exists at that coordinate.
    Paper(vector<vector<bool>> * paper) {
        this->paper = paper;
    }

    // Function: get_dot
    // Description: Gets the dot at the given coordinate
    // Parameters:
    //      int x: The x coordinate
    //      int y: The y coordinate
    // Return:
    //      bool: True if dot exists at the given coordinate, false otherwise
    bool get_dot(int x, int y) {
        return this->paper->at(x).at(y);
    }

    // Function: set_dot
    // Description: Sets the dot at the given coordinate
    // Parameters:
    //      int x: The x coordinate
    //      int y: The y coordinate
    //      bool dot: The dot to set
    void set_dot(int x, int y, bool dot) {
        this->paper->at(x).at(y) = dot;
    }

    // Function: get_width
    // Description: Gets the width of the paper
    // Return:
    //      int: The width of the paper
    int get_width() {
        return this->paper->size();
    }

    // Function: get_height
    // Description: Gets the height of the paper
    // Return:
    //      int: The height of the paper
    int get_height() {
        return this->paper->at(0).size();
    }

    // Function: print
    // Description: Prints the paper
    void print() {
        for (int i = 0; i < this->get_height(); i++) {
            for (int j = 0; j < this->get_width(); j++) {
                if (this->get_dot(j, i)) {
                    cout << "#";
                } else {
                    cout << ".";
                }
            }
            cout << endl;
        }

        cout << endl;
    }

    // Function: remove_column
    // Description: Removes the given column from the paper
    // Parameters:
    //      int col: The column to remove
    void remove_column(int col) {
        this->paper->erase(this->paper->begin() + col);
    }

    // Function: remove_row
    // Description: Removes the given row from the paper
    // Parameters:
    //      int row: The row to remove
    void remove_row(int row) {
        for (int i = 0; i < this->get_width(); i++) {
            this->paper->at(i).erase(this->paper->at(i).begin() + row);
        }
    }

    // Function: fold_x_axis
    // Description: Folds the paper horizontally
    // Parameters:
    //      int x: The x coordinate (column) to fold from
    // Return:
    //      None
    void fold_x_axis(int x) {
        // Assert x is within bounds
        assert(x >= 0 && x < this->get_width());

        // For each value on this axis, assert it is not a dot
        for (int i = 0; i < this->get_height(); i++) {
            assert(!this->get_dot(x, i));
        }

        for (int i = 1; i+x < this->get_width(); i++) {
            // For each dot in the column, logical OR it with the dot in the corresponding negative column
            for (int j = 0; j < this->get_height(); j++) {
                this->set_dot(x-i, j, this->get_dot(x+i, j) || this->get_dot(x-i, j));
            }
        }

        // Remove all columns after x
        int width = this->get_width();
        for (int i = x; i < width; i++) {
            this->remove_column(x);
        }
    }

    // Function: fold_y_axis
    // Description: Folds the paper vertically
    // Parameters:
    //      int y: The y coordinate (row) to fold from
    // Return:
    //      None
    void fold_y_axis(int y) {
        // Assert y is within bounds
        assert(y >= 0 && y < this->get_height());

        // For each value on this axis, assert it is not a dot
        for (int i = 0; i < this->get_width(); i++) {
            assert(!this->get_dot(i, y));
        }

        for (int i = 1; i+y < this->get_height(); i++) {
            // For each dot in the row, logical OR it with the dot in the corresponding negative row
            for (int j = 0; j < this->get_width(); j++) {
                this->set_dot(j, y-i, this->get_dot(j, y+i) || this->get_dot(j, y-i));
            }
        }

        // Remove all rows after y
        for (int i = y; i >= 0; i--) {
            this->remove_row(y);
        }
    }

    // Function: count_dots
    // Description: Counts the number of dots in the paper
    // Return:
    //      int: The number of dots in the paper
    int count_dots() {
        int count = 0;

        for (int i = 0; i < this->get_width(); i++) {
            for (int j = 0; j < this->get_height(); j++) {
                if (this->get_dot(i, j)) {
                    count++;
                }
            }
        }

        return count;
    }

};

struct FOLD_COMMANDS {
    // string axis: The axis to fold
    string axis;

    // int value: The value to fold
    int value;
};

struct INPUT {
    // Vector of vectors of ints representing the coordinates of the dots
    vector<vector<int>> * dots;

    // Vector of FOLD_COMMANDS representing the fold commands
    vector<FOLD_COMMANDS> * fold_commands;
};

// Function: parse_coordinates_and_folds
// Description: Parses the coordinates and folds from input
// Parameters:
//      vector<string> * lines: The lines of the input
// Return:
//      INPUT: The input
INPUT parse_coordinates_and_folds(vector<string> * lines) {
    // Vector of vectors of ints representing the coordinates of the dots
    vector<vector<int>> * dots = new vector<vector<int>>();

    // Vector of FOLD_COMMANDS representing the fold commands
    vector<FOLD_COMMANDS> * fold_commands = new vector<FOLD_COMMANDS>();

    int j = 0;

    // Parse each line until an empty line is reached
    // Each line is of the form "x,y"
    for (int i = 0; i < lines->size(); i++) {

        // If the line is empty, break
        if (lines->at(i).empty()) {
            j = ++i;
            break;
        }

        // Split the line
        vector<string> * line_split = split_string(lines->at(i), ',');

        // Assert two values
        assert(line_split->size() == 2);

        // Parse the line
        // Parse the coordinates
        vector<int> * coordinates = new vector<int>();
        coordinates->push_back(stoi(line_split->at(0)));
        coordinates->push_back(stoi(line_split->at(1)));

        // Add the coordinates to the dots
        dots->push_back(*coordinates);
    
    }

    // Compile regex for: "Fold along [xy]=[0-9]+"
    regex fold_regex("fold along ([xy])=([0-9]+)");

    // Starting at the next line, parse the fold commands
    // Fold commands are of the form "Fold along [xy]=[0-9]+"
    for (int i = j; i < lines->size(); i++) {
        // If the line is empty, break
        if (lines->at(i).empty()) {
            break;
        }

        // Parse the line
        smatch fold_match;
        if (regex_match(lines->at(i), fold_match, fold_regex)) {
            // Get the axis and value
            FOLD_COMMANDS fold_command;
            fold_command.axis = fold_match.str(1);
            fold_command.value = stoi(fold_match.str(2));

            // Add the fold command to the fold commands
            fold_commands->push_back(fold_command);
        }
    }

    // Return the input
    INPUT input;
    input.dots = dots;
    input.fold_commands = fold_commands;
    return input;
}

// Function: create_paper
// Description: Creates a paper from the given coordinates
// Parameters:
//      vector<vector<int>> * coordinates: The coordinates
// Return:
//      Paper: The paper
Paper create_paper(vector<vector<int>> * coordinates) {
    // Create the paper
    vector<vector<bool>> * paper = new vector<vector<bool>>();

    // Find max x and y
    int max_x = 0;
    int max_y = 0;
    for (int i = 0; i < coordinates->size(); i++) {
        if (coordinates->at(i).at(0) > max_x) {
            max_x = coordinates->at(i).at(0);
        }
        if (coordinates->at(i).at(1) > max_y) {
            max_y = coordinates->at(i).at(1);
        }
    }

    // Initialize the paper to all false
    for (int i = 0; i <= max_x; i++) {
        vector<bool> row;
        for (int j = 0; j <= max_y; j++) {
            row.push_back(false);
        }
        paper->push_back(row);
    }

    Paper p = Paper(paper);

    // Set the dots
    for (int i = 0; i < coordinates->size(); i++) {
        p.set_dot(coordinates->at(i).at(0), coordinates->at(i).at(1), true);
    }

    // Return the paper
    return p;
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
    INPUT input = parse_coordinates_and_folds(lines);

    if (DEBUG) {
        // Print input
        cout << "Input:" << endl;
        cout << "Dots:" << endl;
        for (int i = 0; i < input.dots->size(); i++) {
            cout << input.dots->at(i).at(0) << "," << input.dots->at(i).at(1) << endl;
        }
        cout << "Fold Commands:" << endl;
        for (int i = 0; i < input.fold_commands->size(); i++) {
            cout << input.fold_commands->at(i).axis << "," << input.fold_commands->at(i).value << endl;
        }
    }

    // Create the paper
    Paper paper = create_paper(input.dots);

    // Print the paper
    if (DEBUG) {
        paper.print();
    }

    // Print paper dimensions
    if (DEBUG) {
        cout << "Paper dimensions: " << paper.get_width() << "x" << paper.get_height() << endl;
    }

    // Fold the paper
    //paper.fold_y_axis(7);
    paper.fold_x_axis(655);

    // Print the paper
    if (DEBUG) {
        paper.print();
    }
    

    cout << paper.count_dots();

    // 847
}

