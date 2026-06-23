#include "bellman.h"

//Exercise 1: Bellman-Ford Calculation
const int int_max = 2147483647;

void BF(int edgeList[][3], int numEdges, char start_vertex, int BFValue[], int BFPrev[]) {
    vector<char> vertexList;

    // Collect all vertices from edge list
    for (int i = 0; i < numEdges; i++) {
        char u = (char)edgeList[i][0];
        char v = (char)edgeList[i][1];
        if (u == v) continue;
        bool foundU = false, foundV = false;
        for (int j = 0; j < vertexList.size(); j++) {
            if (vertexList[j] == u) foundU = true;
            if (vertexList[j] == v) foundV = true;
        }
        if (!foundU) {
            vertexList.push_back(u);
        }
        if (!foundV) {
            vertexList.push_back(v);
        }
    }

    int vertexCount = vertexList.size();

    // Sort vertices by ASCII value as required
    for (int i = 0; i < vertexCount - 1; i++) {
        for (int j = i + 1; j < vertexCount; j++) {
            if (vertexList[i] > vertexList[j]) {
                char temp = vertexList[i];
                vertexList[i] = vertexList[j];
                vertexList[j] = temp;
            }
        }
    }

    // Create graph
    int** Graph = new int*[vertexCount];
    for (int i = 0; i < vertexCount; i++) {
        Graph[i] = new int[vertexCount];
        for (int j = 0; j < vertexCount; j++) {
            Graph[i][j] = -1;
        }
    }
    for (int i = 0; i < numEdges; i++) {
        char u = (char)edgeList[i][0];
        char v = (char)edgeList[i][1];
        int w = edgeList[i][2];

        int uIndex = -1, vIndex = -1;
        for (int j = 0; j < vertexCount; j++) {
            if (vertexList[j] == u) uIndex = j;
            if (vertexList[j] == v) vIndex = j;
        }
        
        if (w > 0) Graph[uIndex][vIndex] = w;
    }

    int startIndex = -1;
    for (int i = 0; i < vertexCount; i++) {
        if (vertexList[i] == start_vertex) {
            startIndex = i;
            break;
        }
    }

    // Create temp array for BFValue in last step
    int BFValueTemp[vertexCount];
    for (int i = 0; i < vertexCount; i++) {
        if (BFValue[i] == -1) {
            BFValueTemp[i] = int_max;
            continue;
        }
        BFValueTemp[i] = BFValue[i];
    }
    if (BFValue[startIndex] == -1) {
        BFValueTemp[startIndex] = 0;
        BFValue[startIndex] = 0;
    }

    // Next relaxation step
    for (int i = 0; i < vertexCount; i++) {
        for (int j = 0; j < vertexCount; j++) {
            if (BFValueTemp[j] != int_max && Graph[j][i] != -1 && BFValueTemp[j] + Graph[j][i] < BFValueTemp[i]) {
                if (BFValueTemp[j] + Graph[j][i] < BFValue[i] || BFValue[i] == -1) {
                    BFValue[i] = BFValueTemp[j] + Graph[j][i];
                    BFPrev[i] = j;
                }
            }
        }
    }
    
    // Delete Graph
    for (int i = 0; i < vertexCount; i++) {
        delete[] Graph[i];
    }
    delete[] Graph;
}

//Exercise 2: Bellman-Ford Path
string BF_Path(int edgeList[][3], int numEdges, char start_vertex, char goal_vertex) {
    vector<char> vertexList;

    // Collect all vertices from edge list
    for (int i = 0; i < numEdges; i++) {
        char u = (char)edgeList[i][0];
        char v = (char)edgeList[i][1];
        if (u == v) continue;
        bool foundU = false, foundV = false;
        for (int j = 0; j < vertexList.size(); j++) {
            if (vertexList[j] == u) foundU = true;
            if (vertexList[j] == v) foundV = true;
        }
        if (!foundU) {
            vertexList.push_back(u);
        }
        if (!foundV) {
            vertexList.push_back(v);
        }
    }

    int vertexCount = vertexList.size();

    // Sort vertices by ASCII value
    for (int i = 0; i < vertexCount - 1; i++) {
        for (int j = i + 1; j < vertexCount; j++) {
            if (vertexList[i] > vertexList[j]) {
                char temp = vertexList[i];
                vertexList[i] = vertexList[j];
                vertexList[j] = temp;
            }
        }
    }

    int startIndex = -1, goalIndex = -1;
    for (int i = 0; i < vertexCount; i++) {
        if (vertexList[i] == start_vertex) startIndex = i;
        if (vertexList[i] == goal_vertex) goalIndex = i;
    }

    // Handle case where start and goal are the same
    if (start_vertex == goal_vertex) {
        string result = "";
        result += start_vertex;
        return result;
    }

    int BFValue[vertexCount], BFPrev[vertexCount];
    for (int i = 0; i < vertexCount; i++) {
        BFValue[i] = -1;
        BFPrev[i] = -1;
    }
    
    // Do BellMan Algorithm
    for (int i = 0; i < vertexCount; i++) {
        BF(edgeList, numEdges, start_vertex, BFValue, BFPrev);
    }

    // Reconstruct path
    int pathIndex[vertexCount * 2], pathLen = 0;
    int current = goalIndex;
    while (current != -1) {
        pathIndex[pathLen++] = current;
        if (current == startIndex) break;
        current = BFPrev[current];
    }

    // Build result string
    string result = "";
    for (int i = pathLen - 1; i >= 0; i--) {
        result += vertexList[pathIndex[i]];
        if (i != 0) result += " ";
    }

    return result;
}
