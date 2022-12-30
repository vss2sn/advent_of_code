#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

// TODO: Clean up
//       Add const correctness

// #include <chrono>
// #include <cassert>
// #include <thread>
// using namespace std::chrono_literals;

int face_size = 0;

template<typename T>
void print(const std::vector<std::vector<T>>& values) {
  for (const auto& row : values) {
    for (const auto& ele : row) {
      std::cout << ele << ' ';
    }
    std::cout << '\n';
  }
  std::cout << '\n';
}

template<typename T>
void print(const T& values) {
  for (const auto& ele : values) {
    std::cout << ele << '\n';
  }
  std::cout << '\n';
}


// faces arranges so opposites sum to 5
enum Faces {
  FRONT = 0,
  RIGHT,
  TOP,
  BOTTOM,
  LEFT,
  BACK,
  NONE
};

std::unordered_map<Faces, std::string> enum_to_str {
  {FRONT, "FRONT "},
  {RIGHT, "RIGHT "},
  {TOP,   "TOP   "},
  {BOTTOM,"BOTTOM"},
  {LEFT,  "LEFT  "},
  {BACK,  "BACK  "},
  {NONE,  "NONE  "}
};

enum Direction {
  EAST = 0,
  SOUTH,
  WEST,
  NORTH
};

struct Face {
  Face (const Faces id, const std::array<int, 4>& vertices, const int rotation) : id(id), vertices(vertices), rotation(rotation) {}
  Faces id;
  std::array<int, 4> vertices;
  int rotation;
};

struct Edge {
  Edge (const int v1, const int v2, const std::array<std::pair<Faces, Direction>, 2>& face_dirs, const int delta_rotation) : v1(v1), v2(v2), face_dirs(face_dirs), delta_rotation(delta_rotation) {}
  int v1;
  int v2;
  std::array<std::pair<Faces, Direction>, 2> face_dirs; // faing paris point in the drirection of the facing
  int delta_rotation; // change in direction when continmuing to move int hesame way between 2 faces sharing an edge
};

// Defines an edge in relation to the 2 faces it connects. 
// So the edge between the FRONT face and the BOTTOM face is the 
// SOUTH edge for the FRONT face and the 
// NORTH edge for the bottom face
std::vector<Edge> cube {
  Edge(5, 6, {{ {Faces::FRONT, Direction::SOUTH}, {Faces::BOTTOM, Direction::NORTH}}}, 0),
  Edge(6, 7, {{ {Faces::RIGHT, Direction::SOUTH}, {Faces::BOTTOM, Direction::EAST}}}, 1),
  Edge(7, 8, {{ {Faces::BACK, Direction::SOUTH}, {Faces::BOTTOM, Direction::SOUTH}}}, 2),
  Edge(8, 5, {{ {Faces::LEFT, Direction::SOUTH}, {Faces::BOTTOM, Direction::WEST}}}, 3),
  Edge(1, 2, {{ {Faces::FRONT, Direction::NORTH}, {Faces::TOP, Direction::SOUTH}}}, 0),
  Edge(2, 3, {{ {Faces::RIGHT, Direction::NORTH}, {Faces::TOP, Direction::EAST}}}, 3),
  Edge(3, 4, {{ {Faces::BACK, Direction::NORTH}, {Faces::TOP, Direction::NORTH}}}, 2),
  Edge(4, 1, {{ {Faces::LEFT, Direction::NORTH}, {Faces::TOP, Direction::WEST}}}, 1),
  Edge(2, 6, {{ {Faces::FRONT, Direction::EAST}, {Faces::RIGHT, Direction::WEST}}}, 0),
  Edge(3, 7, {{ {Faces::RIGHT, Direction::EAST}, {Faces::BACK, Direction::WEST}}}, 0),
  Edge(4, 8, {{ {Faces::BACK, Direction::EAST}, {Faces::LEFT, Direction::WEST}}}, 0),
  Edge(1, 5, {{ {Faces::LEFT, Direction::EAST}, {Faces::FRONT, Direction::WEST}}}, 0),
};


// {{  top left corner, top right corner, bottom right corner, bottom left corner}}
// faces arranges so opposites sum to 5, though this is not currently used
constexpr std::array<std::array<int, 4>, 7> face_to_vertices {{
    {{ 1, 2, 6, 5 }}, // front
    {{ 2, 3, 7, 6 }}, // right
    {{ 4, 3, 2, 1 }}, // top
    {{ 5, 6, 7, 8 }}, // bottom
    {{ 4, 1, 5, 8 }}, // left
    {{ 3, 4, 8, 7 }}, // back
    {{ 0, 0, 0, 0}}   // None
}};

