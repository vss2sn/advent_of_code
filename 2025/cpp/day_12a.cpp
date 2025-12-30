#include <charconv>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

struct Shape {
    int id;
    std::vector<std::string> display;
    int area;
};

struct Region {
    int dim1;
    int dim2;
    std::vector<int> count;
};

int main(int argc, char* argv[])
{
    std::string input = "../input/day_12_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::ifstream file(input);
    std::string line;

    std::vector<Shape> shapes;
    std::vector<Region> regions;
    int shape = -1;
    while(std::getline(file, line)) {
        if (line.back() == ':') {
            shapes.emplace_back();
            shapes.back().id = std::stoi(line.substr(0, line.size() - 1));
        } else if (line[0] == '.' || line[0] == '#') {
            shapes.back().display.push_back(line);
        } else if (line.find('x') != std::string::npos) {
            auto x_idx = line.find('x');
            auto colon_idx = line.find(':', x_idx + 1);
            regions.emplace_back();
            regions.back().dim1 = std::stoi(line.substr(0, x_idx));
            regions.back().dim2 = std::stoi(line.substr(x_idx+1, colon_idx - x_idx - 1));
            regions.back().count = line.substr(colon_idx+2, line.size() - colon_idx - 2) 
                                | std::views::split(' ') 
                                | std::views::transform([](const auto& view) {
                                    int value;
                                    std::from_chars(view.data(), view.data() + view.size(), value);
                                    return value;
                                })
                                | std::ranges::to<std::vector<int>>();
        }
    }
    for (auto& s : shapes) {
        s.area = 0;
        for (const auto& row : s.display) {
            for (const auto ele : row) {
                if (ele == '#') s.area++;
            }
        }
    }
    int n_regions_fit_presents = 0;
    for (const auto& r : regions) {
        int total_used_area = 0;
        for (auto [idx,  n] : std::views::enumerate(r.count)) {
            total_used_area += (shapes[idx].area * n);
        }
        if ((r.dim1 * r.dim2) - total_used_area >= 0) {
            n_regions_fit_presents++;
        }
    }
    std::cout << n_regions_fit_presents << '\n';
    return 0;
}