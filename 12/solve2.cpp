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

// Class: Cave
// Description: Represents a cave
// Attributes:
//      string name: The name of the cave
//      set<Cave *> * neighbors: A set of neighboring caves
class Cave {
    public:
        string name;
        set<Cave *> * neighbors;

        // Constructor
        Cave(string name) {
            this->name = name;
            this->neighbors = new set<Cave *>();
        }

        // Destructor
        ~Cave() {
            delete this->neighbors;
        }

        // Function: add_neighbor
        // Description: Adds a neighboring cave to the cave
        // Parameters:
        //      Cave * neighbor: The neighboring cave to add
        void add_neighbor(Cave * neighbor) {
            this->neighbors->insert(neighbor);
        }

        // Function: get_neighbors
        // Description: Returns the neighboring caves of the cave
        // Return:
        //      set<Cave *> *: A set of neighboring caves
        set<Cave *> * get_neighbors() {
            return this->neighbors;
        }

        // Function: is_small
        // Description: Returns whether the first character of the cave's name is lowercase
        // Return:
        //      bool: Whether the first character of the cave's name is lowercase
        bool is_small() {
            return this->name[0] >= 'a' && this->name[0] <= 'z';
        }

        // Function: is_large
        // Description: Returns whether the first character of the cave's name is uppercase
        // Return:
        //      bool: Whether the first character of the cave's name is uppercase
        bool is_large() {
            return this->name[0] >= 'A' && this->name[0] <= 'Z';
        }
};

map<string, Cave *> * caves;

// Function: init_caves
// Description: Initializes the caves. Each line is two names separated by a -.
//              The first name is the cave's name, the second name is the cave's neighbor.
//              If the name is "start" or "end", it is the start or end cave.
//              If the cave name doesn't exist, it is created.
// Parameters:
//      vector<string> * lines: The lines of the file
void init_caves(vector<string> * lines) {
    // Initialize the caves
    caves = new map<string, Cave *>();
    Cave *cave;
    Cave *neighbor;

    // Iterate through the lines
    for (string line : *lines) {
        // Split the line
        vector<string> * tokens = split_string(line, '-');

        // Get the cave names
        string cave_name = tokens->at(0);
        string neighbor_name = tokens->at(1);

        // Get the cave        
        if (caves->find(cave_name) == caves->end()) {
            cave = new Cave(cave_name);
            caves->insert(pair<string, Cave *>(cave_name, cave));
        } else {
            cave = caves->find(cave_name)->second;
        }

        // Get the neighbor
        if (caves->find(neighbor_name) == caves->end()) {
            neighbor = new Cave(neighbor_name);
            caves->insert(pair<string, Cave *>(neighbor_name, neighbor));
        } else {
            neighbor = caves->find(neighbor_name)->second;
        }

        // Add the neighbor
        cave->add_neighbor(neighbor);
        neighbor->add_neighbor(cave);
    }
}

// Function: num_times_visited_in_path
// Description: Returns the number of times the cave has been visited
// Parameters:
//      Cave * cave: The cave to check
//      vector<Cave *> * path: The path to check
// Return:
//      int: The number of times the cave has been visited
int num_times_visited_in_path(Cave * cave, vector<Cave *> * path) {
    int num_times_visited = 0;

    // Iterate through the path
    for (Cave * c : *path) {
        // Check if the cave is in the path
        if (c == cave) {
            num_times_visited++;
        }
    }

    return num_times_visited;
}

