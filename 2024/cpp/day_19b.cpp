#include <fstream>
#include <iostream> 
#include <string>
#include <unordered_set> 
#include <unordered_map> 
#include <vector> 

// Memonization

long long check_if_buildable(const std::string s, const std::unordered_set<std::string>& primitives, const int depth, const int max_size) {
    static std::unordered_map<std::string, long long> memory;
    if (s.empty()) return 1;
    long long count = 0;
    for (int i = max_size; i >= 1; i--) {
        if (s.size() < i) continue;
        if (primitives.contains(s.substr(0,i))) {
            const auto remainder = s.substr(i, s.size() - i);
            if (!memory.contains(remainder)) {
                memory[remainder] = check_if_buildable(remainder, primitives, depth + 1, max_size);
            }
            count += memory[remainder];
        }
    }
    return count;
}

int main(int argc, char* argv[]) {
    std::string input = "../input/day_19_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::ifstream file(input);
    std::string line;
    std::unordered_set<std::string> primitives;
    std::getline(file, line);
    std::size_t start = 0;
    std::size_t end = line.find(", ");
    std::size_t max_size = 0;
    while(end != std::string::npos) {
        const auto primitive = line.substr(start, end - start);
        primitives.insert(primitive);
        max_size = std::max(max_size, primitive.size());
        start = end + 2;
        end = line.find(", ", start);
    }
    primitives.insert(line.substr(start, end - start));
    std::getline(file, line);
    long long count = 0;
    while(std::getline(file,line)) {
        count += check_if_buildable(line, primitives, 0, max_size);
    }
    std::cout << count << '\n';;

    return 0;
}