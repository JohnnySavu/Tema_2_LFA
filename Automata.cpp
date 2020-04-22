#include <bits/stdc++.h>
#include "Automata.h"
using namespace std;

void Automata::addEdge(int origin, int node, bool* vizitat)
{
    vizitat[node] = 1;
    for(auto &it:edge[node])
    {
        if (it.first != '$')
        {
            for (auto &it2: it.second)
            {
                bool flag = 0;
                for (auto &it3: edge[origin][it.first])
                    if (it2 == it3)
                        flag = 1;
                if (flag == 0)
                    edge[origin][it.first].emplace_back(it2);

            }

        }
    }
    for (auto &it:edge[node]) if (it.first =='$')
    {
        for (auto& it2: it.second)
            if (vizitat[it2] == 0)
                addEdge(origin, it2, vizitat);
    }
}


void Automata::print()
{
    for (int i = 1; i<= n_nodes; i++)
    {
        for (auto &it : edge[i])
            for (auto &it2: it.second)
            {
                cout << i <<" " <<it2 <<" " << it.first <<"\n";
            }
    }
    for (int i = 1; i<= n_nodes; i++)
    {
        if (final_nodes.find(i) != final_nodes.end())
            cout << i << " ";
    }
    cout<<"\n";
    for (auto& it: lang)
        cout << it <<" ";
    cout<<"\n\n";
}


void Automata::toNFA()
{
    bool vizitat[301];
    for (int node = 1; node <= n_nodes; node ++)
    {
        memset(vizitat, 0, n_nodes);
        for (auto &it: edge[node]['$'])
        {
            addEdge(node, node, vizitat);

        }
    }
    for (int node = 1; node <= n_nodes; node ++)
    {
        edge[node]['$'].clear();
        edge[node].erase('$');
    }
    lang.erase('$');
}


ifstream& operator >> (ifstream& fin, Automata& aut)
{
    fin >> aut.n_nodes >> aut.n_edges;
    fin >> aut.n_final_nodes >> aut.start_node;
    string st;
    int aux, nod1, nod2;
    for (int i = 1; i<= aut.n_final_nodes; i++)
        fin >> aux, aut.final_nodes.insert(aux);
    for (int i = 1; i <= aut.n_edges; i++)
    {
        fin >> nod1 >> nod2 >> st;
        if (st.size() > 1)
            aut.edge[nod1]['$'].emplace_back(nod2), aut.lang.insert('$');
        else
            aut.edge[nod1][st[0]].emplace_back(nod2), aut.lang.insert(st[0]);
    }
}


void Automata::toDFA()
{
    bool vizitat[30000];
    unordered_map<char, int> dfa_edge[30000];
    vector<vector<int>> nodes;
    vector<int> aux;
    unordered_set<char> set2;
    unordered_set<int> set1;
    queue<int> q;
    q.push(0); // the first node is the starting node
    aux.clear();
    aux.emplace_back(start_node);
    nodes.push_back(aux);
    //the first node is formed only by the old start node
    while (!q.empty())
    {
        set2.clear();
        int poz = q.front();
        q.pop();
        //culeg nodul curent
        for (auto& el: nodes[poz])
        {
            for (auto& it: edge[el])
                set2.insert(it.first);
        }
        //plec cu toate caracterele din nodul curent
        for (auto& ch: set2)
        {
            set1.clear();
            for (auto& el: nodes[poz])
            {
                    for (auto& next_nod:edge[el][ch])
                        set1.insert(next_nod);
            }
            //pun elem in vector
            aux.clear();
            for(auto el: set1)
                aux.push_back(el);
            if (aux.size() == 0)
                continue;
            //verific daca nodul mai exista
            sort(aux.begin(), aux.end());
            int is_node = -1;
            int cont = -1;
            for (auto& vec: nodes)
            {
                cont ++;
                if(vec == aux)
                {
                    is_node = cont;
                    break;
                }
            }
            //daca nu exista il pun
            if (is_node == -1)
            {
                nodes.push_back(aux);
                q.push(nodes.size() - 1);
                is_node = nodes.size() - 1;
                dfa_edge[poz + 1][ch] = is_node + 1;
            }
            else
            {
                dfa_edge[poz + 1][ch] = is_node + 1;
            }
            //daca exista, vad daca are sens sa ma duc la el.
            //pun muchia
        }
    }
    for (int i = 1; i <= n_nodes; i++)
    {
       edge[i].clear();
    }
    for (int i = 0; i < nodes.size(); i++)
    {
        // pun noul automat in memorie
        for (auto& it: dfa_edge[i + 1])
            edge[i + 1][it.first].push_back(it.second);
    }
    set1.clear();
    int cont = 0;
    //pun noile noduri finale
    for (auto& vec: nodes)
    {
        cont ++;
        for (auto& el:vec)
        {
            if (final_nodes.find(el) != final_nodes.end())
                set1.insert(cont);
        }
    }
    //pun nodul initial
    n_nodes = nodes.size();
    final_nodes.clear();
    final_nodes = set1;
    start_node = 1;
}


