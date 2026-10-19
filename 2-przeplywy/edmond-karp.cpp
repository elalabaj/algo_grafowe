#include "../dimacs.hpp"
#include "../checker.hpp"
#include <climits>
#include <ios>
#include <unordered_map>
#include <vector>
#include <queue>

struct edmond_karp {
    std::vector<std::vector<int>> graph;
    std::vector<std::unordered_map<int, int>> residual;
    std::vector<int> visited, prev;
    int S, T;

    edmond_karp(const std::string& file) {
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

    int find_augmenting_path(int visit) {
        std::queue<std::pair<int, int>> qu;
        qu.push({S, INT_MAX});

        while (!qu.empty()) {
            auto [v, flow] = qu.front();
            qu.pop();
            if (v == T) return flow;

            for (auto w : graph[v]) {
                if (visited[w] != visit && residual[v][w] > 0) {
                    int new_flow = std::min(flow, residual[v][w]);
                    visited[w] = visit;
                    prev[w] = v;
                    qu.push({w, new_flow});
                }
            }
        }

        return 0;
    }

    int solve() {
        int max_flow = 0, flow;
        int visit = 0;
        while ((flow = find_augmenting_path(++visit))) {
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
    checker::test_dir("graphs-lab2/flow", [](auto file){ return edmond_karp(file).solve(); });

    return 0;
}
