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

// Class number
// Description: Represents a seven-segment number
// Contains:
//      int value: The value of the number
//      vector<char> segments: The segments of the number
// Constructor: Takes no parameters
//      Initializes the value to -1 and the segments to an empty vector
class number {
    public:
        vector<int> values;
        vector<char> segments;

        number() {
            values = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
            segments = vector<char>();
        }

        // Function: set_segments
        // Description: Sets the segments of the number
        // Parameters:
        //      vector<char> segments: The segments to set
        // Return:
        //      void
        void set_segments(vector<char> segments) {
            this->segments = segments;

            // If length of segments is 2, value must be 1
            if (segments.size() == 2) {
                values = {1};
            }

            // If length of segments if 4, value must be 4
            else if (segments.size() == 4) {
                values = {4};
            }

            // If length of segments is 4, value must be in {2, 3, 5}
            else if (segments.size() == 5) {
                values = {2, 3, 5};
            }

            // If length of segments is 6, value must be in {0, 6, 9}
            else if (segments.size() == 6) {
                values = {0, 6, 9};
            }

            // If length of segments is 3, value must be 7
            else if (segments.size() == 3) {
                values = {7};
            }

            // If length of segments is 7, value must be 8
            else if (segments.size() == 7) {
                values = {8};
            }
        }

        // Function: map_is_valid
        // Description: Checks if the map of chr->chr is valid for this number
        // Parameters:
        //      map<char, char> map: The map to check
        // Return:
        //      bool: True if the map is valid, false otherwise
        bool map_is_valid(map<char, char> map) {
            // Ensure the map is valid for all values
            for (int value : this->values) {
                // Ensure the mapped segment is valid
                if (!this->map_is_valid_for_number(value, map)) {
                    return false;
                }
            }

            return true;
        }

        // Function: find_all_valid_values
        // Description: Finds all valid values for this given map
        // Parameters:
        //      map<char, char> map: The map to check
        // Return:
        //      vector<int> *: A vector of valid values
        vector<int> * find_all_valid_values(map<char, char> map) {
            // Vector to store valid values
            vector<int> * valid_values = new vector<int>();

            // Find all valid values
            for (int value : this->values) {
                if (this->map_is_valid_for_number(value, map)) {
                    valid_values->push_back(value);
                }
            }

            return valid_values;
        }

        // Function: map_is_valid_for_number
        // Description: Checks if the map of chr->chr is valid for this number
        // Parameters:
        //       int value: The value of the number
        //       map<char, char> map: The map to check
        // Return:
        //       bool: True if the map is valid, false otherwise
        bool map_is_valid_for_number(int value, map<char, char> map) {

            // Create a set of the segments of the number mapped to the map
            set<char> mapped_segments;
            for (char segment : this->segments) {
                mapped_segments.insert(map[segment]);
            }

            // If the value is 0, the segments must be {a,b,c,e,f,g}
            if (value == 0) {
                return mapped_segments == set<char>({'a', 'b', 'c', 'e', 'f', 'g'});
            }

            // If the value is 1, the segments must be {c, f}
            else if (value == 1) {
                return mapped_segments == set<char>({'c', 'f'});
            }

            // If the value is 2, the segments must be {a,c,d,e,g}
            else if (value == 2) {
                return mapped_segments == set<char>({'a', 'c', 'd', 'e', 'g'});
            }

            // If the value is 3, the segments must be {a,c,d,f,g}
            else if (value == 3) {
                return mapped_segments == set<char>({'a', 'c', 'd', 'f', 'g'});
            }

            // If the value is 4, the segments must be {b,c,d,f}
            else if (value == 4) {
                return mapped_segments == set<char>({'b', 'c', 'd', 'f'});
            }

            // If the value is 5, the segments must be {a,b,d,f,g}
            else if (value == 5) {
                return mapped_segments == set<char>({'a', 'b', 'd', 'f', 'g'});
            }

            // If the value is 6, the segments must be {a,b,d,e,f,g}
            else if (value == 6) {
                return mapped_segments == set<char>({'a', 'b', 'd', 'e', 'f', 'g'});
            }

            // If the value is 7, the segments must be {a,c,f}
            else if (value == 7) {
                return mapped_segments == set<char>({'a', 'c', 'f'});
            }

            // If the value is 8, the segments must be {a,b,c,d,e,f,g}
            else if (value == 8) {
                return mapped_segments == set<char>({'a', 'b', 'c', 'd', 'e', 'f', 'g'});
            }

            // If the value is 9, the segments must be {a,b,c,d,f,g}
            else if (value == 9) {
                return mapped_segments == set<char>({'a', 'b', 'c', 'd', 'f', 'g'});
            }

            // Otherwise, print an error and exit
            else {
                cout << "Error: Invalid value " << value << " for number" << endl;
                exit(1);
            }
        }

};

