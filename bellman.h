#ifndef _BELLMAN_H_
#define _BELLMAN_H_

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

void BF(int edgeList[][3], int numEdges, char start_vertex, int BFValue[], int BFPrev[]);
string BF_Path(int edgeList[][3], int numEdges, char start_vertex, char goal_vertex);

#endif //_BELLMAN_H_
