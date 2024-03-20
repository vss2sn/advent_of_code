#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <queue> 
#include <string>
#include <unordered_set>
#include <vector>
#include <cassert> 
#include <memory>

struct Module {
  std::string name;
  std::vector<std::string> to_modules;
  std::vector<std::string> from_modules;
  virtual std::vector<std::pair<std::string, bool>> evaluate(const std::string& source, const bool is_high) {
    return {};
  };
  virtual void setup() {};
}; 

struct FF : public Module {
  bool state = false;
  std::vector<std::pair<std::string, bool>>  evaluate(const std::string& source, const bool is_high) override {
    std::vector<std::pair<std::string, bool>> return_values;
    if (!is_high) {
      state = !state;
      for (const auto& to : to_modules) {
          // std::cout << "TO: " << to << '\n';
        return_values.emplace_back(to, state);
      }
    }
    return return_values;
  }
};

struct Conjunction : public Module {
  std::unordered_map<std::string, bool> state;
  std::vector<std::pair<std::string, bool>>  evaluate(const std::string& source, const bool is_high) override {
    state[source] = is_high;
    bool is_low = false;
    bool output_value = true;
    for (const auto& [name, value] : state) {
      if (!value) {
        is_low = true;
        break;
      }
    }
    if(!is_low) output_value = false;
    std::vector<std::pair<std::string, bool>> return_values;
    for (const auto& to : to_modules) {
      // std::cout << "TO: " << to << '\n';
      return_values.emplace_back(to, output_value);
    }
    return return_values;
  }
  void setup() override {
    for (const auto& from : from_modules) {
      state[from] = false;
    }
  }
};

struct Broadcaster : public Module {
  std::vector<std::pair<std::string, bool>>  evaluate(const std::string& source, const bool is_high) override {
    std::vector<std::pair<std::string, bool>> return_values;
    for (const auto& to : to_modules) {
      // std::cout << "TO: " << to << '\n';
      return_values.emplace_back(to, is_high);
    }
    return return_values;
  }
};

std::unique_ptr<Module> create_module (const std::string& name, const std::vector<std::string>& connected_to) {  
  std::unique_ptr<Module> m_ptr;
  if(name[0] == '%') {
    FF m;
    m.name = name.substr(1, name.size() - 1);
    m.to_modules = connected_to;
    m_ptr = std::make_unique<FF>(m);
  } else if(name[0] == '&') {
    Conjunction m;
    m.name = name.substr(1, name.size() - 1);
    m.to_modules = connected_to;
    m_ptr = std::make_unique<Conjunction>(m);
  } else if (name == "broadcaster") {
    Broadcaster m;
    m.name = "broadcaster";
    m.to_modules = connected_to;
    m_ptr = std::make_unique<Broadcaster>(m);
  } 
  return m_ptr;
}

std::vector<std::string> extract_connected_to(const std::string& s) {
  std::vector<std::string> substrs;

  std::size_t start = 0;
  std::size_t end = s.find(',', start);
  while (end != std::string::npos) {
    substrs.push_back(s.substr(start, end - start));
    start = end + 2;
    end = s.find(',', start);
  }
  substrs.push_back(s.substr(start, s.size() - start));
  return substrs;
}

std::pair<std::string, std::vector<std::string>> parse (const std::string& line) {
  const auto idx = line.find(' ');
  return {line.substr(0, idx), extract_connected_to(line.substr(idx+4, line.size() - idx - 4))};
}

struct Pulse {
  std::string from;
  std::string to;
  bool value;
};

int main(int argc, char * argv[]) {
  std::string input = "../input/day_20_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  std::unordered_map<std::string, std::unique_ptr<Module>> modules;
  while(std::getline(file, line)) {
    const auto [name_with_type, connected_to] = parse (line);
    auto tmp = create_module(name_with_type, connected_to);
    modules[tmp->name] = std::move(tmp);
    // std::cout << __LINE__ << '\n';
  }
   
  for (const auto& [name, module] : modules) {
    for (const auto& to : module->to_modules) {
      if (modules.find(to) == modules.end()) {
        Module m;
        m.name = to;
        modules[to] = std::make_unique<Module>(std::move(m));
      }
      // std::cout << __LINE__ << '\n';
      modules[to]->from_modules.push_back(module->name);  
      // std::cout << __LINE__ << '\n';
    }
  }

  std::unordered_map<std::string, std::size_t> periods;
  // By inspection it can be seen that rx needs to receive a single pulse from bb (the only module it is connected to)
  // Hence bb needs to receive and remember high pulses from the modules it is connected to 
  // Debug: bb is connected to:
  for (const auto& ele :  modules[modules["rx"]->from_modules[0]]->from_modules ) {
  //   std::cout << ele << " | ";
   periods[ele] = 0; 
  }
  // std::cout << '\n';
  for (const auto& [name, module] : modules) {
    module->setup();
  }

  std::size_t low_count = 0;
  std::size_t high_count = 0;
  int count = 0;
  for (int i = 0; i < 100000000; i++) {
    // std::cout << "Press button" << '\n';
    std::queue<Pulse> to_process; 
    {
      Pulse p;
      p.from = "button";
      p.to = "broadcaster";
      p.value = false;
      to_process.push(p);
    }
    while (!to_process.empty()) {
      const auto pulse = to_process.front();
      to_process.pop();
      // std::cout << "Processing " << pulse.from <<" -" << (pulse.value ? "high" : "low") << "-> " << pulse.to << '\n';
      if(pulse.value) {
        high_count++;
      } else {
        low_count++;
      }
      if (pulse.value && (periods.find(pulse.from) != periods.end())) {
        if (periods[pulse.from] == 0) {
          periods[pulse.from] = i;
          count++;
        }
        if (count == 4) {
          std::size_t ans = 1;
          for (auto& [name, val] : periods) {
            val+=1; // Since pulse count (i) started from 0
            ans = std::lcm(ans, val);
          }
          std::cout << ans << '\n';
          return 0;
        }
      }

      const auto modules_to_process = modules[pulse.to]->evaluate(pulse.from, pulse.value);
      for (const auto module_to_process : modules_to_process) {
        // std::cout << module_to_process.first << '\n';
        Pulse new_pulse;
        new_pulse.from = pulse.to;
        new_pulse.to = module_to_process.first;
        new_pulse.value = module_to_process.second;
        to_process.push(new_pulse);
      }
    }
  }
  return 0;
}