vector<number> * input_values = new vector<number>();
vector<number> * output_values = new vector<number>();
int solution = 0;

// Function: solve_input
// Description: Solves the input
// Parameters:
//         None
// Return:
//         None
void solve_input() {

    string from = "abcdefgh";
    string to = "abcdefgh";

    // Create vector for valid maps pointers
    vector<map<char, char> *> * valid_maps = new vector<map<char, char> *>();


    do {

        map<char, char> *segment_map = new map<char, char>();

        // Create map from from to to
        for (int i = 0; i < from.length(); i++) {
            segment_map->insert(pair<char, char>(from[i], to[i]));
        }

        // For each number in input_values, get all valid values for this map
        // If any number has no valid values, the map is invalid
        bool map_is_valid = true;
        for (number number : *input_values) {
            vector<int> * valid_values = number.find_all_valid_values(*segment_map);
            if (valid_values->size() == 0) {
                map_is_valid = false;
                break;
            }

            // Print valid values
            //for (int value : *valid_values) {
            //    cout << value << " ";
            //}
        }

        // If the map is valid, add it to the list of valid maps
        if (map_is_valid) {
            valid_maps->push_back(segment_map);
        }

        // Otherwise, delete the map
        else {
            delete segment_map;
        }
        
    } while (next_permutation(to.begin(), to.end()));

    //delete valid_maps;

    // Print number of valid maps found
    cout << "Number of valid maps: " << valid_maps->size() << endl;

    // Assert there should only be one valid map
    assert(valid_maps->size() == 1);

    // Create a string from output values and the valid map
    string output_string = "";

    // For each number in output_values, get the valid value for this map
    for (number number : *output_values) {
        vector<int> * valid_values = number.find_all_valid_values(*valid_maps->at(0));
        assert(valid_values->size() == 1);
        output_string += to_string(valid_values->at(0));
    }

    // Print the output string
    cout << "Output string: " << output_string << endl;

    // Convert string to int and add it to the solution
    solution += stoi(output_string);

    // Clear input values and output values
    input_values->clear();
    output_values->clear();

}

// Function: parse_input
// Description: Parses the input file
//              Input matches the following format:
//                  ([a-g]+ )+| ([a-g]+ )+
// Parameters:
//      vector<string> * lines: The lines of the input file
// Return:
//      None
void parse_input(vector<string> * lines) {
    // Loop through lines
    for (string line : *lines) {

        // If debug is enabled, print the line
        if (DEBUG) {
            cout << line << endl;
        }

        bool is_input = true;

        // Split line into segments
        vector<string> * segments = split_string(line, ' ');

        // Loop through segments
        for (string segment : *segments) {
            // If segment is "|", continue
            if (segment == "|") {
                is_input = false;
                continue;
            }

            // If debug is enabled, print the segment and if it's input or output
            if (DEBUG) {
                cout << segment << " " << (is_input ? "input" : "output") << endl;
            }

            // Create a new number
            number * num = new number();

            // Split string into characters
            vector<char> *chars = new vector<char>();
            for (char c : segment) {
                chars->push_back(c);
            }

            // Set the segments of the number
            num->set_segments(*chars);

            // Add number to vector
            if (is_input) {
                input_values->push_back(*num);
            } else {
                output_values->push_back(*num);
            }
        }
        solve_input();
    }

    // If debug is enabled, print number of input and output values
    if (DEBUG) {
        cout << "Input values: " << input_values->size() << endl;
        cout << "Output values: " << output_values->size() << endl;
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

    parse_input(lines);

    cout << "Solution: " << solution << endl;
}