std::pair<Face, std::array<int, 4>> set_points_by_direction (const std::vector<Edge> & cube, const Face known_face,
  std::vector<std::vector<int>>& vertex_grid, const int v1, const int v2) {
  Faces new_face_id = Faces::NONE;
  Direction new_dir = Direction::EAST;
  int d_rot = -1;
  int dd = 0;
  for (const auto& edge : cube) {
    if ((v1 == edge.v1 && v2 == edge.v2) || (v1 == edge.v2 && v2 == edge.v1)) {
      if (edge.face_dirs[0].first == known_face.id) {
        new_face_id = edge.face_dirs[1].first;
        new_dir = edge.face_dirs[1].second;
        d_rot = edge.delta_rotation + edge.face_dirs[1].second - 1;
        dd = edge.delta_rotation;
      } else {
        new_face_id = edge.face_dirs[0].first;
        new_dir = edge.face_dirs[0].second;
        d_rot = -edge.delta_rotation + edge.face_dirs[0].second - 1;
        dd = -edge.delta_rotation;
      }
      break;
    }
  }
  auto new_vertices = face_to_vertices[new_face_id];
  int count = 0;

  while (!(new_vertices[0] == v1 && new_vertices[1] == v2)) {
    std::rotate(std::begin(new_vertices), std::next(std::begin(new_vertices), 1), std::end(new_vertices));
    count++;
    if (count >= 4) {
      std::cout << "This should not happen" << '\n';
      exit(0);
    }
  }
  // exit(0);
  // std::cout << enum_to_str[new_face_id] << '\n';
  // std::cout << d_rot << ' ' << known_face.rotation << ' ' << dd << ' ' << count << '\n';
  // std::cout << "Deduced " << enum_to_str[new_face_id] << " from " << enum_to_str[known_face.id] << '\n';
  return {Face(new_face_id, face_to_vertices[new_face_id], (dd+known_face.rotation+4) % 4), new_vertices};
}

// State
struct State {
  State(const int row = 0, const int col = 0, const Faces face = Faces::NONE, const int dir = 0) :
        row(row), col(col), face(face), dir(dir) {}
  
  int row = 0;
  int col = 0;
  Faces face = Faces::NONE;
  int dir = 0;

  bool operator == (const State& p) const {
    return p.row == row && p.col == col && p.face == face && p.dir == dir;
  }

  friend std::ostream& operator<<(std::ostream& os, const State& p);
};

std::ostream& operator<<(std::ostream& os, const State& p) {
  os << '(' << p.row << ' ' << p.col << ' ' << enum_to_str[p.face] << ' ' << p.dir << ')';
  return os;
}

