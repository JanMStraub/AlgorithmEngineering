#include <vector>
#include <iostream>
#include <list>
#include <fstream>

using namespace std;

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
    int _numberOfVertices;               // Number of vertices in the graph
    vector<vector<Edge> > _adjacencyList; // Adjacency list to represent the graph

    void _DFS(int node, vector<bool> &visited, list<int> &Nodes)
    {
        visited[node] = true;

        for (Edge &edge : _adjacencyList[node])
            if (!visited[edge.destination])
                _DFS(edge.destination, visited, Nodes);

        Nodes.push_front(node);
    }

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
    Graph(int vertices) : _numberOfVertices(vertices)
    {
        _adjacencyList.resize(_numberOfVertices);
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
        _adjacencyList[source - 1].push_back(Edge(source - 1, destination - 1, weight));
    }

    void topoSort()
    {
        list<int> Nodes;
        vector<bool> visited(_numberOfVertices, false);

        for (int node = 0; node < _numberOfVertices; ++node)
            if (!visited[node])
                _DFS(node, visited, Nodes);

        for (int node : Nodes)
            cout << node + 1 << " ";
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
    ifstream file(path + "example8.txt");
    if (!file.is_open())
    {
        cerr << "Failed to open the file." << endl;
        return 1;
    }

    // Read the number of nodes and edges from standard input
    int numberOfNodes, numberOfEdges;
    file >> numberOfNodes >> numberOfEdges;

    // Create a graph with the read number of nodes
    Graph G(numberOfNodes);

    // Read the edges from standard input and add them to the graph
    for (int line = 0; line < numberOfEdges * 2; ++line)
    {
        int source, destination, weight;
        file >> source >> destination >> weight;
        G.addEdge(source, destination, weight);
    }

    G.topoSort();

    cout << '\n';

    // Return 0 on successful execution
    return 0;
}

/*
all nodes of the graph in a topological order: 7 3 4 6 1 2 5
*/