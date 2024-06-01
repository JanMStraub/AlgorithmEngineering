#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <climits>
#include <random>

using namespace std;

/**
 * @struct Node
 * @brief Structure to represent a node in the graph.
 * @param id The ID of the node.
 */
struct Node
{
    int id; // ID of the node

    /**
     * @brief Constructor for Node.
     * @param id The ID of the node.
     */
    Node(int id) : id(id) {}
};

/**
 * @struct Edge
 * @brief Structure to represent an edge in the graph.
 * @param source The source vertex of the edge.
 * @param destination The destination vertex of the edge.
 * @param weight The weight of this edge.
 */
struct Edge
{
    int source;      // Source vertex of the edge
    int destination; // Destination vertex of the edge
    int weight;      // Weight of this edge

    /**
     * @brief Constructor for Edge.
     * @param src The source vertex of the edge.
     * @param dest The destination vertex of the edge.
     * @param w The weight of the edge.
     */
    Edge(int src, int dest, int w) : source(src), destination(dest), weight(w) {}

    // Overload the '<' operator to compare edges based on their weight
    bool operator<(const Edge &other) const
    {
        return weight > other.weight;
    }
};

/**
 * @class Graph
 * @brief Class to represent a graph.
 * @param _numberOfVertices The number of vertices in the graph.
 * @param _adjacencyList The adjacency list to represent the graph.
 * @param _edgeList The list of edges in the graph.
 */
class Graph
{
private:
    int _numberOfVertices; // Number of vertices in the graph
    int _numberOfEdges;
    vector<Edge> _edgeList; // List of edges in the graph
    vector<Edge> _matching;

public:
    /**
     * @brief Constructor for Graph.
     *
     * This constructor initializes the graph with the specified number of vertices.
     * It resizes the adjacency list to hold the edges for each vertex and reserves space for the node list.
     * It also creates a node for each vertex and adds it to the node list.
     *
     * @param vertices The number of vertices in the graph.
     */
    Graph(int vertices, int edges) : _numberOfVertices(vertices), _numberOfEdges(edges)
    {
        // Reserve space for the edge list
        _edgeList.reserve(_numberOfEdges);
    }

    /**
     * @brief Function to add an edge to the graph.
     *
     * This function adds an edge from the source vertex to the destination vertex with the specified weight.
     * The edge is added to the adjacency list of the source vertex.
     * The vertices are decremented by 1 because they are 0-indexed in the adjacency list.
     *
     * @param source The source vertex of the edge.
     * @param destination The destination vertex of the edge.
     * @param weight The weight of the edge.
     */
    void addEdge(int source, int destination, int weight)
    {
        if (source < destination)
            _edgeList.push_back(Edge(source - 1, destination - 1, weight));
    }

    int computeMaximumWeightMatching(vector<Edge> &edgeList)
    {
        vector<bool> selectedMatchingEdges(_numberOfVertices, false);
        
        int totalWeight = 0;

        for (const Edge &edge : edgeList)
        {
            if (!selectedMatchingEdges[edge.source] && !selectedMatchingEdges[edge.destination])
            {
                _matching.push_back(edge);
                totalWeight += edge.weight;
                selectedMatchingEdges[edge.source] = true;
                selectedMatchingEdges[edge.destination] = true;
            }
        }

        return totalWeight;
    }

    void compareMatchings()
    {
        int highestScore = 0;
        vector<Edge> highestMatching;

        // Without sorting
        int currentWeight = computeMaximumWeightMatching(_edgeList);
        if (currentWeight > highestScore)
        {
            highestScore = currentWeight;
            highestMatching = _matching;
        }
        
        _matching.clear();

        // With max sorting
        sort(_edgeList.begin(), _edgeList.end());
        currentWeight = computeMaximumWeightMatching(_edgeList);
        if (currentWeight > highestScore)
        {
            highestScore = currentWeight;
            highestMatching = _matching;
        }

        // Random sort
        for (int i = 0; i < 250; ++i)
        {
            _matching.clear();
            random_device rd;
            mt19937 g(rd());
            shuffle(_edgeList.begin(), _edgeList.end(), g);
            currentWeight = computeMaximumWeightMatching(_edgeList);
            if (currentWeight > highestScore)
            {
                highestScore = currentWeight;
                highestMatching = _matching;
            }
        }


        cout << highestMatching.size() << endl;
        cout << highestScore << endl;

        for (const Edge &edge : highestMatching)
            cout << edge.source + 1 << " " << edge.destination + 1 << endl;
    }
};

/**
 * @brief Main function
 * @return 0 on successful execution
 */
int main()
{
    // Improve the performance of cin
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string path = "/Users/jan/Documents/code/AlgorithmEngineering/";
    ifstream file(path + "example7.txt");
    if (!file.is_open())
    {
        cerr << "Failed to open the file." << endl;
        return 1;
    }

    // Read the number of nodes and edges from standard input
    int numberOfNodes, numberOfEdges;
    file >> numberOfNodes >> numberOfEdges;

    // Create a graph with the read number of nodes
    Graph G(numberOfNodes, numberOfEdges);

    // Read the edges from standard input and add them to the graph
    for (int line = 0; line < numberOfEdges * 2; ++line)
    {
        int source, destination, weight;
        file >> source >> destination >> weight;
        G.addEdge(source, destination, weight);
    }

    G.compareMatchings();

    cout << endl;

    // Return 0 on successful execution
    return 0;
}

/*
number M of edges in the matching: 3
weight of the matching: 13
list of edges in the matching:
7 1
6 4
3 2
*/