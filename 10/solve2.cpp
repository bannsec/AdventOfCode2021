#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<map>
#include<set>
#include<algorithm>
#include<assert.h>
#include<gmpxx.h>
#include<gmp.h>

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



// Global variables

// Enum: bracket_type
// Description: The type of bracket
// Fields:
//      SQUARE_BRACKET: Square bracket
//      CURLY_BRACKET: Curly bracket
//      ROUND_BRACKET: Round bracket
//      TRIANGLE_BRACKET: Triangle bracket
enum bracket_type {
    SQUARE_BRACKET,
    CURLY_BRACKET,
    ROUND_BRACKET,
    TRIANGLE_BRACKET
};

// Set of legal opening brackets of "{", "[", "(", "<"
set<char> opening_brackets = {'{', '[', '(', '<'};
set<char> closing_brackets = {'}', ']', ')', '>'};

int total_score = 0;

// Create mpz_t total difference score
mpz_t total_difference_score;

// Function: convert_bracket_type
// Description: Converts a bracket to its corresponding bracket type
// Parameters:
//      char bracket: The bracket to convert
// Return:
//      bracket_type: The corresponding bracket type
bracket_type convert_bracket_type(char bracket) {
    switch (bracket) {
        case '{':
            return CURLY_BRACKET;
        case '[':
            return SQUARE_BRACKET;
        case '(':
            return ROUND_BRACKET;
        case '<':
            return TRIANGLE_BRACKET;
        case '}':
            return CURLY_BRACKET;
        case ']':
            return SQUARE_BRACKET;
        case ')':
            return ROUND_BRACKET;
        case '>':
            return TRIANGLE_BRACKET;
        default:
            // Print error and exit
            cout << "Invalid bracket of " << bracket << endl;
            exit(1);
    }
}

// Function: bracket_type_to_string
// Description: Converts a bracket type to a string
// Parameters:
//      bracket_type btype: The bracket type
//      bool is_opening: Whether the bracket is an opening bracket
// Return:
//      string: The string representation of the bracket type
string bracket_type_to_string(bracket_type btype, bool is_opening) {
    switch (btype) {
        case SQUARE_BRACKET:
            return is_opening ? "[" : "]";
        case CURLY_BRACKET:
            return is_opening ? "{" : "}";
        case ROUND_BRACKET:
            return is_opening ? "(" : ")";
        case TRIANGLE_BRACKET:
            return is_opening ? "<" : ">";
        default:
            // Print error and exit
            cout << "Invalid bracket type" << endl;
            exit(1);
    }
}

// Class: chunk
// Description: Represents a chunk of code
// Fields:
//          btype: The type of bracket
//          chunks: The chunks inside this bracket
class Chunk {
public:
    bracket_type btype;
    vector<Chunk *> * chunks;

    Chunk(bracket_type btype) {
        this->btype = btype;
        this->chunks = new vector<Chunk *>();
    }

    ~Chunk() {
        for (Chunk * chunk : *chunks) {
            delete chunk;
        }
        delete chunks;
    }

    // Function: add_chunk
    // Description: Adds a chunk to this chunk
    // Parameters:
    //      Chunk * chunk: The chunk to add
    void add_chunk(Chunk * chunk) {
        chunks->push_back(chunk);
    }

    // Function: to_string
    // Description: Converts this chunk to a string
    // Creates nested opening and closing brackets to represent the chunk
    // Parameters:
    //      None
    // Return:
    //      string: The string representation of this chunk
    string to_string() {
        string str = bracket_type_to_string(btype, true);
        for (Chunk * chunk : *chunks) {
            str += chunk->to_string();
        }
        str += bracket_type_to_string(btype, false);
        return str;
    }

};

// Function: lookup_bracket_score
// Description: Looks up the score of a bracket
// Scores: SQUARE_BRACKET: 57
//         CURLY_BRACKET: 1197
//         ROUND_BRACKET: 3
//         TRIANGLE_BRACKET: 25137
// Parameters:
//      bracket_type btype: The bracket type
// Return:
//      int: The score of the bracket
int lookup_bracket_score(bracket_type btype) {
    switch (btype) {
        case SQUARE_BRACKET:
            return 2;
        case CURLY_BRACKET:
            return 3;
        case ROUND_BRACKET:
            return 1;
        case TRIANGLE_BRACKET:
            return 4;
        default:
            // Print error and exit
            cout << "Invalid bracket type" << endl;
            exit(1);
    }
}

// Function: calculate_difference_score
// Description: Calculates the difference score of a string
// Algorithm:
//      1. Initialize the total_difference_score to 0
//      2. Multiply score by 5
//      3. add the score of the bracket (")" == 1, "]" == 2, "}" == 3, ">" == 4)
//      4. Repeat for each character in the string
// Parameters:
//      string str: The string to calculate the difference score of
// Return:
//      int
int calculate_difference_score(string str) {
    mpz_init(total_difference_score);
    mpz_set_ui(total_difference_score, 0);

    for (char c : str) {
        mpz_mul_ui(total_difference_score, total_difference_score, 5);
        mpz_add_ui(total_difference_score, total_difference_score, lookup_bracket_score(convert_bracket_type(c)));
    }

    // Print the difference score
    cout << "Difference score: " << mpz_get_str(NULL, 10, total_difference_score) << endl;

    return mpz_get_ui(total_difference_score);
}


/*
mpz_t *calculate_difference_score(string str) {
    // Create multi-precision integer pointer
    mpz_t * score = new mpz_t();


    unsigned long long score = 0;
    for (char c : str) {
        score *= 5;
        score += lookup_bracket_score(convert_bracket_type(c));
    }
    return score;
}
*/

