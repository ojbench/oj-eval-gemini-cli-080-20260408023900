#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

const int MAXN = 100005;

struct Edge {
    int to;
    int id;
};

vector<Edge> adj[MAXN];
int dfn[MAXN], low[MAXN], timer;
vector<pair<int, int>> st;
int good[MAXN];
vector<int> bcc_adj[MAXN];
int bcc_id[MAXN];
int current_bcc;
int color[MAXN];

void dfs(int u, int p_edge) {
    dfn[u] = low[u] = ++timer;
    for (auto edge : adj[u]) {
        int v = edge.to;
        int id = edge.id;
        if (id == p_edge) continue;
        if (dfn[v]) {
            low[u] = min(low[u], dfn[v]);
            if (dfn[v] < dfn[u]) {
                st.push_back({u, v});
            }
        } else {
            st.push_back({u, v});
            dfs(v, id);
            low[u] = min(low[u], low[v]);
            if (low[v] >= dfn[u]) {
                current_bcc++;
                vector<int> nodes;
                vector<pair<int, int>> edges;
                while (true) {
                    auto e = st.back();
                    st.pop_back();
                    edges.push_back(e);
                    if (bcc_id[e.first] != current_bcc) {
                        bcc_id[e.first] = current_bcc;
                        nodes.push_back(e.first);
                    }
                    if (bcc_id[e.second] != current_bcc) {
                        bcc_id[e.second] = current_bcc;
                        nodes.push_back(e.second);
                    }
                    if (e == make_pair(u, v)) break;
                }
                
                for (auto e : edges) {
                    bcc_adj[e.first].push_back(e.second);
                    bcc_adj[e.second].push_back(e.first);
                }
                
                bool is_bipartite = true;
                for (int node : nodes) {
                    color[node] = 0;
                }
                
                for (int start_node : nodes) {
                    if (color[start_node] == 0) {
                        color[start_node] = 1;
                        queue<int> q;
                        q.push(start_node);
                        while (!q.empty()) {
                            int curr = q.front();
                            q.pop();
                            for (int nxt : bcc_adj[curr]) {
                                if (color[nxt] == 0) {
                                    color[nxt] = 3 - color[curr];
                                    q.push(nxt);
                                } else if (color[nxt] == color[curr]) {
                                    is_bipartite = false;
                                }
                            }
                        }
                    }
                }
                
                if (!is_bipartite) {
                    for (int node : nodes) {
                        good[node] = 1;
                    }
                }
                
                for (int node : nodes) {
                    bcc_adj[node].clear();
                }
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m;
    if (!(cin >> n >> m)) return 0;
    
    int edge_id = 0;
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        if (u == v) continue; // Ignore self-loops
        adj[u].push_back({v, edge_id});
        adj[v].push_back({u, edge_id});
        edge_id++;
    }
    
    for (int i = 1; i <= n; ++i) {
        if (!dfn[i]) {
            dfs(i, -1);
        }
    }
    
    int ans = 0;
    for (int i = 1; i <= n; ++i) {
        if (!good[i]) {
            ans++;
        }
    }
    cout << ans << "\n";
    return 0;
}
