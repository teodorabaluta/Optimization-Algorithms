#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

void tarjan_scc(int node,
				const vector<vector<int>>& adj,
				const vector<vector<int>>& adj_neigh,
				vector<int>& parent,
				vector<int>& vizitat,
				vector<int>& low_link,
				stack<int>& nodes_stack,
				vector<bool>& in_stack,
				int& timestamp,
				int& nr,
				int gara,
				const vector<int>& in_degree) {
	vizitat[node] = ++timestamp;
	low_link[node] = vizitat[node];
	nodes_stack.push(node);
	in_stack[node] = true;

	for (int neigh : adj[node]) {
		// verificam daca vecinul este deja vizitat si daca se afla in stiva
		if (parent[neigh] != -1) {
			if (in_stack[neigh]) {
				// actualizam valoarea "low_link" -ului nodului curent
				low_link[node] = min(low_link[node], vizitat[neigh]);
			}
			continue;
		}
		parent[neigh] = node;
		tarjan_scc(neigh, adj, adj_neigh, parent, vizitat,
					low_link, nodes_stack, in_stack, timestamp,
					nr, gara, in_degree);
		// actualizam valoarea "low_link" a nodului curent
		low_link[node] = min(low_link[node], low_link[neigh]);
	}

	// verificam daca nodul curent formeaza o componenta tare-conexa
	if (low_link[node] == vizitat[node]) {
		vector<int> scc;

		do {
			int x = nodes_stack.top();
			// verificam daca nodul este un nod izolat si nu este gara
			if (nodes_stack.size() == 1) {
				if (x != gara && in_degree[x] == 0) {
					nr++;
					nodes_stack.pop();
					in_stack[x] = false;
					return;
				}
			}

			nodes_stack.pop();
			in_stack[x] = false;
			scc.push_back(x);
		} while (scc.back() != node);

		// verificam daca componenta tare-conexa contine gara
		if (find(scc.begin(), scc.end(), gara) != scc.end()) {
			return;
		}

		// verificam daca toti vecinii nodurilor din componenta tare-conexa
		// se afla si ei in componenta tare-conexa
		for (int nod : scc) {
			bool all_neighbors_in_scc = true;
			for (int neigh : adj_neigh[nod]) {
				if (find(scc.begin(), scc.end(), neigh) == scc.end()) {
					all_neighbors_in_scc = false;
					break;
				}
			}
			if (!all_neighbors_in_scc) {
				return;
			}
		}
		nr++;
	}
}

int main() {
	ifstream in("ferate.in");
	ofstream out("ferate.out");

	int n, m, gara;
	in >> n >> m >> gara;

	// lista de adiacenta pentru graful initial
	vector<vector<int>> adj(n + 1);
	// lista de adiacenta pentru a verifica
	// vecinii in cadrul componentelor tare-conexe
	vector<vector<int>> adj_neigh(n + 1);
	// numarul de muchii de intrare pentru fiecare nod
	vector<int> in_degree(n + 1);

	// initializare vectori
	for (int i = 1; i <= n; i++) {
		adj[i].clear();
		adj_neigh[i].clear();
	}

	// citim muchiile grafului si actualizam vectorii de adiacenta si in_degree
	for (int i = 1, x, y; i <= m; i++) {
		in >> x >> y;
		adj[x].push_back(y);
		++in_degree[y];
		adj_neigh[y].push_back(x);
	}
	in.close();

	// initializam vectorii si variabilele necesare algoritmului Tarjan
	vector<int> parent(n + 1, -1);
	vector<int> vizitat(n + 1, -1);
	vector<int> low_link(n + 1, -1);
	stack<int> nodes_stack;
	vector<bool> in_stack(n + 1, false);
	int timestamp = 0;
	int nr = 0;

	// aplicam algoritmul lui Tarjan pentru fiecare nod nevizitat
	for (int node = 1; node <= n; ++node) {
		if (parent[node] == -1) {
			parent[node] = node;
			tarjan_scc(node, adj, adj_neigh, parent, vizitat,
						low_link, nodes_stack, in_stack, timestamp,
						nr, gara, in_degree);
		}
	}

	out << nr << endl;
	out.close();

	return 0;
}
