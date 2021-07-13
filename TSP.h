#include <math.h>       /* pow */
#include <iostream>
#include <vector>
#include <unordered_map>
#include<algorithm>
#include <numeric>
#include <chrono>


using namespace std;


class Index
{
public:
    int curVrtx;
    vector<int> vertexSet;
    Index();
    Index(const int& x, const vector<int>& z);
    void setIndex(int x, vector<int> z);
    bool operator==(const Index& other) const;

};


class Hasher {
public:
    int operator()(const Index& other) const;

};


vector<vector<int>> generateAllSubsets(vector<int>& values);
int g(vector<int>& set, int& prevVrtx, unordered_map<Index, int, Hasher>& D);
void GenerateCleaningPath(const vector <vector<int>>& env_costs);




