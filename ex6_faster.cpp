#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <climits>
#include <fstream>

using namespace std;

struct Edge
{
    int source, destination, weight;
};

void disassembleSubtree(int node, const vector<int> &previousNode, vector<bool> &visited, vector<int> &subtree)
{
    if (visited[node])
        return;
    visited[node] = true;
    subtree.push_back(node);
    if (previousNode[node] != -1)
    {
        disassembleSubtree(previousNode[node], previousNode, visited, subtree);
    }
}

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

    int numNodes, numEdges;
    file >> numNodes >> numEdges;

    vector<Edge> edges(numEdges);
    for (int i = 0; i < numEdges; i++)
    {
        int source, destination, weight;
        file >> source >> destination >> weight;
        edges[i] = (Edge){source, destination, weight};
    }

    // Initialize vectors
    vector<int> cycleNodes;
    cycleNodes.reserve(numNodes);
    vector<int> distances(numNodes, INT_MAX);
    distances.reserve(numNodes);
    vector<int> previousNode(numNodes, -1);
    int lastNodeInCycle = -1;

    for (int i = 0; i < numNodes; ++i)
    {
        lastNodeInCycle = -1;
        for (const Edge &edge : edges)
        {
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

    if (lastNodeInCycle == -1)
    {
        cout << "No negative cycle found.\n";
    }
    else
    {
        vector<bool> visited(numNodes, false);
        vector<int> subtree;

        disassembleSubtree(lastNodeInCycle, previousNode, visited, subtree);

        reverse(subtree.begin(), subtree.end());

        // Find the cycle in the subtree
        vector<int> cycle;
        vector<bool> inCycle(numNodes, false);
        for (int i = 0; i < subtree.size(); ++i) {
            int node = subtree[i];
            if (inCycle[node]) continue;
            cycle.push_back(node);
            inCycle[node] = true;
            if (node == lastNodeInCycle && cycle.size() > 1)
                break;
        }

        cout << cycle.size() << '\n';
        for (int node : cycle)
            cout << node << ' ';
        cout << cycle[0] << endl;
    }
    return 0;
}