void Automata::delNodes()
{
    unordered_map<char, vector<int>> rev_edge[3000];
    unordered_map<char, vector<int>> ans_edge[3000];
    unordered_map<int, int> new_node;
    //fac graful transpus
    for (int i = 1; i <= n_nodes; i++)
        for (auto& muchie: edge[i])
            for (auto& next_node: muchie.second)
            {
                rev_edge[next_node][muchie.first].push_back(i);
            }
    queue <int> q;
    bool vizitat[3000];
    bool rev_vizitat[30000];
    bool viz[3000];
    memset(vizitat, 0, n_nodes);
    memset(rev_vizitat, 0, n_nodes);
    q.push(start_node);
    vizitat[start_node] = 1;
    //fac un bfs sa vad nodurile accesibile din start
    while (!q.empty())
    {
        int nod = q.front();
        q.pop();
        for (auto& it: edge[nod])
            for (auto& it2: it.second)
                if (!vizitat[it2])
                {
                    vizitat[it2] = 1;
                    q.push(it2);
                }
    }
    for (auto& fin: final_nodes)
    {
        rev_vizitat[fin] = 1;
        q.push(fin);
    }
    //fac un bfs sa vad nodurile neaccesibile din graful transpus
    while (!q.empty())
    {
        int nod = q.front();
        q.pop();
        for (auto& it: rev_edge[nod])
            for (auto& it2: it.second)
                if (!rev_vizitat[it2])
                {
                    rev_vizitat[it2] = 1;
                    q.push(it2);
                }
    }
    //atunci automatul nu accepta nimic
    if(!vizitat[start_node] or !rev_vizitat[start_node])
    {
        start_node = -1;
        return;
    }
    //refac graful doar cu nodurile vizitate in ambele sensuri
    q.push(start_node);
    int n_final = 1;
    new_node.clear();
    new_node[start_node] = 1;
    
    memset(viz, 0, n_nodes);
    viz[start_node] = 1;
    while (!q.empty())
    {
        int node = q.front();
        q.pop();
        for (auto& m : edge[node])
        {
            for (auto& it: m.second) if (vizitat[it] and rev_vizitat[it])
            {
                
                if (new_node.find(it) == new_node.end())
                    n_final++, new_node[it] = n_final;
                ans_edge[new_node[node]][m.first].push_back(new_node[it]);
                if (!viz[it])
                {
                    viz[it] = 1;
                   q.push(it);
                }
            }
    }
    }
    start_node = 1;
    for (auto it: final_nodes)
        q.push(it);
    final_nodes.clear();
    while (!q.empty())
    {
        final_nodes.insert(new_node[q.front()]);
        q.pop();
    }
    for (int i = 1; i<= n_nodes; i++)
        edge[i].clear();
    n_nodes = n_final;
    
    for(int i = 1; i <= n_nodes; i++)
        edge[i] = ans_edge[i];
}

void Automata::minimizeDFA()
{
    delNodes();
    int oldEqClass[n_nodes + 20];
    int newEqClass[n_nodes + 20];
    memset(oldEqClass, -1, n_nodes * sizeof(int));
    memset(newEqClass, -1, n_nodes * sizeof(int));
    int no_class = 1;
    for (int i = 1; i <= n_nodes; i++)
        if (final_nodes.find(i) == final_nodes.end())
            oldEqClass[i] = 1;
        else
            oldEqClass[i] = 2;

    newEqClass[1] = oldEqClass[1]; //this one doesn't change
    

    while (true)
    {
        for(int i = 1; i<= n_nodes; i++)
            newEqClass[i] = -1;
        newEqClass[1] = oldEqClass[1];
        no_class = 1;
        for (int i = 2; i <= n_nodes; i++)
        {

            for (int j = 1; j < i; j++)
            {
                bool isEq = 1;
                for (auto& ch : lang)
                {
                    if (edge[i][ch].size() != edge[j][ch].size())
                    {
                        isEq = 0;
                        break;
                    }
                    if (edge[i][ch].size() != 0)
                    {
                        // daca sunt in clase de echivalenta diferite
                        if (oldEqClass[edge[i][ch][0]] != oldEqClass[edge[j][ch][0]])
                        {
                            isEq = 0;
                            break;
                        }
                    }
                }
                if (isEq == 1)
                {
                    newEqClass[i] = newEqClass[j];
                    break;
                }
            }
            if (newEqClass[i] == -1)
            {
                no_class++;
                newEqClass[i] = no_class;
            }
        }

        bool flag = 1;
        for (int i = 1; i <= n_nodes; i++)
        {
            if (newEqClass[i] != oldEqClass[i])
                flag = 0;
            oldEqClass[i] = newEqClass[i];
        }

        if (flag == 1)
            break;
    }
    unordered_map<char, vector<int>> ans_edge[3000];
    unordered_set<int> am_fost;
   
    for (int i =1 ;i<= n_nodes; i++)
    {

        if (am_fost.find(newEqClass[i]) == am_fost.end())
        {

            am_fost.insert(newEqClass[i]);
            for (auto& ch : edge[i])
                for (auto& it: ch.second)
                    ans_edge[newEqClass[i]][ch.first].push_back(newEqClass[it]);
        }
        edge[i].clear();   
    }

    for (int i = 1; i<= no_class; i++)
        edge[i] = ans_edge[i];
    am_fost.clear();
    for (auto& it:final_nodes)
        am_fost.insert(newEqClass[it]);
    final_nodes = am_fost;
    start_node = newEqClass[start_node];
    n_nodes = no_class;

}