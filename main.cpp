#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

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

int main(int argc, char *argv[]){
    return 0;
}