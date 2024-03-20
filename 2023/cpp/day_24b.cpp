#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <limits>
#include <queue>
#include <regex> 
#include <stack>
#include <string>
#include <unordered_set>
#include <vector>
#include <cassert> 

// Let (x0, y0, z0) and (vx0, vy0, vz0) be the position from and velocity with which the rock must be thrown
// Let (xi, yi, zi) and (vxi, vyi, vzi) be the position and velocity of the ith hailstone, where i ranges from 1 to some `n`
// hence there exists a time ti for every hailstone where the hailstone and the rock are at exactly the same position
// hence 
// x0 + vx0 * ti = xi + vxi * ti ... (1)
// => (x0 - xi) = - (vx0 - vxi) * ti 
// => (x0 - xi) / (vx0 - vxi) = -ti ... (2) (Assuming vx0 != vxi)
// Similarly
// (y0 - yi) / (vy0 - vyi) = - ti ... (3)
// (z0 - zi) / (vz0 - vzi) = - ti ... (4)
// Since the LHSs of (2) and (3) are both ti
// (x0 - xi) / (vx0 - vxi) = (y0 - yi) /  (vy0 - vyi)
// (x0 - xi) * (vy0 - vyi) = (y0 - yi) * (vx0 - vxi) 
// x0 * vy0 - xi * vy0 - x0 * vyi + xi * vyi = y0 * vx0 - yi * vx0 - y0 * vxi + yi * vxi
// This hold true for any i
// Let i = 1: x0 * vy0 - x1 * vy0 - x0 * vy1 + x1 * vy1 = y0 * vx0 - y1 * vx0 - y0 * vx1 + y1 * vx1 ... (5)
// Let i = 2: x0 * vy0 - x2 * vy0 - x0 * vy2 + x2 * vy2 = y0 * vx0 - y2 * vx0 - y0 * vx2 + y2 * vx2 ... (6)
// Subtract (6) from (5):
// (-x1 + x2) * vy0 + (-vy1 + vy2) * x0 + (x1 * vy1) - (x2 * vy2)
//                   = (-y1 + y2) * vx0 + (-vx1 + vx2) * y0 +( y1 * vx1) + (y2 * vx2)
// Rearranging the terms:
// (-vy1 + vy2) * x0 - (-vx1 + vx2) * y0 - (-y1 + y2) * vx0 + (-x1 + x2) * vy0 
//                   = -(x1 * vy1) + (x2 * vy2) +( y1 * vx1) + (y2 * vx2)
// Where all the coefficients of the position and velocities of the hailstones 1 and 2 are known, so this becomes
// c1 * x0 + c2 * y0 + c3 * vx0 + c4 * vy0 = c5 where ci is some known constant ... (7)
// Also, while (7) was obtained using ((1) and (2)), observe that this sort of an equation can be reached even by using ((2) and (4)) or ((3) and (4))
// c6 * y0 + c7 * z0 + c8 * vy0 + c9 * vz0 = c10 where ci is some known constant
// c11 * y0 + c12 * z0 + c13 * vy0 + c14 * vz0 = c15 where ci is some known constant
// Also note that while here hailstones 1 and 2 were chosen, this can be done for any pair of hailstones
// In (7) there are 4 unknowns, so by taking 4 pairs of hailstones, 4 equations can be obtained
// These can then be put in a matrix:
// [Some     ][x0 ] =  [Some other]
// [matrix   ][y0 ]    [matrix    ]
// [of       ][vx0]    [of        ]
// [constants][vy0]    [constants ]
// Multiplying both sides by the inverse of first matrix gives
// [x0 ] = Inverse([Some     ]) * [Some other]
// [y0 ] =         [matrix   ]    [matrix    ]
// [vx0] =         [of       ]    [of        ]
// [vy0] =         [constants]    [constants ]
// This provides 4 out of th e6 unknowns
// For the other 2 the same process is repeated, but with 2 equations this time with y and z instead of x and y
// Only 2 equations are needed since the values of y have already been calculated, 
// but the values of y can be recalculated using 4 equations to sanity check the solution
// This provides all the unknowns and hence the answer

