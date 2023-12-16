#include <algorithm>
#include <cmath>
#include <fstream>
#include <ostream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

int calculate_hash(const std::string& line) {
  int value = 0;
  for (const auto c : line) {
    value += int(c);
    value *= 17;
    value %= 256;
  }
  return value;
}

struct Instruction {
  std::string label;
  char op;
  int focal_length;
  int box;
};


Instruction parse_substr(const std::string substr) {
  std::size_t idx = substr.find_first_of("-=");
  Instruction p;
  p.label = substr.substr(0, idx);
  p.op = substr[idx];
  if (p.op == '=') p.focal_length = std::stoi(substr.substr(idx + 1, substr.size() - idx - 1));
  return p;
}

std::vector<Instruction> parse(const std::string& s) {
  std::vector<Instruction> instructions;
  std::size_t start = 0;
  std::size_t end = s.find(',', start);
  while (end != std::string::npos) {
    instructions.push_back(parse_substr(s.substr(start, end - start)));
    start = end + 1;
    end = s.find(',', start);
  }
  instructions.push_back(parse_substr(s.substr(start, s.size() - start)));
  return instructions;
}

struct LensInBox {
  std::string label;
  int focal_length;
  std::string next_lens_label;
  std::string previous_lens_label;

  friend std::ostream& operator << (std::ostream& os, const LensInBox& lens);
};

std::ostream& operator << (std::ostream& os, const LensInBox& lens) {
  os << "[" << lens.label << ' ' << lens.focal_length << ']'; // << ',' <<lens.next_lens_label << ',' << lens.previous_lens_label << "}";
  return os;
}

struct Box {
  int id;
  std::unordered_map<std::string, LensInBox> lenses;
  std::string first = "";
  std::string last = "";
  friend std::ostream& operator << (std::ostream& os, const Box& box);

  void remove_lens(const std::string& label) {
    if (auto it = lenses.find(label); it != lenses.end()) {
      if (it->second.next_lens_label != "") {
        lenses[it->second.next_lens_label].previous_lens_label = it->second.previous_lens_label;
      }
      if (it->second.previous_lens_label != "") {
        lenses[it->second.previous_lens_label].next_lens_label = it->second.next_lens_label;
      }
      if (first == label) {
        first = it->second.next_lens_label;
      }
      if (last == label) {
        last = it->second.previous_lens_label;
      }
      lenses.erase(it);
    }
  }

  void add_lens(const std::string& label, const int focal_length) {
    auto it = lenses.find(label);
    if (it == lenses.end()) {
      if (first == "") first = label;
      LensInBox lens;
      lens.label = label;
      lens.focal_length = focal_length;
      lens.next_lens_label = "";
      lens.previous_lens_label = last;
      lenses[last].next_lens_label = label;
      last = label;
      lenses[label] = lens;
      return;
    }
    it->second.focal_length = focal_length;
  }
};

std::ostream& operator << (std::ostream& os, const Box& box) {
  os << box.id << ": ";
  auto current = box.first;
  while (current != "") {
    const auto& lens = box.lenses.at(current);
    os << lens << ' ';
    current = lens.next_lens_label;
  }
  return os;
}


int main(int argc, char * argv[]) {
  std::string input = "../input/day_15_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  std::getline(file, line);
  const auto instructions = parse(line);
  std::unordered_map<int, Box> boxes;

  for (auto& instruction : instructions) {
    // std::cout << instruction.label << '\n';
    const auto box_id = calculate_hash(instruction.label);
    if (instruction.op == '=') {
      if (boxes.find(box_id) == boxes.end()) boxes[box_id].id = box_id;
      boxes[box_id].add_lens(instruction.label, instruction.focal_length);
    } else if (instruction.op == '-') {
       boxes[box_id].remove_lens(instruction.label);
    } else {
      std::cout << "This should not happen" << '\n';
      std::cout << '|' << instruction.op << '|' << '\n';
    }
    // for (const auto& [id, box] : boxes) {
    //   std::cout << box << '\n';
    // }
  }

  // std::cout << "-------------------" << '\n';
  std::size_t total = 0;
  for (const auto [id, box] : boxes) {
    auto current = box.first;
    int slot = 1;
    while (current != "") {
      const auto& lens = box.lenses.at(current);
      // const auto temp = (id + 1) * slot * lens.focal_length;
      // std::cout << lens << ": " << temp << '\n';
      total += (id + 1) * slot * lens.focal_length;
      slot++;
      current = lens.next_lens_label;
    }
  }
  std::cout << total << '\n';
  return 0;
}