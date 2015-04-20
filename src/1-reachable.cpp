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

typedef std::string                                 Node;
typedef ics::ArraySet           <Node>              NodeSet;
typedef ics::pair               <Node, NodeSet>     GraphEntry;
typedef ics::ArrayQueue         <Node>              GraphQ;
typedef ics::ArrayPriorityQueue <GraphEntry>        GraphPQ;
typedef ics::ArrayMap           <Node, NodeSet>     Graph;

Graph read_graph (std::ifstream& file) {
    //Read an open file of edges (node names separated by semicolons, with an
    //  edge going from the first node name to the second node name) and return a
    //  Graph (Map) of each node name associated with the Set of all node names to
    //  which there is an edge from the key node name.
    Graph graph;
    std::string line;

    while (getline(file, line)) {
        std::vector<std::string> nodes = ics::split(line, ";");
        graph[nodes.front()].insert(nodes.back());
    }
    file.close();
    return graph;
}

bool entry_in_alphabetical_order(const GraphEntry& a, const GraphEntry& b) {
    return a.first < b.first;
}

void print_graph (const Graph& graph) {
    //Print a label and all the entries in the Graph, in alphabetical order.
    //Use a "->" to separate the key node name from the Set of node names to
    //  which it has an edge.
    GraphPQ sorted_graph(entry_in_alphabetical_order);
    sorted_graph.enqueue(graph.ibegin(), graph.iend());

    std::cout<<"\n Graph: source -> {destination} edges"<<std::endl;
    for (GraphEntry kv : sorted_graph)
        std::cout << "  " << kv.first << " -> " << kv.second << std::endl;
}

NodeSet reachable (const Graph& graph, std::string start) {
    //Return the Set of node names reaching in the Graph starting at the
    //  specified (start) node.
    //Use a local Set and a Queue to respectively store the reachable nodes and
    //  the nodes that are being explored.

    //Algorithm to compute all the reachable nodes in a graph:
    //  1. Create local variables:
    //    - a Set (initially empty) of reached nodes
    //    - a Queue (initially containing the parameter start node) of nodes to explore
    //  2. While the exploring queue still has nodes:
    //    - remove the first node and put it into the reached set
    //    - if this node is a key in the graph:
    //      - then for all its destination nodes not already in the reached set, put them in the exploring queue
    //  3. When the exploring queue becomes empty, return the reached set.
    NodeSet reached_nodes_set;
    GraphQ nodes_being_explored_queue = {start};

    while (!nodes_being_explored_queue.empty()) {
        Node current_node = nodes_being_explored_queue.dequeue();
        reached_nodes_set.insert(current_node);

        if (graph.has_key(current_node))
            for (Node node : graph[current_node])
                if (!reached_nodes_set.contains(node))
                    nodes_being_explored_queue.enqueue(node);
    }
    return reached_nodes_set;
}

int main () {
    //Prompt the user for a file, create a graph from its edges, print the graph,
    //  and then repeatedly (until the user enters "quit") prompt the user for a
    //  starting node name and then either print an error (if that the node name
    //  is not a source node in the graph) or print the Set of node names
    //  reachable from it using the edges in the Graph.

    try {
        std::ifstream graph_data_file;
        ics::safe_open(graph_data_file, "Enter file name", "graph1.txt");
        Graph graph = read_graph(graph_data_file);
        print_graph(graph);

        std::string user_input;
        while (user_input != "quit") {
            user_input = ics::prompt_string("\nEnter starting node (enter 'quit' to quit)");
            if (graph.has_key(user_input))
                std::cout << "Reachable from node name " << user_input << " = " << reachable(graph, user_input) << std::endl;
            else if (user_input != "quit")
                std::cout << "  " << user_input << " is not a source node name in the graph" << std::endl;
        }
    }

    catch (ics::IcsError& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
