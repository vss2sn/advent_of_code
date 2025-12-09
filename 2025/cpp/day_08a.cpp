#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <vector>
#include <cmath>

struct Point3D {
    long long x;
    long long y;
    long long z;
};

struct Edge {
    int idx1;
    int idx2;
    long long weight;
};

Edge createEdge(const int idx1, const int idx2, const std::vector<Point3D>& points) {
    Edge e;
    e.idx1 = idx1;
    e.idx2 = idx2;
    const Point3D& p1 = points[idx1];
    const Point3D& p2 = points[idx2];
    e.weight = ((p1.x - p2.x) * (p1.x - p2.x)) + 
               ((p1.y - p2.y) * (p1.y - p2.y)) + 
               ((p1.z - p2.z) * (p1.z - p2.z));
    return e;
}

struct EdgeCompare {
    bool operator () (const Edge& e1, const Edge& e2) const {
        return e1.weight > e2.weight;
    }
};

int main(int argc, char* argv[])
{
    std::string input = "../input/day_08_input";
    if (argc > 1) {
        input = argv[1];
    }

    std::ifstream file(input);
    std::string line;
    std::vector<Point3D> points;
    while(std::getline(file, line)) {
        Point3D p;
        auto idx = line.find(',');
        p.x = std::stoi(line.substr(0, idx));
        auto idx2 = line.find(',', idx+1);
        p.y = std::stoll(line.substr(idx+1, idx2-idx-1));
        p.z = std::stoll(line.substr(idx2+1, line.size() - idx2 - 1));
        points.push_back(p);
    }

    std::priority_queue<Edge, std::vector<Edge>, EdgeCompare> pq;
    for (int idx1 = 0; idx1 < points.size(); idx1++) {
        for (int idx2 = idx1+1; idx2 < points.size(); idx2++) {
            pq.push(createEdge(idx1, idx2, points));
        }
    }

    std::vector<std::set<int>> sets_of_connected_points(points.size());
    std::vector<int> set_idx_of_point(points.size()); 
    for (int i = 0; i < set_idx_of_point.size(); i++) {
        set_idx_of_point[i] = i;
        sets_of_connected_points[i] = {i};
    }
    for (int i = 0; i < 1000; i++ && !pq.empty()) {
        const auto e = pq.top();
        pq.pop();
        if (set_idx_of_point[e.idx1] != set_idx_of_point[e.idx2]) {
            const auto prev_set_idx = set_idx_of_point[e.idx1];
            for (const auto ele : sets_of_connected_points[set_idx_of_point[e.idx1]]) {
                sets_of_connected_points[set_idx_of_point[e.idx2]].insert(ele);
                set_idx_of_point[ele] = set_idx_of_point[e.idx2];
            }
            sets_of_connected_points[prev_set_idx].clear();
        } 
    }

    std::partial_sort(
        std::begin(sets_of_connected_points), 
        std::begin(sets_of_connected_points)+3, 
        std::end(sets_of_connected_points), 
        [](const auto& s1, const auto& s2) {return s1.size() > s2.size(); });

    int ans = 1;
    for (int i = 0; i < 3; i++) {
        ans *= sets_of_connected_points[i].size();
    }
    std::cout << ans << '\n';
    return 0;
}