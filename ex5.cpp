#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <deque>

using namespace std;

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
    int level;   // Weight of the node
    bool source;
    bool target;

    Node(int id) : id(id), level(-1), source(false), target(false) {}
};

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
    int source;
    int destination; // Destination vertex of the edge
    int flow;      // Weight of the edge
    int capacity;

    Edge(int src, int dest, int cap) : source(src), destination(dest), flow(0), capacity(cap) {}
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
    int _sourceID;
    int _targetID;
    vector<vector<Edge> > _adjacencyList; // Adjacency list to represent the graph
    vector<Node> _nodeList;              // List of nodes in the graph

    bool _BFS (int source, int target)
    {
        for (Node &node : _nodeList)
            node.level = -1;

        Node &sourceNode = _nodeList[source];
        sourceNode.level = 0;

        deque<int> q;
        q.push_back(source);

        while (!q.empty())
        {
            int currentNodeIndex = q.front();
            q.pop_front();
            Node &currentNode = _nodeList[currentNodeIndex];

            for (const Edge &edge : _adjacencyList[currentNode.id])
            {
                Node &destinationNode = _nodeList[edge.destination];
                if ((destinationNode.level < 0) && (edge.flow < edge.capacity))
                {
                    q.push_back(destinationNode.id);
                    destinationNode.level = currentNode.level + 1;
                }
            }
        }

        //cout << "BFS: " << (_nodeList[target].level >= 0) << endl;
    
        return _nodeList[target].level >= 0;
    }

    int _sendFlow(const Node& currentNode, int flow)
    {
        if (currentNode.target)
            return flow;

        for (Edge& edge : _adjacencyList[currentNode.id])
        {
            cout << "source level: " << currentNode.level << "  destination level: " << _nodeList[edge.destination].level << endl;
            if (currentNode.level + 1 == _nodeList[edge.destination].level && edge.flow < edge.capacity)
            {
                int currentFlow = min(flow, edge.capacity - edge.flow);
                cout << "current flow: " << currentFlow << endl;
                int tempFlow = _sendFlow(_nodeList[edge.destination], currentFlow);
                
                if (tempFlow > 0)
                {
                    edge.flow += tempFlow;
                    return tempFlow;
                }
            }
            cout << "edge source: " << edge.source + 1 << " edge destination: " << edge.destination + 1 << " edge flow: " << edge.flow << endl;
        }
        return 0;
    }

public:
    // Constructor
    Graph(int vertices) : _numberOfVertices(vertices)
    {
        _adjacencyList.resize(_numberOfVertices);
        _nodeList.reserve(_numberOfVertices);
    }

    /**
     * @brief Function to add a node to the graph
     * @param id ID of the node
     */
    void addNode(int id)
    {
        _nodeList.emplace_back(Node(id));
    }

    /**
     * @brief Function to add an edge to the graph
     * @param source Source vertex of the edge
     * @param destination Destination vertex of the edge
     * @param weight Weight of the edge
     */
    void addEdge(int source, int destination, int capacity)
    {
        _adjacencyList[source - 1].push_back(Edge(source - 1, destination - 1, capacity));
    }

    void setSourceAndTarget(int sourceID, int targetID)
    {
        _sourceID = sourceID - 1;
        _targetID = targetID - 1;
        _nodeList[sourceID].source = true;
        _nodeList[targetID].target = true;
    }

    void computeMaxFlowDinic()
    {
        if (_sourceID == _targetID) 
        {
            cout << "no correcto inputo" << endl;
            return;
        }

        int total = 0;

        while (_BFS(_sourceID, _targetID))
        {
            vector<bool> visited(_nodeList.size(), false);
            while (int flow = _sendFlow(_nodeList[_sourceID], 10000))
                total += flow;
        }

        cout << "computed cut: " << total << endl;

    }
};

/**
 * @brief Main function
 * @return 0 on successful execution
 */
int main()
{
    string path = "/Users/jan/Documents/code/AlgorithmEngineering/";
    ifstream file(path + "example5.txt");
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

    // Add nodes to the graph
    for (int line = 0; line < numberOfNodes; ++line)
    {
        G.addNode(line);
    }

    // Add edges to the graph
    for (int line = 0; line < numberOfEdges * 2; ++line)
    {
        int source, destination, weight;
        file >> source >> destination >> weight;
        G.addEdge(source, destination, weight);
    }

    int source, target;
    file >> source >> target;
    G.setSourceAndTarget(source, target);

    G.computeMaxFlowDinic();

    return 0;
}

/*
value of the computed cut: 13
list of nodes in one of the blocks of the computed cut: 7 4 6 2 3 5
*/