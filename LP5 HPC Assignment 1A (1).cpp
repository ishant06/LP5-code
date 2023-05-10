// Cedrick Andrade
// COBA006

#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

using namespace std;

const int MAXN = 10000; // maximum number of nodes

vector<int> adjList[MAXN];
int distance[MAXN];

int main() {
    int n, m;
    cin >> n >> m;

    // read in the graph
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        adjList[u].push_back(v);
        adjList[v].push_back(u);
    }

    // initialize distances to -1 (unvisited)
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        distance[i] = -1;
    }

    // set up the BFS starting point
    int source = 0;
    distance[source] = 0;
    queue<int> q;
    q.push(source);

    // do BFS in parallel
    while (!q.empty()) {
        // process all nodes at the current level in parallel
        #pragma omp parallel
        {
            queue<int> privateQ;
            #pragma omp for nowait
            for (int i = 0; i < q.size(); i++) {
                int node = q.front();
                q.pop();
                privateQ.push(node);
                for (int j = 0; j < adjList[node].size(); j++) {
                    int neighbor = adjList[node][j];
                    if (distance[neighbor] == -1) {
                        distance[neighbor] = distance[node] + 1;
                        q.push(neighbor);
                    }
                }
            }
            // merge private queues back into main queue
            #pragma omp critical
            {
                while (!privateQ.empty()) {
                    q.push(privateQ.front());
                    privateQ.pop();
                }
            }
        }
    }

    // print out the distances
    for (int i = 0; i < n; i++) {
        cout << "Distance from " << source << " to " << i << " is " << distance[i] << endl;
    }

    return 0;
}