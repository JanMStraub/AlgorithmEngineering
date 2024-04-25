#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

int main()
{

    int n, m;
    cin >> n >> m;

    vector<int> degree(n, 0);
    vector<int> weightedDegree(n, 0);
    int totalWeight = 0;

    // Loop through each edge from the input
    for (int i = 0; i < (m * 2); i++)
    {
        int source, destination, weight;
        cin >> source >> destination >> weight;
        source -= 1;                      // Adjust fot 0-based indexing
        degree[source] += 1;              // Increment the degree of the source node
        weightedDegree[source] += weight; // Add the weight to the weighted degree

        // Get total edge weight
        totalWeight += weight;
    }

    // Get the maximum degree by selecting the largest degree element
    int maxDegree = *max_element(degree.begin(), degree.end());

    // Get the minimum degree by selecting the smalles degree element
    int minDegree = *min_element(degree.begin(), degree.end());

    // Get the maximum weighted degree by selecting the largest weighted degree element
    int maxWeightedDegree = *max_element(weightedDegree.begin(), weightedDegree.end());

    // Get the minimum weighted degree by selecting the largest smalles degree element
    int minWeightedDegree = *min_element(weightedDegree.begin(), weightedDegree.end());

    cout << maxDegree << endl;
    cout << minDegree << endl;
    cout << maxWeightedDegree << endl;
    cout << minWeightedDegree << endl;
    cout << totalWeight / 2 << endl; // Must divide by 2 because we count the weight of both directions

    return 0;
}