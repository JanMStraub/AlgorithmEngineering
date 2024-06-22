#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

struct Edge
{
    int u;
    int v;
    int color = 0;
};

int main()
{
    int numNodes, numEdges;
    cin >> numNodes >> numEdges;
    vector<vector<Edge>> neighbours(numNodes);
    vector<vector<int>> usedColors(numNodes);
    int maxColors = 0;

    for (int i = 0; i < numEdges * 2; i++)
    {
        Edge e;
        int u, v, w;
        cin >> u >> v >> w;
        e.u = u - 1;
        e.v = v - 1;
        if (e.u < e.v)
        {
            neighbours[e.u].push_back(e);
        }
    }

    for (int i = 0; i < neighbours.size(); i++)
    {
        int color = 1;
        for (int j = 0; j < neighbours[i].size(); j++)
        {
            bool colorUsed = true;
            while (colorUsed)
            {
                colorUsed = false;
                if ((find(usedColors[i].begin(), usedColors[i].end(), color) != usedColors[i].end()) || (find(usedColors[neighbours[i][j].v].begin(), usedColors[neighbours[i][j].v].end(), color) != usedColors[neighbours[i][j].v].end()))
                {
                    color++;
                    if (maxColors < color)
                    {
                        maxColors = color;
                    }
                    colorUsed = true;
                }
            }
            neighbours[i][j].color = color;
            usedColors[i].push_back(color);
            usedColors[neighbours[i][j].v].push_back(color);
        }
    }

    cout << maxColors << endl;
    for (int i = 0; i < numNodes; i++)
    {
        for (int j = 0; j < neighbours[i].size(); j++)
        {
            cout << neighbours[i][j].u + 1 << " " << neighbours[i][j].v + 1 << " " << neighbours[i][j].color << endl;
        }
    }

    return 0;
}