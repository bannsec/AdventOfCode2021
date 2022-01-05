#include<iostream>
#include<fstream>
#include<vector>
#include<string>

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

            // If length of segments is 3, value must be 7
            else if (segments.size() == 3) {
                values = {7};
            }

            // If length of segments is 7, value must be 8
            else if (segments.size() == 7) {
                values = {8};
            }
        }
};

vector<number> * input_values = new vector<number>();
vector<number> * output_values = new vector<number>();

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

    // Find and print the number of output values that contain only one possible value
    int count = 0;
    for (number output : *output_values) {
        if (output.values.size() == 1) {
            count++;
        }
    }
    cout << count << endl;
    // 512
}
