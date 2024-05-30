#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <climits>
#include <fstream>

using namespace std;

// Define the structure for an edge in the graph
struct Edge
{
    int source, destination, weight;
};

int main()
{
    // Improve the performance of cin
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string path = "/Users/jan/Documents/code/AlgorithmEngineering/";
    ifstream file(path + "example6.txt");
    if (!file.is_open())
    {
        cerr << "Failed to open the file." << endl;
        return 1;
    }

    // Read the number of nodes and edges
    int numNodes, numEdges;
    file >> numNodes >> numEdges;

    // Create a vector to store all edges
    vector<Edge> edges(numEdges);
    for (int i = 0; i < numEdges; i++)
    {
        // Read the source, destination and weight of each edge
        int source, destination, weight;
        file >> source >> destination >> weight;
        edges[i] = (Edge){source - 1, destination - 1, weight};
    }

    // Initialize vectors for cycle nodes, distances and previous nodes
    vector<int> cycleNodes;
    cycleNodes.reserve(numNodes);
    vector<int> distances(numNodes, INT_MAX);
    distances.reserve(numNodes);
    distances[numNodes-1] = INT_MAX-1;
    vector<int> previousNode(numNodes, -1);
    int lastNodeInCycle = -1;

    // Run the Bellman-Ford algorithm
    for (int i = 0; i < numNodes; ++i)
    {
        lastNodeInCycle = -1;
        for (const Edge &edge : edges)
        {
            // Relax the edges
            if (distances[edge.source] < INT_MAX)
            {
                if (distances[edge.source] + edge.weight < distances[edge.destination])
                {
                    distances[edge.destination] = max(-INT_MAX, distances[edge.source] + edge.weight);
                    previousNode[edge.destination] = edge.source;
                    lastNodeInCycle = edge.destination;
                }
            }
        }
    }

    // Check if a negative cycle was found
    if (lastNodeInCycle == -1)
    {
        cout << "No negative cycle found.\n";
    }
    else
    {
        // Find the nodes in the cycle
        for (int i = 0; i < numNodes; ++i)
        {
            lastNodeInCycle = previousNode[lastNodeInCycle];
        }
        
        vector<int> cycle;
        cycle.reserve(numNodes);
        for (int node = lastNodeInCycle;; node = previousNode[node])
        {
            cycle.push_back(node);
            if (node == lastNodeInCycle && cycle.size() > 1)
                break;
        }
        // Reverse the cycle to get it in the correct order
        reverse(cycle.begin(), cycle.end());

        // Print the size of the cycle and the nodes in the cycle
        cout << cycle.size() - 1 << '\n';
        for (int node : cycle)
            cout << node + 1 << " ";
        cout << '\n';
    }
    return 0;
}