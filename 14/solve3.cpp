#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<map>
#include<set>
#include<algorithm>
#include<assert.h>
#include<regex>

// Compile with: g++ -std=c++11 -o solve2 solve2.cpp
// To compile with ASAN enabled: g++ -g -fsanitize=address -std=c++11 -o solve2 solve2.cpp
// To compile optimized for speed: g++ -O3 -std=c++11 -o solve3 solve3.cpp
// To compile for code profiling: g++ -pg -std=c++11 -o solve3 solve3.cpp
//       to visualize the profiling results: gprof solve2 gmon.out > profile.txt
//       to visualize the profiling results in a nice graph: kcachegrind profile.txt
//       convert gmon.out to png: kcachegrind profile.txt > profile.png
// To use callgrind: valgrind --tool=callgrind --callgrind-out-file=callgrind.out ./solve3
// To valgrind to see memory usage: valgrind --tool=memcheck --leak-check=full --show-reachable=yes ./solve3

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

// Map to store the pair insertions, where the key is the pair and the value is the letter to insert
//map<string, char> *pair_insertions = new map<string, char>();
// 2-Dimensional pair_insertions array where both indexes are chars
char pair_insertions[26][26] = {0};

// Function: calculate_letter_frequency
// Description: Calculates the frequency of each letter in the polymer template
// Parameters:
//      string polymer: The polymer template
// Return:
//      map<char, int> *: A map of the letter and the frequency of the letter
map<char, int> * calculate_letter_frequency(string polymer) {
    // Create a map of the letter and the frequency of the letter
    map<char, int> *letter_frequency = new map<char, int>();

    // For each letter in the polymer template
    for (char letter : polymer) {
        // If the letter is not in the map
        if (letter_frequency->find(letter) == letter_frequency->end()) {
            // Add the letter to the map
            letter_frequency->insert(pair<char, int>(letter, 1));
        } else {
            // Increment the frequency of the letter
            (*letter_frequency)[letter]++;
        }
    }

    return letter_frequency;
}

// Function: sort_letter_frequency
// Description: Sorts the letter frequency in descending order
// Parameters:
//      map<char, int> *letter_frequency: The letter frequency
// Return:
//      vector<pair<char, int>> *: The sorted letter frequency
vector<pair<char, int>> * sort_letter_frequency(map<char, int> *letter_frequency) {
    // Vector of the sorted letter frequency
    vector<pair<char, int>> *sorted_letter_frequency = new vector<pair<char, int>>();

    // For each letter in the letter frequency
    for (pair<char, int> letter_frequency_pair : *letter_frequency) {
        // Add the letter frequency pair to the vector
        sorted_letter_frequency->push_back(letter_frequency_pair);
    }

    // Sort the vector in descending order
    sort(sorted_letter_frequency->begin(), sorted_letter_frequency->end(), [](pair<char, int> a, pair<char, int> b) {
        return a.second > b.second;
    });

    return sorted_letter_frequency;
}

// Function: calculate_score
// Description: Calculates the score of the polymer template
// Algorithm:
//      1. Calculate the sorted letter frequency of the polymer template
//      2. Subtract the letter with the lowest frequency from the letter with the highest frequency
// Parameters:
//      string polymer: The polymer template
// Return:
//      int: The score of the polymer template
int calculate_score(string polymer) {
    // Calculate the sorted letter frequency of the polymer template
    map<char, int> *letter_frequency = calculate_letter_frequency(polymer);

    // Sort the letter frequency in descending order
    vector<pair<char, int>> *sorted_letter_frequency = sort_letter_frequency(letter_frequency);

    // Get the letter with the lowest frequency
    char lowest_frequency_letter = sorted_letter_frequency->at(0).first;

    // Get the letter with the highest frequency
    char highest_frequency_letter = sorted_letter_frequency->at(sorted_letter_frequency->size() - 1).first;

    // Calculate the score of the polymer template
    int score = sorted_letter_frequency->at(0).second - sorted_letter_frequency->at(sorted_letter_frequency->size() - 1).second;

    // Delete the letter frequency
    delete letter_frequency;

    // Delete the sorted letter frequency
    delete sorted_letter_frequency;

    return score;
}

// Function: run_pair_insertions
// Description: Runs all pair insertions
// Parameters:
//      string polymer: The polymer template
// Algorithm:
//      1. For each two letters in the polymer template
//      2. If the two letters are a pair
//      3. Insert the letter to the pair
// Return:
//      string: The polymer template after the pair insertions
string run_pair_insertions(string polymer) {
    string new_polymer = "";
    char letter_1;
    char letter_2;

    // For each two letters in the polymer template
    for (int i = 0; i < polymer.size() - 1; i++) {
        // Get the first letter
        letter_1 = polymer[i];

        // Get the second letter
        letter_2 = polymer[i + 1];

        // Append the first letter to the new polymer
        new_polymer += letter_1;
    
        // If there is an insertion for the pair
        if (pair_insertions[letter_1 - 'A'][letter_2 - 'A'] != 0) {
            // Insert the letter to the pair
            //new_polymer += pair_insertions->at(string(1, letter_1) + string(1, letter_2));
            new_polymer += pair_insertions[letter_1 - 'A'][letter_2 - 'A'];
        } 
    }

    new_polymer += letter_2;

    return new_polymer;
}

// Function: lookup_pair_insertion
// Description: Looks up the pair insertion for the pair
// Parameters:
//      char letter_1: The first letter
//      char letter_2: The second letter
// Return:
//      char: The letter to insert or 0 if there is no pair insertion
char lookup_pair_insertion(char letter_1, char letter_2) {
    if ( pair_insertions[letter_1 - 'A'][letter_2 - 'A'] != 0 ) {
        return pair_insertions[letter_1 - 'A'][letter_2 - 'A'];
    } else {
        return 0;
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

    // First line is the polymer template
    string polymer = lines->at(0);

    // If debug, print the polymer template
    if (DEBUG) {
        cout << "Polymer template: " << polymer << endl;
    }

    // Starting at third line, each line is a pair insertion of the form AB -> C
    for (int i = 2; i < lines->size(); i++) {
        // Split the line into the two letters
        string line = lines->at(i);
        regex r("(\\w)(\\w) -> (\\w)");
        smatch m;
        regex_search(line, m, r);

        // Add the pair insertion to the map
        //pair_insertions->insert(pair<string, char>(m[1].str() + m[2].str(), m[3].str()[0]));
        pair_insertions[m[1].str()[0] - 'A'][m[2].str()[0] - 'A'] = m[3].str()[0];

        // If debug, print the pair insertion
        if (DEBUG) {
            cout << "Pair insertion: " << m[1].str() + m[2].str() << " -> " << m[3].str() << endl;
        }
    }

    for (int i=0; i < 20; i++) {
        cout << "i: " << i << endl;

        // Run all pair insertions
        polymer = run_pair_insertions(polymer);

        // If debug, print the polymer template
        if (DEBUG) {
            cout << "Polymer template: " << polymer << endl;
        }
    }

    // Calculate the score of the polymer template
    int score = calculate_score(polymer);

    // Print the score
    cout << "Score: " << score << endl;

    // Delete the lines
    delete lines;

    // Slowest part of the program is += string concatenation... Not sure how to fix that.

    return 0;
}
