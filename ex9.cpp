#include <vector>
#include <iostream>
#include <queue>
#include <unordered_set>
#include <fstream>
#include <algorithm>

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
    int color;       // Color of this edge

    /**
     * @brief Constructor for Edge.
     * @param src The source vertex of the edge.
     * @param dest The destination vertex of the edge.
     * @param w The weight of the edge.
     */
    Edge(int src, int dest, int w) : source(src), destination(dest), weight(w), color(-1) {}
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
    vector<vector<Edge> > _adjacencyList; // Adjacency list to represent the graph
    vector<bool> _visited;
    int _highestColor = 0;

    void _coloring()
    {
        // Create a vector of pairs (degree, vertexID) for all vertices
        vector<pair<int, int> > degrees(_numberOfVertices);
        for (int i = 0; i < _numberOfVertices; i++)
        {
            degrees[i] = make_pair(_adjacencyList[i].size(), i);
        }

        // Sort the vertices in descending order of their degrees
        sort(degrees.begin(), degrees.end(), greater<pair<int, int> >());

        // Color the vertices in the order determined by their degrees
        for (const auto &degree : degrees)
        {
            int currentNodeID = degree.second;
            _colorVertex(currentNodeID);
        }
    }

    void _colorVertex(int currentNodeID)
    {
        if (_visited[currentNodeID])
            return;

        _visited[currentNodeID] = true;

        unordered_set<int> colored;

        // Collect colors of all adjacent edges
        for (const Edge &e : _adjacencyList[currentNodeID])
            if (e.color != -1)
                colored.insert(e.color);

        // Find the smallest color that's not used by adjacent edges
        int c = 0;
        while (colored.find(c) != colored.end())
            c++;

        // Color all edges from currentNodeID
        for (Edge &edge : _adjacencyList[currentNodeID])
        {
            if (edge.color == -1)
            {
                edge.color = c;
                if (_highestColor < c)
                    _highestColor = c;

                // Color the reverse edge as well
                for (Edge &revEdge : _adjacencyList[edge.destination])
                {
                    if (revEdge.destination == currentNodeID && revEdge.source == edge.destination && revEdge.color == -1)
                    {
                        revEdge.color = c;
                        break;
                    }
                }
            }
        }
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
    Graph(int vertices, int edges) : _numberOfVertices(vertices), _numberOfEdges(edges)
    {
        _adjacencyList.resize(_numberOfVertices);
        _visited.resize(_numberOfVertices);
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

    void edgeColoring()
    {
        _coloring();

        cout << _highestColor + 1 << "\n";
        for (int nodeID = 0; nodeID < _numberOfVertices; ++nodeID)
            for (const Edge &edge : _adjacencyList[nodeID])
                if (edge.source < edge.destination)
                    cout << edge.source + 1 << " " << edge.destination + 1 << " " << edge.color + 1 << "\n";
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

    string path = "/Users/jan/Documents/code/ae/";
    ifstream file(path + "example9.txt");
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

    G.edgeColoring();

    cout << '\n';

    // Return 0 on successful execution
    return 0;
}

/*
number of used colors: 5
list of edges in E, one by line, followed by its color:
1 2 1
1 3 2
1 7 3
2 3 3
2 5 2
2 4 4
3 4 1
3 5 4
4 5 3
4 6 2
4 7 5
6 7 1
*/