#include <chrono>
#include <iostream>
#include <numeric>

#include "BFS.hpp"
#include "Utils.hpp"

int main() {
  std::vector<std::chrono::milliseconds> dur1;
  std::vector<std::chrono::milliseconds> dur2;

  auto graph = Utils::getCubeGraph(400);
  for (size_t times = 0; times < 5; ++times) {
    auto tpBegin1 = std::chrono::high_resolution_clock::now();

    auto res1 = bfs<BFSType::Seq>(graph);

    auto tpEnd1 = std::chrono::high_resolution_clock::now();

    dur1.emplace_back(std::chrono::duration_cast<std::chrono::milliseconds>(
        tpEnd1 - tpBegin1));
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(
                     dur1.back())
                     .count()
              << std::endl;

    auto tpBegin2 = std::chrono::high_resolution_clock::now();

    auto res2 = bfs<BFSType::Par>(graph);

    auto tpEnd2 = std::chrono::high_resolution_clock::now();

    dur2.emplace_back(std::chrono::duration_cast<std::chrono::milliseconds>(
        tpEnd2 - tpBegin2));
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(
                     dur2.back())
                     .count()
              << std::endl;

    if (res1 != res2) {
      throw std::runtime_error("res1 != res2");
    }
  }

  for (auto i : dur1) {
    std::cout << i.count() << ' ';
  }
  std::cout << std::accumulate(dur1.begin(), dur1.end(),
                               std::chrono::milliseconds{0})
                       .count() /
                   dur1.size()
            << std::endl;

  for (auto i : dur2) {
    std::cout << i.count() << ' ';
  }
  std::cout << std::accumulate(dur2.begin(), dur2.end(),
                               std::chrono::milliseconds{0})
                       .count() /
                   dur2.size()
            << std::endl;
  return 0;
}
