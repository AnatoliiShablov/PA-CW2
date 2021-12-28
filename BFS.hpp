#ifndef BFS_HPP
#define BFS_HPP

#include <cstdint>
#include <vector>

enum class BFSType { Seq, Par };

template <BFSType type>
std::vector<int32_t> bfs(std::vector<std::vector<uint32_t>> const& edges);

#endif  // BFS_HPP
