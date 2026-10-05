from pathlib import Path
import sys
from queue import PriorityQueue

parent_dir = Path(__file__).resolve().parent.parent
sys.path.append(str(parent_dir))
from dimacs import *

sys.setrecursionlimit(500000)

def read_graph(file):
    (n, E) = loadWeightedGraph(file)
    s, t = 1, 2

    graph = [[] for _ in range(n+1)]
    for a, b, c in E:
        graph[a].append((b, c))
        graph[b].append((a, c))

    return (n, E, graph, s, t)

def solve_fau(file):
    n, E, _, s, t = read_graph(file)

    fau = [i for i in range(n+1)]
    def find(a):
        if fau[a] == a:
            return a
        fau[a] = find(fau[a])
        return fau[a]

    def union(a, b):
        a = find(a)
        b = find(b)
        fau[b] = a

    E.sort(key=lambda x: x[2], reverse=True)

    for a, b, c in E:
        union(a, b)
        if find(s) == find(t):
            return c

    return -1

def solve_binsearch(name):
    n, E, graph, S, T = read_graph(name)    
    l, r = float('inf'), 0
    for _, _, c in E:
        l = min(l, c)
        r = max(r, c)

    visited = [0] * (n+1)
    def dfs(v, t, visit, min_weight):
        visited[v] = visit
        if v == t:
            return True
        for w, weight in graph[v]:
            if visited[w] != visit and weight >= min_weight:
                if dfs(w, t, visit, min_weight):
                    return True
        return False

    visit = 1
    while l < r:
        m = l + (r - l + 1) // 2
        if dfs(S, T, visit, m):
            l = m
        else:
            r = m - 1
        visit += 1

    return l

def solve_dijkstra(file):
    n, E, graph, S, T = read_graph(file)

    min_weight = [0] * (n+1)
    min_weight[S] = float('inf')

    pq = PriorityQueue()
    pq.put((-min_weight[S], S))

    while not pq.empty():
        curr_min_weight, v = pq.get()
        curr_min_weight = -curr_min_weight

        if min_weight[v] < curr_min_weight:
            continue

        for w, weight in graph[v]:
            if min(weight, curr_min_weight) > min_weight[w]:
                min_weight[w] = min(weight, curr_min_weight)
                pq.put((-min_weight[w], w))

    return min_weight[T]

def test(func):
    dir = "graphs-lab1"
    folder = Path(dir)

    for file in folder.iterdir():
        name = dir + '/' + file.name
        ans = func(name)
        sol = int(readSolution(name))

        if ans == sol:
            print("OK", file.name)
        else:
            print("ANS", file.name, ans, sol)
            break

if __name__ == "__main__":
    print("Find and Union")
    test(solve_fau)
    print("\n\nBinary Search")
    test(solve_binsearch)
    print("\n\nDijkstra")
    test(solve_dijkstra)