// Function: small_cave_ok_to_visit
// Description: Returns whether the small cave can be visited
//              Given a path and a cave, follows this algorithm
//              1. If the cave is the start cave, return false
//              2. If the cave is the end cave, return true
//              3. If any small cave has been visited twice already, return false
//              4. Otherwise return true
// Parameters:
//      vector<Cave *> * path: The path to the cave
//      Cave * cave: The cave to check
// Return:
//      bool: Whether the small cave can be visited
bool small_cave_ok_to_visit(vector<Cave *> * path, Cave * cave) {
    // If path is empty, return true
    if (path->size() == 0) {
        return true;
    }

    // Print path
    if (DEBUG) {
        cout << "Path: ";
        for (Cave * c : *path) {
            cout << c->name << ",";
        }
        cout << endl;
    }

    // If the cave is the start cave, return true
    if (cave->name == "start") {
        return false;
    }

    // If the cave is the end cave, return true
    if (cave->name == "end") {
        return true;
    }

    // For each cave in the path
    for (Cave * path_cave : *path) {
        // If the cave is small, count the number of times it has been visited
        if (path_cave->is_small()) {
            if (count(path->begin(), path->end(), path_cave) > 1) {
                // If the cave_path is the cave, this would be 3, so return false
                if (path_cave == cave) {
                    return false;
                } 

                // Else, if the number of times cave has been visited is greater than 0, return false
                else if (num_times_visited_in_path(cave, path) > 0) {
                    return false;
                }
                
            }
        }
    }

    // Otherwise return true
    return true;
}

// Function: path_to_str
// Description: Returns the path as a string
// Parameters:
//      vector<Cave *> * path: The path to the cave
// Return:
//      string: The path as a string
string path_to_str(vector<Cave *> * path) {
    string path_str = "";

    // For each cave in the path
    for (Cave * cave : *path) {
        // Add the cave's name to the path string
        path_str += cave->name + ",";
    }

    return path_str;
}

// Function: find_all_paths
// Description: Recursively finds all the paths from the start cave to the end cave
//              Can only visit a small cave once
// Parameters:
//     Cave * start: The start cave
//     Cave * end: The end cave
//     vector<Cave *> * path: The current path
// Return:
//     vector<vector<Cave *>> *: A vector of paths
vector<vector<Cave *>> * find_all_paths(Cave * start, Cave * end, vector<Cave *> * path) {
    // Create the paths vector
    vector<vector<Cave *>> * paths = new vector<vector<Cave *>>();
    vector<Cave *> *new_path;

    // If path is null, create a new path
    if (path == nullptr) {
        path = new vector<Cave *>();
    }

    if ( path_to_str(path) == "start,b,A,b,A,") {
        cout << "Found path" << endl;
    }

    new_path = new vector<Cave *>(*path);

    // If start is a small cave and it has already been visited, return null
    if (start->is_small() && !small_cave_ok_to_visit(new_path, start)) {
        return nullptr;
    }

    // Push back start
    new_path->push_back(start);

    // If the start and end caves are the same, add the path to the paths vector
    if (start == end) {
        paths->push_back(*new_path);
        return paths;
    }

    // Iterate through the neighbors
    for (Cave * neighbor : *start->get_neighbors()) {
        // print neighbors
        if (DEBUG) {
            cout << "Neighbor: ";
                cout << neighbor->name << " ";
            cout << endl;
        }

        // Call find_all_paths on the neighbor
        vector<vector<Cave *>> * neighbor_paths = find_all_paths(neighbor, end, new_path);

        // Merge the paths
        if (neighbor_paths != nullptr) {
            cout << "Merging paths" << endl;
            // Print neighbor paths count
            if (DEBUG) {
                cout << "Neighbor paths count: " << neighbor_paths->size() << endl;
            }
            paths->insert(paths->end(), neighbor_paths->begin(), neighbor_paths->end());

            // Delete the neighbor paths
            delete neighbor_paths;

            // Print count of paths
            cout << "Path count: " << paths->size() << endl;
        }

    }

    return paths;
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

    init_caves(lines);

    Cave *start = caves->find("start")->second;
    Cave *end = caves->find("end")->second;

    // Find all paths
    vector<vector<Cave *>> * paths = find_all_paths(start, end, nullptr);

    // Print count of paths
    cout << "Paths: " << paths->size() << endl;

    // Print paths
    for (vector<Cave *> path : *paths) {
        for (Cave * cave : path) {
            cout << cave->name << ",";
        }
        cout << endl;
    }

    // 152480
}
