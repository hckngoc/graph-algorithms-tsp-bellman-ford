#include "tsm.h"
using namespace std;

const int int_max = 2147483647;

int getIndex(char c, const vector<char>& vertexSet) {
    for (int i = 0; i < vertexSet.size(); ++i)
        if (vertexSet[i] == c) return i;
    return -1;
}

// Recursive tour builder with sorted neighbors by increasing weight
bool recursiveBuildTour(int currentVertex, const vector<vector<int>>& adj,
                        vector<bool>& visited, vector<int>& tour, int startVertex) {
    int n = adj.size();

    if (tour.size() == n) {
        return adj[currentVertex][startVertex] != -1;
    }

    // Sort next candidates by increasing edge weight from currentVertex
    vector<pair<int, int>> candidates;
    for (int next = 0; next < n; ++next) {
        if (!visited[next] && adj[currentVertex][next] != -1) {
            candidates.push_back({adj[currentVertex][next], next});
        }
    }
    sort(candidates.begin(), candidates.end());

    for (const auto& p : candidates) {
        int next = p.second;
        visited[next] = true;
        tour.push_back(next);
        if (recursiveBuildTour(next, adj, visited, tour, startVertex)) return true;
        tour.pop_back();
        visited[next] = false;
    }

    return false;
}

string Traveling(int edgeList[][3], int numEdges, char start_vertex) {
    vector<char> vertexSet;

    for (int i = 0; i < numEdges; ++i) {
        char u = (char)edgeList[i][0];
        char v = (char)edgeList[i][1];
        if (find(vertexSet.begin(), vertexSet.end(), u) == vertexSet.end())
            vertexSet.push_back(u);
        if (find(vertexSet.begin(), vertexSet.end(), v) == vertexSet.end())
            vertexSet.push_back(v);
    }

    sort(vertexSet.begin(), vertexSet.end());
    int n = vertexSet.size();

    int startIndex = getIndex(start_vertex, vertexSet);

    vector<vector<int>> adj(n, vector<int>(n, -1));
    for (int i = 0; i < numEdges; ++i) {
        int u = getIndex((char)edgeList[i][0], vertexSet);
        int v = getIndex((char)edgeList[i][1], vertexSet);
        adj[u][v] = edgeList[i][2];
    }

    if (n <= 20) {
        int totalMask = (1 << n);
        vector<vector<int>> dp(totalMask, vector<int>(n, int_max));
        vector<vector<int>> parent(totalMask, vector<int>(n, -1));
        dp[1 << startIndex][startIndex] = 0;

        for (int mask = 0; mask < totalMask; ++mask) {
            for (int u = 0; u < n; ++u) {
                if (!(mask & (1 << u)) || dp[mask][u] == int_max) continue;
                for (int v = 0; v < n; ++v) {
                    if ((mask & (1 << v)) || adj[u][v] == -1) continue;
                    int nextMask = mask | (1 << v);
                    int cost = dp[mask][u] + adj[u][v];
                    if (cost < dp[nextMask][v]) {
                        dp[nextMask][v] = cost;
                        parent[nextMask][v] = u;
                    }
                }
            }
        }

        int minCost = int_max, last = -1;
        for (int u = 0; u < n; ++u) {
            if (u != startIndex && adj[u][startIndex] != -1 && dp[totalMask - 1][u] != int_max) {
                int cost = dp[totalMask - 1][u] + adj[u][startIndex];
                if (cost < minCost) {
                    minCost = cost;
                    last = u;
                }
            }
        }

        if (minCost == int_max) return "";
        vector<int> path;
        int mask = totalMask - 1;
        while (last != -1) {
            path.push_back(last);
            int temp = parent[mask][last];
            mask ^= (1 << last);
            last = temp;
        }
        reverse(path.begin(), path.end());

        string res = "";
        res += vertexSet[startIndex];
        for (int v : path)
            if (v != startIndex) res += " " + string(1, vertexSet[v]);
        res += " " + string(1, vertexSet[startIndex]);
        return res;
    }

    vector<bool> visited(n, false);
    vector<int> tour;
    visited[startIndex] = true;
    tour.push_back(startIndex);

    bool found = recursiveBuildTour(startIndex, adj, visited, tour, startIndex);

    string res = "";
    for (int i = 0; i < tour.size(); ++i) {
        res += vertexSet[tour[i]];
        if (i != tour.size() - 1) res += " ";
    }
    res += " " + string(1, vertexSet[startIndex]);
    return res;
}
