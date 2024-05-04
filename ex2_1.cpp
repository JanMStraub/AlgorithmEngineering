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
    int _numberOfVertices;
    vector<vector<Edge> > _adjacencyList;

public:
    /**
     * @brief Constructs a Graph object with the specified number of vertices.
     * @param vertices The number of vertices in the graph.
     */
    Graph(int vertices) : _numberOfVertices(vertices)
    {
        _adjacencyList.resize(_numberOfVertices);
    }

    /**
     * @brief Adds an edge to the graph.
     * @param source The source vertex of the edge.
     * @param destination The destination vertex of the edge.
     * @param weight The weight of the edge.
     */
    void addEdge(int source, int destination, int weight)
    {
        _adjacencyList[source - 1].push_back(Edge(source - 1, destination - 1, weight));
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
        int minDegree = 1000;

        for (int vertex = 0; vertex < _numberOfVertices; ++vertex)
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
        int maxWeightedDegree = 0;

        for (int vertex = 0; vertex < _numberOfVertices; ++vertex)
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
        int minWeightedDegree = 1000;

        for (int vertex = 0; vertex < _numberOfVertices; ++vertex)
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

        for (int vertex = 0; vertex < _numberOfVertices; ++vertex)
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
        cout << endl;
        for (int vertex = 0; vertex < _numberOfVertices; ++vertex)
        {
            cout << "Vertex " << vertex + 1 << endl;
            for (const Edge &edge : _adjacencyList[vertex])
            {
                cout << edge.source + 1 << " - " << edge.destination + 1 << " (weight: " << edge.weight << ")" << endl;
            }
            cout << endl;
        }
    }

    /**
     * @brief Computes the edge-cut metrics for the graph partition.
     * @param partition A vector containing the partition of each vertex.
     * @return The edge-cut of the partition.
     */
    int getEdgeCut(const vector<int> &partition)
    {
        int edgeCut = 0;
        for (int vertex = 0; vertex < _numberOfVertices; ++vertex)
        {
            for (const Edge &edge : _adjacencyList[vertex])
            {
                int sourcePartition = partition[vertex];
                int destPartition = partition[edge.destination];
                if (sourcePartition != destPartition)
                {
                    edgeCut++;
                }
            }
        }
        return edgeCut / 2;
    }

    /**
     * @brief Computes the weighted edge-cut metrics for the graph partition.
     * @param partition A vector containing the partition of each vertex.
     * @return The weighted edge-cut of the partition.
     */
    int getWeightedEdgeCut(const vector<int> &partition)
    {
        int weightedEdgeCut = 0;
        for (int vertex = 0; vertex < _numberOfVertices; ++vertex)
        {
            int sourcePartition = partition[vertex];
            for (const Edge &edge : _adjacencyList[vertex])
            {
                int destPartition = partition[edge.destination];
                if (sourcePartition != destPartition)
                {
                    weightedEdgeCut += edge.weight;
                }
            }
        }
        return weightedEdgeCut / 2;
    }

    /**
     * @brief Computes the balance metric for the graph partition.
     * @param partition A vector containing the partition of each vertex.
     * @param numberOfBlocks The number of blocks in the partition.
     * @return The balance of the partition.
     */
    double getBalance(const vector<int> &partition, int numberOfBlocks)
    {
        vector<int> blockSizes(numberOfBlocks, 0);
        for (int vertex = 0; vertex < _numberOfVertices; ++vertex)
        {
            blockSizes[partition[vertex]]++;
        }
        int maxBlockSize = *max_element(blockSizes.begin(), blockSizes.end());
        double balance = static_cast<double>(maxBlockSize) / (_numberOfVertices / static_cast<double>(numberOfBlocks));
        return balance;
    }

    /**
     * @brief Computes the initial cut of the max cut problem.
     * @return The initial cut.
     */
    vector<int> computeInitialCut()
    {
        vector<int> cut(_numberOfVertices);
        for (int i = 0; i < _numberOfVertices; ++i)
        {
            cut[i] = i % 2; // Alternating partition
        }
        return cut;
    }

    /**
     * @brief Computes the maximum cut of the graph.
     *
     * This function initializes the cut and then iteratively improves it by toggling the partition of each vertex.
     * The process stops when no more beneficial moves can be found.
     */
    void computeMaxCut()
    {
        vector<int> cutPartition = computeInitialCut();
        int localHighWeightedEdgeCut = getWeightedEdgeCut(cutPartition);
        bool improved;
        int numIterationsWithoutImprovement = 0;

        do
        {
            improved = false;

            for (int vertex = 0; vertex < _numberOfVertices; ++vertex)
            {
                int oldPartition = cutPartition[vertex];
                cutPartition[vertex] ^= 1; // Toggle partition for current vertex
                int newWeightedEdgeCut = getWeightedEdgeCut(cutPartition);

                if (newWeightedEdgeCut > localHighWeightedEdgeCut)
                {
                    localHighWeightedEdgeCut = newWeightedEdgeCut;
                    improved = true;
                    numIterationsWithoutImprovement = 0;
                }
                else
                {
                    cutPartition[vertex] = oldPartition; // Revert partition change
                }
            }

            numIterationsWithoutImprovement++;
        } while (improved && numIterationsWithoutImprovement < _numberOfVertices); // Stop if no improvement after one full iteration

        cout << localHighWeightedEdgeCut << endl;

        for (int i = 0; i < _numberOfVertices; i++)
        {
            if (!cutPartition[i])
            {
                cout << i + 1 << " ";
            }
        }
    }
};

/**
 * @brief The main function.
 * @return 0 on success, 1 on failure.
 */
int main()
{
    string path = "/Users/jan/Documents/code/AlgorithmEngineering/";
    ifstream file(path + "example3.txt");
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
    for (int i = 0; i < numberOfEdges * 2; ++i)
    {
        int source, destination, weight;
        file >> source >> destination >> weight;
        G.addEdge(source, destination, weight); // 0-based indexing
    }

    // Read partition
    // vector<int> partition(numberOfNodes);
    // for (int i = 0; i < numberOfNodes; ++i)
    // {
    //    file >> partition[i];
    // }

    // G.printGraph();

    G.computeMaxCut();

    return 0;
}

/*
30
3 5 7
*/