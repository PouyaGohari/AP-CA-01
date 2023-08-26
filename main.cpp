#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

const char pipe = '|';
const char colon = ':';
const char space = ' ';
struct Input_shape{
    int time_slot_counts;
    vector<string> participants;
    vector<vector<string>> votes;
};

vector<string> reading_from_file(string file_name){
    string line;
    vector<string> result;
    ifstream input_file(file_name);
    while(getline(input_file, line)){
        result.push_back(line);
    }
    return result;
}

void replace_symbols_with_space(string &line){
    for(auto &character : line){
        if(character == pipe || character == colon){
            character = space;
        }
    }
}

vector<string> break_line_to_vector(string line){
    istringstream iss(line);
    vector<string> names;
    string name;
    while (iss >> name){
        names.push_back(name);
    }
    return names;
}

vector<vector<string>> filling_votes(vector<string> lines){
    vector<vector<string>> votes;
    for(auto line : lines){
        replace_symbols_with_space(line);
        votes.push_back(break_line_to_vector(line));
    }
    return votes;
}

Input_shape filing_input_shape(vector<string> lines){
    vector<string> new_lines(lines.begin() + 2, lines.end());
    Input_shape reordered;
    reordered.time_slot_counts = stoi(lines[0]);
    reordered.participants = break_line_to_vector(lines[1]);
    reordered.votes = filling_votes(new_lines);
    return reordered;
}

int main(int argc, char *argv[]){
    Input_shape inputs = filing_input_shape(reading_from_file(argv[1]));
    return 0;
}