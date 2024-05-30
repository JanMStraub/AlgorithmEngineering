#include <vector>
#include <iostream>
#include <queue>

using namespace std;

/**
 * @struct Node
 * @brief Structure to represent a node in the graph.
 * @param id The ID of the node.
 * @param level The level of the node in the BFS tree.
 */
struct Node
{
    int id;    // ID of the node
    int level; // Weight of the node

    /**
     * @brief Constructor for Node.
     * @param id The ID of the node.
     */
    Node(int id) : id(id), level(-1) {}
};

/**
 * @struct Edge
 * @brief Structure to represent an edge in the graph.
 * @param source The source vertex of the edge.
 * @param destination The destination vertex of the edge.
 * @param flow The current flow in this edge.
 * @param capacity The capacity of the edge.
 */
struct Edge
{
    int source;      // Source vertex of the edge
    int destination; // Destination vertex of the edge
    int flow;        // Current flow in this edge
    int capacity;    // Capacity of the edge

    /**
     * @brief Constructor for Edge.
     * @param src The source vertex of the edge.
     * @param dest The destination vertex of the edge.
     * @param cap The capacity of the edge.
     */
    Edge(int src, int dest, int cap) : source(src), destination(dest), flow(0), capacity(cap) {}
};

/**
 * @class Graph
 * @brief Class to represent a graph.
 * @param _numberOfVertices The number of vertices in the graph.
 * @param _sourceID The source vertex for the flow.
 * @param _targetID The target vertex for the flow.
 * @param _adjacencyList The adjacency list to represent the graph.
 * @param _nodeList The list of nodes in the graph.
 */
class Graph
{
private:
    int _numberOfVertices;               // Number of vertices in the graph
    int _sourceID;                       // Source vertex for the flow
    int _targetID;                       // Target vertex for the flow
    vector<vector<Edge> > _adjacencyList; // Adjacency list to represent the graph
    vector<Node> _nodeList;              // List of nodes in the graph

    /**
     * @brief Function to perform Breadth-First Search (BFS) in the residual graph.
     *
     * This function is used to check if there is a path from source to target in the residual graph.
     * It also assigns levels to nodes which are used in Dinic's algorithm to decide whether to consider an edge or not.
     *
     * @return True if there is a path from source to target, false otherwise.
     */
    bool _BFS()
    {
        // Initialize all nodes' level as -1
        for (Node &node : _nodeList)
            node.level = -1;

        // Set the level of source node as 0
        _nodeList[_sourceID].level = 0;

        // Create a queue, enqueue source vertex for BFS traversal
        queue<int> q;
        q.push(_sourceID);

        // Standard BFS loop
        while (!q.empty())
        {
            int currentNodeID = q.front();
            q.pop();
            Node &currentNode = _nodeList[currentNodeID];

            // Traverse all edges of current node
            for (Edge &edge : _adjacencyList[currentNodeID])
            {
                Node &destinationNode = _nodeList[edge.destination];

                // If residual capacity of an edge is greater than 0 and the destination node has not been visited, then visit the node
                if (edge.capacity > edge.flow && destinationNode.level == -1)
                {
                    // Assign level to destination node
                    destinationNode.level = currentNode.level + 1;

                    // Enqueue destination node
                    q.push(edge.destination);
                }
            }
        }

        // If we reached sink in BFS starting from source, then return true, else false
        return _nodeList[_targetID].level != -1;
    }

    /**
     * @brief Function to send flow from source to target.
     *
     * This function uses Depth-First Search (DFS) to find a path from the current node to the target node.
     * It also updates the flow along the path found.
     *
     * @param currentNodeID The ID of the current node.
     * @param currentFlow The current flow.
     * @param visited The visited nodes.
     * @return The flow sent.
     */
    int _sendFlow(int currentNodeID, int currentFlow, vector<int> &visited)
    {
        // If the current node is the target node, return the current flow
        if (currentNodeID == _targetID)
        {
            return currentFlow;
        }

        // Traverse all edges of the current node
        for (int &currentID = visited[currentNodeID]; currentID < _adjacencyList[currentNodeID].size(); ++currentID)
        {
            Edge &edge = _adjacencyList[currentNodeID][currentID];

            // If the residual capacity of the edge is greater than 0 and the level of the destination node is level of current node + 1
            if (edge.capacity - edge.flow > 0 && _nodeList[edge.destination].level == _nodeList[currentNodeID].level + 1)
            {
                // Find the minimum of the current flow and the residual capacity of the edge
                currentFlow = min(currentFlow, edge.capacity - edge.flow);

                // Recursively call the function for the destination node
                int tempFlow = _sendFlow(edge.destination, currentFlow, visited);

                // If flow is sent from the destination node
                if (tempFlow > 0)
                {
                    // Add the flow to the current edge
                    edge.flow += tempFlow;

                    // Subtract the flow from the reverse edge
                    for (Edge &reverseEdge : _adjacencyList[edge.destination])
                    {
                        if (reverseEdge.destination == edge.source)
                        {
                            reverseEdge.flow -= tempFlow;
                            break;
                        }
                    }

                    // Return the flow sent
                    return tempFlow;
                }
            }
        }

        // If no path is found, return 0
        return 0;
    }

