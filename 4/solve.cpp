#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include <dirent.h>


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

// Class: BingoBoard
// Description: Represents a bingo board
class BingoBoard {
    public:
        // Constructor
        BingoBoard(vector<string> * lines) {
            // Initialize the board
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    board[i][j] = 0;
                    board_filled[i][j] = false;
                }
            }

            // Parse the lines
            for (int i = 0; i < lines->size(); i++) {
                vector<string> * line_split = split_string((*lines)[i], ' ');
                for (int j = 0; j < line_split->size(); j++) {
                    board[i][j] = stoi((*line_split)[j]);
                }
            }
        }

        // Function: check_row
        // Description: Checks if a row is filled
        // Parameters:
        //      int row: The row to check
        // Return:
        //      bool: True if the row is filled, false otherwise
        bool check_row(int row) {
            for (int i = 0; i < 5; i++) {
                if (board_filled[row][i] == false) {
                    return false;
                }
            }
            return true;
        }

        // Function: check_column
        // Description: Checks if a column is filled
        // Parameters:
        //      int column: The column to check
        // Return:
        //      bool: True if the column is filled, false otherwise
        bool check_column(int column) {
            for (int i = 0; i < 5; i++) {
                if (board_filled[i][column] == false) {
                    return false;
                }
            }
            return true;
        }

        // Function: check_diagonal
        // Description: Checks if a diagonal is filled
        // Parameters:
        //      int diagonal: The diagonal to check
        // Return:
        //      bool: True if the diagonal is filled, false otherwise
        bool check_diagonal(int diagonal) {
            if (diagonal == 0) {
                for (int i = 0; i < 5; i++) {
                    if (board_filled[i][i] == false) {
                        return false;
                    }
                }
                return true;
            } else {
                for (int i = 0; i < 5; i++) {
                    if (board_filled[i][4 - i] == false) {
                        return false;
                    }
                }
                return true;
            }
        }

        // Function: check_bingo
        // Description: Checks if the board is filled
        // Return:
        //      bool: True if the board is filled, false otherwise
        bool check_bingo() {
            for (int i = 0; i < 5; i++) {
                if (check_row(i) || check_column(i)) {
                    return true;
                }
            }
            /*
            if (check_diagonal(0) && check_diagonal(1)) {
                return true;
            }
            */
            return false;
        }

        // Function: print_board
        // Description: Prints the board
        void print_board() {
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    cout << board[i][j] << " ";
                }
                cout << endl;
            }
        }

        // Function: print_board_filled
        // Description: Prints the board filled
        void print_board_filled() {
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    cout << board_filled[i][j] << " ";
                }
                cout << endl;
            }
        }

        // Function: fill_board
        // Description: Fills the board
        // Parameters:
        //      int number: The number to fill the board with
        void fill_board(int number) {
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    if (board[i][j] == number) {
                        board_filled[i][j] = true;
                    }
                }
            }
        }

        // Function: sum_filled_board
        // Description: Sums all spot where the board is filled
        // Return:
        //      int: The sum of all filled spots
        int sum_filled_board() {
            int sum = 0;
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    if (board_filled[i][j] == true) {
                        sum += board[i][j];
                    }
                }
            }
            return sum;
        }

        // Function: sum_unfilled_board
        // Description: Sums all spot where the board is not filled
        // Return:
        //      int: The sum of all unfilled spots
        int sum_unfilled_board() {
            int sum = 0;
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    if (board_filled[i][j] == false) {
                        sum += board[i][j];
                    }
                }
            }
            return sum;
        }

    private:
        int board[5][5];
        bool board_filled[5][5];
};

// Function: load_board
// Description: Loads a bingo board from a file
// Parameters:
//      string file_name: The file to load the board from
// Return:
//      BingoBoard *: The bingo board
BingoBoard * load_board(string file_name) {
    vector<string> * lines = read_file(file_name);
    if (lines == nullptr) {
        return nullptr;
    }

    return new BingoBoard(lines);
}

// Vector of boards
vector<BingoBoard *> boards;

// Function: load_boards
// Description: Loads a vector of bingo boards from a directory into global boards vector
// All files in the directory must be bingo boards regardless of their name
// Prints debugging information if DEBUG is defined
// Parameters:
//      string directory: The directory to load the boards from
// Return:
//      bool: True if the boards were loaded, false otherwise
bool load_boards(string directory) {
    // Open directory
    DIR * dir;
    struct dirent * ent;
    if ((dir = opendir(directory.c_str())) != NULL) {
        // Read directory
        while ((ent = readdir(dir)) != NULL) {
            // Get file name
            string file_name = ent->d_name;

            // Check if file is a bingo board
            if (file_name.find(".txt") != string::npos) {
                // Load board
                BingoBoard * board = load_board(directory + "/" + file_name);
                if (board == nullptr) {
                    cout << "Failed to load board: " << file_name << endl;
                    return false;
                }

                // Add board to boards vector
                boards.push_back(board);

                // Print debugging information
                if (DEBUG) {
                    cout << "Loaded board: " << file_name << endl;
                }
            }
        }
        closedir(dir);
    } else {
        cout << "Failed to open directory: " << directory << endl;
        return false;
    }

    return true;
}

// Function: check_for_bingo
// Description: Checks if a board in global boards has bingo
// Parameters:
//      None
// Return:
//      bool: True if any board board has bingo, false otherwise
bool check_for_bingo() {
    for (int i = 0; i < boards.size(); i++) {
        if (boards[i]->check_bingo()) {
            return true;
        }
    }
    return false;
}

int main(int argc, char *argv[]) {

    // Check if the user has provided a file name
    // Help message uses argv[0]
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <boards_dir> <numbers>" << endl;
        return 1;
    }

    // Load boards
    if (!load_boards(argv[1])) {
        return 1;
    }
    
    // Print number of boards loaded
    cout << "Loaded " << boards.size() << " boards" << endl;

    // Load numbers, split on commas
    vector<string> *numbers_str = split_string(argv[2], ',');
    if (numbers_str == nullptr) {
        return 1;
    }
    // Convert numbers_str to vector of ints
    vector<int> numbers;
    for (int i = 0; i < numbers_str->size(); i++) {
        int number = stoi((*numbers_str)[i]);
        numbers.push_back(number);
    }

    // Print number of numbers loaded
    cout << "Loaded " << numbers.size() << " numbers" << endl;

    int number_just_called = 0;

    // While no board has bingo and there are numbers left
    while (!check_for_bingo() && numbers.size() > 0) {
        // Fill all boards with the first number
        for (int i = 0; i < boards.size(); i++) {
            boards[i]->fill_board(numbers[0]);
        }

        number_just_called = numbers[0];

        // Remove the first number
        numbers.erase(numbers.begin());
    }

    // If bingo was found, print the board
    if (check_for_bingo()) {
        for (int i = 0; i < boards.size(); i++) {
            if (boards[i]->check_bingo()) {
                boards[i]->print_board();
                // Print solution: Sum of all unfilled numbers in the board multiplied by the number just called
                cout << "Solution: " << boards[i]->sum_unfilled_board() * number_just_called << endl;
            }
        }
    } else {
        cout << "No bingo" << endl;
    }


}
