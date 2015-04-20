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


typedef ics::ArrayQueue<std::string>              CandidateQueue;
typedef ics::ArraySet<std::string>                CandidateSet;
typedef ics::ArrayMap<std::string,int>            CandidateTally;

typedef ics::ArrayMap<std::string,CandidateQueue> Preferences;
typedef ics::pair<std::string,CandidateQueue>     PreferencesEntry;

typedef ics::pair<std::string,int>                TallyEntry;
typedef ics::ArrayPriorityQueue<TallyEntry>       TallyEntryPQ;


//Read an open file stating voter preferences (each line is (a) a voter
//  followed by (b) all the candidates, in preference order (from most to
//  least preferred), separated by a semicolons), and return a Map of the
//  preferences as a Map whose keys are voter names and whose values are
//  a queue of candidate preferences.
Preferences read_voter_preferences(std::ifstream &file) {
}


//Print a label and all the entries in the preferences Map, in alphabetical
//  order according to the voter.
//Use a "->" to separate the voter name from the Queue of candidate names.
void print_voter_preferences(const Preferences& preferences) {
}


//Print the message followed by all the entries in the CandidateTally, in
//   the order specified by *has_higher_priority: i is printed before j, if
//   has_higher_priority(i,j) returns true.
//Use a "->" to separate the candidat name from the number of votes they
//  received.
void print_tally(std::string message, const CandidateTally& tally, bool (*has_higher_priority)(const TallyEntry& i,const TallyEntry& j)) {
}


//Return the CandidateTally: a Map of candidates (as keys) and the number of
//  votes they received, based on the unchanging Preferences (read from the
//  file) and the candidates who are currently still in the election.
//Every possible candidate should appear as a key in the resulting tally.
//Each voter should tally one vote: for their highest-ranked candidate who is
//  still in the the election.
CandidateTally evaluate_ballot(const Preferences& preferences, const CandidateSet& candidates) {
}


//Return the Set of candidates who are still in the election, based on the
//  tally of votes: compute the minimum number of votes and return a Set of
//  all candidates receiving more than that minimum; if all candidates
//  receive the same number of votes, the empty Set is returned.
CandidateSet remaining_candidates(const CandidateTally& tally) {
}


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
int main() {
  try {
  } catch (ics::IcsError& e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
}