// Struct: PCHUNK
// Description: A pair of chunk and its index
// Fields:
//      Chunk * chunk: The chunk
//      int index: The index of the chunk
//      int index_end: The index of the end of the chunk
struct PCHUNK {
    Chunk * chunk;
    int index;
    int index_end;
    bool incomplete = false;
};

// Function: parse_chunk
// Description: Parses a chunk of code
// Algorithm:
//      1. Start parsing index into the line
//      2. Create a chunk of the type of the first bracket
//      3. Recursively parse the chunk starting at the index of the first bracket
//      4. Return the chunk with index and index_end
// Parameters:
//            string line: The line to parse
//            int index: The index to start parsing from
// Return:
//            PCHUNK: The parsed chunk
PCHUNK parse_chunk(string line, int index) {
    // Get the first bracket
    char bracket = line[index];
    bool is_complete = false;

    // if debug, print the line starting at the index and the index
    if (DEBUG) {
        // print substring of line starting at index
        cout << "Parsing line: " << line.substr(index) << endl;
        cout << "Parsing index: " << index << endl;
    }

    // Create a chunk of the type of the first bracket
    Chunk * chunk = new Chunk(convert_bracket_type(bracket));

    // Recursively parse the chunk starting at the index of the first bracket
    int index_end = index + 1;
    while (index_end < line.length()) {
        // Get the next bracket
        char next_bracket = line[index_end];

        // If the next bracket is a closing bracket, then break
        if (closing_brackets.find(next_bracket) != closing_brackets.end()) {
            // print next_bracket, next_braket type, and chunk->btype
            if (DEBUG) {
                cout << "Next bracket: " << next_bracket << endl;
                cout << "Next bracket type: " << convert_bracket_type(next_bracket) << endl;
                cout << "Chunk bracket type: " << chunk->btype << endl;
            }
            // If closing bracket is the same as the opening bracket, then break
            if ( convert_bracket_type(next_bracket) == chunk->btype ) {
                is_complete = true;
                break;
            }

            // If closing bracket is not the same as the opening bracket, then print error and exit
            else {
                cout << "Invalid bracket of " << next_bracket << endl;
                cout << "Expected bracket of " << bracket << endl;

                // Update score
                total_score += lookup_bracket_score(convert_bracket_type(next_bracket));

                // Delete chunk
                delete chunk;

                return PCHUNK{NULL, index, index_end};
            }
        }

        // Parse the chunk
        PCHUNK pchunk = parse_chunk(line, index_end);

        // If pchunk is NULL, then return NULL
        if (pchunk.chunk == NULL) {
            // Delete chunk
            delete chunk;

            return PCHUNK{NULL, index, index_end};
        }

        // Add the chunk to the chunk
        chunk->add_chunk(pchunk.chunk);


        if (pchunk.incomplete) {
            return PCHUNK{chunk, index, index_end, true};
        }

        // Update the index_end
        index_end = pchunk.index_end + 1;
    }

    // If the chunk is not complete, then print error and exit
    if (!is_complete) {
        cout << "Incomplete chunk discovered" << endl;
        return PCHUNK{chunk, index, index_end, true};
    }

    // Return the chunk with index and index_end
    return {chunk, index, index_end, false};
}




int main(int argc, char *argv[]) {

    // Check if the user has provided a file name
    // Help message uses argv[0]
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <file_name>" << endl;
        return 1;
    }

    vector<string> *lines = read_file(argv[1]);
    vector<string> *differences = new vector<string>();

    if (lines == nullptr) {
        return -1;
    }

    vector<Chunk *> * chunks = new vector<Chunk *>();

    // Iterate through the lines
    for (string line : *lines) {
        int index=0;
        // clear chunks
        chunks->clear();

        while ( index < line.length() ) {
            // Parse the chunk
            PCHUNK pchunk = parse_chunk(line, index);

            // If pchunk is NULL, then break
            if (pchunk.chunk == NULL) {
                break;
            }

            // print Chunk
            if (DEBUG) {
                // print chunks
                for (Chunk * chunk : *chunks) {
                    cout << chunk->to_string() << endl;
                }
                cout << "Chunk: " << pchunk.chunk->to_string() << endl;
            }

            if (pchunk.incomplete) {
                cout << "Found incomplete chunk: " << line << endl;
                // Generate expected chunk by appending all chunks->to_string() including pchunk.chunk
                string expected_chunk = "";
                for (Chunk * chunk : *chunks) {
                    expected_chunk += chunk->to_string();
                }
                expected_chunk += pchunk.chunk->to_string();
                // print expected
                cout << "Expected: " << expected_chunk << endl;
                // Print the difference between the expected and the actual. expected will always be longer
                string difference = expected_chunk.substr(line.length());
                cout << "Difference: " << difference << endl;
                // Add the difference to the differences vector
                differences->push_back(difference);
                break;
            }

            // Add the chunk to the chunk
            chunks->push_back(pchunk.chunk);

            // Update the index
            index = pchunk.index_end + 1;
        }
    }

    // Create vector of the difference scores
    vector<int> *difference_scores = new vector<int>();

    // Iterate through the differences
    for (string difference : *differences) {
        // Add the difference score to the difference_scores vector
        difference_scores->push_back(calculate_difference_score(difference));
    }

    // Print the difference scores
    for (int difference_score : *difference_scores) {
        cout << difference_score << endl;
    }

    // Sort the difference scores
    sort(difference_scores->begin(), difference_scores->end());

    // Print the middle difference score
    cout << "Middle difference score: " << difference_scores->at(difference_scores->size()/2) << endl;

    // 3654963618 is correct
    // Ended up using libgmp to avoid the overflow, then printing it out and sorting with nix commands
}
