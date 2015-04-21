#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits>                    //Biggest int: std::numeric_limits<int>::max()
#include "ics46goody.hpp"
#include "array_queue.hpp"
#include "array_priority_queue.hpp"
#include "array_set.hpp"
#include "array_map.hpp"

typedef std::string                                             Candidate;
typedef ics::ArraySet           <Candidate>                     CandidateSet;
typedef ics::ArrayQueue         <Candidate>                     CandidateQueue;
typedef ics::ArrayMap           <Candidate, int>                CandidateTally;
typedef ics::pair               <Candidate, int>                CandidateTallyEntry;


typedef std::string                                             Voter;
typedef ics::ArrayMap           <Voter, CandidateQueue>         Preferences;
typedef ics::pair               <Voter, CandidateQueue>         PreferencesEntry;
typedef ics::ArrayPriorityQueue <PreferencesEntry>              PreferencesEntryPQ;

typedef ics::pair               <Candidate, int>                TallyEntry;
typedef ics::ArrayPriorityQueue <TallyEntry>                    TallyEntryPQ;


Preferences read_voter_preferences (std::ifstream& file) {
    //Read an open file stating voter preferences (each line is (a) a voter
    //  followed by (b) all the candidates, in preference order (from most to
    //  least preferred), separated by a semicolons), and return a Map of the
    //  preferences as a Map whose keys are voter names and whose values are
    //  a queue of candidate preferences.
    Preferences preferences;
    std::string line;

    while(getline(file, line)) {
        std::vector<std::string> line_as_vector = ics::split(line, ";");
        std::string voter = line_as_vector.front();
        line_as_vector.erase(line_as_vector.begin());

        CandidateQueue cq;
        for (std::string candidate_pref : line_as_vector)
            cq.enqueue(candidate_pref);
        preferences[voter] = cq;
    }
    return preferences;
}

bool pref_entry_alphabetically (const PreferencesEntry& a, const PreferencesEntry& b) {
    return a.first < b.first;
}
void print_voter_preferences (const Preferences& preferences) {
    //Print a label and all the entries in the preferences Map, in alphabetical
    //  order according to the voter.
    //Use a "->" to separate the voter name from the Queue of candidate names.
    PreferencesEntryPQ sorted_preferences(pref_entry_alphabetically);
    sorted_preferences.enqueue(preferences.ibegin(), preferences.iend());

    std::cout << "\nVoter Preferences" << std::endl;
    for (PreferencesEntry kv : sorted_preferences)
        std::cout << "  " << kv.first << " -> " << kv.second << std::endl;
}

bool tally_entry_alphabetically (const TallyEntry& a, const TallyEntry& b) {
    return a.first < b.first;
}
bool tally_entry_numerically (const TallyEntry& a, const TallyEntry& b) {
    return a.second > b.second;
}
void print_tally (std::string message, const CandidateTally& tally, bool (* has_higher_priority) (const TallyEntry& i, const TallyEntry& j)) {
    //Print the message followed by all the entries in the CandidateTally, in
    //   the order specified by *has_higher_priority: i is printed before j, if
    //   has_higher_priority(i,j) returns true.
    //Use a "->" to separate the candidate name from the number of votes they
    //  received.
    TallyEntryPQ sorted_tally(has_higher_priority);
    sorted_tally.enqueue(tally.ibegin(), tally.iend());

    std::cout << message << std::endl;
    for (TallyEntry kv : sorted_tally)
        std::cout << "  " << kv.first << " -> " << kv.second << std::endl;
}

CandidateTally evaluate_ballot (const Preferences& preferences, const CandidateSet& candidates) {
    //Return the CandidateTally: a Map of candidates (as keys) and the number of
    //  votes they received, based on the unchanging Preferences (read from the
    //  file) and the candidates who are currently still in the election.
    //Every possible candidate should appear as a key in the resulting tally.
    //Each voter should tally one vote: for their highest-ranked candidate who is
    //  still in the the election.
    CandidateTally ct;
    for (PreferencesEntry prefs : preferences) {
        for (Candidate cand : prefs.second) {
            if (candidates.contains(cand)) {    //if the candidate is still in the race ..
                ++ct[cand];                     //.. then add one vote for said candidate
                break;                          //.. and break out of the loop
            }
        }
    }
    return ct;
}

CandidateSet remaining_candidates (const CandidateTally& tally) {
    //Return the Set of candidates who are still in the election, based on the
    //  tally of votes: compute the minimum number of votes and return a Set of
    //  all candidates receiving more than that minimum; if all candidates
    //  receive the same number of votes, the empty Set is returned.

    //Computing the minimum number of votes
    int min_votes = std::numeric_limits<int>::max();
    for (CandidateTallyEntry ct : tally)
        if (ct.second < min_votes)
            min_votes = ct.second;

    //Adding all candidates that received more votes than the minimum
    CandidateSet rem_cands;
    for (CandidateTallyEntry ct : tally)
        if (ct.second > min_votes)
            rem_cands.insert(ct.first);
    return rem_cands;
}

int main () {
    //Prompt the user for a file, create a voter preference Map, and print it.
    //Determine the Set of all the candidates in the election, from this Map.
    //Repeatedly evaluate the ballot based on the candidates (still) in the
    //  election, printing the vote count (tally) two ways: with the candidates
    //  (a) shown alphabetically increasing and (b) shown with the vote count
    //  decreasing (candidates with equal vote counts are shown alphabetically
    //  increasing); from this tally compute which candidates remains in the
    //  election: all candidates receiving more than the minimum number of votes;
    //  continue this process until there are less than 2 candidates.
    //Print the final result: there may 1 candidate left, the winner, or 0, no
    //  winner.

    try {
        std::ifstream file;
        ics::safe_open(file, "Enter file name", "votepref1.txt");
        Preferences prefs = read_voter_preferences(file);
        print_voter_preferences(prefs);

        //Create initial set of candidates.
        //I suppose I could just loop through a single PreferencesEntry's CandidateQueue, but what about write-in ballots, etc?
        CandidateSet rem_cands;
        for (PreferencesEntry kv : prefs)
            for (Candidate v : kv.second)
                rem_cands.insert(v);

        int ballot_num = 1;
        while (rem_cands.size() > 1) {
            CandidateTally tally = evaluate_ballot(prefs, rem_cands);

            std::stringstream ss_alpha, ss_numeric;
            ss_alpha << "\nVote count on ballot #" << ballot_num << " with candidates alphabetically: still in election = " << rem_cands;
            ss_numeric << "\nVote count on ballot #" << ballot_num << " with candidates numerically: still in election = " << rem_cands;
            print_tally(ss_alpha.str(), tally, tally_entry_alphabetically);
            print_tally(ss_numeric.str(), tally, tally_entry_numerically);

            rem_cands = remaining_candidates(tally);
            ++ballot_num;
        }

        if (rem_cands.size() == 1)
            std::cout << "\nWinner is " << *rem_cands.ibegin() << std::endl;
        else
            std::cout << "\nNo winner: election is a tie among candidates remaining on the final ballot" << std::endl;
    }

    catch (ics::IcsError& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
