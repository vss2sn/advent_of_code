#include <fstream>
#include <iostream>
#include <string>
#include <vector>  

struct Elf {
    int prev;
    int next;
    bool active = true;
};

int main(int argc, char* argv[]) {
  std::string input = "../input/day_19_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::ifstream file(input);
  std::string line;
  std::getline(file, line);
  const int n_elves = std::stoi(line);
  std::vector<Elf> elves (n_elves);
  for (int i = 1; i <= n_elves;i++) {
    elves[i].next = i+1;
    elves[i].prev = i-1;
    elves[i].active = true;
  }
  elves[n_elves].next = 1;
  elves[1].prev = n_elves;
  int current = 1;
  int to_remove = current; 
  int n_removed = 0;
  {
    const int jump = n_elves / 2;
    for (int i = 0; i < jump; i++) {
        to_remove = elves[to_remove].next;
    }
  }
  while(elves[current].next != current) {    
    {
        // Remove
        auto prev = elves[to_remove].prev;
        auto next = elves[to_remove].next;
        while (!elves[prev].active) prev = elves[prev].prev;
        while (!elves[next].active) next = elves[next].next;
        elves[prev].next = next;
        elves[next].prev = prev;
        // std::cout << to_remove << " removed" << '\n';
        elves[to_remove].active = false;
        n_removed++;
    }
    current = elves[current].next;
    to_remove = elves[to_remove].next; 
    while (!elves[to_remove].active) to_remove = elves[to_remove].next;
    if (n_elves % 2 == 0 && n_removed % 2 == 0) to_remove = elves[to_remove].next; 
    else if (n_elves % 2 == 1 && n_removed % 2 == 1) to_remove = elves[to_remove].next; 
  }
  std::cout << current << '\n';
  return 0;
}