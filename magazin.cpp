#include <iostream>
#include <fstream>
#include <vector>


using namespace std;

void dfs(int node,
        vector<vector<int>>& adj,
        vector<int>& found,
        vector<int>& end,
        vector<int>& nodeOrder,
        vector<bool>& vizitat,
        int& timestamp) {
    vizitat[node] = true;       // marcheaza nodul ca fiind vizitat
    found[node] = ++timestamp;      // actualizeaza momentul gasirii nodului
    nodeOrder[timestamp] = node;

    // parcurgem vecinii nodului curent
    for (int neigh : adj[node]) {
        if (!vizitat[neigh]) {
            // verificam vecinii nevizitati
            dfs(neigh, adj, found, end, nodeOrder, vizitat, timestamp);
        }
    }

    // actualizam timestamp ul de final
    end[node] = timestamp;
}

int main() {
    ifstream in("magazin.in");
    ofstream out("magazin.out");

    // citim numarul de noduri si numarul de intrebari din fisierul de intrare
    int n, m;
    in >> n >> m;

    // initializam vectorii si matricea de adiacenta
    vector<int> found(n + 1);
    vector<int> end(n + 1);
    vector<int> nodeOrder(n + 1);
    vector<vector<int>> adj(n + 1);

    for (int node = 1; node <= n; node++) {
        adj[node].clear();
    }

    // citim depozitele din fisierul de intrare
    for (int i = 2, x; i <= n; i++) {
        in >> x;
        adj[x].push_back(i);
    }

    // citim intrebarile din fisierul de intrare
    vector<pair<int, int>> questions(m);
    for (int i = 0; i < m; i++) {
        in >> questions[i].first >> questions[i].second;
    }

    in.close();

    // initializam vectorul de rezultate cu -1
    vector<int> answers(m, -1);

    // initializam vectorul de vizitati si timestamp-ul
    vector<bool> vizitat(n + 1, false);
    int timestamp = 0;

    // parcurgem graful si aplicam DFS
    for (int node = 1; node <= n; node++) {
        if (!vizitat[node]) {
            dfs(node, adj, found, end, nodeOrder, vizitat, timestamp);
        }
    }

    // pentru fiecare intrebare, verificăm conditiile
    // si actualizam rezultatele(raspunsurile)
    for (int i = 0; i < m; i++) {
        int x = questions[i].first;
        int y = questions[i].second;
        if (found[x] + y <= n) {
            if (end[x] >= end[nodeOrder[found[x] + y]]) {
                answers[i] = nodeOrder[found[x] + y];
            }
        }
    }

    for (int i = 0; i < m; i++) {
        out << answers[i] << "\n";
    }

    out.close();

    return 0;
}
