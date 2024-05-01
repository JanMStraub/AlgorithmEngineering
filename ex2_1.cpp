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
    int _numberOfVertices; // number of vertices
    vector<vector<Edge> > _adjacencyList;

public:
    /**
     * @brief Constructs a Graph object with the specified number of vertices.
     * @param vertices The number of vertices in the graph.
     */
    Graph(int vertices) : _numberOfVertices(vertices)
    {
        _adjacencyList.resize(_numberOfVertices + 1);
    }

    /**
     * @brief Adds an edge to the graph.
     * @param source The source vertex of the edge.
     * @param destination The destination vertex of the edge.
     * @param weight The weight of the edge.
     */
    void addEdge(int source, int destination, int weight)
    {
        _adjacencyList[source].push_back(Edge(source, destination, weight));
    }

    /**
     * @brief Returns the maximum degree of the graph
     * @return The maximum degree
     */
    int getMaxDegree() const
    {
        int maxDegree = 0;

        for (int vertex = 0; vertex < _numberOfVertices; ++vertex)
            maxDegree = max(maxDegree, static_cast<int>(_adjacencyList[vertex].size()));

        return maxDegree;
    }

    /**
     * @brief Returns the minimum degree of the graph
     * @return The minimum degree
     */
    int getMinDegree() const
    {
        int minDegree = INT_MAX;

        for (int vertex = 1; vertex < _numberOfVertices; ++vertex)
            minDegree = min(minDegree, static_cast<int>(_adjacencyList[vertex].size()));

        return minDegree;
    }

    /**
     * @brief Returns the weighted degree of a vertex
     * @param vertex The vertex
     * @return The weighted degree of the vertex
     */
    int getWeightedDegree(int vertex) const
    {
        int weightedDegree = 0;
        for (const Edge &edge : _adjacencyList[vertex])
        {
            weightedDegree += edge.weight;
        }

        return weightedDegree;
    }

    /**
     * @brief Returns the maximum weighted degree of the graph
     * @return The maximum weighted degree
     */
    int getMaxWeightedDegree() const
    {
        int maxWeightedDegree = INT_MIN;

        for (int vertex = 1; vertex <= _numberOfVertices; ++vertex)
        {
            maxWeightedDegree = max(maxWeightedDegree, getWeightedDegree(vertex));
        }

        return maxWeightedDegree;
    }

    /**
     * @brief Returns the minimum weighted degree of the graph
     * @return The minimum weighted degree
     */
    int getMinWeightedDegree() const
    {
        int minWeightedDegree = INT_MAX;

        for (int vertex = 1; vertex <= _numberOfVertices; ++vertex)
        {
            minWeightedDegree = min(minWeightedDegree, getWeightedDegree(vertex));
        }

        return minWeightedDegree;
    }

    /**
     * @brief Returns the total weight of the graph
     * @return The total weight
     */
    int getTotalWeight() const
    {
        int totalWeight = 0;

        for (int vertex = 1; vertex <= _numberOfVertices; ++vertex)
        {
            for (const Edge &edge : _adjacencyList[vertex])
            {
                totalWeight += edge.weight;
            }
        }

        return totalWeight / 2;
    }

    /**
     * @brief Prints the graph.
     *
     * This method prints the graph by iterating over each vertex and its edges.
     * For each vertex, it prints the destination vertex and the weight of the edge.
     */
    void printGraph()
    {
        for (int vertex = 1; vertex <= _numberOfVertices; ++vertex)
        {
            cout << "Vertex " << vertex << endl;
            for (const Edge &edge : _adjacencyList[vertex])
            {
                cout << edge.source << " - " << edge.destination << " (weight: " << edge.weight << ")" << endl;
            }
            cout << endl;
        }
    }

    /**
     * @brief Computes partitioning metrics for the graph.
     * @param partition A vector containing the partition of each vertex.
     * @param k The number of blocks in the partition.
     * @param edgeCut The edge-cut of the partition.
     * @param weightedEdgeCut The weighted edge-cut of the partition.
     * @param balance The balance of the partition.
     */
    void computePartitionMetrics(const vector<int> &partition, int numberOfBlocks, int &edgeCut, int &weightedEdgeCut, double &balance) const
    {
        edgeCut = 0;
        weightedEdgeCut = 0;
        vector<int> blockSizes(numberOfBlocks, 0);

        for (int vertex = 1; vertex <= _numberOfVertices; ++vertex)
        {
            for (const Edge &edge : _adjacencyList[vertex])
            {
                int sourcePartition = partition[vertex - 1];
                int destPartition = partition[edge.destination - 1];
                if (sourcePartition != destPartition)
                {
                    edgeCut++;
                    weightedEdgeCut += edge.weight;
                }
            }
            blockSizes[partition[vertex - 1]]++;
        }

        int maxBlockSize = *max_element(blockSizes.begin(), blockSizes.end());
        balance = static_cast<double>(maxBlockSize) / (_numberOfVertices / static_cast<double>(numberOfBlocks));
    }
};

/**
 * @brief The main function.
 * @return 0 on success, 1 on failure.
 */
int main()
{

    ifstream file("/Users/jan/Documents/code/AlgorithmEngineering/example1_2.txt");
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

    // Read partition
    // vector<int> partition(numberOfNodes);
    // for (int i = 0; i < numberOfNodes; ++i)
    // {
    //    file >> partition[i];
    // }

    // G.printGraph();

    return 0;
}

/*
3
1 2 3 4 5 7
*/