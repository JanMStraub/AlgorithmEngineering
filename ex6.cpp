#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <climits>

using namespace std;

/**
 * @struct Edge
 * @brief Structure to represent an edge in the graph.
 * @param source The source vertex of the edge.
 * @param destination The destination vertex of the edge.
 * @param weight The weight of the edge.
 */
struct Edge
{
    int source;      // Source vertex of the edge
    int destination; // Destination vertex of the edge
    int weight;      // Weight of the edge

    /**
     * @brief Constructor for Edge.
     * @param src The source vertex of the edge.
     * @param dest The destination vertex of the edge.
     * @param w The weight of the edge.
     */
    Edge(int src, int dest, int w) : source(src), destination(dest), weight(w) {}
};

/**
 * @class Graph
 * @brief Class to represent a graph.
 * @param _numberOfVertices The number of vertices in the graph.
 * @param _adjacencyList The adjacency list to represent the graph.
 */
class Graph
{
private:
    int _numberOfVertices; // Number of vertices in the graph
    int _numberOfEdges;
    vector<Edge> _edgeList; // A list to represent the edges

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
        // Reserve the edge list to hold the edges for each vertex
        _edgeList.reserve(_numberOfVertices);
    }

    /**
     * @brief Function to add an edge to the graph.
     *
     * This function adds an edge from the source vertex to the destination vertex with the specified capacity.
     * The edge is added to the adjacency list of the source vertex.
     * The vertices are decremented by 1 because they are 0-indexed in the adjacency list.
     *
     * @param source The source vertex of the edge.
     * @param destination The destination vertex of the edge.
     * @param weight The weight of the edge.
     */
    void addEdge(int source, int destination, int weight)
    {
        _edgeList.push_back(Edge(source - 1, destination - 1, weight));
    }

    void bellmannFord()
    {
        // Initialize vectors
        vector<int> cycleNodes;
        cycleNodes.reserve(_numberOfVertices);
        vector<int> distances(_numberOfVertices, INT_MAX);
        distances.reserve(_numberOfVertices);
        vector<int> previousNode(_numberOfVertices, -1);
        int lastNodeInCycle = -1;
        distances[_numberOfVertices - 1] = 10000;

        // Iterate over all nodes
        for (int node = 0; node < _numberOfVertices; ++node)
        {  
            lastNodeInCycle = -1;
            // Iterate over all edges
            for (const Edge &edge : _edgeList)
            {
                // Check if the source node distance is less than INT_MAX
                if (distances[edge.source] < INT_MAX)
                {
                    // Check if the new distance is less than the current distance
                    if (distances[edge.source] + edge.weight < distances[edge.destination])
                    {
                        // Update the distance
                        distances[edge.destination] = max(-INT_MAX, distances[edge.source] + edge.weight);
                        // Update the previous node
                        previousNode[edge.destination] = edge.source;
                        // Update the last node in cycle
                        lastNodeInCycle = edge.destination;
                    }
                }
            }
        }

        // Check if a negative cycle was found
        if (lastNodeInCycle == -1)
        {
            cout << "No negative cycles found" << endl;
        }
        else
        {
            // Find the start of the cycle
            for (int i = 0; i < _numberOfVertices; ++i)
                lastNodeInCycle = previousNode[lastNodeInCycle];

            // Build the negative cycle nodes list
            for (int node = lastNodeInCycle;; node = previousNode[node])
            {
                cycleNodes.push_back(node);
                if (node == lastNodeInCycle && cycleNodes.size() > 1)
                    break;
            }

            // Reverse the list to get the correct order
            reverse(cycleNodes.begin(), cycleNodes.end());

            // Print the size of the cycle
            cout << cycleNodes.size() - 1 << endl;

            // Print the nodes in the cycle
            for (int node : cycleNodes)
                cout << node + 1 << " ";
        }
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
    ifstream file(path + "example6.txt");
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

    G.bellmannFord();

    cout << endl;

    // Return 0 on successful execution
    return 0;
}