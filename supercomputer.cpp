#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

int n;      // numarul de task-uri
int nr_dependente;      // numarul de dependente
vector<int> grades;         // vectorul de grade
vector<vector<int>> adj;        // lista de adiacenta
vector<int> in_degree;      // vectorul de grade de intrare

int sortare_topologica() {
    // daca nu exista dependente, rezultatul este 0
    if (nr_dependente == 0) {
        return 0;
    }

    // retinem vectorul de grade de intrare
    vector<int> in_degreeCopy = in_degree;
    // vectorul pentru stocarea lungimilor maxime ale secventelor
    vector<int> dp(n + 1, 0);

    queue<int> q;
    for (int i = 1; i <= n; i++) {
        // adaugam in coada nodurile fara dependente
        if (in_degreeCopy[i] == 0) {
            q.push(i);
        }
    }

    while (!q.empty()) {
        int node = q.front();
        q.pop();

        for (int neigh : adj[node]) {
            // scadem gradul de intrare al vecinului
            in_degreeCopy[neigh]--;

            // daca neigh nu mai are alte dependente il adaugam in coada
            if (in_degreeCopy[neigh] == 0) {
                q.push(neigh);
            }

            // lungime maxima = lungimea maxima de taskuri care pot fi executate
            // succesiv, fara a necesita context switch uri
            // daca gradele sunt diferite, atunci actualizam lungimea maxima
            // daca sunt egale, nu se schimba lungimea maxima
            if (grades[node] != grades[neigh]) {
                dp[neigh] = max(dp[neigh], dp[node] + 1);
            } else {
                dp[neigh] = max(dp[neigh], dp[node]);
            }
        }
    }

    // aflam lungimea maxima a secventei
    int res = 0;
    for (int i = 1; i <= n; i++) {
        res = max(res, dp[i]);
    }
    return res;
}

int main() {
    ifstream in("supercomputer.in");
    ofstream out("supercomputer.out");

    // citire numarul de taskuri si de dependente
    in >> n >> nr_dependente;

    grades.resize(n + 1);
    adj.resize(n + 1);
    // initializam cu 0 vectorul grade de intrare
    in_degree.resize(n + 1, 0);

    // citim gradele pentru fiecare task
    for (int i = 1; i <= n; i++) {
        in >> grades[i];
    }

    for (int i = 0; i < nr_dependente; i++) {
        int x, y;
        in >> x >> y;
        // adaugam o muchie în graf de la x la y
        adj[x].push_back(y);
        // incrementam gradul de intrare al nodului y
        in_degree[y]++;
    }

    int res = sortare_topologica();
    out << res << endl;         // afisare

    in.close();
    out.close();

    return 0;
}
