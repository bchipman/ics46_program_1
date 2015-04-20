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

typedef ics::ArraySet           <std::string>               NodeSet;
typedef ics::pair               <std::string, NodeSet>      GraphEntry;
typedef ics::ArrayPriorityQueue <GraphEntry>                GraphPQ;
typedef ics::ArrayMap           <std::string, NodeSet>      Graph;

//Read an open file of edges (node names separated by semicolons, with an
//  edge going from the first node name to the second node name) and return a
//  Graph (Map) of each node name associated with the Set of all node names to
//  which there is an edge from the key node name.
Graph read_graph (std::ifstream& file) {

    Graph graph;
    std::string line;

    while (getline(file, line)) {
        std::vector<std::string> nodes = ics::split(line, ";");
        graph[nodes.front()].insert(nodes.back());
    }
    file.close();
    return graph;
}

//Print a label and all the entries in the Graph, in alphabetical order.
//Use a "->" to separate the key node name from the Set of node names to
//  which it has an edge.
void print_graph (const Graph& graph) {
}

//Return the Set of node names reaching in the Graph starting at the
//  specified (start) node.
//Use a local Set and a Queue to respectively store the reachable nodes and
//  the nodes that are being explored.
NodeSet reachable (const Graph& graph, std::string start) {
}

//Prompt the user for a file, create a graph from its edges, print the graph,
//  and then repeatedly (until the user enters "quit") prompt the user for a
//  starting node name and then either print an error (if that the node name
//  is not a source node in the graph) or print the Set of node names
//  reachable from it using the edges in the Graph.
int main () {
    try {
        std::ifstream graph_data_file;
        ics::safe_open(graph_data_file, "Enter file name", "graph1.txt");
        read_graph(graph_data_file);


    }
    catch (ics::IcsError& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
