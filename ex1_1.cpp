#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

int main() {
    
    int n, m;
    cin >> n >> m;

    vector<int> degree(n, 0);
    vector<int> weightedDegree(n, 0);
    int totalWeight = 0;

    for (int i = 0; i < (m * 2); i++) {
        int source, destination, weight;
        cin >> source >> destination >> weight;
        source -= 1;
        degree[source] += 1;
        weightedDegree[source] += weight;

        // total edge weight
        totalWeight += weight;
        
    }

    // maximum degree
    int maxDegree = *max_element(degree.begin(), degree.end());

    // minimum degree 
    int minDegree = *min_element(degree.begin(), degree.end());

    // maximum weighted degree
    int maxWeightedDegree = *max_element(weightedDegree.begin(), weightedDegree.end());

    // minimum weighted degree
    int minWeightedDegree = *min_element(weightedDegree.begin(), weightedDegree.end());

    cout << maxDegree << endl;
    cout << minDegree << endl;
    cout << maxWeightedDegree << endl;
    cout << minWeightedDegree << endl;
    cout << totalWeight / 2 << endl;

    return 0;
}