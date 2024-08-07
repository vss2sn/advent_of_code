#include <fstream>
#include <iostream>
#include <string>
#include <vector>  
#include <unordered_map>  
#include <optional>   

std::optional<uint16_t> get_value(const std::string& input, const std::unordered_map<std::string, uint16_t>& vars) {
  if (std::isdigit(input[0])) return std::stoi(input);
  const auto it = vars.find(input);
  return (it == vars.end()) ? std::nullopt : std::optional<uint16_t>(it->second);
}

int main(int argc, char* argv[]) {
  std::string input = "../input/day_07_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::ifstream file(input);
  std::string line;
  std::unordered_map<std::string, uint16_t> vars;
  std::vector<std::vector<std::string>> connections;
  while(std::getline(file, line)) {
    std::vector<std::string> connection;
    const std::string delimiter = " ";
    size_t start = 0;
    size_t end = line.find(delimiter);
    while (end != std::string::npos) {
      connection.push_back(line.substr(start, end - start));
      start = end + 1;
      end = line.find(delimiter, start);
    }
    connection.push_back(line.substr(start, line.size() - start));
    connections.push_back(connection);
  }
  // This can be made more efficient, 
  // can construct the circuit rather than 
  // running the loop until the value of a is set
  // which runs the instructions multiple times
  while(vars.find("a") == vars.end()) {
    for (const auto connection : connections){
        std::optional<uint16_t> val1 = 0;
        std::optional<uint16_t> val2 = 0;
        if (connection.size() == 3) {
            val1 = get_value(connection[0], vars);
            if (val1) {
              vars[connection[2]] = val1.value();
            }
        }
        else if (connection.size() == 4 && connection[0] == "NOT") {
            val1 = get_value(connection[1], vars);
            if (val1) {
              vars[connection[3]] = ~val1.value();
            }
        }
        else if (connection.size() == 5 && connection[1] == "AND") {
            val1 = get_value(connection[0], vars);
            val2 = get_value(connection[2], vars);
            if (val1 && val2)  vars[connection[4]] = val1.value() & val2.value();
        }
        else if (connection.size() == 5 && connection[1] == "OR") {
          val1 = get_value(connection[0], vars);
          val2 = get_value(connection[2], vars);
          if (val1 && val2)  vars[connection[4]] = val1.value() | val2.value();
        }
        else if (connection.size() == 5 && connection[1] == "LSHIFT") {
          val1 = get_value(connection[0], vars);
          val2 = get_value(connection[2], vars);
          if (val1 && val2)  vars[connection[4]] = val1.value() << val2.value();
        }
        else if (connection.size() == 5 && connection[1] == "RSHIFT") {
          val1 = get_value(connection[0], vars);
          val2 = get_value(connection[2], vars);
          if (val1 && val2)  vars[connection[4]] = val1.value() >> val2.value();
        }
        else {
            std::cout << line << '\n';
            std::cout << "This should not happen" << '\n';
        }
    }
  }
  std::cout << vars["a"] << '\n';
  return 0;
}