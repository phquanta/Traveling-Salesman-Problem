#include <math.h>       /* pow */
#include <iostream>
#include <vector>
#include <unordered_map>
#include<algorithm>
#include <numeric>
#include <chrono>

using namespace std;



// index class for mapping tuple (Vertex, {Set}) to cost
class Index
{



public:
    int curVrtx;
    vector<int> vertexSet;

    // default constructor;
    Index() : curVrtx(0), vertexSet({}) { }

    // copy constructor
    Index(const int& x, const vector<int>& z) :curVrtx(x), vertexSet(z) { }

    void setIndex(int x, vector<int> z)
    {
        curVrtx = x;
        vertexSet = z;
    }

    // overriding(adding) operator for hashing purposes as unordered_map<K,V> requires custom hash funcion and definition for == operator
    bool operator==(const Index& other) const
    {
        return (curVrtx == other.curVrtx
            && vertexSet == other.vertexSet && vertexSet.size() == other.vertexSet.size()
            && &other != NULL);
    }

};


// Hashing function for Index object - hash collision is possible  - did not check for hash collisions
class Hasher {
public:

    int operator()(const Index& other) const {
        int seed = other.vertexSet.size();
        for (auto& i : other.vertexSet) {
            seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed + other.curVrtx * 31;


    }

};



vector<vector<int>> generateAllSubsets(vector<int>& values)
{
    int len = values.size();
    // this is the array of values


    // all subsets will be in vector "subsets"
    vector<vector<int>> subsets;
    for (int i = 0; i < pow(2, len); i++) {
        int t = i;
        vector<int> v;
        for (int j = 0; j < len; j++) {
            if (t & 1)
                v.push_back(values[j]);
            t >>= 1;
        }
        subsets.push_back(v);
    }

    // sort in increasing size 
    int i = 1;
    while (i < subsets.size())
    {

        if (subsets[i].size() < subsets[i - 1].size()) {
            vector<int> tmp = subsets[i - 1];
            subsets[i - 1] = subsets[i];
            subsets[i] = tmp;
            i = i - 1;
        }
        else {
            i++;
        }
    }


    return subsets;
};


int g(vector<int>& set, int& prevVrtx, unordered_map<Index, int, Hasher>& D) {
    vector<int> tmpHolder(set);
    tmpHolder.erase(remove(tmpHolder.begin(), tmpHolder.end(), prevVrtx), tmpHolder.end());
    Index index(prevVrtx, tmpHolder);
    int cost = D[index];
    return cost;
};

void GenerateCleaningPath(const vector <vector<int>>& env_costs) 
{
    
    
    
    //starting vertex - can be 0 ... size(vertices) - Make sure it starts from 0 not 1, although in output everything incremented by 1
    int strt = 0; 
    
    int I = 999999; // cost for transitions that do not exist

    
    unordered_map<Index, int, Hasher> D;  // keep track of <vertex,{set of vertices}>  -> cost 
    unordered_map<Index, int, Hasher> parent; // keep track of parents to unwind path



    


    vector<int> vertices;

    vertices.push_back(strt);


    //create vertices array
    for (int i = 0; i < env_costs.size(); i++)
        if (i != strt)
            vertices.push_back(i);


    //create and subtract starting point from matroces
    vector<int> verticesSubtractStart(vertices);
    verticesSubtractStart.erase(remove(verticesSubtractStart.begin(), verticesSubtractStart.end(), strt), verticesSubtractStart.end());
    vector<vector<int>> allSubsets = generateAllSubsets(verticesSubtractStart);



    // some initialization stuff
    Index index = Index();
    int minCost = I;
    int minPrevVrtx = -1;


    // main loop of  Held-Karp algorithm

    for (auto set : allSubsets) {
        for (auto curVrtx : vertices) {

            if (find(set.begin(), set.end(), curVrtx) != set.end()) {
                continue;
            }


            index.setIndex(curVrtx, set);
            minCost = I;

            for (auto prevVrtx : set) {
                int cost = env_costs[prevVrtx][curVrtx] + g(set, prevVrtx, D);
                if (cost < minCost) {
                    minCost = cost;
                    parent[index] = prevVrtx;

                }
            }
            if (set.empty()) {
                minCost = env_costs[strt][curVrtx];

            }
            D[index] = minCost;
        }
    }


    // this part here does print and unwind travel path
    vector<int> followSet = allSubsets.back();
    vector<int> path;
    minPrevVrtx = strt;

    
    //keep iterating until complete unwind
    while (!followSet.empty())
    {
        minPrevVrtx = parent[index];
        followSet.erase(remove(followSet.begin(), followSet.end(), minPrevVrtx), followSet.end());
        index.setIndex(minPrevVrtx, followSet);
        path.push_back(minPrevVrtx + 1);
    }

    cout << "##########################################################\n";
    cout << "\n";
    reverse(path.begin(), path.end());
    

    cout << "Path: " << strt + 1 << " -> ";

    for (auto elem : path)
        cout << elem << " -> ";
    cout << strt + 1 << "\n";
    cout << "\n";

    path.insert(path.begin(), strt + 1);
    path.push_back(strt + 1);




    // Sanity check
    int summ = 0;
    for (int i = 0; i < path.size() - 1; i++)
    {
        summ += env_costs[path[i] - 1][path[i + 1] - 1];
    }
    index.setIndex(strt, allSubsets.back());
    cout << "Algorithm Computed cost:     " << D[index] << "\n";
    cout << "Path Cumulative cost(Sanity Check):    " << summ << "\n";






}






int main()
{
        
    auto timeStart = chrono::high_resolution_clock::now();
    int I = 999999; // cost for transitions that do not exist

    vector<vector<int>> env_costs = {
    { I, 3, 5, 48, 48, 8, 8, 5},
    { 3, I, 3, 48, 48, I, 8, 5},
    { 5, 3, I, 72, 72, 48, 48, 24},
    { 48, 48, 74, I, 0, 6, 6, 12},
    { 48, 48, 74, 0, I, 6, 6, 12},
    { 8, 8, 50, 6, 6, I, 0, 8},
    { 8, 8, 50, 6, 6, 0, I, 8},
    { 5, 5, 26, 12, 12, 8, 8, I}
    };


    
    vector<vector<int>> env_costs1 = {
    { I, 2, 9, 10 },
    { 1, I, 6, 4  },
    { 15, 7, I, 8 },
    { 6, 3, 12, I },
    };
    vector<vector<int>> env_costs2 = {
    { I, 11, 10, 9, 6 },
    { 8, I, 7, 3, 4 },
    { 8, 4, I, 4, 8  },
    { 11, 10, 5, I, 5 },
    { 6, 9, 2, 5, I }
    };

    vector<vector<int>> env_costs3 = {
        {I, 64,378, 519, 434, 200 },
        {64,  I,   318, 455, 375, 164},
        {378, 318, I,    170, 265, 344},
        {519, 455, 170, I,    223,428},
        {434, 375, 265, 223, I,    273},
        {200, 164, 344, 428, 273, I}
    };

    
    

    GenerateCleaningPath(env_costs);
    GenerateCleaningPath(env_costs1);
    GenerateCleaningPath(env_costs2);
    GenerateCleaningPath(env_costs3);


    auto timeEnd = chrono::high_resolution_clock::now();
    auto timeDiff = chrono::duration_cast<chrono::milliseconds>(timeEnd - timeStart);

    cout << "Running time(ms) :    " << timeDiff.count() << "\n";
    cout << "\n";
    cout << "##########################################################\n";

    
    /*
    vector<vector<int>> env_costs = {
    { I, 2, 9, 10 },
    { 1, I, 6, 4  },
    { 15, 7, I, 8 },
    { 6, 3, 12, I },
    };
    */

    /*
    vector<vector<int>> env_costs = {
    { I, 11, 10, 9, 6 },
    { 8, I, 7, 3, 4 },
    { 8, 4, I, 4, 8  },
    { 11, 10, 5, I, 5 },
    { 6, 9, 2, 5, I }
    };
    */



}

