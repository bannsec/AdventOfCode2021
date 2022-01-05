#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<map>
#include<set>
#include<algorithm>
#include<assert.h>

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
vector<pair <int, int>> * already_checked = new vector<pair <int, int>>();

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

// Function: find_basin_from_point
// Description: Recursively finds the basin from a point
// Parameters:
//      int x: The x coordinate of the point
//      int y: The y coordinate of the point
// Algorithm:
//          a. create a vector of pairs called basin, initialized with x,y
//          b. Find the neighbors of the lowest point
//          c. For each neighbor:
//              i. If the neighbor is value 9, ignore it
//              ii. Otherwise, recursively call find_basin_from_point on the neighbor
//              iii. Merge the basin vector with the basin vector of the neighbor
//          d. Return the basin
// Return:
//      vector<pair<int, int>> *: A vector of pairs of the basin
vector<pair<int, int>> * find_basin_from_point(int x, int y) {

    // Add x,y to already checked
    already_checked->push_back(make_pair(x, y));

    // Create the basin
    vector<pair<int, int>> * basin = new vector<pair<int, int>>();
    basin->push_back(make_pair(x, y));

    // Get the neighbors of the lowest point
    vector<pair<int, int>> * neighbors = board->get_cell_neighbors(x, y);

    // Loop through the neighbors
    for (int i = 0; i < neighbors->size(); i++) {
        // Get the neighbor
        pair<int, int> neighbor = neighbors->at(i);

        // If neighbor in already_checked, ignore it
        if (find(already_checked->begin(), already_checked->end(), neighbor) != already_checked->end()) {
            continue;
        }

        // Check if the neighbor is a value 9
        if (board->get_cell(neighbor.first, neighbor.second) != 9) {

            // Recursively call find_basin_from_point on the neighbor
            vector<pair<int, int>> * neighbor_basin = find_basin_from_point(neighbor.first, neighbor.second);

            // Merge the basin vector with the basin vector of the neighbor
            basin->insert(basin->end(), neighbor_basin->begin(), neighbor_basin->end());

            // Delete the neighbor basin
            delete neighbor_basin;
        }
    }

    // Delete the neighbors
    delete neighbors;

    return basin;
}

// Function: find_basins
// Description: Finds the basins on the board
// Algorithm:
//      1. Find the lowest points
//      2. For each lowest point:
//          a. create a vector of pairs called basin, initialized with the lowest point
//          b. Find the neighbors of the lowest point
//          c. For each neighbor:
//              i. If the neighbor is value 9, ignore it
//              ii. Otherwise, add the neighbor to the basin and repeat step c for this neighbor
//          d. add basin to the basins vector
// Parameters:
//      None
// Return:
//      vector<vector<pair<int, int>>> *: A vector of vectors of pairs of the basins

vector<vector<pair<int, int>>> * find_basins() {
    vector<vector<pair<int, int>>> * basins = new vector<vector<pair<int, int>>>();

    // Find the lowest points
    vector<pair<int, int>> * lowest_points = find_lowest_points();

    // For each lowest point
    for (int i = 0; i < lowest_points->size(); i++) {
        // Create a vector of pairs called basin, initialized with the lowest point
        vector<pair<int, int>> * basin = new vector<pair<int, int>>();
        basin->push_back(lowest_points->at(i));

        // Find the neighbors of the lowest point
        vector<pair<int, int>> * neighbors = board->get_cell_neighbors(lowest_points->at(i).first, lowest_points->at(i).second);

        // For each neighbor
        for (int j = 0; j < neighbors->size(); j++) {
            // If the neighbor is value 9, ignore it
            if (board->get_cell(neighbors->at(j).first, neighbors->at(j).second) == 9) {
                continue;
            }

            // Add the neighbor to the basin and repeat step c for this neighbor
            basin->push_back(neighbors->at(j));
        }

        // Add basin to the basins vector
        basins->push_back(*basin);

        // Delete the basin
        delete basin;
    }

    // Delete the lowest points
    delete lowest_points;

    return basins;
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

    // if debug print the board
    if (DEBUG) {
        board->print();
    }

    // vector of ints to store the solution
    vector<int> *solution = new vector<int>();

    // For each lowest point
    //     a. clear the already_checked vector
    //     b. find the basin from the lowest point
    //     c. store the size of the basin in the solution vector
    //     d. done
    vector<vector<pair<int, int>>> * basins = find_basins();
    for (int i = 0; i < basins->size(); i++) {
        already_checked->clear();
        vector<pair<int, int>> * basin = find_basin_from_point(basins->at(i).at(0).first, basins->at(i).at(0).second);

        // If debug, print the basin
        if (DEBUG) {
            cout << "Basin " << i << ": ";
            for (int j = 0; j < basin->size(); j++) {
                cout << "(" << basin->at(j).first << ", " << basin->at(j).second << ") ";
            }
            cout << endl;
        }

        solution->push_back(basin->size());
        delete basin;
    }

    // Find the 3 largest basins and multiply their sizes
    int largest_basin_size = 0;
    int second_largest_basin_size = 0;
    int third_largest_basin_size = 0;
    for (int i = 0; i < solution->size(); i++) {
        if (solution->at(i) > largest_basin_size) {
            third_largest_basin_size = second_largest_basin_size;
            second_largest_basin_size = largest_basin_size;
            largest_basin_size = solution->at(i);
        } else if (solution->at(i) > second_largest_basin_size) {
            third_largest_basin_size = second_largest_basin_size;
            second_largest_basin_size = solution->at(i);
        } else if (solution->at(i) > third_largest_basin_size) {
            third_largest_basin_size = solution->at(i);
        }
    }

    // Print the solution
    cout << largest_basin_size * second_largest_basin_size * third_largest_basin_size << endl;

    // 1235430
}
