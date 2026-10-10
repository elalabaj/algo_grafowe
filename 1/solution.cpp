#include "../checker.hpp"
#include "../dimacs.hpp"
#include <algorithm>
#include <climits>
#include <queue>
#include <vector>

int solve_fau(std::string file_name) {
    int s = 1, t = 2;
    auto [n, edges] = dimacs::load_weighted_graph(file_name);
    n++;
    std::vector<std::vector<std::pair<int, int>>> graph(n);
    for (auto [a, b, c] : edges) {
        graph[a].push_back({b, c});
        graph[b].push_back({a, c});
    }

    std::vector<int> fau(n);
    for (int i = 0; i < n; i++)
        fau[i] = i;

    std::function<int(int)> _find = [&](int a) {
        if (fau[a] == a)
            return a;
        return fau[a] = _find(fau[a]);
    };

    auto _union = [&](int a, int b) {
        a = _find(a);
        b = _find(b);
        fau[b] = a;
    };

    std::sort(edges.begin(), edges.end(), [](auto &lhs, auto &rhs) {
        auto [a, b, w1] = lhs;
        auto [c, d, w2] = rhs;
        return w1 > w2;
    });
    for (auto [a, b, c] : edges) {
        _union(a, b);
        if (_find(s) == _find(t))
            return c;
    }

    return 0;
}

bool dfs(int v, int target, int min_weight, int visit,
         std::vector<std::vector<std::pair<int, int>>> &graph,
         std::vector<int> &visited) {

    visited[v] = visit;
    if (v == target)
        return true;
    for (auto [w, weight] : graph[v]) {
        if (visited[w] != visit && weight >= min_weight) {
            if (dfs(w, target, min_weight, visit, graph, visited))
                return true;
        }
    }
    return false;
}

int solve_binsearch(std::string file_name) {
    int s = 1, t = 2;
    auto [n, edges] = dimacs::load_weighted_graph(file_name);
    n++;
    std::vector<std::vector<std::pair<int, int>>> graph(n);
    int l = INT_MAX, r = INT_MIN;
    for (auto [a, b, c] : edges) {
        graph[a].push_back({b, c});
        graph[b].push_back({a, c});
        l = std::min(l, c);
        r = std::max(r, c);
    }

    std::vector<int> visited(n);
    int visit = 0;

    while (l < r) {
        int m = l + (r - l + 1) / 2;
        if (dfs(s, t, m, ++visit, graph, visited))
            l = m;
        else
            r = m - 1;
    }

    return l;
}

int solve_dijkstra(std::string file_name) {
    auto [n, edges] = dimacs::load_weighted_graph(file_name);
    n++;
    int s = 1, t = 2;
    std::vector<std::vector<std::pair<int, int>>> graph(n);
    for (auto [a, b, c] : edges) {
        graph[a].push_back({b, c});
        graph[b].push_back({a, c});
    }

    std::vector<int> dist(n, -INT_MAX);
    dist[s] = INT_MAX;
    std::priority_queue<std::pair<int, int>> pq;
    pq.push({dist[s], s});

    while (!pq.empty()) {
        auto [d, v] = pq.top();
        pq.pop();
        if (d < dist[v]) continue;

        for (auto [w, weight] : graph[v]) {
            if (std::min(dist[v], weight) > dist[w]) {
                dist[w] = std::min(dist[v], weight);
                pq.push({dist[w], w});
            }
        }
    }

    return dist[t];
}

int main() {
    std::cout << "Find and union\n";
    checker::test_dir("graphs-lab1", solve_fau);

    std::cout << "\n\nBinary search\n";
    checker::test_dir("graphs-lab1", solve_binsearch);

    std::cout << "\n\nDijkstra\n";
    checker::test_dir("graphs-lab1", solve_dijkstra);

    return 0;
}
