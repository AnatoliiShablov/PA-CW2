#include "Utils.hpp"

namespace {
[[nodiscard]] uint32_t getPos(std::array<uint32_t, 3> l,
                              uint32_t size) noexcept {
  uint32_t res = 0;
  for (auto i : l) {
    res = res * size + i;
  }
  return res;
}

} // namespace

std::vector<std::vector<uint32_t>> Utils::getCubeGraph(uint32_t size) {
  std::vector<std::vector<uint32_t>> edges;
  edges.resize(size * size * size);
  for (uint32_t i = 0; i < size; ++i) {
    for (uint32_t j = 0; j < size; ++j) {
      for (uint32_t k = 0; k < size; ++k) {
        std::array<uint32_t, 3> const l{i, j, k};
        auto &ref = edges[getPos(l, size)];
        for (size_t d = 0; d < 3; ++d) {
          if (l[d] > 0) {
            auto c = l;
            --c[d];
            ref.push_back(getPos(c, size));
          }

          if (l[d] + 1 < size) {
            auto c = l;
            ++c[d];
            ref.push_back(getPos(c, size));
          }
        }
      }
    }
  }
  return edges;
}