    /**
     * @brief Function to find nodes in the computed cut.
     *
     * This function uses Depth-First Search (DFS) to find all nodes that can be reached from the source node in the residual graph.
     * The visited nodes represent the set of nodes in the computed cut.
     *
     * @param visited A vector to keep track of visited nodes.
     * @param currentNodeID The ID of the current node.
     */
    void _findNodesInComputedCut(vector<bool> &visited, int currentNodeID)
    {
        // Mark the current node as visited
        visited[currentNodeID] = true;

        // Traverse all edges of the current node
        for (const Edge &edge : _adjacencyList[currentNodeID])
        {
            // If the destination node has not been visited and the residual capacity of the edge is greater than 0
            if (!visited[edge.destination] && edge.capacity - edge.flow > 0)
            {
                // Recursively call the function for the destination node
                _findNodesInComputedCut(visited, edge.destination);
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
    Graph(int vertices) : _numberOfVertices(vertices)
    {
        // Resize the adjacency list to hold the edges for each vertex
        _adjacencyList.resize(_numberOfVertices);

        // Reserve space for the node list
        _nodeList.reserve(_numberOfVertices);

        // Create a node for each vertex and add it to the node list
        for (int i = 0; i < vertices; ++i)
        {
            _nodeList.emplace_back(i);
        }
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
     * @param capacity The capacity of the edge.
     */
    void addEdge(int source, int destination, int capacity)
    {
        _adjacencyList[source - 1].push_back(Edge(source - 1, destination - 1, capacity));
    }

    /**
     * @brief Function to set source and target vertices.
     *
     * This function sets the source and target vertices for the flow.
     * The IDs are decremented by 1 because the vertices are 0-indexed in the adjacency list and node list.
     *
     * @param sourceID The ID of the source vertex.
     * @param targetID The ID of the target vertex.
     */
    void setSourceAndTarget(int sourceID, int targetID)
    {
        // Set the source vertex for the flow
        _sourceID = sourceID - 1;

        // Set the target vertex for the flow
        _targetID = targetID - 1;
    }

    /**
     * @brief Function to compute maximum flow using Dinic's algorithm.
     *
     * This function computes the maximum flow from the source to the target in the graph using Dinic's algorithm.
     * It first checks if the source and target are the same, in which case it prints an error message and returns.
     * Then it repeatedly finds a blocking flow in the layered residual graph and adds it to the maximum flow.
     * After that, it prints the maximum flow.
     * Finally, it finds and prints the nodes in the computed cut.
     */
    void computeMaxFlowDinic()
    {
        // If the source and target are the same, print an error message and return
        if (_sourceID == _targetID)
        {
            cout << "source and target are the same" << endl;
            return;
        }

        // Initialize the maximum flow as 0
        int maxFlow = 0;

        // While there is a path from the source to the target in the residual graph
        while (_BFS())
        {
            // Initialize the visited nodes
            vector<int> visited(_numberOfVertices, 0);

            // While there is a blocking flow in the layered residual graph
            while (int flow = _sendFlow(_sourceID, 10000, visited))
                // Add the blocking flow to the maximum flow
                maxFlow += flow;
        }

        // Print the maximum flow
        cout << maxFlow << endl;

        // Initialize the visited nodes
        vector<bool> visited(_numberOfVertices, false);

        // Find the nodes in the computed cut
        _findNodesInComputedCut(visited, _sourceID);

        // Print the nodes in the computed cut
        for (int i = 0; i < _numberOfVertices; ++i)
            if (visited[i])
                cout << i + 1 << " ";
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

    // Read the number of nodes and edges from standard input
    int numberOfNodes, numberOfEdges;
    cin >> numberOfNodes >> numberOfEdges;

    // Create a graph with the read number of nodes
    Graph G(numberOfNodes);

    // Read the edges from standard input and add them to the graph
    for (int line = 0; line < numberOfEdges * 2; ++line)
    {
        int source, destination, weight;
        cin >> source >> destination >> weight;
        G.addEdge(source, destination, weight);
    }

    // Read the source and target from standard input and set them in the graph
    int source, target;
    cin >> source >> target;
    G.setSourceAndTarget(source, target);

    // Compute the maximum flow from the source to the target using Dinic's algorithm and print the maximum flow and the nodes in the computed cut
    G.computeMaxFlowDinic();

    // Return 0 on successful execution
    return 0;
}