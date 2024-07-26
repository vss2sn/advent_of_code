#include <algorithm>
#include <array>
#include <cassert>    
#include <cmath>
#include <fstream>
#include <iostream>
#include <ranges>
#include <sstream>  
#include <string>
#include <vector>  
#include <iomanip>

// MD5 algorithm implemented with priority given to ease of understanding as opposed to optimization; hence it might take some time for the code to run
// Bytes of the message need to be ordered in lowest to highest significance hence the swapping of orders. Note that this does not apply to the length added to the end of the message
// Also note that this assumes that the object to be hashed is made of complete bytes which is true given that a string is a collection of chars which are each one byte. 
// Assumes that the size of the input will fit in std::size_t

constexpr std::array<uint32_t, 64> createTableOfConstants() {
  std::array<uint32_t, 64> table {};
  float multiplication_factor = std::pow(2, 32);
  for (std::size_t i = 0; i < table.size(); i++) {
    table[i] = uint32_t(std::fabs(std::sin(i + 1)) * multiplication_factor);
  }
  return table;
};

std::vector<bool> convert_str_to_bits(const std::string& s) {
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
      s_bits.push_back(ele);
    }
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
  const std::size_t size = s.size() % 512;
  const auto rem = 512 - size;
  std::size_t padding = rem;
  if (rem < 64) {
      padding += 512; 
  }
  padding -= 64;
  for (std::size_t idx = 0; idx < padding; idx++) {
    s.push_back(false);  
  }
  for (int i = 0; i < 64; i++) {
    s.push_back(false);  
  }
  // Given that max<size_t> is 2^64 - 1 on 64 bit systems this, 
  // it can be assumed that original_length < 2 ^ 64, 
  // else the algorithm will need some modification 
  // to use unsigned 128 bit integers (or greater) for size instead of size_t
  // and then the line below can be modified to use
  // const auto binary = convertDecimalToBinary(original_length % std::pow(2, 64))
  const auto binary = convertDecimalToBinary(original_length); 
  for(std::size_t i = 0; i < binary.size(); i++) {
    s[s.size() + i - binary.size()] = binary[i];
  }
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
  std::vector<std::vector<uint32_t>> blocks;
  for (std::size_t i = 0; i < s_bits.size()/512; i++) {
    std::vector<uint32_t> block;
    for (int j = 0; j < 16; j++) {
      std::vector<bool> word;
      for (int k = 0; k < 32; k++) {
        word.push_back(s_bits[i * 512 + j * 32 + k]);
      }
      // Convert to little endian
      std::vector<bool> word_swapped(32, false);
      for (int k = 0; k < 32; k++) {
        word_swapped[(3 - (k / 8)) * 8 + (k % 8)] = word[k];
      }
      if (!(i == (s_bits.size()/512 - 1) && (j == 15 || j == 14))) word = word_swapped;
      block.push_back(convertBinaryToDecimal(word));
    }
    if(i == (s_bits.size()/512 - 1)) std::swap(block[block.size()-1], block[block.size()-2]);
    blocks.push_back(block);
  }
  return blocks;
}

uint32_t leftrotate(const uint32_t& a, const uint32_t& b) {
  return (a << b)|(a >> (32 - b));
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
  auto s_bits = convert_str_to_bits(s_str);
  padding(s_bits);
  assert(s_bits.size() == 512);
  auto blocks = convert_to_blocks(s_bits);
  uint32_t AA = A_prime;
  uint32_t BB = B_prime;
  uint32_t CC = C_prime;
  uint32_t DD = D_prime;
  for (auto& block : blocks) {
    A = AA;
    B = BB;
    C = CC;
    D = DD;
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
      const auto temp = D;
      D = C;
      C = B;
      B = B + leftrotate(A+ans+K[i]+block[g], s[i]);
      A = temp;
    }
    AA = uint32_t(AA) + uint32_t(A);
    BB = uint32_t(BB) + uint32_t(B);
    CC = uint32_t(CC) + uint32_t(C);
    DD = uint32_t(DD) + uint32_t(D);
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
  return std::array<uint32_t, 4>{{AA,BB, CC, DD}};
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
    return stream_main.str();
}


int main(int argc, char* argv[]) {
  std::string input = "../input/day_04_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::ifstream file(input);
  std::string secret_key;
  while(std::getline(file, secret_key)) {
    std::size_t i = 0;
    while(true) {
      const auto to_hash = secret_key + std::to_string(i);
      const auto hash_value = convertHashToString(md5(to_hash));
      int count = 0;
      for (const auto ele : hash_value) {
          if (ele == '0') count++;
          else break;
      }  
      if (count >= 5) {
          std::cout << hash_value << '\n';
          break;
      }
      i++;
    }
  }
  return 0;
}