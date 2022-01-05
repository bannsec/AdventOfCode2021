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

/*
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
*/

// Class: Letter
// Description: Represents a letter in the Polymer Template
// Parameters:
//      char letter: The letter
// Attributes:
//      char letter: The letter
//      vector<char> *to_append: The letters to append to the letter
// Methods:
//      void append(char letter): Appends a letter to the letter append vector
class Letter {
    public:
        char letter;
        vector<char> *to_append;

        Letter(char letter) {
            this->letter = letter;
            this->to_append = new vector<char>();
        }

        void append(char letter) {
            this->to_append->push_back(letter);
        }
};

// Class: Polymer
// Description: Represents a polymer template
// Parameters:
//      string letters: The letters of the polymer template
// Attributes:
//      vector<Letter> *letters: The letters of the polymer template
// Methods:
//      void append(char letter): Appends a letter to the polymer template
class Polymer {
    public:
        vector<Letter> *letters;

        Polymer(string letters) {
            this->letters = new vector<Letter>();

            // For each letter in the letters string
            for (char letter : letters) {
                // Create a new letter
                Letter *l = new Letter(letter);

                // Append the letter to the polymer template
                this->letters->push_back(*l);
            }
        }

        ~Polymer() {
            delete this->letters;
        }

        // Method: to_string
        // Description: Converts the polymer template to a string
        // Return:
        //      string: The string representation of the polymer template
        string to_string() {
            string str = "";

            // For each letter in the polymer template
            for (Letter l : *this->letters) {
                // Append the letter to the string
                str += l.letter;

                // For each letter to append to the letter
                for (char letter : *l.to_append) {
                    // Append the letter to the string
                    str += letter;
                }
            }

            return str;
        }

        // Method: flatten_polymer
        // Description: Flattens the polymer template by creating a new polymer template from the to_string of the polymer template
        // Return:
        //      Polymer *: The flattened polymer template
        Polymer * flatten_polymer() {
            // Create a new polymer template from the to_string of the polymer template
            Polymer * flattened_polymer = new Polymer(this->to_string());

            return flattened_polymer;
        }
};

// Struct: PAIR_INSERTION
// Description: Represents a pair insertion
// Parameters:
//      char letter_1: The first letter
//      char letter_2: The second letter
//      char letter_insert: The letter to insert
struct PAIR_INSERTION {
    char letter_1;
    char letter_2;
    char letter_insert;

    PAIR_INSERTION(char letter_1, char letter_2, char letter_insert) {
        this->letter_1 = letter_1;
        this->letter_2 = letter_2;
        this->letter_insert = letter_insert;
    }
};

// Pair insertions vector
vector<PAIR_INSERTION> *pair_insertions = new vector<PAIR_INSERTION>();

// Function: run_pair_insertion
// Description: Runs a pair insertion
//              Looks for letter_1 directly followed by letter_2 in the polymer template and, if found inserts a letter in to_append
// Parameters:
//      Polymer *polymer: The polymer template
//      PAIR_INSERTION *pair_insertion: The pair insertion
void run_pair_insertion(Polymer *polymer, PAIR_INSERTION *pair_insertion) {
    // For each letter in the polymer template
    for (int i = 0; i < polymer->letters->size(); i++) {
        // Get the letter
        Letter l = polymer->letters->at(i);

        // If the letter is the first letter of the pair insertion
        if (l.letter == pair_insertion->letter_1) {
            // If the letter is the second letter of the pair insertion
            if (i + 1 < polymer->letters->size() && polymer->letters->at(i + 1).letter == pair_insertion->letter_2) {
                // Insert the letter to append
                l.to_append->push_back(pair_insertion->letter_insert);

                // Replace the letter in the polymer template
                polymer->letters->at(i) = l;
            }
        }
    }
}

// Function: run_pair_insertions
// Description: Runs all pair insertions
// Parameters:
//      Polymer *polymer: The polymer template
void run_pair_insertions(Polymer *polymer) {
    // For each pair insertion
    for (PAIR_INSERTION pair_insertion : *pair_insertions) {
        // Run the pair insertion
        run_pair_insertion(polymer, &pair_insertion);
    }
}

// Function: calculate_letter_frequency
// Description: Calculates the frequency of each letter in the polymer template
// Parameters:
//      Polymer *polymer: The polymer template
// Return:
//      map<char, int> *: A map of the letter and the frequency of the letter
map<char, int> * calculate_letter_frequency(Polymer *polymer) {
    // Map of the letter and the frequency of the letter
    map<char, int> *letter_frequency = new map<char, int>();

    // For each letter in the polymer template
    for (Letter l : *polymer->letters) {
        // If the letter is in the map
        if (letter_frequency->find(l.letter) != letter_frequency->end()) {
            // Increment the frequency of the letter
            letter_frequency->at(l.letter)++;
        } else {
            // Add the letter to the map
            letter_frequency->insert(pair<char, int>(l.letter, 1));
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
//      Polymer *polymer: The polymer template
// Return:
//      int: The score of the polymer template
int calculate_score(Polymer *polymer) {
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
    string polymer_template = lines->at(0);
    Polymer *polymer = new Polymer(polymer_template);

    // If debug, print the polymer template
    if (DEBUG) {
        cout << "Polymer template: " << polymer->to_string() << endl;
    }

    // Starting at third line, each line is a pair insertion of the form AB -> C
    for (int i = 2; i < lines->size(); i++) {
        // Split the line into the two letters
        string line = lines->at(i);
        regex r("(\\w)(\\w) -> (\\w)");
        smatch m;
        regex_search(line, m, r);

        // Create a pair insertion
        PAIR_INSERTION *pair_insertion = new PAIR_INSERTION(m[1].str()[0], m[2].str()[0], m[3].str()[0]);

        // Append the pair insertion to the pair insertions vector
        pair_insertions->push_back(*pair_insertion);

        // If debug, print the pair insertion
        if (DEBUG) {
            cout << "Pair insertion: " << pair_insertion->letter_1 << pair_insertion->letter_2 << " -> " << pair_insertion->letter_insert << endl;
        }
    }

    for (int i=0; i < 10; i++) {

        // Run all pair insertions
        run_pair_insertions(polymer);

        // Flatten the polymer template
        Polymer *flattened_polymer = polymer->flatten_polymer();
        delete polymer;
        polymer = flattened_polymer;

        // If debug, print the polymer template
        if (DEBUG) {
            cout << "Polymer template: " << polymer->to_string() << endl;
        }
    }

    // Calculate the score of the polymer template
    int score = calculate_score(polymer);

    // Print the score
    cout << "Score: " << score << endl;

    // 2891    

}