struct Hailstone {
  std::array<long double, 3> position;
  std::array<long double, 3> velocity; 
};

Hailstone parse_input(const std::string& line) {
  const std::regex pattern(R"((-?[0-9]+),(-?[0-9]+),(-?[0-9]+)@(-?[0-9]+),(-?[0-9]+),(-?[0-9]+))");
  std::smatch match;
  std::regex_search(line, match, pattern); 
  Hailstone hs;
  hs.position[0] = std::stod(match[1]);
  hs.position[1] = std::stod(match[2]);
  hs.position[2] = std::stod(match[3]);
  hs.velocity[0] = std::stod(match[4]);
  hs.velocity[1] = std::stod(match[5]);
  hs.velocity[2] = std::stod(match[6]);
  return hs;
}

template<size_t N>
constexpr std::array<std::array<long double, N>, N> inverse_using_LU_decomp(std::array<std::array<long double, N>, N> mat) {
  std::array<std::array<long double, N>, N> p;
  std::array<std::array<long double, N>, N> inversed_data;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (i == j) p[i][j] = 1;
      else p[i][j] = 0;
    }
  }
  for (int i = 0; i < N - 1; i++){
    auto max_idx = i;
    for (int k  = i+1; k < N; k++) {
      if (std::abs(mat[k][i]) > std::abs(mat[max_idx][i])) {
        max_idx = k;
      }
    }

    std::swap(mat[i], mat[max_idx]);
    std::swap(p[i], p[max_idx]);

    for (int j = i+1; j < N; j++) {
      mat[j][i] /= mat[i][i];
      for (int k = i+1; k < N; k++) {
        mat[j][k] -= mat[j][i] * mat[i][k];
      }
    }
  }

  for (int i_main = 0; i_main < N; i_main++) {
    std::array<long double, N> b;
    for (int j = 0; j < N; j++) {
      if (i_main == j) {
        b[j] = 1;
      } else {
        b[j] = 0;
      }
    }

    auto temp = b;
    for (int i = 0; i < N; i++) {
      temp[i] = std::inner_product(std::begin(p[i]), std::end(p[i]), std::begin(b), 0.);
    }
    b = temp;

    for (int i = 0; i < N-1; i++){
      for (int j = i+1; j < N; j++)  {
        b[j] -= mat[j][i] * b[i];
      }
    }

    for (int i = N-1; i >=0; i--) {
      b[i] /= mat[i][i];
      for (int j = 0; j < i; j++) {
        b[j] -= mat[j][i] * b[i];
      }
    }

    for (int k =0; k < N; k++) {
      inversed_data[k][i_main] = b[k];
    }
  }
  return inversed_data;
}

