#include "BFS.hpp"

#include "datapar.hpp"
#include "parray.hpp"
#include <algorithm>
#include <atomic>
#include <numeric>

template <>
std::vector<int32_t>
bfs<BFSType::Seq>(const std::vector<std::vector<uint32_t>> &edges) {
  std::vector<int32_t> dist(edges.size(), -1);
  dist[0] = 0;
  std::vector<uint32_t> curFrontier{0};
  std::vector<uint32_t> nextFrontier;

  for (int32_t level = 1; !curFrontier.empty(); ++level) {
    nextFrontier.clear();
    for (auto v : curFrontier) {
      for (uint32_t u : edges[v]) {
        if (dist[u] == -1) {
          dist[u] = level;
          nextFrontier.push_back(u);
        }
      }
    }
    std::swap(curFrontier, nextFrontier);
  }
  return dist;
}

template <>
std::vector<int32_t>
bfs<BFSType::Par>(const std::vector<std::vector<uint32_t>> &edges) {
  pasl::pctl::parray<std::atomic<int32_t>> dist(
      edges.size(), [](size_t idx) { return INT32_C(-1); });
  dist[0] = 0;

  pasl::pctl::parray<uint32_t> curFrontier{0};

  for (int32_t level = 1; curFrontier.size() > 0; ++level) {
    pasl::pctl::parray<uint32_t> nextFrontierPS(
        curFrontier.size(), [&curFrontier, &edges](size_t idx) {
          return edges[curFrontier[idx]].size();
        });
    nextFrontierPS = pasl::pctl::scan(
        nextFrontierPS.begin(), nextFrontierPS.end(), UINT32_C(0),
        std::plus<uint32_t>{}, pasl::pctl::scan_type::forward_exclusive_scan);

    pasl::pctl::parray<int32_t> nextFrontier(
        nextFrontierPS[nextFrontierPS.size() - 1] +
            edges[curFrontier[curFrontier.size() - 1]].size(),
        INT32_C(-1));

    pasl::pctl::parallel_for(
        static_cast<size_t>(0), static_cast<size_t>(curFrontier.size()),
        [&](size_t i) {
          for (size_t cIdx{0}; cIdx < edges[curFrontier[i]].size(); ++cIdx) {
            int32_t _ = -1;
            if (dist[edges[curFrontier[i]][cIdx]].compare_exchange_strong(
                    _, level)) {
              nextFrontier[nextFrontierPS[i] + cIdx] =
                  edges[curFrontier[i]][cIdx];
            }
          }
        });

    {
      auto tmp = pasl::pctl::filter(nextFrontier.begin(), nextFrontier.end(),
                                    [](int32_t v) { return v != -1; });
      curFrontier = pasl::pctl::parray<uint32_t>(
          static_cast<size_t>(tmp.size()),
          [&tmp](size_t idx) { return static_cast<uint32_t>(tmp[idx]); });
    }
  }

  std::vector<int32_t> result;
  result.resize(dist.size());
  pasl::pctl::parallel_for(static_cast<size_t>(0),
                           static_cast<size_t>(dist.size()),
                           [&](size_t i) { result[i] = dist[i]; });
  return result;
}
