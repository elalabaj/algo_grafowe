#ifndef CHECKER_HPP
#define CHECKER_HPP

#include "dimacs.hpp"
#include <filesystem>
#include <functional>
#include <iostream>
#include <string>

namespace checker {

inline bool test_file(const std::string &file_name,
                       std::function<int(std::string)> func) {
  int solution = dimacs::read_solution(file_name);
  return func(file_name) == solution;
}

inline bool test_dir(const std::string &dir_name,
                      std::function<int(std::string)> func, bool debug = true) {
  for (const auto &file : std::filesystem::directory_iterator(dir_name)) {
    if (file.is_regular_file()) {
      std::string path = file.path().string();
      if (debug)
        std::cout << std::setw(20) << std::left << file.path().filename().generic_string()<< ' ';
      int solution = dimacs::read_solution(path);
      int program_answer = func(path);
      if (solution != program_answer) {
        if (debug)
          std::cout << "ANS\nYour answer: " << program_answer
                    << "\nCorrect answer: " << solution << '\n';
        return false;
      } else if (debug)
        std::cout << "OK\n";
    }
  }
  return true;
}
} // namespace check

#endif
