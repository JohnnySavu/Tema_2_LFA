#include <bits/stdc++.h>
#include "Automata.cpp"

using namespace std;

int main()
{
    ifstream fin("input.in");
    Automata aut;
    fin >> aut;
    aut.print();
    //aut.delNodes();
    aut.minimizeDFA();
    aut.print();
}