// Transforms for each edge (12) that can be crossed, once for each direction from which the edge is crossed (2)
// Total transforms = 12 * 2 = 24
// TODO: Convert this to a single line transform, though keep this commented for reference
// Every cube is always transformed to the followwing form
//      TOP
// LEFT FRONT RIGHT BACK
//      BOTTOM
State cube_transforms(const State& s) {
  if (s.face == Faces::FRONT  && s.dir == Direction::SOUTH) return State(0, s.col, Faces::BOTTOM, Direction::SOUTH);
  if (s.face == Faces::BOTTOM && s.dir == Direction::NORTH) return State(face_size-1, s.col, Faces::FRONT, Direction::NORTH);;
  if (s.face == Faces::RIGHT  && s.dir == Direction::SOUTH) return State(s.col, face_size-1, Faces::BOTTOM, Direction::WEST);;
  if (s.face == Faces::BOTTOM && s.dir == Direction::EAST ) return State(face_size-1, s.row, Faces::RIGHT, Direction::NORTH);

  if (s.face == Faces::BACK   && s.dir == Direction::SOUTH) return State(face_size-1, face_size - 1 - s.col, Faces::BOTTOM, Direction::NORTH);
  if (s.face == Faces::BOTTOM && s.dir == Direction::SOUTH) return State(face_size-1, face_size - 1 - s.col, Faces::BACK, Direction::NORTH);
  if (s.face == Faces::LEFT   && s.dir == Direction::SOUTH) return State(face_size - 1 - s.col, 0, Faces::BOTTOM, Direction::EAST);
  if (s.face == Faces::BOTTOM && s.dir == Direction::WEST ) return State(face_size-1, face_size - 1 - s.row, Faces::LEFT, Direction::NORTH);

  if (s.face == Faces::FRONT  && s.dir == Direction::NORTH) return State(face_size-1, s.col, Faces::TOP, Direction::NORTH);
  if (s.face == Faces::TOP    && s.dir == Direction::SOUTH) return State(0, s.col, Faces::FRONT, Direction::SOUTH);
  if (s.face == Faces::RIGHT  && s.dir == Direction::NORTH) return State(face_size-1 - s.col, face_size-1, Faces::TOP, Direction::WEST);
  if (s.face == Faces::TOP    && s.dir == Direction::EAST ) return State(0, face_size-1 - s.row, Faces::RIGHT, Direction::SOUTH);

  if (s.face == Faces::BACK   && s.dir == Direction::NORTH) return State(0, face_size-1 - s.col, Faces::TOP, Direction::SOUTH);
  if (s.face == Faces::TOP    && s.dir == Direction::NORTH) return State(0, face_size-1 - s.col, Faces::BACK, Direction::SOUTH);
  if (s.face == Faces::LEFT   && s.dir == Direction::NORTH) return State(s.col , 0, Faces::TOP, Direction::EAST);
  if (s.face == Faces::TOP    && s.dir == Direction::WEST ) return State(0, s.row,Faces::LEFT, Direction::SOUTH);

  if (s.face == Faces::FRONT  && s.dir == Direction::EAST ) return State(s.row, 0, Faces::RIGHT, Direction::EAST);
  if (s.face == Faces::RIGHT  && s.dir == Direction::WEST ) return State(s.row, face_size-1, Faces::FRONT, Direction::WEST);
  if (s.face == Faces::RIGHT  && s.dir == Direction::EAST ) return State(s.row, 0, Faces::BACK, Direction::EAST);
  if (s.face == Faces::BACK   && s.dir == Direction::WEST ) return State(s.row, face_size-1, Faces::RIGHT, Direction::WEST);

  if (s.face == Faces::BACK   && s.dir == Direction::EAST ) return State(s.row, 0, Faces::LEFT, Direction::EAST);
  if (s.face == Faces::LEFT   && s.dir == Direction::WEST ) return State(s.row, face_size-1, Faces::BACK, Direction::WEST);
  if (s.face == Faces::LEFT   && s.dir == Direction::EAST ) return State(s.row, 0, Faces::FRONT, Direction::EAST);
  if (s.face == Faces::FRONT  && s.dir == Direction::WEST ) return State(s.row, face_size-1, Faces::LEFT, Direction::WEST);

  std::cout << "This should not occur" << '\n';
  exit(0);

  return State(-1, -1, Faces::NONE, Direction::EAST);
};

