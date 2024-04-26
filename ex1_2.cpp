#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
using namespace std;

int main()
{
    // get graph metrics
    int n, m, k;
    cin >> n >> m >> k;

    // create data structures: 2D vector for edges, normal vector for partition of each node
    vector<vector<int>> edges(n, vector<int>(n, 0));
    vector<int> partitions(n);

    int edge_cut = 0;
    int weighted_edge_cut = 0;

    // get edge data
    for (int i = 0; i < m * 2; i++)
    {
        int source, destination, weight;
        cin >> source >> destination >> weight;
        edges[source - 1][destination - 1] = weight;
    }

    // get node partition information
    for (int i = 0; i < n; i++)
    {
        cin >> partitions[i];
    }

    // compare each node with each other
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (partitions[i] != partitions[j]) // find nodes in different partitions
            {
                if (edges[i][j] != 0)
                { // check if nodes are connected
                    edge_cut += 1;
                }
                weighted_edge_cut += edges[i][j];
            }
        }
    }

    // search for biggest partition
    vector<int> partSizes(k, 0);

    for (int i = 0; i < n; i++)
    {
        partSizes[partitions[i]] += 1;
    }

    // cut all weights in half, because each edge was added twice #efficiency
    edge_cut /= 2;
    weighted_edge_cut /= 2;

    double max_partition_size = *max_element(partSizes.begin(), partSizes.end());

    double ideal_partition_size = (float)n / (float)k;

    double balance = max_partition_size / ideal_partition_size;

    cout << edge_cut << endl;
    cout << weighted_edge_cut << endl;
    cout << balance << endl;

    return 0;
}
