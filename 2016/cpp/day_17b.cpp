#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>  
#include <cmath>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <bitset>    
#include <cassert>    
#include <sstream> 
#include <unordered_map> 
#include <unordered_set> 
#include <queue>

// Bytes of the message need to be ordered in lowest to highest significance hence the swapping of orders. Note that this does not apply to the length added to the end of the message
// Also note that this assumes that the object to be hashed is made of complete bytes which is true given that a sring is a collection of chars which are wach one byte. 

constexpr std::array<uint32_t, 64> createTableOfConstants() {
  std::array<uint32_t, 64> table {};
  float multiplication_factor = std::pow(2, 32);
  for (std::size_t i = 0; i < table.size(); i++) {
    // std::cout << std::dec << i << " | " << table.size() << " | " << bool(i < table.size()) << '\n';
    table[i] = uint32_t(std::fabs(std::sin(i + 1)) * multiplication_factor);
    // std::cout << std::hex << table[i] << '\n';
  }
  return table;
};

std::vector<bool> convert_str_to_bits(const std::string& s) {
  // if (s == "abc1803305") std::cout << "Converting " << s << " to bits." << '\n';
  std::vector<bool> s_bits;
  s_bits.reserve(s.size() * 8);
  for (const auto& c : s) {
    std::vector<bool> temp;
    auto i = uint8_t(c);
    while(i > 0) {
      temp.push_back((i % 2 == 0 ? false : true));
      i /= 2;
    }
    while (temp.size() < 8) temp.push_back(false);
    std::reverse(temp.begin(), temp.end());
    for (const auto& ele :temp) {
      // if (s == "abc1803305") std::cout << ele;
      s_bits.push_back(ele);
    }
    // if (s == "abc1803305") std::cout << '\n';
  }
  return s_bits;
}

std::vector<bool> convertDecimalToBinary(uint32_t n) {
  std::vector<bool> binary;
  while(n > 0) {
    binary.push_back(n%2);
    n/=2;
  }
  while(binary.size() < 32) {
    binary.push_back(false);
  }
  std::reverse(binary.begin(), binary.end());
  return binary;
}
void padding(std::vector<bool>& s) {
  const auto original_length = s.size();
  s.push_back(true);
  // std::cout << "Original length: " << original_length << '\n';
  const std::size_t size = s.size() % 512;
  std::size_t padding = 0;
    const auto rem = 512 - size;
    padding = rem;
    if (rem < 64) {
        padding += 512; 
    }
    padding -= 64;
//   std::cout << "padding: " << padding << '\n';
//   std::cout << s.size() << '\n';
  for (std::size_t idx = 0; idx < padding; idx++) {
    s.push_back(false);  
  }
//   std::cout << s.size() << '\n';
  for (int i = 0; i < 64; i++) {
    s.push_back(false);  
  }
//   std::cout << s.size() << '\n';

  const auto binary = convertDecimalToBinary(original_length % std::size_t(std::pow(2, 64)));
  // std::cout << binary.size() << '\n';
  for(std::size_t i = 0; i < binary.size(); i++) {
    s[s.size() + i - binary.size()] = binary[i];
  }
//   std::cout << s.size() << '\n';
}

uint32_t convertBinaryToDecimal(const std::vector<bool>& binary) {
    assert(binary.size() == 32);

  if (binary.empty()) return 0;
  uint32_t decimal = 0;
  uint32_t base = 1;
  for (auto i = binary.size()-1; i > 0; i--) {
    decimal += binary[i] * base;
    base *= 2;
  }
  decimal += binary[0] * base;
  return decimal;
}

std::vector<std::vector<uint32_t>> convert_to_blocks(std::vector<bool>& s_bits) {
    // for (const auto ele : s_bits) {
    //     std::cout << ele;
    // }
    // std::cout << '\n';
  std::vector<std::vector<uint32_t>> blocks;
  for (std::size_t i = 0; i < s_bits.size()/512; i++) {
    std::vector<uint32_t> block;
    for (int j = 0; j < 16; j++) {
      std::vector<bool> word;
      for (int k = 0; k < 32; k++) {
        word.push_back(s_bits[i * 512 + j * 32 + k]);
      }
      std::vector<bool> word_swapped(32, false);
      for (int k = 0; k < 32; k++) {
        word_swapped[(3 - (k / 8)) * 8 + (k % 8)] = word[k];
      }
      if (!(i == (s_bits.size()/512 - 1) && (j == 15 || j == 14))) word = word_swapped;
    //   else std::cout << "not reverseing" << '\n';
      // for (const auto ele : word) {
      //   std::cout << ele;
      // }
      // std::cout << '\n';
      // std::cout << convertBinaryToDecimal(word) << '\n';
      block.push_back(convertBinaryToDecimal(word));
    }
    if(i == (s_bits.size()/512 - 1)) std::swap(block[block.size()-1], block[block.size()-2]);
    blocks.push_back(block);
  }
//   std::cout << "size of blocks:" << blocks.size() << '\n';
  return blocks;
}

uint32_t leftrotate(const uint32_t& a, const uint32_t& b) {
  return (a << b)|(a >> (32 - b));
}