std::tuple<State, std::vector<std::vector<std::string>>, std::unordered_map<Faces, State>> create_grid (const std::vector<std::string>& grid_2d) {
  
  const auto ratio_f = float(grid_2d.size()) / float(grid_2d[0].size());
  int normalize = 1;
  while (ratio_f * normalize != int(ratio_f * normalize)) normalize++;
  const int ratio = normalize * ratio_f;
  const int n_faces_in_col = normalize * ratio_f; // n_rows
  const int n_faces_in_row = normalize; // n_cols
  face_size = grid_2d[0].size() / n_faces_in_row;
  
  // Scale the input grid down to 6 squares of unit length, where 1 represents a square, and 0 is empty space
  auto reduced_grid = std::vector<std::vector<int>>(n_faces_in_col, std::vector<int>(n_faces_in_row, 0));

  // Map each face of the cube to the reduced grid
  std::unordered_map<Faces, State> face_to_reduced_grid;

  for (int row = 0; row < n_faces_in_col; row++) {
    for (int col = 0; col < n_faces_in_row; col++) {
      if (grid_2d[row * face_size][col * face_size] != ' ') {
        reduced_grid[row][col] = 1;
      }
    }
  }

  // print(reduced_grid);
  // New grid == reduced_grid with named faces in place of the 1s in the reduced grid and 
  // Faces::NONE in place of the 0s
  // It needs to be figured out
  auto new_grid = std::vector<std::vector<Face>>(reduced_grid.size(), std::vector<Face>(reduced_grid[0].size(), Face(Faces::NONE, {{-1,-1,-1,-1}}, 0)));
  
  // Vertex grid is a grid of dimension + 1 for each dimension of new grid and shows the position of each vextex (1-8)
  auto vertex_grid = std::vector<std::vector<int>>(reduced_grid.size()+1, std::vector<int>(reduced_grid[0].size()+1, -1));
  bool set_start = false;

  // Set in a way that the topmost row always contains the "TOP" face,
  // then dont need to worry about unfolding the back face on top of the top face
  // So every cube is always transformed into:
  //      TOP
  // LEFT FRONT RIGHT BACK
  //      BOTTOM
  for (int row = 0; row < reduced_grid.size() && !set_start; row++) {
    for (int col = 0; col < reduced_grid[0].size() && !set_start; col++) {
      if (reduced_grid[row][col] && new_grid[row][col].id == Faces::NONE) {
        new_grid[row][col] = Face(Faces::TOP, face_to_vertices[Faces::TOP], 0);
        face_to_reduced_grid[Faces::TOP] = State(row, col, Faces::TOP, 0);
        vertex_grid[row][col] = face_to_vertices[Faces::TOP][0];
        vertex_grid[row][col+1] = face_to_vertices[Faces::TOP][1];
        vertex_grid[row+1][col+1] = face_to_vertices[Faces::TOP][2];
        vertex_grid[row+1][col] = face_to_vertices[Faces::TOP][3];
        set_start = true;
      }
    }
  }

  // TODO: Optimize this
  int count = 1;
  while(count < 6) {
    for (int row = 0; row < reduced_grid.size(); row++) {
      for (int col = 0; col < reduced_grid[0].size(); col++) {
        if (reduced_grid[row][col] && new_grid[row][col].id == Faces::NONE) {
          if (row > 0 && reduced_grid[row - 1][col] && new_grid[row-1][col].id != Faces::NONE) {
            const auto [face, pts] = set_points_by_direction (cube, new_grid[row-1][col], vertex_grid, vertex_grid[row][col], vertex_grid[row][col+1]);
            new_grid[row][col] = face;
            face_to_reduced_grid[face.id] = State(row, col, face.id, 0);
            vertex_grid[row+1][col+1] = pts[2];
            vertex_grid[row+1][col] = pts[3];
            count++;
          } else if (col > 0 && reduced_grid[row][col-1] && new_grid[row][col-1].id != Faces::NONE) {
            const auto [face, pts] = set_points_by_direction (cube, new_grid[row][col-1], vertex_grid, vertex_grid[row+1][col], vertex_grid[row][col]);
            face_to_reduced_grid[face.id] = State(row, col, face.id, 0);
            new_grid[row][col] = face;
            vertex_grid[row][col+1] = pts[2];
            vertex_grid[row+1][col+1] = pts[3];
            count++;
          } else if (row < reduced_grid.size()-1 && reduced_grid[row + 1][col] && new_grid[row+1][col].id != Faces::NONE) {
            const auto [face, pts] = set_points_by_direction (cube, new_grid[row+1][col], vertex_grid, vertex_grid[row+1][col+1], vertex_grid[row+1][col]);
            face_to_reduced_grid[face.id] = State(row, col, face.id, 0);
            new_grid[row][col] = face;
            vertex_grid[row][col] = pts[2];
            vertex_grid[row][col+1] = pts[3];
            count++;
          } else if (col < reduced_grid[0].size() && reduced_grid[row][col+1] && new_grid[row][col+1].id != Faces::NONE) {
            const auto [face, pts] = set_points_by_direction (cube, new_grid[row][col+1], vertex_grid, vertex_grid[row][col+1], vertex_grid[row+1][col+1]);
            face_to_reduced_grid[face.id] = State(row, col, face.id, 0);
            new_grid[row][col] = face;
            vertex_grid[row+1][col] = pts[2];
            vertex_grid[row][col] = pts[3];
            count++;
          }
        }
      }
    }
    // Print 2d grid with the names of faces
    // std::cout << '\n';
    // for (const auto& row : new_grid) {
    //   for (const auto ele : row) {
    //     std::cout << enum_to_str[ele.id] << ' ';
    //   }
    //   std::cout << '\n';
    // }
    // std::cout << '\n';
    //
    // // Print 2d grid with the vertices at their assigned positions
    // std::cout << '\n';
    // for (const auto& row : vertex_grid) {
    //   for (const auto ele : row) {
    //     if (ele == -1) {
    //       std::cout << '-' << ' ';
    //     } else {
    //       std::cout << ele << ' ';
    //     }
    //   }
    //   std::cout << '\n';
    // }
    // std::cout << '\n';
    //
  }

  // 6 sides represented as 6 grids
  auto start = State();
  start.row = 0;
  start.dir = Direction::EAST;
  start.face = Faces::TOP;

  std::vector<std::vector<std::string>> layered_grid (6, std::vector<std::string>(face_size, std::string(face_size, ' ')));
  int min_index = face_size;
  for (int i = 0; i < 6; i++) {
    bool set = false;
    for (int row = 0; row < new_grid.size() && !set; row++) {
      const auto row_start = row * face_size;
      for (int col = 0; col < new_grid[0].size() && !set; col++) {
        if (new_grid[row][col].id == static_cast<Faces>(i)) {
          const auto col_start = col * face_size;
          for (int j = 0; j < face_size; j++) {
            for (int k = 0; k < face_size; k++) {
              layered_grid[i][j][k] = grid_2d[row_start + j][col_start + k];
              if (row_start == 0 && k < min_index && grid_2d[row_start + j][col_start + k] == '.') {
                min_index = k;
              }
            }
          }
          set = true;
          // std::cout << "----------------------------" << '\n';
          for (int kk = 0; kk < (new_grid[row][col].rotation + 4) % 4; kk++) {
            // std::cout << "Rotated: " << kk << '\n';
            auto temp = layered_grid[i];
            for (int ii = 0; ii < temp.size(); ii++) {
              for (int jj = 0; jj < temp[0].size(); jj++) {
                layered_grid[i][jj][temp.size() - ii - 1] = temp[ii][jj];
              }
            }
          }
          // std::cout <<  enum_to_str[static_cast<Faces>(i)] << '\n';
          // std::cout << (new_grid[row][col].rotation + 4) % 4 << '\n';
          // print(layered_grid[i]);
        }
      }
    }
  }
  start.col = min_index;
  return {start, layered_grid, face_to_reduced_grid};
}

