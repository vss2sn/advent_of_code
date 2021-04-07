#include <algorithm>
#include <iterator>
#include <fstream>
#include <vector>

int main() {
  constexpr int total = 2020;
  std::ifstream file;
  file.open("../input/day_1_input");
  int num = 0;

  std::vector<int> entries_v{std::istream_iterator<int>{file}, {}};  // Ref: C++'s most vexing parse :D
  std::sort(std::begin(entries_v), std::end(entries_v));

  const size_t size = entries_v.size();
  for(size_t start = 0; start < size - 2; ++start) {

    size_t mid = start + 1;
    size_t end = size - 1;
    while(mid < end) {
      const int rem = total - entries_v[start] - entries_v[mid] - entries_v[end];
      if(rem == 0) {
        return entries_v[start] * entries_v[mid] * entries_v[end];
      } else if (rem > 0) {
        ++mid;
      } else {
        --end;
      }
    }
  }
  return 0;
}
