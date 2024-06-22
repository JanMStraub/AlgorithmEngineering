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

    /**
     * @brief Constructor for Edge.
     * @param src The source vertex of the edge.
     * @param dest The destination vertex of the edge.
     * @param w The weight of the edge.
     */
    Edge(int src, int dest, int w) : source(src), destination(dest), weight(w) {}
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
    vector<vector<Edge> > _adjacencyList; // Adjacency list to represent the graph

    /**
     * @brief Function to perform BFS and return the distances from the start node.
     * @param startNode The starting node for BFS.
     * @return A pair of maximum distance and the farthest node from the start.
     */
    pair <int, int> _BFS(int startNode)
    {
        vector<int> distances(_numberOfVertices, -1);
        queue<int> q;
        q.push(startNode);
        distances[startNode] = 0;
        int maxDistance = 0;
        int farthestNode = startNode;

        while (!q.empty())
        {
            int currentNode = q.front();
            q.pop();

            for (const Edge &edge : _adjacencyList[currentNode])
            {
                int neighbor = edge.destination;
                if (distances[neighbor] == -1)
                {
                    distances[neighbor] = distances[currentNode] + 1;
                    q.push(neighbor);

                    if (distances[neighbor] > maxDistance)
                    {
                        maxDistance = distances[neighbor];
                        farthestNode = neighbor;
                    }
                }
            }
        }
        return make_pair(maxDistance, farthestNode);
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
    Graph(int vertices, int edges) : _numberOfVertices(vertices)
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

    /**
     * @brief Function to find the diameter of the graph.
     */
    void findDiameter()
    {
        // Start BFS from an arbitrary node (0)
        pair <int, int> currentResult = _BFS(0);
        int maxDistance = currentResult.first;
        int node1 = 0;
        int node2 = currentResult.second;

        for (int i = 1; i < _numberOfVertices; ++i)
        {
            pair <int, int> newCurrentResult = _BFS(i);

            if (newCurrentResult.first > maxDistance) {
                maxDistance = newCurrentResult.first;
                node1 = i;
                node2 = newCurrentResult.second;
            }
        }

        cout << maxDistance << "\n";
        cout << node1 + 1 << " " << node2 + 1 << "\n";
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

    G.findDiameter();

    cout << '\n';

    // Return 0 on successful execution
    return 0;
}

/*
unweighted diameter of the graph: 3
pair of nodes whose unweighted distance is equal to the unweighted diameter of the graph: 1 6
*/