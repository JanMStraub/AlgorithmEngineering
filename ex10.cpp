#include <vector>
#include <iostream>
#include <queue>
#include <fstream>

using namespace std;

class Graph
{
private:
    int _numberOfVertices; // Number of vertices in the graph
    vector<vector<int> > _adjacencyList; // Adjacency list to represent the graph

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

            for (const int &edge : _adjacencyList[currentNode])
            {
                int neighbor = edge;
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
    Graph(int vertices, int edges) : _numberOfVertices(vertices)
    {
        _adjacencyList.resize(_numberOfVertices);
    }

    void addEdge(int source, int destination)
    {
        _adjacencyList[source - 1].push_back(destination - 1);
    }

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


int main()
{
    // Improve the performance of cin
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string path = "/Users/jan/Documents/code/ae/";
    ifstream file(path + "example10.txt");
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
        G.addEdge(source, destination);
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