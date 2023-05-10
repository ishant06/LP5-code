// Cedrick Andrade
// COBA006

#include <iostream>
#include <vector>
#include <stack>
#include <omp.h>

using namespace std;

const int MAXN = 10000; // maximum number of nodes

vector<int> adjList[MAXN];
bool visited[MAXN];

void dfs(int node) {
    visited[node] = true;
    for (int i = 0; i < adjList[node].size(); i++) {
        int neighbor = adjList[node][i];
        if (!visited[neighbor]) {
            dfs(neighbor);
        }
    }
}

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

    // initialize visited array to false
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        visited[i] = false;
    }

    // set up the DFS starting point
    int source = 0;

    // do DFS in parallel
    #pragma omp parallel
    {
        #pragma omp single
        {
            dfs(source);
        }
    }

    // print out the visited nodes
    for (int i = 0; i < n; i++) {
        if (visited[i]) {
            cout << "Node " << i << " is visited." << endl;
        }
    }

    return 0;
}