std::string convertHashToString (const std::array<uint32_t, 4>& hash_parts){
    std::stringstream stream_main;
    for (const auto& hash_part : hash_parts) {
      std::stringstream stream;
      stream << std::hex << hash_part;
      for(int i = 0;i < 8 - stream.str().size(); i++) {
        stream_main << '0';
      }
      stream_main << std::hex << hash_part;
    }
    // std::stringstream stream;

    // stream << std::hex << hash_parts[0] << hash_parts[1] << hash_parts[2] << hash_parts[3] << std::dec;
    // std::cout << std::hex << stream.str() << '\n';
    return stream_main.str();
}

std::array<uint32_t, 4> md5(const std::string& s_str) {
  
  constexpr uint32_t A_prime = 0x67452301;
  constexpr uint32_t B_prime = 0xefcdab89;
  constexpr uint32_t C_prime = 0x98badcfe;
  constexpr uint32_t D_prime = 0x10325476;

  uint32_t A = 0; 
  uint32_t B = 0; 
  uint32_t C = 0; 
  uint32_t D = 0; 
  
  auto F = [&B, &C, &D](){ return ((B & C) | (~B & D)); };
  auto G = [&B, &C, &D](){ return ((B & D) | (C & ~D)); };
  auto H = [&B, &C, &D]() { return (B ^ C ^ D); };
  auto I = [&B, &C, &D]() { return (C ^ (B | ~D)); };

  constexpr std::array<uint32_t, 64> K = createTableOfConstants();
  constexpr std::array<uint32_t, 64> s = {{7,12,17,22,7,12,17,22,7,12,17,22,7,12,17,22,5,9,14,20,5,9,14,20,5,9,14,20,5,9,14,20,4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23 ,6,10,15,21,6,10,15,21,6,10,15,21,6,10,15,21 }};
  
  uint32_t AA = A_prime;
  uint32_t BB = B_prime;
  uint32_t CC = C_prime;
  uint32_t DD = D_prime;
  auto s_bits = convert_str_to_bits(s_str);
  // for (const auto ele : s_bits) {
  //   std::cout << ele;
  // }
  // std::cout << '\n';

  padding(s_bits);
  // for (const auto ele : s_bits) {
  //   std::cout << ele;
  // }
  // std::cout << '\n';
  assert(s_bits.size() % 512 == 0);
  auto blocks = convert_to_blocks(s_bits);
//   for (auto& block : blocks) {
//     for (const auto ele : block) {
//       std::cout << ele;
//     }
//     std::cout << '\n';
//   }
//   std::cout << '\n';
  for (auto& block : blocks) {
    // std::cout << "block: ";
    // for (const auto ele : block) {
    //   std::cout << ele;
    // }
    // std::cout << '\n';
    A = AA;
    B = BB;
    C = CC;
    D = DD;
    // std::cout << A << ' '  << B << ' ' << C << ' ' << D << '\n';

    for (uint32_t i = 0; i < 64; i++) {
      uint32_t g = 0;
      uint32_t ans = 0;
      if (i < 16) {
        ans = F();
        g = i;
      } else  if (i >= 16 && i < 32) {
        ans = G();
        g = (5 * i + 1) % 16;
      } else  if (i >= 32 && i < 48) {
        ans = H();
        g = (3 * i + 5) % 16;
      } else  if (i >= 48 && i < 64) {
        ans = I();
        g = (7 * i) % 16;
      }
      // std::cout << std::dec << i<< ": " << A << " | " << B << " | " << C << " | " << D << " | " <<
      //                        ans <<  " | " << block[g] << " | " << s[i] << " | " << K[i] << '\n';
      auto temp = D;
      D = C;
      C = B;
      // std::cout << leftrotate(A+ans+K[i]+block[g], s[i]) << '\n';
      B = B + leftrotate(A+ans+K[i]+block[g], s[i]);
      A = temp;
      // std::cout << std::dec << A << " | " << B << " | " << C << " | " << D << '\n';
      // exit(0);
    }
    // std::cout << A << ' '  << B << ' ' << C << ' ' << D << '\n';
    AA = (uint32_t(AA) + uint32_t(A));// % int(std::pow(2, 32));
    BB = (uint32_t(BB) + uint32_t(B));// % int(std::pow(2, 32));
    CC = (uint32_t(CC) + uint32_t(C));// % int(std::pow(2, 32));
    DD = (uint32_t(DD) + uint32_t(D));// % int(std::pow(2, 32));
    // std::cout << AA << ' '  << BB << ' ' << CC << ' ' << DD << '\n';

  }
  const auto t = [](uint32_t n) {
    const std::vector<bool> temp = convertDecimalToBinary(n);
    assert(temp.size() == 32);
    auto temp2 = temp;
    for (int k = 0; k < 32; k++) {
      temp2[(3 - (k / 8)) * 8 + (k % 8)] = temp[k];
    }
    assert(temp2.size() == 32);
    return convertBinaryToDecimal(temp2);
  };
  AA = t(AA);
  BB = t(BB);
  CC = t(CC);
  DD = t(DD);
  // std::cout << std::hex << AA << BB << CC << DD << '\n';
  return std::array<uint32_t, 4>{{AA,BB, CC, DD}};
  // const auto leading_zeros = []() {
  //   int i = 0;
  //   while ()
  // };
  // return std::to_string(AA) + std::to_string(BB) + std::to_string(CC) + std::to_string(DD);
}



