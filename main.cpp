#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <random>
#include "bellman.h"
#include "tsm.h"
using namespace std;
#define MAX 1000

//Function to print an edge
void printedge(int[]);
//Function to generate a random edges list
int edgeListGen(int[][3], int&, int, int);

//main Function
int main()
{
    //Function name for checking:
    enum Func_check {BF, BF_Path, Traveling, none};
    
    bool randGen = true;
    int edgeList[MAX][3];
    int numVertices = 5;
    int initlimit = 30;
    int numEdges = 0;

    if(!randGen){
        //Read from file
        ifstream fin("EdgeList.txt");
        for(int i = 0; i < numEdges; i++){
            fin >> edgeList[i][0] >> edgeList[i][1] >> edgeList[i][2];
        }
        fin.close();
    }
    else{
        //Generate a random edgelist: strongly connected directed graph
        if(edgeListGen(edgeList, numEdges, numVertices, initlimit) < 0) return -1;
    }
    
    //Check the chosen function:
    Func_check func = Traveling;

    char start_vertices = edgeList[0][0];
    char end_vertices = edgeList[numEdges - 1][0];
    int* BFValue = new int[numVertices];
    int* BFPrev = new int[numVertices];
    for (int i = 0; i < numVertices; i++) {
        BFValue[i] = -1;
        BFPrev[i] = -1;
    }

    switch(func){
        case BF:
            cout << "=== Bellman - Ford Calculation ===" << endl;
            for (int i = 0; i < numVertices; i++) {
                cout << "Step " << i + 1 << ":" << endl;
                ::BF(edgeList, numEdges, start_vertices, BFValue, BFPrev);

                for (int i = 0; i < numVertices; i++) {
                    cout << BFValue[i] << '\t';
                }
                cout << endl;

                for (int i = 0; i < numVertices; i++) {
                    cout << BFPrev[i] << '\t';
                }
                cout << endl;
            }
            break;
        case BF_Path:
            cout << "=== Bellman - Ford Path ===" << endl;
            cout << ::BF_Path(edgeList, numEdges, start_vertices, end_vertices);
            break;
        case Traveling:
            cout << "=== Travelling salesman path Calculation ===" << endl;
            cout << ::Traveling(edgeList, numEdges, start_vertices);
            break;
        default: cout << "No such case";
    }
    delete[] BFValue;
    delete[] BFPrev;
    return 0;
}

//support function definition
void printedge(int edge[]){
    cout<<char(edge[0])<<char(edge[1])<<","<<edge[2]<<endl;
}

int edgeListGen(int edgeList[][3], int& numEdges, int numVertices, int initlimit) {
    if (numVertices < 2) {
        std::cout << "Number of vertices must be at least 2.\n";
        return -1;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, std::max(1, initlimit));

    // Generate random ASCII values for vertex names
    std::vector<int> verName;
    for (int i = 33; i <= 126; ++i) verName.push_back(i); // printable characters
    std::shuffle(verName.begin(), verName.end(), gen);

    std::vector<int> verList(numVertices);
    for (int i = 0; i < numVertices; ++i) verList[i] = verName[i];

    std::vector<std::pair<int, int>> addedEdges;
    int edgeCount = 0;

    // Step 1: Create a simple cycle (A->B->C->...->A)
    for (int i = 0; i < numVertices; ++i) {
        int from = verList[i];
        int to = verList[(i + 1) % numVertices]; // wrap around
        int weight = dist(gen);

        edgeList[edgeCount][0] = from;
        edgeList[edgeCount][1] = to;
        edgeList[edgeCount][2] = weight;
        edgeCount++;

        addedEdges.push_back({from, to});
    }

    // Step 2: Add extra random edges (without duplication)
    for (int i = 0; i < numVertices; ++i) {
        for (int j = 0; j < numVertices; ++j) {
            if (i == j) continue;

            int from = verList[i];
            int to = verList[j];

            // Check if edge already exists
            bool exists = false;
            for (int k = 0; k < addedEdges.size(); ++k) {
                if (addedEdges[k].first == from && addedEdges[k].second == to) {
                    exists = true;
                    break;
                }
            }
            if (exists) continue;

            // Randomly decide whether to add this edge
            if (dist(gen) % 2 == 0) continue;

            edgeList[edgeCount][0] = from;
            edgeList[edgeCount][1] = to;
            edgeList[edgeCount][2] = dist(gen);
            edgeCount++;

            addedEdges.push_back({from, to});
        }
    }

    numEdges = edgeCount;
    return 1;
}




