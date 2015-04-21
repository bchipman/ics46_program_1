#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits>                           //I used std::numeric_limits<int>::max()
#include "ics46goody.hpp"
#include "array_queue.hpp"
#include "array_priority_queue.hpp"
#include "array_set.hpp"
#include "array_map.hpp"

typedef ics::ArrayQueue<std::string>         WordQueue;
typedef ics::ArraySet<std::string>           FollowSet;
typedef ics::pair<WordQueue,FollowSet>       CorpusEntry;
typedef ics::ArrayPriorityQueue<CorpusEntry> CorpusPQ;
typedef ics::ArrayMap<WordQueue,FollowSet>   Corpus;



Corpus read_corpus (int os, std::ifstream& file) {
    //Read an open file of lines of words (separated by spaces) and return a
    //  Corpus (Map) of each sequence (Queue) of os (Order-Statistic) words
    //  associated with the Set of all words that follow them somewhere in the
    //  file.
    //You may assume the first line contains at least Order-Statstic words.
    std::string line;
    std::vector<std::string> words_vector;
    while (getline(file, line))
        for (std::string w : ics::split(line, " "))  //ics::split(line, " ")   gives   std::vector<std::string>
            words_vector.push_back(w);

    Corpus corpus;
    WordQueue wq;
    for (std::string word : words_vector) {
        if (wq.size() < os)
            wq.enqueue(word);
        else {
            corpus[wq].insert(word);
            wq.dequeue();
            wq.enqueue(word);
        }
    }
    return corpus;
}

bool queue_gt (const CorpusEntry& a, const CorpusEntry& b) {
    //One queue is lexically greater than another, if its first value is smaller; or if
    //  its first value is the same and its second value is smaller; or if its first
    //  and second values are the same and its third value is smaller...
    //If any of its values is less than the corresponding value in the other queue,
    //  the first queue is not greater.
    //Note that the queues sizes are the same: each stores Order-Statistic words
    //Important: Use iterators for examining the queue values: DO NOT CALL DEQUEUE.
    ics::Iterator<std::string>& it_a = a.first.ibegin();
    ics::Iterator<std::string>& it_b = b.first.ibegin();

    while (it_a != a.first.iend()) {
        if (*it_a != *it_b)
            return (*it_a < *it_b);
        ++it_a;
        ++it_b;
    }
    return false;
}
void print_corpus (const Corpus& corpus) {
    //Print "Corpus" and all entries in the Corpus, in lexical alphabetical order
    //  (with the minimum and maximum set sizes at the end).
    //Use a "can be followed by any of" to separate the key word from the Set of words
    //  that can follow it.
    CorpusPQ sorted_corpus(queue_gt);
    sorted_corpus.enqueue(corpus.ibegin(), corpus.iend());

    int max = 0;
    int min = std::numeric_limits<int>::max();

    std::cout << "\nCorpus" << std::endl;
    for (CorpusEntry kv : sorted_corpus) {
        max = (kv.second.size() > max) ? kv.second.size() : max;
        min = (kv.second.size() < min) ? kv.second.size() : min;
        std::cout << "  " << kv.first << " -> " << kv.second << std::endl;
    }
    std::cout << "min/max = " << min << "/" << max << std::endl;
}

std::string random_in_set (const FollowSet& words) {
    //Return a random word in the words set (use in produce_text)
    int index = ics::rand_range(1, words.size());
    int i = 0;
    for (std::string s : words)
        if (++i == index)
            return s;
    return "?";
}
WordQueue produce_text (const Corpus& corpus, const WordQueue& start, int count) {
    //Return a Queue of words, starting with those in start and including count more
    //  randomly selected words using corpus to decide which word comes next.
    //If there is no word that follows the previous ones, put "None" into the queue
    //  and return immediately this list (whose size is <= start.size() + count).
    WordQueue wq_key = start;
    WordQueue wq_gen = start;

    for (int i = 0; i < count; ++i) {
        FollowSet choices = corpus[wq_key];
        if (choices.empty()) {
            wq_gen.enqueue("None");
            return wq_gen;
        }
        std::string chosen = random_in_set(choices);
        wq_gen.enqueue(chosen);
        wq_key.dequeue();
        wq_key.enqueue(chosen);
    }
    return wq_gen;    
}

int main () {
    //Prompt the user for (a) the order statistic and (b) the file storing the text.
    //Read the text as a Corpus and print it appropriately.
    //Prompt the user for order statistic words from the text.
    //Prompt the user for number of random words to generate
    //Call the functions above to solve the problem, and print the appropriate information

    try {
        int order_stat = ics::prompt_int("Enter order statistic", 2);
        std::ifstream wg_text_file;
        ics::safe_open(wg_text_file, "Enter file name to process", "wginput1.txt");
        Corpus corpus = read_corpus(order_stat, wg_text_file);
        print_corpus(corpus);

        std::cout << "\nEnter " << order_stat << " words to start with" << std::endl;
        WordQueue start_words;
        for (int i = 0; i < order_stat; ++i) {
            std::stringstream ss;
            ss << "Enter word " << (i+1);
            std::string word_input = ics::prompt_string(ss.str());
            start_words.enqueue(word_input);
        }
        int word_count = ics::prompt_int("Enter # of words to generate");
        WordQueue random_text = produce_text(corpus, start_words, word_count);
        std::cout << "Random text = " << random_text << std::endl;
    }

    catch (ics::IcsError& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
