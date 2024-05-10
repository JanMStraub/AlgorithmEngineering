#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

/**
 * @struct Edge
 * @brief Structure to represent an edge in the graph
 * @var Edge::destination
 * Member 'destination' represents the destination vertex of the edge
 * @var Edge::weight
 * Member 'weight' represents the weight of the edge
 */
struct Edge
{
    int destination; // Destination vertex of the edge
    int weight;      // Weight of the edge
    Edge() : destination(0), weight(0) {}
    Edge(int dest, int w) : destination(dest), weight(w) {}
};

/**
 * @struct Node
 * @brief Structure to represent a node in the graph
 * @var Node::id
 * Member 'id' represents the ID of the node
 * @var Node::degree
 * Member 'degree' represents the degree of the node (number of edges)
 * @var Node::weight
 * Member 'weight' represents the weight of the node
 */
struct Node
{
    int id;     // ID of the node
    int degree; // Degree of the node (number of edges)
    int weight; // Weight of the node
    Node() : id(0), degree(0), weight(0) {}
    Node(int i, int deg, int w) : id(i), degree(deg), weight(w) {}
    // Overload the '<' operator to compare nodes based on their weight
    bool operator<(const Node &other) const
    {
        return weight > other.weight;
    }
};

/**
 * @class Graph
 * @brief Class to represent a graph
 * @var Graph::_numberOfVertices
 * Member '_numberOfVertices' represents the number of vertices in the graph
 * @var Graph::_adjacencyList
 * Member '_adjacencyList' represents the adjacency list to represent the graph
 * @var Graph::_nodeList
 * Member '_nodeList' represents the list of nodes in the graph
 */
class Graph
{
private:
    int _numberOfVertices;               // Number of vertices in the graph
    vector<vector<Edge> > _adjacencyList; // Adjacency list to represent the graph
    vector<Node> _nodeList;              // List of nodes in the graph

public:
    // Constructor
    Graph(int vertices) : _numberOfVertices(vertices)
    {
        _adjacencyList.reserve(_numberOfVertices);
        _nodeList.reserve(_numberOfVertices);
    }

    /**
     * @brief Function to add an edge to the graph
     * @param source Source vertex of the edge
     * @param destination Destination vertex of the edge
     * @param weight Weight of the edge
     */
    void addEdge(int source, int destination, int weight)
    {
        _adjacencyList[source - 1].emplace_back(destination - 1, weight);
    }

    /**
     * @brief Function to add a node to the graph
     * @param id ID of the node
     * @param degree Degree of the node
     * @param weight Weight of the node
     */
    void addNode(int id, int degree, int weight)
    {
        _nodeList[id] = Node(id, degree, weight);
    }

    /**
     * @brief Function to compute the weighted edge cut of a partition
     * @param partition Vector representing the partition of the graph
     * @return Weighted edge cut of the partition
     */
    int getWeightedEdgeCut(const vector<int> &partition)
    {
        int weightedEdgeCut = 0;
        for (int vertex = 0; vertex < _numberOfVertices; ++vertex)
            for (const Edge &edge : _adjacencyList[vertex])
                if (partition[vertex] != partition[edge.destination])
                    weightedEdgeCut += edge.weight;

        return weightedEdgeCut / 2;
    }

    /**
     * @brief Function to compute the maximum cut of the graph
     */
    void computeMaxCut()
    {
        vector<int> cutPartition(_numberOfVertices, 0);
        vector<int> usedNodes(_numberOfVertices, 0);

        // Initialize the nodes with their weights
        for (int vertex = 0; vertex < _numberOfVertices; ++vertex)
        {
            int weight = 0;
            for (const Edge &edge : _adjacencyList[vertex])
                weight += edge.weight;

            addNode(vertex, _adjacencyList[vertex].size(), weight);
        }

        // Sort the vertices based on their weight
        sort(_nodeList.begin(), _nodeList.end(), [](const Node &a, const Node &b)
             { return a.weight > b.weight; });

        // Initialize the first two nodes
        cutPartition[_nodeList[1].id] = 1;
        usedNodes[_nodeList[0].id] = 1;
        usedNodes[_nodeList[1].id] = 1;

        // Compute the maximum cut
        for (int vertex = 2; vertex < _numberOfVertices; ++vertex)
        {
            int nodeID = _nodeList[vertex].id;
            int connection0 = 0;
            int connection1 = 0;

            for (const Edge &edge : _adjacencyList[nodeID])
                if (cutPartition[edge.destination])
                    connection1 += usedNodes[edge.destination] * edge.weight;
                else
                    connection0 += usedNodes[edge.destination] * edge.weight;

            cutPartition[nodeID] = (connection1 < connection0) ? 1 : 0;
            usedNodes[nodeID] = 1;
        }

        // Print the weighted edge cut
        cout << getWeightedEdgeCut(cutPartition) << endl;

        // Print the nodes in the cut
        for (int i = 0; i < _numberOfVertices; ++i)
        {
            if (!cutPartition[i])
            {
                cout << i + 1 << " ";
            }
        }
    }
};

/**
 * @brief Main function
 * @return 0 on successful execution
 */
int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Read the number of nodes and edges
    int numberOfNodes, numberOfEdges;
    cin >> numberOfNodes >> numberOfEdges;

    // Create a graph
    Graph G(numberOfNodes);

    // Add edges to the graph
    for (int i = 0; i < numberOfEdges * 2; ++i)
    {
        int source, destination, weight;
        cin >> source >> destination >> weight;
        G.addEdge(source, destination, weight);
    }

    // Compute the maximum cut
    G.computeMaxCut();

    return 0;
}