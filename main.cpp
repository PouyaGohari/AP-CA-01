#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

const char pipe = '|';
const char colon = ':';
const char space = ' ';
const string YES = "YES";
const string NO = "NO";
const string IF = "IF_NECESSARY";
struct Input_shape{
    int time_slot_counts;
    vector<string> participants;
    vector<vector<string>> votes;
};

struct time_slot{
    int id;
    int yes_numbers;
    int no_numbers;
    int if_numbers;
    vector<string> yes_participants;
    vector<string> no_participants;
    vector<string> if_participants;
};

vector<string> reading_from_file(const string file_name){
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

vector<string> break_line_to_vector(const string line){
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

vector<time_slot> initialize_time_slots(const int time_slot_counts){
    vector<time_slot> whole_time_slots(time_slot_counts);
    int i = 0;
    for(auto &slot : whole_time_slots){
        slot.id = i;
        slot.if_numbers = time_slot_counts;
        slot.no_numbers = 0;
        slot.yes_numbers = 0;
        i += 1;
    }
    return whole_time_slots;
}

void assign_if_participants(vector<time_slot> &slots, const vector<string> participants){
    for(auto &slot : slots){
        slot.if_participants = participants;
    }
}

void remove_string_from_vector(vector<string> &vec, const string str_to_remove){
    vec.erase(remove(vec.begin(), vec.end(), str_to_remove), vec.end());
}

void modify_answers(time_slot &slot, const string answer, const string name){
    if(answer == NO){
        slot.no_numbers += 1;
        slot.no_participants.push_back(name);
        remove_string_from_vector(slot.if_participants, name);
        slot.if_numbers -= 1;
    }
    else if(answer == YES){
        slot.yes_numbers += 1;
        slot.yes_participants.push_back(name);
        remove_string_from_vector(slot.if_participants, name);
        slot.if_numbers -= 1;
    }
}

void counting_each_slot_answers(vector<time_slot> &slots, const Input_shape inputs){
    for(auto vote : inputs.votes){
        for(int i = 1; i < vote.size() - 1 ; i = i + 2){
            modify_answers(slots[stoi(vote[i])], vote[i + 1], vote[0]);
        }
    }
}

vector<time_slot> filling_slots_with_inputs(const Input_shape inputs){
    vector<time_slot> slots = initialize_time_slots(inputs.time_slot_counts);
    assign_if_participants(slots, inputs.participants);
    counting_each_slot_answers(slots, inputs);
    return slots;
} 

int main(int argc, char *argv[]){
    Input_shape inputs = filing_input_shape(reading_from_file(argv[1]));
    vector<time_slot> slots = filling_slots_with_inputs(inputs);
    return 0;
}