#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

const char PIPE = '|';
const char COLON = ':';
const char SPACE = ' ';
const string YES = "YES";
const string NO = "NO";
const string IF_NECESSARY = "IF_NECESSARY";
const string TIME_SLOT = "Time slot";
const string SYMBOL_BETWEEN_SLOT = "###";

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
    input_file.close();
    return result;
}

void replace_symbols_with_space(string &line){
    for(auto &character : line){
        if(character == PIPE || character == COLON){
            character = SPACE;
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
        slot.no_numbers = 0;
        slot.yes_numbers = 0;
        i += 1;
    }
    return whole_time_slots;
}

void assign_if_participants(vector<time_slot> &slots, const vector<string> participants){
    for(auto &slot : slots){
        slot.if_participants = participants;
        slot.if_numbers = participants.size();
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

bool compare_no_answers(time_slot slot1, time_slot slot2){
    return slot1.no_numbers < slot2.no_numbers;
}

void sort_with_no_answers(vector<time_slot> &slots){
    sort(slots.begin(), slots.end(), compare_no_answers);
}

void prioritized_time_slots(vector<time_slot> &slots){
    sort_with_no_answers(slots);
    for(int i = 0; i < slots.size() - 1; i++){
        if(slots[i].no_numbers == slots[i + 1].no_numbers){
            if(slots[i].if_numbers > slots[i + 1].if_numbers){
                swap(slots[i], slots[i + 1]);
            }
            else if(slots[i].id > slots[i + 1].id){
                swap(slots[i], slots[i + 1]);
            }
        }
    }
}

bool comprator_vec(const string& a, const string& b,const vector<string> originial_vector){
    auto it1 = find(originial_vector.begin(), originial_vector.end(), a);
    auto it2 = find(originial_vector.begin(), originial_vector.end(), b);
    return it1 < it2;
}

void sorting_vector_based_another(vector<string>& sortingvec, const vector<string> original){
    sort(sortingvec.begin(), sortingvec.end()
    , [&](const string& a, const string& b){return comprator_vec(a, b, original);});
}

void print_vector(vector<string> &vec, vector<string> participants){
    sorting_vector_based_another(vec, participants);
    for(auto str : vec){
        cout << SPACE << str;
    }
    cout << SPACE;
    cout << endl;
}

void print_slots(vector<time_slot> slots, const vector<string> participants){
    for(auto slot : slots){
        cout << TIME_SLOT << SPACE << slot.id << COLON << endl;
        cout << YES << COLON;
        print_vector(slot.yes_participants, participants);
        cout << NO << COLON ;
        print_vector(slot.no_participants, participants);
        cout << IF_NECESSARY << COLON;
        print_vector(slot.if_participants, participants);
        cout << SYMBOL_BETWEEN_SLOT << endl;
    }
}

void print_output(const vector<time_slot> slots, const vector<string> participants){
    if(slots.size() <= 3){
        print_slots(slots, participants);
    }
    else{
        vector<time_slot> sub_slots(slots.begin(), slots.begin()+3);
        print_slots(sub_slots, participants);
    }
}

void writing_in_file(vector<time_slot> &slots, const vector<string> participants, const string file_name){
    ofstream output_file(file_name);
    streambuf* original_cout_buffer = cout.rdbuf();
    cout.rdbuf(output_file.rdbuf());
    prioritized_time_slots(slots);
    print_output(slots, participants);
    cout.rdbuf(original_cout_buffer);
    output_file.close();
}

int main(int argc, char *argv[]){
    Input_shape inputs = filing_input_shape(reading_from_file(argv[1]));
    vector<time_slot> slots = filling_slots_with_inputs(inputs);
    writing_in_file(slots, inputs.participants, argv[2]);
    return 0;
}