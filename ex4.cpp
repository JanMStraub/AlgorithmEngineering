#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <unordered_map>
#include <queue>

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
    int id;       // ID of the node
    int weight;   // Weight of the node
    float cost;   // Cost of the node
    bool blocked; // Marker if node is blocked
    bool chosen;  // Marker if node is chosen

    Node(int i, int weight, float cost, bool blocked, bool chosen) : id(i), weight(weight), cost(cost), blocked(blocked), chosen(chosen) {}

    // Overload the '<' operator to compare nodes based on their weight
    bool operator<(const Node &other) const
    {
        return cost > other.cost;
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
 * @var Graph::_nodeQueue
 * Member '_nodeQueue' represents the queue for the sorted nodes
 */
class Graph
{
private:
    int _numberOfVertices;               // Number of vertices in the graph
    vector<vector<Edge>> _adjacencyList; // Adjacency list to represent the graph
    vector<Node> _nodeList;              // List of nodes in the graph

    /**
     * @brief Calculates the cost of each vertex in the graph.
     * @return * void
     */
    void _calculateVertexCost()
    {
        for (Node &node : _nodeList)
        {
            int nodeCost = 0;
            int nodeID = node.id;
            for (const Edge &edge : _adjacencyList[nodeID])
                nodeCost += _nodeList[edge.destination].weight;

            node.cost = sqrt(nodeCost + _adjacencyList[nodeID].size()) / node.weight;
        }
    }

public:
    // Constructor
    Graph(int vertices) : _numberOfVertices(vertices)
    {
        _adjacencyList.resize(_numberOfVertices);
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
        _adjacencyList[source - 1].push_back(Edge(destination - 1, weight));
    }

    /**
     * @brief Function to add a node to the graph
     * @param id ID of the node
     * @param degree Degree of the node
     * @param weight Weight of the node
     * @param blocked Node blocked for further expansion
     * @param chosen Node chosen for set
     */
    void addNode(int id, int weight, float cost = 0, bool blocked = false, bool chosen = false)
    {
        _nodeList.emplace_back(Node(id, weight, cost, blocked, chosen));
    }

    /**
     * @brief Calculates the maximum set of nodes based on a priority queue.
     * @return * void
     */
    void calculateMaxSet()
    {
        int selectedNodeCount = 0;
        int totalWeightOfSelectedNodes = 0;

        _calculateVertexCost();

        // Sort the vertices based on their cost
        sort(_nodeList.begin(), _nodeList.end(), [](const Node &a, const Node &b)
             { return a.cost < b.cost; });

        for (Node &node : _nodeList)
        {
            if (!node.blocked)
            {
                node.chosen = true;
                selectedNodeCount++;
                totalWeightOfSelectedNodes += node.weight;

                // Block all nodes adjacent to the current node
                for (const Edge &edge : _adjacencyList[node.id])
                {
                    int destinationNode = edge.destination;
                    // Only block the node if it hasn't been chosen
                    if (!_nodeList[destinationNode].chosen)
                        _nodeList[destinationNode].blocked = true;
                }
            }
        }

        cout << selectedNodeCount << endl
             << totalWeightOfSelectedNodes << endl;

        for (const Node &node : _nodeList)
            if (node.chosen)
                cout << node.id + 1 << " ";

        cout << endl;
    }
    /*
    2
    7
    7 3
    */
};

/**
 * @brief Main function
 * @return 0 on successful execution
 */
int main()
{
    ifstream file("/Users/jan/Documents/code/AlgorithmEngineering/example4.txt");
    if (!file.is_open())
    {
        cerr << "Failed to open the file." << endl;
        return 1;
    }

    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Read the number of nodes and edges
    int numberOfNodes, numberOfEdges;
    file >> numberOfNodes >> numberOfEdges;

    // Create a graph
    Graph G(numberOfNodes);

    // Add edges to the graph
    for (int line = 0; line < numberOfEdges * 2; ++line)
    {
        int source, destination, weight;
        file >> source >> destination >> weight;
        G.addEdge(source, destination, weight);
    }

    // Add nodes to the graph
    for (int line = 0; line < numberOfNodes; ++line)
    {
        int weight;
        file >> weight;
        G.addNode(line, weight);
    }

    G.calculateMaxSet();

    return 0;
}