// The function get_equation below provides the cofficients of equation 7 
// With the returned array containing c1, c2, c3, c4 and the returned double containing c5
// idx0 and idx1 help choose whether the function uses the coefficients of 
// (x,y) or (y,z) or (x,z) by using the values
// (0,1) or (1,2) or (0,2)
std::tuple<bool, std::array<long double, 4>, long double> get_equation(const Hailstone& hs1, const Hailstone& hs2, const int idx0, const int idx1) {
  if (hs1.position[idx1] == hs2.position[idx1]) return {false, std::array<long double, 4>(), 0};
  if (hs1.velocity[idx1] == hs2.velocity[idx1]) return {false, std::array<long double, 4>(), 0};
  const std::tuple<bool, std::array<long double, 4>, long double> ans = {
    true,
    {
      (hs2.velocity[idx1] - hs1.velocity[idx1]), // coeffecient of x0 if idx0 = 0
      -(hs2.velocity[idx0] - hs1.velocity[idx0]), // coeffecient of y0 if idx1 = 1
      -(hs2.position[idx1] - hs1.position[idx1]), // coeffecient of vx0 if idx0 = 0
      (hs2.position[idx0] - hs1.position[idx0]) // coeffecient of vy0 if idx1 = 1
    },
    ((hs1.position[idx1] * hs1.velocity[idx0]) - (hs1.position[idx0] * hs1.velocity[idx1]))
    - ((hs2.position[idx1] * hs2.velocity[idx0]) - (hs2.position[idx0] * hs2.velocity[idx1])) // RHS
  };
  // std::cout << std::get<1>(ans)[0] << ' ' 
  //           << std::get<1>(ans)[1] << ' ' 
  //           << std::get<1>(ans)[2] << ' ' 
  //           << std::get<1>(ans)[3] << " | " 
  //           << std::get<2>(ans) << '\n';
  return ans;
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_24_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  std::vector<Hailstone> hailstones;
  while(std::getline(file, line)) {
    line.erase(std::remove_if(line.begin(), line.end(), [](const char c) {return c == ' ';}), line.end());
    hailstones.emplace_back(parse_input(line));
  }
  std::array<long double, 3> ans_position;
  std::array<long double, 3> ans_velocity;
  // Calculating for (x,y)
  {
    int count = 0;
    std::array<std::array<long double, 4>, 4> main_lhs;
    std::array<long double, 4> main_rhs;
    for(int i = 0; i < hailstones.size()-1 && count < 4; i++) {
      for(int j = i+1; j < hailstones.size() && count < 4; j++) {
        const auto [valid, lhs, rhs] = get_equation(hailstones[i], hailstones[j], 0, 1);
        if (valid) {
          main_lhs[count] = lhs;
          main_rhs[count] = rhs;
          count++;
        }
      }
    }
    const auto inverse = inverse_using_LU_decomp<4>(main_lhs);
    std::array<long double, 4> answer_of_calcs {0,0,0,0};
    for (int row = 0; row < 4; row++) {
      for (int col = 0; col < 4; col++) {
        answer_of_calcs[row] += inverse[row][col] * main_rhs[col]; 
      }
    }
    ans_position[0] = answer_of_calcs[0];
    ans_position[1] = answer_of_calcs[1];
    ans_velocity[0] = answer_of_calcs[2];
    ans_velocity[1] = answer_of_calcs[3];
  }
  // Calculating for (x,z)
  {
    int count = 0;
    std::array<std::array<long double, 4>, 4> main_lhs;
    std::array<long double, 4> main_rhs;
    for(int i = 0; i < hailstones.size()-1 && count < 4; i++) {
      for(int j = i+1; j < hailstones.size() && count < 4; j++) {
        const auto [valid, lhs, rhs] = get_equation(hailstones[i], hailstones[j], 1, 2);
        if (valid) {
          main_lhs[count] = lhs;
          main_rhs[count] = rhs;
          count++;
        }
      }
    }
    const auto inverse = inverse_using_LU_decomp<4>(main_lhs);
    std::array<long double, 4> answer_of_calcs {0,0,0,0};
    for (int row = 0; row < 4; row++) {
      for (int col = 0; col < 4; col++) {
        answer_of_calcs[row] += inverse[row][col] * main_rhs[col]; 
      }
    }
    // assert(ans_position[0] == answer_of_calcs[0]);
    // assert(ans_position[2] == answer_of_calcs[2]);
    ans_position[2] = answer_of_calcs[1];
    ans_velocity[2] = answer_of_calcs[3];
  }
  // std::cout << "Answers: " << '\n';
  // for (const auto& ele : ans_position) {
  //   std::cout << std::fixed << ele << ' ';
  // } 
  // std::cout << '\n';
  // for (const auto& ele : ans_velocity) {
  //   std::cout << std::fixed << ele << ' ';
  // } 
  // std::cout << '\n';
  long long answer = 0;
  for (const auto& ele : ans_position) {
    answer += static_cast<long long>(ele);
  }
  std::cout << std::fixed << answer << '\n';
  return 0;
}
