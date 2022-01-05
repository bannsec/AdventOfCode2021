#include<iostream>
#include<fstream>
#include<vector>
#include<string>

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
// Global variables

class Lanternfish;

// Vector * of lanternfish
vector<Lanternfish *> *lanternfish = new vector<Lanternfish *>();


// Class representing a lanternfish
// Description:
//      A lanternfish is a fish. It contains an internal timer (int) and a method called tick()
class Lanternfish {
    public:
        // Constructor
        Lanternfish(int timer) {
            this->timer = timer;
        }

        // Method: tick
        // Description: Follows this algorithm:
        //      If the timer is greater than 0, decrement the timer
        //      If the timer is 0, create a new lanternfish with a timer of 8 and add it to the vector, also reset this timer to 6
        // Parameters:
        //      None
        // Return:
        //      None
        void tick() {
            if (this->timer > 0) {
                this->timer--;
            } else {
                this->timer = 6;
                lanternfish->push_back(new Lanternfish(8));
            }
        }

        // Method: get_timer
        // Description: Returns the internal timer
        // Parameters:
        //      void
        // Return:
        //      int: The internal timer
        int get_timer() {
            return this->timer;
        }

    private:
        // Internal timer
        int timer;
};

// Function: parse_file
// Description: Parses the file and creates a vector of lanternfish
//              File is a comma separated list of integers representing the timer of each lanternfish
//              Example: 1,2,3,4,5,6,7,8,9,10
//              The lanternfish with a timer of 1 will be created, then the lanternfish with a timer of 2, etc. Each will be added to the global vector
// Parameters:
//        vector<string> * lines: The lines of the file
// Return:
//        bool: True if the file was parsed successfully, false otherwise
bool parse_file(vector<string> * lines) {
    // Check if the file is empty
    if (lines->size() == 0) {
        cout << "File is empty" << endl;
        return false;
    }

    // Split the first line
    vector<string> * split_line = split_string(lines->at(0), ',');

    // Check if the line is empty
    if (split_line->size() == 0) {
        cout << "File is empty" << endl;
        return false;
    }

    // Create the lanternfish
    for (int i = 0; i < split_line->size(); i++) {
        lanternfish->push_back(new Lanternfish(stoi(split_line->at(i))));
    }

    return true;
}

// Function: do_tick
// Description: Calls the tick() method of each lanternfish then prints the timer of each lanternfish
// Parameters:
//        void
// Return:
//        None
void do_tick() {
    // Iterate through the lanternfish
    int size = lanternfish->size();
    for (int i = 0; i < size; i++) {
        lanternfish->at(i)->tick();
    }

    if ( DEBUG ) {
        // Print the timer of each lanternfish
        for (int i = 0; i < size; i++) {
            cout << lanternfish->at(i)->get_timer() << " ";
        }
        cout << endl;
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

    // Parse the file
    if (!parse_file(lines)) {
        return -1;
    }

    // Do the tick 18 times
    for (int i = 0; i < 80; i++) {
        do_tick();
    }

    // Print solution: number of fish
    cout << lanternfish->size() << endl;
}