struct PointWithHistory {
    int row;
    int col;
    std::string history;
    bool operator == (const PointWithHistory& pwh) const {
        return row == pwh.row && col == pwh.col && history == pwh.history;
    }
};

struct PointWithHistoryHasher {
    std::size_t operator () (const PointWithHistory& pwh) const {
        std::size_t hash;
        for (const auto h : pwh.history) {
            hash += (h < 'Z') ? h - 'A' : h - 'a';
        }
        return hash;
    }
};

struct ComparePointWithHistory {
    bool operator () (const PointWithHistory& pwh1, const PointWithHistory& pwh2) const {
        return pwh1.history.size() > pwh2.history.size();
    } 
};

const std::vector<PointWithHistory> moves = {
  PointWithHistory(-1,0,"U"),
  PointWithHistory(1,0,"D"),
  PointWithHistory(0,-1,"L"),
  PointWithHistory(0,1,"R")
};

int main(int argc, char* argv[]) {
//     std::cout << convertHashToString(md5("kglvqrroDDUDRLRULRLRURDRLLDRLUDRULLRRDRULLLURDLDRLUDD")) << '\n';
// std::cout << convertHashToString(md5("kglvqrroDDUDRLRULRLRURDRLLDRLUDRULLRRDRULLLURDLDRLUDRLUR")) << '\n';
// std::cout << convertHashToString(md5("kjasbclasjkvbkjasvnJEWHLJWLHjvbkjnknlLWWKJFKHWKAkjljhwciuthnuinhwhlakjHALFJHEjkLKJBJKHhvHhljVUTCuTUCvGFgHvhgCHvjkbKYGfhgVKJHvhvjhVKVghvkjVJHVjhvJHVjhURrsTSTstSfxtXTREzyeTRxfXxtrFtQWErtyuiDfbnXcvbnSDFGHjkERTYuSDFGHJsdfghjRTYUIxcvbnmDFGHJdfghjxCVBNsdfghRTYUJcvbnmDFGHJKertyuisdfasxcfrsxdsxcfrdxcfgtfcvgfcvbgvbhgvnjnbnjmnmjnjnjhujhbhgvgvcfgfcfgfcfcxdxzxdxz")) << '\n';

// exit(0);
  std::string input = "../input/day_17_input";
  if (argc > 1) {
    input = argv[1];
  }
  std::string passcode;
  std::fstream file(input);
  
  const auto in_bounds = [](const PointWithHistory &pwh) {
    return pwh.row < 4 && pwh.col < 4 && pwh.row >= 0 && pwh.col >= 0;
  };

  while(std::getline(file, passcode)) {
    std::priority_queue<PointWithHistory, std::vector<PointWithHistory>, ComparePointWithHistory> pq;
    std::unordered_set<PointWithHistory, PointWithHistoryHasher> seen;
    PointWithHistory start;
    start.row = 0;
    start.col = 0;
    start.history = passcode;
    // std::cout << '|' << passcode << '|' << '\n';
    pq.push(start);
    std::string longest_path = "";
    while(!pq.empty()) {
      const auto current = pq.top();
    //   std::cout << pq.size() << '\n';
      pq.pop();
    //   std::cout << __LINE__ << '\n';
      if (seen.find(current) != seen.end()) {
        // std::cout << "Continue" << '\n';
        continue;
      }
      seen.insert(current);
    //   if (current.history.size() > 370) {
    //     std::cout << "WTH" << '\n';
    //   }  
      if (current.row == 3 && current.col == 3) {
        longest_path = current.history.substr(passcode.size(), current.history.size() - passcode.size());
        // std::cout << "Found path for " << passcode << ":  " << longest_path << ": " << longest_path.size() << '\n';
        continue;
      }
    //   std::cout << __LINE__ << '\n';
      const auto current_hash = convertHashToString(md5(current.history));
    //   std::cout << current.history << " ----> " << current_hash << '\n';
    //   std::cout << __LINE__ << '\n';
      // std::cout << '|' << current.history << '|' << current_hash << '|' << '\n';
      for (int i = 0; i < moves.size(); i++) {
        if (current_hash[i] >= 'b' && current_hash[i] <= 'f') {
          PointWithHistory new_point(current.row + moves[i].row, current.col + moves[i].col, current.history + moves[i].history);
          if (!in_bounds(new_point)) continue;
        //   if (seen.find(new_point) != seen.end()) continue;
          pq.push(new_point);
        }
      }
    //   std::cout << __LINE__ << '\n';
    }
    if(longest_path.empty()) {
        std::cout << "No path" << '\n';
    } else {
        // std::cout << longest_path << ": " << longest_path.size() << '\n';
        std::cout << longest_path.size() << '\n';
    }
  }
  
  return 0;
}