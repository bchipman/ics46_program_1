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

typedef std::string                                         Input;
typedef std::string                                         State;
typedef ics::ArraySet           <State>                     States;
typedef ics::ArrayQueue         <Input>                     InputsQueue;
typedef ics::ArrayMap           <Input, States>             InputStatesMap;
typedef ics::ArrayMap           <State, InputStatesMap>     NDFA;
typedef ics::pair               <State, InputStatesMap>     NDFAEntry;
typedef ics::ArrayPriorityQueue <NDFAEntry>                 NDFAPQ;
typedef ics::pair               <Input, States>             Transitions;
typedef ics::ArrayQueue         <Transitions>               TransitionsQueue;


const NDFA read_ndfa (std::ifstream& file) {
    //Read an open file describing the non-deterministic finite automaton (each
    //  line starts with a state name followed by pairs of transitions from that
    //  state: (input followed by a new state) all separated by semicolons), and
    //  return a Map whose keys are states and whose associated values are another
    //  Map with each input in that state (keys) and the resulting set of states it
    //  can lead to.
    NDFA ndfa;
    std::string line;

    while (getline(file, line)) {
        InputStatesMap ism;  //must be inside loop
        std::vector<std::string> line_as_vector = ics::split(line, ";");
        State main_state = line_as_vector.front();

        for (int i=0; i<line_as_vector.size(); ++i) {
            if (i%2 == 0 && i != 0) {
                Input input = line_as_vector[i-1];
                State state = line_as_vector[i];
                ism[input].insert(state);
            }
        }
        ndfa[main_state] = ism;
    }
    return ndfa;

}
bool ndfa_entry_alphabetically (const NDFAEntry& a, const NDFAEntry& b) {
    return a.first < b.first;
}
void print_ndfa (const NDFA& ndfa) {
    //Print a label and all the entries in the finite automaton Map, in
    //  alphabetical order of the states: each line has a state, the text
    //  "transition:" and the Map of its transitions.
    NDFAPQ sorted_ndfa(ndfa_entry_alphabetically);
    sorted_ndfa.enqueue(ndfa.ibegin(), ndfa.iend());

    std::cout << "\nNon-Deterministic Finite Automaton Description" << std::endl;
    for (NDFAEntry kv : sorted_ndfa)
        std::cout << "  " << kv.first << " trasnsitions: " << kv.second <<std::endl;
}

TransitionsQueue process (const NDFA& ndfa, State state, const InputsQueue& inputs) {
    //Return a queue of the calculated transition pairs, based on the non-deterministic
    //  finite automaton, initial state, and queue of inputs; each pair in the returned
    //  queue is of the form: input, set of new states.
    //The first pair contains "" as the input and the initial state.
    //If any input i is illegal (does not lead to any state in the non-deterministic finite
    //  automaton, ignore it).

    //Create the set and add state parameter to it
    States current_states;
    current_states.insert(state);

    //Create the queue and add the initial pair with "" as input to it
    TransitionsQueue trans_queue;
    trans_queue.enqueue(Transitions("", current_states));

    for (Input input : inputs) {
        States temp_states;
        for (State curr_state : current_states) {
            InputStatesMap ism = ndfa[curr_state];  //must be inside loop

            if (ism.has_key(input)) {
                States resulting_states = ism[input];
                for (State resulting_state : resulting_states)
                    temp_states.insert(resulting_state);
            }
        }
        Transitions trans_pair(input, temp_states);
        trans_queue.enqueue(trans_pair);
        current_states = temp_states;
    }
    return trans_queue;
}

void interpret (TransitionsQueue& tq) {  //or TransitionsQueue or TransitionsQueue&&
    //Print a TransitionsQueue (the result of calling process) in a nice form.
    //Print the Start state on the first line; then print each input and the
    //  resulting new states indented on subsequent lines; on the last line, print
    //  the Stop state.
    States last_states;
    for (Transitions t : tq) {
        if (t.first == "")
            std::cout << "Start state = " << t.second << std::endl;
        else {
            std::cout << "  Input = " << t.first << "; new possible states = " << t.second << std::endl;
            last_states = t.second;
        }
    }
    std::cout << "Stop state(s) = " << last_states << std::endl;
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
        ics::safe_open(file, "Enter file name of Non-Deterministic Finite Automaton", "ndfaendin01.txt");
        //ics::safe_open(file, "Enter file name of Non-Deterministic Finite Automaton", "ndfatrain.txt");
        //ics::safe_open(file, "Enter file name of Non-Deterministic Finite Automaton", "ndfare.txt");
        NDFA ndfa = read_ndfa(file);
        print_ndfa(ndfa);

        std::ifstream inputs_file;
        ics::safe_open(inputs_file, "\nEnter file name of start-states and inputs", "ndfainputendin01.txt");
        //ics::safe_open(inputs_file, "\nEnter file name of start-states and inputs", "ndfainputtrain.txt");
        //ics::safe_open(inputs_file, "\nEnter file name of start-states and inputs", "ndfainputre.txt");

        std::string line;
        while (getline(inputs_file, line)) {
            std::vector<std::string> line_as_vector = ics::split(line, ";");
            State state = line_as_vector[0];
            InputsQueue iq;
            for (std::vector<std::string>::iterator it = line_as_vector.begin()+1; it != line_as_vector.end(); ++it)
                iq.enqueue(*it);
            TransitionsQueue tq = process(ndfa, state, iq);
            std::cout << "\nStarting new simulation with description: " << line << std::endl;
            interpret(tq);
        }
    }

    catch (ics::IcsError& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
