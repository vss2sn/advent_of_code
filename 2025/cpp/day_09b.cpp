#include <array>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <vector>

// largest_rectangle() {
//     std::size_t largest_rectangle = 0;
//     for (const auto& p1 : points) {
//         for (const auto& p2 : points) {
//             largest_rectangle = std::max(
//                 largest_rectangle, 
//                 static_cast<std::size_t>(
//                     (std::abs(p1[0] - p2[0])+1) * 
//                     (std::abs(p1[1] - p2[1])+1)));
//         }
//     }    
//     std::cout << largest_rectangle << '\n';
// }

// maintain order of points as they show linkage order
std::vector<std::array<int, 2>> compressCooordinates(const std::vector<std::array<long long, 2>>& points) {
    // deduplicate and sort by using a set
    std::set<int> cols;
    std::set<int> rows;
    std::vector<std::array<int, 2>> compressed_points;
    compressed_points.reserve(points.size());
    for (const auto& p : points) {
        rows.insert(p[0]);
        cols.insert(p[1]);
    }
    for (const auto& p : points) {
        compressed_points.push_back(
            std::array<int, 2> {
                std::distance(std::begin(rows), rows.find(p[0])),
                std::distance(std::begin(cols), cols.find(p[1]))
            }
        );
    }
    // std::cout << __LINE__ << '\n';
    return compressed_points;
}

// can be optimized as using compressed points
std::vector<std::vector<int>> createMap (const std::vector<std::array<int, 2>>& points) {
    const auto col_max = (*std::max_element(std::begin(points), std::end(points), [](const auto& p1, const auto& p2) {return p1[1] < p2[1];}))[1];
    const auto row_max = (*std::max_element(std::begin(points), std::end(points), [](const auto& p1, const auto& p2) {return p1[0] < p2[0];}))[0];
    // std::cout << "max: " << row_max << ' ' << col_max << '\n';
    auto map = std::vector<std::vector<int>>(row_max+1, std::vector<int>(col_max+1, 0));
    for (int i = 1; i < points.size(); i++) {
        const auto d_r = points[i][0] - points[i-1][0];
        const auto d_c = points[i][1] - points[i-1][1];
        const auto delta = std::array<int, 2>{
            (d_r != 0) ? d_r/std::abs(d_r) : 0,
            (d_c != 0) ? d_c/std::abs(d_c) : 0,
        };
        auto current = points[i-1];
        // std::cout << "p1: " << points[i-1][0] << ' ' << points[i-1][1] << '\n';
        // std::cout << "p2: " << points[i][0] << ' ' << points[i][1] << '\n';
        // std::cout << "delta: " << delta[0] << ' ' << delta[1] << '\n';
        while (current != points[i]) {
            // std::cout << current[0] << ' ' << current[1] << '\n';
            map[current[0]][current[1]] = 1;
            current[0] +=  delta[0];
            current[1] +=  delta[1];
        }
    }
    return map;
}

void floodFill(std::vector<std::vector<int>>& map) {
    //get starting point 
    std::array<int, 2> p{0,0};
    {
        int idx = 0;
        while(map[idx][0] == 0) {
            idx++;
        }
        p = {idx+1, 1};
    }
    std::queue<std::array<int, 2>> q;
    q.push(p);
    const std::vector<std::array<int,2>> adj_delta {
        {1,0},
        {0,-1},
        {0,1},
        {-1,0}
    };

    while(!q.empty()) {
        const auto c = q.front();
        // std::cout << c[0] << ' ' << c[1] << ' ' << map[c[0]][c[1]] << '\n';
        q.pop();
        map[c[0]][c[1]] = 2;
        // std::cout << c[0] << ' ' << c[1] << ' ' << map[c[0]][c[1]] << '\n';
        for (const auto& d : adj_delta) {
            const auto n = std::array<int, 2>{c[0]+d[0], c[1]+d[1]};
            // std::cout << n[0] << ' ' << n[1] << '\n';
            if (n[0] >= 0 && n[0] < map.size() && 
                n[1] >= 0 && n[1] < map[0].size() && 
                map[n[0]][n[1]] == 0) {
                q.push(n);
                map[n[0]][n[1]] = -1;
            } else {
                // std::cout << "not in range" << '\n';
            }
        }
    }
}

bool isValidRectangle(
    const std::vector<std::vector<int>>& map, 
    const std::array<int, 2>& p1, 
    const std::array<int, 2>& p2) {
    for (int r = std::min(p1[0], p2[0]); r <= std::max(p1[0], p2[0]); r++) {
        for (int c = std::min(p1[1], p2[1]); c <= std::max(p1[1], p2[1]); c++) {
            if (map[r][c] == 0) return false;
        }
    }
    return true;
}

std::size_t findLargestRectangle(
    const std::vector<std::vector<int>>& map, 
    const std::vector<std::array<int, 2>>& compressed_points, 
    const std::vector<std::array<long long, 2>>& points) {
    std::size_t area = 0;
    for (int i = 0;  i < points.size(); i++) {
        for (int j = 0;  j < points.size(); j++) {
            if(isValidRectangle(map, compressed_points[i], compressed_points[j])) {
                area = std::max(
                    area, 
                    static_cast<std::size_t>(std::abs((points[i][0] - points[j][0]) + 1) * (std::abs(points[i][1] - points[j][1]) + 1))
                );
            }
        }
    }
    return area;
}

int main(int argc, char* argv[])
{
    std::string input = "../input/day_09_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::string line;
    std::ifstream file(input);
    std::vector<std::array<long long, 2>> points;
    while(std::getline(file, line)) {
        auto idx = line.find(',');
        const auto row = std::stoll(line.substr(0, idx));
        const auto col = std::stoll(line.substr(idx + 1, line.size() - idx - 1));
        points.push_back(std::array<long long, 2>{row, col});
    }
    points.push_back(points[0]); // Close the polygon
    const auto compressed_points = compressCooordinates(points);
    auto map = createMap(compressed_points);
    floodFill(map);
    std::cout << findLargestRectangle(map, compressed_points, points) << '\n';
    // for (const auto& row : map) {
    //     for (const auto& col : row) {
    //         std::cout << col; 
    //     }
    //     std::cout << '\n';
    // }
    return 0;
}