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

int total_flashes = 0;

// Class: Dumbo
// Description: A class to represent a dumbo
// Fields:
//      int energy: The energy of the dumbo
//      bool flashed: Whether the dumbo has flashed (defaults to false)
class Dumbo {
    public:
        int energy;
        bool flashed;

        Dumbo(int energy) {
            this->energy = energy;
            this->flashed = false;
        }

        // Function: step
        // Description: Increments the energy of the dumbo by 1. If energy is greater than 9, the dumbo flashes
        void step() {
            this->energy++;
            if (this->energy > 9) {
                // If not already flashed, flash
                if (!this->flashed) {
                    this->flashed = true;
                    total_flashes++;
                }
            }
        }
};

// Class: Dumbo_Grid
// Description: A class to represent a dumbo grid
//              A dumbo grid is a 2D grid of dumbos of size 10,10
//              The grid is indexed by a pair of ints (x,y)  (0,0) is the top left corner
//              Grid is initialized with all dumois having energy 0
// Fields:
//      vector<vector<Dumbo> > grid: The grid of dumbos
class Dumbo_Grid {
    public:
        vector<vector<Dumbo *> > grid;

        Dumbo_Grid() {
            // Initialize grid with all dumois having energy 0
            for (int i = 0; i < 10; i++) {
                vector<Dumbo *> row;
                for (int j = 0; j < 10; j++) {
                    row.push_back(new Dumbo(0));
                }
                grid.push_back(row);
            }
        }

        // Function: get_dumbo
        // Description: Gets the dumbo at the given coordinates
        // Parameters:
        //      int x: The x coordinate
        //      int y: The y coordinate
        // Return:
        //      Dumbo: The dumbo at the given coordinates
        Dumbo *get_dumbo(int x, int y) {
            return grid[y][x];
        }

        // Function: set_dumbo
        // Description: Sets the dumbo at the given coordinates
        // Parameters:
        //      int x: The x coordinate
        //      int y: The y coordinate
        //      Dumbo dumbo *: The dumbo to set
        void set_dumbo(int x, int y, Dumbo *dumbo) {
            grid[y][x] = dumbo;
        }

        // Function: print_grid
        // Description: Prints the grid
        void print_grid() {
            for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 10; j++) {
                    cout << grid[i][j]->energy << " ";
                }
                cout << endl;
            }

            cout << endl;
        }

        // Function: get_xy
        // Description: Gets the x and y coordinates of the dumbo at the given index
        // Parameters:
        //      Dumbo dumbo*: The dumbo to get the coordinates of
        // Return:
        //      pair<int,int>: The x and y coordinates of the dumbo
        pair<int,int> get_xy(Dumbo *dumbo) {
            for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 10; j++) {
                    if (grid[i][j] == dumbo) {
                        return make_pair(j,i);
                    }
                }
            }
            return make_pair(-1,-1);
        }

        // Function: get_neighbors
        // Description: Gets the neighbors of the given dumbo including diagonal neighbors
        // Parameters:
        //      int x: The x coordinate
        //      int y: The y coordinate
        // Return:
        //      vector<Dumbo *> *: A vector of pointers to the neighbors
        vector<Dumbo *> *get_neighbors(int x, int y) {
            vector<Dumbo *> *neighbors = new vector<Dumbo *>();

            // Get the neighbors of the given dumbo
            for (int i = x - 1; i <= x + 1; i++) {
                for (int j = y - 1; j <= y + 1; j++) {
                    if (i >= 0 && i < 10 && j >= 0 && j < 10 && !(i == x && j == y)) {
                        neighbors->push_back(grid[j][i]);
                    }
                }
            }

            return neighbors;
        }

        // Function: step
        // Description: Steps the grid
        // Algorithm:
        //      1. Step each dumbo in the grid
        //      2. Create a set of dumois that have flashed
        //      3. For each dumbo in the set, get the neighbors
        //      4. For each neighbor, step the neighbor
        void step() {
            // Step each dumbo in the grid
            for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 10; j++) {
                    grid[i][j]->step();
                }
            }

            // Create a set of dumbos that have flashed
            set<Dumbo *> flashed;
            set<Dumbo *> flashed_new;
            int num_flashed_prev;

            // Loop until no more dubmos have flashed

            do {

                num_flashed_prev = flashed.size();

                for (int i = 0; i < 10; i++) {
                    for (int j = 0; j < 10; j++) {
                        if (grid[i][j]->flashed && !flashed.count(grid[i][j])) {
                            flashed_new.insert(grid[i][j]);
                        }
                    }
                }

                // For each dumbo in the set, get the neighbors
                for (set<Dumbo *>::iterator it = flashed_new.begin(); it != flashed_new.end(); it++) {
                    vector<Dumbo *> *neighbors = get_neighbors(get_xy(*it).first, get_xy(*it).second);

                    // For each neighbor, step the neighbor
                    for (vector<Dumbo *>::iterator it2 = neighbors->begin(); it2 != neighbors->end(); it2++) {
                        (*it2)->step();
                    }

                    delete neighbors;
                }

                // Merge flashed_new into flashed
                for (set<Dumbo *>::iterator it = flashed_new.begin(); it != flashed_new.end(); it++) {
                    flashed.insert(*it);
                }

                // Clear flashed_new
                flashed_new.clear();

            } while (flashed.size() != num_flashed_prev);

            // For each dumbo who has flashed, set their energy to 0 and set flashed to false
            for (set<Dumbo *>::iterator it = flashed.begin(); it != flashed.end(); it++) {
                (*it)->energy = 0;
                (*it)->flashed = false;
            }
        }

        // Function: get_total_energy
        // Description: Gets the total energy of the grid
        // Return:
        //      int: The total energy of the grid
        int get_total_energy() {
            int total_energy = 0;

            for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 10; j++) {
                    total_energy += grid[i][j]->energy;
                }
            }

            return total_energy;
        }
};

// Global Variables
Dumbo_Grid dumbo_grid = Dumbo_Grid();


// Function: init_grid
// Description: Initializes the grid with the given lines. Each line represents a row of the grid
//              Each character in the line represents the energy of a dumbo
// Example input line:
//                 1234567890
// Parameters:
//      vector<string> * lines: The lines of the file
void init_grid(vector<string> * lines) {
    // Iterate through the lines
    for (int i = 0; i < 10; i++) {
        // Convert each character of the line as an int of dumbo energy
        for (int j = 0; j < 10; j++) {
            dumbo_grid.get_dumbo(j, i)->energy = lines->at(i)[j] - '0';
        }
    }
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

    // Initialize the grid
    init_grid(lines);

    // Print the grid
    dumbo_grid.print_grid();

    // Step the grid 10 times
    for (int i = 0; i < 100; i++) {
        dumbo_grid.step();
        dumbo_grid.print_grid();
    }

    cout << "Total Flashes: " << total_flashes << endl;

    return 0;

    // 1620
}
