#include <algorithm>
#include <cstddef>
#include <deque>
#include <fstream>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

constexpr int n_steps = 6;

template <class... Args>
auto createNDimVectorWithDefault(size_t n, Args&&... args) {
  if constexpr (sizeof...(args) == 1)
    return std::vector(n, args...);
  else
    return std::vector(n, createNDimVectorWithDefault(args...));
}

template <typename>
struct is_std_vector : std::false_type {};

template <typename T, typename A>
struct is_std_vector<std::vector<T, A>> : std::true_type {};

template <typename T, typename F>
void iterate(const T& input, F& f) {
  if constexpr (is_std_vector<T>::value) {
    for (const auto& in : input) {
      iterate(in, f);
    }
  } else {
    f(input);
  }
}

template <typename T1, typename T2, typename F>
void dualIterateAndModify(T1& input1, T2& input2, F& f) {
  if constexpr (is_std_vector<T1>::value) {
    for (size_t i = 0; i < input1.size(); ++i) {
      dualIterateAndModify(input1[i], input2[i], f);
    }
  } else {
    f(input1, input2);
  }
}

template <typename T1, typename T2, typename F>
void dualIterateAndUpdateT2UsingIndices(const T1& input1, T2& input2, F& f,
                                        std::deque<size_t>& indices) {
  if constexpr (is_std_vector<T1>::value) {
    for (size_t i = 0; i < input1.size(); ++i) {
      indices.push_back(i);
      dualIterateAndUpdateT2UsingIndices(input1[i], input2[i], f, indices);
      indices.pop_back();
    }
  } else {
    input2 = f(indices);
  }
}

template <typename>
struct is_std_vv : std::false_type {};

template <typename T, typename A>
struct is_std_vv<std::vector<std::vector<T, A>>> : std::true_type {};

template <typename T, typename F>
void iterateOverNeighbours(bool is_center, const T& input, F& f,
                           std::deque<size_t>& indices) {
  if constexpr (is_std_vv<T>::value) {
    auto cur = indices.front();
    indices.pop_front();
    for (int delta = -1; delta < 2; ++delta) {
      const int new_index = cur + delta;
      if (new_index < input.size() && new_index >= 0) {
        iterateOverNeighbours(is_center && (delta == 0), input[new_index], f,
                              indices);
      }
    }
    indices.push_front(cur);
  } else {
    auto cur = indices.front();
    indices.pop_front();
    for (int delta = -1; delta < 2; ++delta) {
      const int new_index = cur + delta;
      if (new_index < input.size() && new_index >= 0 &&
          !(is_center && (delta == 0))) {
        f(input[new_index]);
      }
    }
    indices.push_front(cur);
  }
}

int main() {
  std::fstream file{"../input/day_17_input"};
  std::string line;

  std::vector<std::string> g_plane;
  while (std::getline(file, line)) {
    line.erase(std::remove_if(std::begin(line), std::end(line),
                              [](auto c) { return !isprint(c); }),
               std::end(line));
    if (line == "") continue;
    g_plane.push_back(line);
  }

  auto hypercube = createNDimVectorWithDefault<char>(
      2 * n_steps + g_plane.size(), 2 * n_steps + g_plane[0].size(),
      2 * n_steps + 1, 2 * n_steps + 1, '.');

  for (int i = 0; i < g_plane.size(); ++i) {
    for (int j = 0; j < g_plane[0].size(); ++j) {
      hypercube[i + n_steps][j + n_steps][n_steps][n_steps] = g_plane[i][j];
    }
  }

  auto hypercube_nc = createNDimVectorWithDefault(
      2 * n_steps + g_plane.size(), 2 * n_steps + g_plane[0].size(),
      2 * n_steps + 1, 2 * n_steps + 1, 0);

  for (int i = 0; i < n_steps; ++i) {
    // Count active neighburs
    // Ref to note below for thoughts on this structure and alternative
    auto anc_function =
        [&hypercube = std::as_const(hypercube)](std::deque<size_t>& indices) {
          int active_nc = 0;
          auto f_count = [&](const char c) mutable {
            if (c == '#') {
              active_nc++;
            }
          };
          iterateOverNeighbours(true, hypercube, f_count,
                                indices);  // active neighbour counting function
          return active_nc;
        };
    std::deque<size_t> indices;
    dualIterateAndUpdateT2UsingIndices(hypercube, hypercube_nc, anc_function,
                                       indices);

    // Update accroding to rules
    auto update_functon = [](char& c, const int& nc) {
      if (c == '#' && !(nc == 2 || nc == 3))
        c = '.';
      else if (c == '.' && nc == 3)
        c = '#';
    };
    dualIterateAndModify(hypercube, hypercube_nc, update_functon);
  }

  int active_nc = 0;
  auto f_count = [&](const char c) mutable {
    if (c == '#') active_nc++;
  };
  iterate(hypercube, f_count);
  std::cout << active_nc << '\n';

  return 0;
}

// Options for counting neighbours
// Option 1:
// Pass hypercube and hypercube_nc as const inputs to dualIterateAndUpdateInput2
// so dualIterateAndUpdateInput2(hypercube, hypercube_nc, hypercube,
// hypercube_nc, anc_function, indices); where the second refs to hypercube and
// hypercube_nc are const and are passed to successive calls as total input, ie:
// void dualIterateAndUpdateInput2(const T1& input1, T2& input2, const T1&
// total_input1, const T2& total_input2, F& f, std::deque<size_t>& indices) and
// the lambda below takes hypercube, ie, total_input1 as an input auto
// anc_function = [] (const auto& hypercube, std::deque<size_t>& indices) { PRO:
// no capture by lamdba outside scope that is then passed in to a function that
// then uses the same value (both dualIterateAndUpdateInput2 and the lambda use
// hypercube) CON: A subsection of the total_input1 (input1) is not const, while
// total_input1 is const in the same function OR Option 2: Capture by hypercube
// by ref here and add as_const for correctness Since scope of use is limited to
// this section of the code, no errors arise and it's easy to understand so the
// lambda and function are auto anc_function = [&hypercube =
// std::as_const(hypercube)] (std::deque<size_t>& indices) and
// dualIterateAndUpdateInput2(hypercube, hypercube_nc, anc_function, indices)
// PRO: hypercube passed to dualIterateAndUpdateInput2 only once, no mix of
// const and non const CON: capture by lambda
