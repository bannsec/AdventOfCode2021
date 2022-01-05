#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<map>
#include<set>
#include<algorithm>
#include<assert.h>

using namespace std;

#define DEBUG 0

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

// Class: Board
// represents a x by y sized board where each cell is an int
class Board {
    public:
        // Constructor
        Board(int x, int y) {
            this->x = x;
            this->y = y;
            this->board = new int*[x];
            for (int i = 0; i < x; i++) {
                this->board[i] = new int[y];
            }
        }

        // Destructor
        ~Board() {
            for (int i = 0; i < this->x; i++) {
                delete[] this->board[i];
            }
            delete[] this->board;
        }

        // Function: get_cell
        // Description: Gets the value of a cell
        // Parameters:
        //      int x: The x coordinate of the cell
        //      int y: The y coordinate of the cell
        // Return:
        //      int: The value of the cell
        int get_cell(int x, int y) {
            return this->board[x][y];
        }

        // Function: set_cell
        // Description: Sets the value of a cell
        // Parameters:
        //      int x: The x coordinate of the cell
        //      int y: The y coordinate of the cell
        //      int value: The value to set the cell to
        void set_cell(int x, int y, int value) {
            this->board[x][y] = value;
        }

        // Function: print
        // Description: Prints the board to the console
        void print() {
            for (int i = 0; i < this->x; i++) {
                for (int j = 0; j < this->y; j++) {
                    cout << this->board[i][j] << " ";
                }
                cout << endl;
            }
        }

        // Function: get_x
        // Description: Gets the x size of the board
        // Return:
        //      int: The x size of the board
        int get_x() {
            return this->x;
        }

        // Function: get_y
        // Description: Gets the y size of the board
        // Return:
        //      int: The y size of the board
        int get_y() {
            return this->y;
        }

        // Function: get_cell_neighbors
        // Description: Gets the neighbors of a cell
        // Parameters:
        //      int x: The x coordinate of the cell
        //      int y: The y coordinate of the cell
        // Return:
        //      vector<pair<int, int>> *: A vector of pairs of the neighbors
        vector<pair<int, int>> * get_cell_neighbors(int x, int y) {
            vector<pair<int, int>> * neighbors = new vector<pair<int, int>>();

            // Check if the cell is on the edge of the board
            if (x == 0) {
                if (y == 0) {
                    neighbors->push_back(make_pair(x, y + 1));
                    neighbors->push_back(make_pair(x + 1, y));
                } else if (y == this->y - 1) {
                    neighbors->push_back(make_pair(x, y - 1));
                    neighbors->push_back(make_pair(x + 1, y));
                } else {
                    neighbors->push_back(make_pair(x, y - 1));
                    neighbors->push_back(make_pair(x, y + 1));
                    neighbors->push_back(make_pair(x + 1, y));
                }
            } else if (x == this->x - 1) {
                if (y == 0) {
                    neighbors->push_back(make_pair(x, y + 1));
                    neighbors->push_back(make_pair(x - 1, y));
                } else if (y == this->y - 1) {
                    neighbors->push_back(make_pair(x, y - 1));
                    neighbors->push_back(make_pair(x - 1, y));
                } else {
                    neighbors->push_back(make_pair(x, y - 1));
                    neighbors->push_back(make_pair(x, y + 1));
                    neighbors->push_back(make_pair(x - 1, y));
                }
            } else {
                if (y == 0) {
                    neighbors->push_back(make_pair(x, y + 1));
                    neighbors->push_back(make_pair(x - 1, y));
                    neighbors->push_back(make_pair(x + 1, y));
                } else if (y == this->y - 1) {
                    neighbors->push_back(make_pair(x, y - 1));
                    neighbors->push_back(make_pair(x - 1, y));
                    neighbors->push_back(make_pair(x + 1, y));
                } else {
                    neighbors->push_back(make_pair(x, y - 1));
                    neighbors->push_back(make_pair(x, y + 1));
                    neighbors->push_back(make_pair(x - 1, y));
                    neighbors->push_back(make_pair(x + 1, y));
                }
            }

            return neighbors;
        }

    private:
        int x;
        int y;
        int ** board;
};

// Global variables
Board * board;

// Function: init_board
// Description: Initializes a board from a string vector
// Parameters:
//      vector<string> * lines: The lines of the board
void init_board(vector<string> * lines) {
    // Get the x and y size of the board
    int x = lines->size();
    int y = lines->at(0).length();

    // Create the board
    board = new Board(x, y);

    // Fill the board with the values from the lines
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            board->set_cell(i, j, lines->at(i)[j] - '0');
        }
    }
}

// Function: find_lowest_points
// Description: Finds the lowest points on the board
//              The lowest points are the cells who have no neighbors with higher values
// Parameters:
//      None
// Return:
//      vector<pair<int, int>> *: A vector of pairs of the lowest points
vector<pair<int, int>> * find_lowest_points() {
    vector<pair<int, int>> * lowest_points = new vector<pair<int, int>>();

    // Loop through the board
    for (int i = 0; i < board->get_x(); i++) {
        for (int j = 0; j < board->get_y(); j++) {
            // Get the neighbors of the cell
            vector<pair<int, int>> * neighbors = board->get_cell_neighbors(i, j);

            // Check if the cell is a lowest point
            bool is_lowest_point = true;
            for (int k = 0; k < neighbors->size(); k++) {
                if (board->get_cell(neighbors->at(k).first, neighbors->at(k).second) <= board->get_cell(i, j)) {
                    is_lowest_point = false;
                    break;
                }
            }

            // If the cell is a lowest point, add it to the lowest points
            if (is_lowest_point) {
                lowest_points->push_back(make_pair(i, j));
            }

            // Delete the neighbors
            delete neighbors;
        }
    }

    return lowest_points;
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

    init_board(lines);
    vector<pair<int, int>> * lowest_points = find_lowest_points();

    // if debug print the board
    if (DEBUG) {
        board->print();

        // Print the lowest points
        for (int i = 0; i < lowest_points->size(); i++) {
            cout << lowest_points->at(i).first << " " << lowest_points->at(i).second << endl;
        }
    }

    // Print the sum of the lowest points values plus one for each lowest point
    int sum = 0;
    for (int i = 0; i < lowest_points->size(); i++) {
        sum += board->get_cell(lowest_points->at(i).first, lowest_points->at(i).second) + 1;
    }

    cout << sum << endl;

    // 524 is the answer
}
