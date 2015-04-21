#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "ics46goody.hpp"
#include "array_queue.hpp"
#include "array_priority_queue.hpp"
#include "array_set.hpp"
#include "array_map.hpp"

typedef ics::ArrayQueue         <std::string>                   InputsQueue;
typedef ics::ArrayMap           <std::string, std::string>      InputStateMap;

typedef ics::ArrayMap           <std::string, InputStateMap>    FA;
typedef ics::pair               <std::string, InputStateMap>    FAEntry;
typedef ics::ArrayPriorityQueue <FAEntry>                       FAPQ;

typedef ics::pair               <std::string, std::string>      Transition;
typedef ics::ArrayQueue         <Transition>                    TransitionQueue;


const FA read_fa (std::ifstream& file) {
    //Read an open file describing the finite automaton (each line starts with
    //  a state name followed by pairs of transitions from that state: (input
    //  followed by new state) all separated by semicolons), and return a Map
    //  whose keys are states and whose associated values are another Map with
    //  each input in that state (keys) and the resulting state it leads to.
    FA fa;
    InputStateMap ism;
    std::string line;

    while (getline(file, line)) {
        std::vector<std::string> line_as_vector = ics::split(line, ";");
        std::string main_state = line_as_vector.front();

        for (int i=0; i<line_as_vector.size(); ++i) {
            if (i%2 == 0 && i != 0) {
                std::string input = line_as_vector[i-1];
                std::string state = line_as_vector[i];
                ism[input] = state;
            }
        }
        fa[main_state] = ism;
    }
    return fa;
}

bool fa_entry_alphabetically (const FAEntry& a, const FAEntry& b) {
    return a.first < b.first;
}
void print_fa (const FA& fa) {
    //Print a label and all the entries in the finite automaton Map, in
    //  alphabetical order of the states: each line has a state, the text
    //  "transition:" and the Map of its transitions.
    FAPQ sorted_fa(fa_entry_alphabetically);
    sorted_fa.enqueue(fa.ibegin(), fa.iend());

    std::cout << "\nFinite Automaton Description" << std::endl;
    for (FAEntry kv : sorted_fa)
        std::cout << "  " << kv.first << " transitions: " << kv.second << std::endl;
}

TransitionQueue process (const FA& fa, std::string state, const InputsQueue& inputs) {
    //Return a queue of the calculated transition pairs, based on the finite
    //  automaton, initial state, and queue of inputs; each pair in the returned
    //  queue is of the form: input, new state.
    //The first pair contains "" as the input and the initial state.
    //If any input i is illegal (does not lead to a state in the finite
    //  automaton, then the last pair in the returned queue is i,"None".
}

void interpret (TransitionQueue& tq) {  //or TransitionQueue or TransitionQueue&&
    //Print a TransitionQueue (the result of calling process) in a nice form.
    //Print the Start state on the first line; then print each input and the
    //  resulting new state (or "illegal input: terminated", if the state is
    //  "None") indented on subsequent lines; on the last line, print the Stop
    //  state (which might be "None").
}

int main () {
    //Prompt the user for a file, create a finite automaton Map, and print it.
    //Prompt the user for a file containing any number of simulation descriptions
    //  for the finite automaton to process, one description per line; each
    //  description contains a start state followed by inputs, all separated by
    //  semicolons.
    //Repeatedly read a description, print that description, put each input in a
    //  Queue, process the Queue and print the results in a nice form.

    try {
        std::ifstream file;
        ics::safe_open(file, "Enter file name", "faparity.txt");
        FA fa = read_fa(file);
        print_fa(fa);






    }

    catch (ics::IcsError& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
