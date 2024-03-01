#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

const int INF = numeric_limits<int>::max();
const int NMAX = 10000;

struct Edge {
    int destination;
    int cost;

    Edge(int dest, int cst) : destination(dest), cost(cst) {}
};

struct DijkstraResult {
    vector<int> distances;
    vector<int> parents;
};

DijkstraResult dijkstra(int source, int nodes, const vector<vector<Edge>>& graph) {
    vector<int> distances(nodes + 1, INF);
    vector<int> parents(nodes + 1, 0);
    vector<bool> visited(nodes + 1, false);

    // Coada de priorități pentru a alege următorul nod cu distanța minimă
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    distances[source] = 0;
    pq.push(make_pair(0, source));

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        if (visited[u]) {
            continue;
        }

        visited[u] = true;

        for (const auto& edge : graph[u]) {
            int v = edge.destination;
            int cost = edge.cost;

            // Verificăm dacă putem utiliza teleportul
            if (cost == 1) {
                // Pentru fiecare t între distanța curentă și distanța curentă + cost,
                // verificăm dacă putem ajunge mai rapid la nodul v
                for (int t = distances[u]; t <= distances[u] + cost; t++) {
                    int newTime = t * cost;
                    if (newTime >= distances[u] && newTime < distances[v]) {
                        distances[v] = newTime;
                        parents[v] = u;
                        pq.push(make_pair(distances[v], v));
                    }
                }
            } else {
                if (distances[u] + cost < distances[v]) {
                    distances[v] = distances[u] + cost;
                    parents[v] = u;
                    pq.push(make_pair(distances[v], v));
                }
            }
        }
    }

    for (int node = 1; node <= nodes; node++) {
        if (distances[node] == INF) {
            distances[node] = -1;
        }
    }

    return {distances, parents};
}

int main() {
    ifstream inputFile("teleportare.in");
    ofstream outputFile("teleportare.out");

    int N, M, K;
    inputFile >> N >> M >> K;

    vector<vector<Edge>> graph(N + 1);

    for (int i = 0; i < M; i++) {
        int X, Y, T;
        inputFile >> X >> Y >> T;
        graph[X].emplace_back(Y, T);
        graph[Y].emplace_back(X, T);
    }

    for (int i = 0; i < K; i++) {
        int X, Y, P;
        inputFile >> X >> Y >> P;
        for (int j = 0; j <= N / P; j++) {
            graph[X].emplace_back(Y, 1);
            graph[Y].emplace_back(X, 1);
        }
    }

    DijkstraResult result = dijkstra(1, N, graph);
    int shortest = result.distances[N];

    outputFile << shortest << endl;

    inputFile.close();
    outputFile.close();

    return 0;
}
