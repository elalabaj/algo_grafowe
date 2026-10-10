#ifndef DIMACS_HPP
#define DIMACS_HPP

#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <tuple>

namespace dimacs {

    inline std::pair<int, std::vector<std::tuple<int, int, int>>> load_weighted_graph(const std::string& file_name) {
        int V = 0;
        std::vector<std::tuple<int, int, int>> edges;

        std::ifstream f(file_name);
        if (!f.is_open()) return {0, {}};
        std::string line;
        while (std::getline(f, line)) {
            std::stringstream ss(line);
            std::string token;
            ss >> token;
            if (token == "" || token == "c") continue;
            else if (token == "p") {
                std::string type;
                ss >> type >> V;
            }
            else if (token == "e") {
                int a, b, c;
                ss >> a >> b >> c;
                edges.push_back({a, b, c});
            }
        }
        f.close();
        return {V, edges};
    }

    inline int read_solution(const std::string& file_name) {
        std::ifstream f(file_name); 
        if (!f.is_open()) return -1;
        std::string line;
        int solution = -1;
        if (std::getline(f, line)) {
            std::stringstream ss(line);
            std::string curr, last;
            while (ss >> curr) last = curr;
            solution = std::stoi(last);
        }
        return solution;
    }
}

#endif
