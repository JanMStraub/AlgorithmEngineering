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

    /**
     * @brief Computes random initial cut of the max cut problem.
     * @return The initial cut.
     */
    vector<int> computeRandomInitialCut()
    {
        unsigned int seed = static_cast<unsigned int>(time(nullptr));
        srand(seed);
        vector<int> cut(_numberOfVertices);
        int randomVertex = rand() % _numberOfVertices;
        cut[randomVertex] = 1;
        cout << "selected vertex: " << randomVertex << endl;
        return cut;
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

    int findOptimalVertexForRandomCut(vector<int> partition)
    {
        int highestWeight = -1;
        int optimalVertex = -1;

        for (int vertex = 0; vertex < _numberOfVertices; ++vertex)
        {
            if (partition[vertex] == 0)
            {
                for (const Edge &edge : _adjacencyList[vertex])
                {
                    int destinationWeight = getWeightedDegree(edge.destination);
                    if (highestWeight < destinationWeight && partition[edge.destination] == 1)
                    {
                        highestWeight = destinationWeight;
                        optimalVertex = vertex;
                    }
                }
            }
        }

        if (optimalVertex == -1)
        {
            throw std::runtime_error("No optimal vertex found");
        }

        return optimalVertex;
    }

    /**
     * @brief Finds the neighborhood of a given vertex.
     * @param vertex The vertex for which to find the neighborhood.
     * @return The vector of neighboring vertices.
     */
    vector<int> findNeighborhood(int vertex)
    {
        vector<int> neighborhood;
        for (const Edge &edge : _adjacencyList[vertex])
        {
            if (edge.destination != vertex - 1) // Exclude the source vertex itself
            {
                neighborhood.push_back(edge.destination);
            }
        }

        // Sort the neighborhood
        sort(neighborhood.begin(), neighborhood.end());

        // Remove duplicates
        neighborhood.erase(unique(neighborhood.begin(), neighborhood.end()), neighborhood.end());

        return neighborhood;
}

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

    void getNeighborhood(int src, int dest)
    {
        auto neighborhood1 = findNeighborhood(src);
        auto neighborhood2 = findNeighborhood(dest);

        // Compute intersection
        vector<int> intersection;
        set_intersection(neighborhood1.begin(), neighborhood1.end(),
                        neighborhood2.begin(), neighborhood2.end(),
                        back_inserter(intersection));

        // Compute union
        vector<int> uni;
        merge(neighborhood1.begin(), neighborhood1.end(),
            neighborhood2.begin(), neighborhood2.end(),
            back_inserter(uni));

        // Output results
        for (const auto &value : intersection)
        {
            cout << value << " ";
        }
        cout << endl;

        for (const auto &value : uni)
        {
            cout << value << " ";
        }
        cout << endl;
    }

    /**
     * @brief Computes the maximum cut of the graph.
     *
     * This function initializes the cut and then iteratively improves it by toggling the partition of each vertex.
     * The process stops when no more beneficial moves can be found.
     */
    void computeMaxCut()
    {
        vector<int> cutPartition = computeRandomInitialCut();
        int localHighWeightedEdgeCut = getWeightedEdgeCut(cutPartition);
        bool improved;
        int numIterationsWithoutImprovement = 0;
        cout << findOptimalVertexForRandomCut(cutPartition) << endl;

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
    ifstream file(path + "example2_1.txt");
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

    int v1, v2;
    file >> v1 >> v2;

    G.getNeighborhood(v1, v2);

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