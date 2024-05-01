#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
using namespace std;

struct Edge
{
    int source;
    int destination;
    int weight;

    Edge(int src, int dest, int w) : source(src), destination(dest), weight(w) {}
};

/**
 * @class Graph
 * @brief Represents a graph data structure.
 *
 * The Graph class provides methods to create and manipulate a graph.
 */
class Graph
{
private:
    int numberOfVertices; // number of vertices
    vector<vector<Edge> > adjacencyList;

public:
    /**
     * @brief Constructs a Graph object with the specified number of vertices.
     * @param vertices The number of vertices in the graph.
     */
    Graph(int vertices) : numberOfVertices(vertices)
    {
        adjacencyList.resize(numberOfVertices + 1);
    }

    /**
     * @brief Adds an edge to the graph.
     * @param source The source vertex of the edge.
     * @param destination The destination vertex of the edge.
     * @param weight The weight of the edge.
     */
    void addEdge(int source, int destination, int weight)
    {
        adjacencyList[source].push_back(Edge(source, destination, weight));
    }

    /**
     * @brief Prints the graph.
     *
     * This method prints the graph by iterating over each vertex and its edges.
     * For each vertex, it prints the destination vertex and the weight of the edge.
     */
    void printGraph()
    {
        for (int vertex = 1; vertex <= numberOfVertices; ++vertex)
        {
            cout << "Vertex " << vertex << endl;
            for (const Edge &edge : adjacencyList[vertex])
            {
                cout <<  edge.source << " -> " << edge.destination << " (weight: " << edge.weight << ")" << endl;
            }
            cout << endl;
        }
    }
};

/**
 * @brief The main function.
 * @return 0 on success, 1 on failure.
 */
int main()
{

    ifstream file("/Users/jan/Documents/code/AlgorithmEngineering/example2_1.txt");
    if (!file.is_open())
    {
        cerr << "Failed to open the file." << endl;
        return 1;
    }

    // get graph metrics
    int numberOfNodes, numberOfEdges;
    file >> numberOfNodes >> numberOfEdges;

    Graph G(numberOfNodes);

    // get edge data
    for (int i = 0; i < numberOfEdges * 2; i++)
    {
        int source, destination, weight;
        file >> source >> destination >> weight;
        G.addEdge(source, destination, weight);
    }

    G.printGraph();

    return 0;
}

/*
3
1 2 3 4 5 7
*/