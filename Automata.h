#ifndef AUTOMATA_H
#define AUTOMATA_H
#include <bits/stdc++.h>
using namespace std;

class Automata
{
private:
    int n_nodes, n_edges, n_final_nodes;
    unordered_set <int> final_nodes;
    int start_node;
    unordered_map<char, vector<int>> edge[30000];
    unordered_set<char> lang;
    void delNodes();
    void addEdge(int , int , bool* );
public:
    friend ifstream& operator >> (ifstream&, Automata&);
    void print();
    void toNFA();
    void toDFA();
    void minimizeDFA();
};



#endif // AUTOMATA_H
