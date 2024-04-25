#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;
#include <fstream>

int main()
{
    ifstream inputFile("/Users/jan/Documents/code/AlgorithmEngineering/example2.txt");
    if (!inputFile.is_open())
    {
        cerr << "Failed to open the input file." << endl;
        return 1;
    }

    int n, m, k;
    inputFile >> n >> m >> k;

    vector<vector<pair<int, int>>> graph(n);

    for (int i = 0; i < m; i++)
    {
        int source, destination, weight;
        inputFile >> source >> destination >> weight;
        source -= 1;
        destination -= 1;
        graph[source].push_back(make_pair(destination, weight));
        graph[destination].push_back(make_pair(source, weight));
    }

    vector<int> partition(n);
    for (int i = 0; i < n; ++i)
    {
        inputFile >> partition[i];
    }

    int edge_cut = 0;
    int weighted_edge_cut = 0;
    vector<int> block_sizes(k, 0);

    double balance = *max_element(block_sizes.begin(), block_sizes.end()) / static_cast<double>(n / k);

    cout << "Edge-cut: " << edge_cut << endl;
    cout << "Weighted edge-cut: " << weighted_edge_cut << endl;
    cout << "Balance: " << balance << endl;

    inputFile.close();

    return 0;
}

/*
4
14
1.14286
*/

/*
The objective of this challenge is to compute fundamental metrics of graph partitioning. First, you should read the input (a weighted adjacency list and a partition) and represent it in some data structure. Let n be the number of vertices of the graph and k be the number of blocks of the partition. Then, you should correctly output the following partitioning metrics: (1) edge-cut: number of edges crossing blocks; (2) weighted edge-cut: total weight of edges crossing blocks; and (3) balance: amount of nodes in the heaviest block divided by (n/k).

Input Format

The first line of the input has exactly 3 integers, n, m and k, which respectively represent the amounts of vertices and edges in the input graph and the amount of blocks in the partition. Each of the following 2m lines contains 3 integers, respectively representing (1) source node, (2) destination node, and (3) arc weight. Each of the following n lines contains an integer in the interval [0,k-1] representing the block of the respective node. In other words, the ith of these lines contains the block of node i. Observation 1: Recall that the input is an undirected graph. Hence, each undirected edge is represented by two directed arcs with the same weight and opposite endpoints. Observation 2: Nodes indices are the consecutive integer numbers {1,...,n} and the nodes are unweighted.

Constraints

The input contains no self edges and no multiple edges between the same pair of nodes. Weights of edges are strictly positive integers.

Output Format

The output should contain exactly 3 lines. The first line should contain an integer number representing the edge-cut. The second line should contain an integer number representing the weighted edge-cut The third line should contain a real number representing the balance of the partition.
*/