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
            return 57;
        case CURLY_BRACKET:
            return 1197;
        case ROUND_BRACKET:
            return 3;
        case TRIANGLE_BRACKET:
            return 25137;
        default:
            // Print error and exit
            cout << "Invalid bracket type" << endl;
            exit(1);
    }
}

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

        // Update the index_end
        index_end = pchunk.index_end + 1;
    }

    // Return the chunk with index and index_end
    return {chunk, index, index_end};
}


// Function: parse_line
// Description: Parses a line of code and returns a vector of Chunks
// Algorithm:
//      1. Create a vector of Chunks for tracking unfinished chunks
//      2. Iterate through the line by character
//          a. If the current character is an opening bracket, create a new chunk
//          b. If the current character is a closing bracket, check that closing bracket is the expected closing bracket type
//              i. If it is, 

// Parameters:
//      string line: The line of code to parse
// Return: 


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

    vector<Chunk *> * chunks = new vector<Chunk *>();

    // Iterate through the lines
    for (string line : *lines) {
        // Parse the line
        PCHUNK pchunk = parse_chunk(line, 0);

        // If pchunk is NULL, then ignore
        if (pchunk.chunk == NULL) {
            continue;
        }

        // Add the chunk to the chunks
        chunks->push_back(pchunk.chunk);
    }

    // Print total score
    cout << "Total score: " << total_score << endl;

    // 299793
}