const std::array<State, 4> deltas {{
  State( 0,  1, Faces::NONE, 0),
  State( 1,  0, Faces::NONE, 1),
  State( 0, -1, Faces::NONE, 2),
  State(-1,  0, Faces::NONE, 3)
}};

bool moved_out_of_bounds(const std::vector<std::vector<std::string>>& grid, const State& s) {
  return s.row < 0 || s.col < 0 || s.row >= grid[0].size() || s.col >= grid[0][0].size();
}

State move(const std::vector<std::vector<std::string>>& grid, const State& start, const int n) {
  int i = 0;
  auto current = start;
  while (i < n) {
    // std::cout << current << '\n';
    // std::this_thread::sleep_for(1000ms);
    const auto prev_acceptable = current;
    const auto delta = deltas[current.dir];
    current.row += delta.row;
    current.col += delta.col;
    if (moved_out_of_bounds(grid, current)) {
      current = cube_transforms(current);
    }
    if(grid[current.face][current.row][current.col] == '#') {
      // std::cout << "Obstacle; reverting" << '\n';
      current = prev_acceptable;
      break;
    }
    // std::cout << current << "(Move " << i << ")" << '\n';
    i++;
  }
  return current;
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_22_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  // 2d grid (ie input map)
  std::vector<std::string> grid;

  while(std::getline(file, line)) {
    if (line == "") break;
    grid.push_back(line);
  }
  std::size_t max_len = 0;
  for (const auto& row : grid) {
    max_len = std::max(max_len, row.size());
  }
  for (auto& row : grid) {
    if (row.size() < max_len) {
      row.resize(max_len, ' ');
    }
    // std::cout << '|' << row << '|' << '\n';
  }

  std::getline(file, line);

  const std::string path = line;
  // std::cout << line << '\n';
  std::vector<std::size_t> direction_indixes;
  for (int i = 0; i < path.size(); i++) {
    if (path[i] == 'L' || path[i] == 'R') {
      direction_indixes.push_back(i);
    }
  }

  const auto [start, layered_grid, face_to_reduced_grid] = create_grid(grid);
  // std::cout << start << '\n';
  std::size_t start_i = 0;
  auto current = start;
  for (int i = 0; i < direction_indixes.size(); i++) {
    const int delta_val = std::stoi(path.substr(start_i, direction_indixes[i] - start_i));
    // std::cout << delta_val << '\n';
    // std::cout << "Starting move from: " << current << '\n';
    current = move(layered_grid, current, delta_val);
    // std::cout <<  "Now at: " << current << '\n';
    const char delta_dir = path[direction_indixes[i]];
    current.dir += (delta_dir == 'R' ? 1 : -1);
    current.dir += 4;
    current.dir %= 4;
    // std::cout << current << '\n';
    start_i = direction_indixes[i]+1;
  }
  if (direction_indixes.back() != path.size()) {
    const int delta_val = std::stoi(path.substr(start_i, path.size() - start_i));
    current = move(layered_grid, current, delta_val);
  }
  const auto current_face = face_to_reduced_grid.find(current.face)->second;
  std::cout << 1000 * (current_face.row * face_size + current.row+1)  + 4 * (current_face.col * face_size + current.col+1) + current.dir << '\n';
  return 0;
}
