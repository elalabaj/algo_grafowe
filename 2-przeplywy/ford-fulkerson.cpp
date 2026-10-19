#include "../dimacs.hpp"
#include "../checker.hpp"
#include <climits>
#include <ios>
#include <unordered_map>
#include <vector>

struct ford_fulkerson {
    std::vector<std::vector<int>> graph;
    std::vector<std::unordered_map<int, int>> residual;
    std::vector<int> visited, prev;
    int S, T;

    ford_fulkerson(const std::string& file) {
        auto [n, edges] = dimacs::load_weighted_graph(file);
        graph.resize(n+1); visited.resize(n+1, 0); prev.resize(n+1);
        residual.resize(n+1); 
        S = 1; T = n;
         
        for (auto [a, b, w] : edges) {
            graph[a].push_back(b);
            graph[b].push_back(a);
            residual[a][b] = w;
            residual[b][a] = 0;
        }
    }

    int find_augmenting_path(int v, int flow, int visit) {
        if (flow == 0) return 0;
        if (v == T) return flow;
        visited[v] = visit; 
        for (auto w : graph[v]) {
            if (visited[w] != visit && residual[v][w] > 0) {
                prev[w] = v;
                int new_flow = std::min(flow, residual[v][w]);
                if ((new_flow = find_augmenting_path(w, new_flow, visit))) {
                    prev[w] = v;
                    return new_flow;
                }
            }
        }
        return 0;
    }

    int solve() {
        int max_flow = 0, flow;
        int visit = 0;
        while ((flow = find_augmenting_path(S, INT_MAX, ++visit))) {
            int v = T; 
            while (v != S) {
                residual[prev[v]][v] -= flow;
                residual[v][prev[v]] += flow;
                v = prev[v];
            }
            max_flow += flow;
        }
        return max_flow;
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    checker::test_dir("graphs-lab2/flow", [](auto file){ return ford_fulkerson(file).solve(); });

    return 0;
}
