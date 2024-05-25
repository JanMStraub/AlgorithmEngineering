#include <vector>
#include <iostream>
#include <queue>
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
    vector<vector<Edge> > _adjacencyList; // Adjacency list to represent the graph

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
        // Resize the adjacency list to hold the edges for each vertex
        _adjacencyList.resize(_numberOfVertices);
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
        _adjacencyList[source - 1].push_back(Edge(source - 1, destination - 1, weight));
    }

    void bellmannFord()
    {
        vector<int> negativeCycleNodes;

        vector<int> distances(_numberOfVertices, 1000);
        distances[0] = 0;
        for(int i=1; i<_numberOfVertices; i++)
            for (int nodeID = 0; nodeID < _numberOfVertices; ++nodeID)
                for (const Edge &edge : _adjacencyList[nodeID])
                    if (distances[edge.source] != 1000 && distances[nodeID] + edge.weight < distances[edge.destination])
                    {
                        distances[edge.destination] = distances[nodeID] + edge.weight;
                    }

        for (int nodeID = 0; nodeID < _numberOfVertices; nodeID++)
            for (const Edge &edge : _adjacencyList[nodeID])
                if (distances[edge.source] != 1000 && distances[nodeID] + edge.weight < distances[edge.destination])
                    negativeCycleNodes.push_back(edge.destination);

        cout << negativeCycleNodes.size() << endl;

        for (int i = 0; i < negativeCycleNodes.size(); ++i)
            cout << negativeCycleNodes[i] + 1 << " ";

        cout << negativeCycleNodes[0]+1 << endl;

        /*
        amount of nodes in the found negative cycle: 5
        list of nodes in the found negative cycle repeating the first node at the end: 4 7 1 2 5 4
        */
    }
};

/**
 * @brief Main function
 *
 * This function reads a graph from standard input, computes the maximum flow from a source to a target using Dinic's algorithm, and prints the maximum flow and the nodes in the computed cut.
 *
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

    // Return 0 on successful execution
    return 0;
}