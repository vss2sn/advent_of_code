#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

typedef __int128 int128_t;

constexpr int128_t n_cards = 119315717514047;
constexpr int128_t n_shuffles = 101741582076661;
constexpr int128_t final_position = 2020;

std::ostream& operator<<( std::ostream& dest, __int128_t value ) {
    std::ostream::sentry s( dest );
    if ( s ) {
        __uint128_t tmp = value < 0 ? -value : value;
        char buffer[ 128 ];
        char* d = std::end( buffer );
        do
        {
            -- d;
            *d = "0123456789"[ tmp % 10 ];
            tmp /= 10;
        } while ( tmp != 0 );
        if ( value < 0 ) {
            -- d;
            *d = '-';
        }
        int len = std::end( buffer ) - d;
        if ( dest.rdbuf()->sputn( d, len ) != len ) {
            dest.setstate( std::ios_base::badbit );
        }
    }
    return dest;
}

int128_t mod(int128_t a, int128_t b) {
  return (a >= 0) ? (a % b) : (b + a % b);
}

int128_t gcd_extended(int128_t a, int128_t b, int128_t& x, int128_t& y) {
  if (a == 0) {
    x = 0;
    y = 1;
    return b;
  }

  int128_t x1, y1;
  int128_t gcd = gcd_extended(b % a, a, x1, y1);
  x = y1 - (b / a) * x1;
  y = x1;
  return gcd;
}

int128_t modular_inverse(int128_t b, int128_t n) {
  int128_t x, y;
  int128_t g = gcd_extended(b, n, x, y);
  return (g != 1) ? -1 : mod(x, n);
}

int128_t modular_divide(int128_t a, int128_t b, int128_t n) {
  a = mod(a, n);
  int128_t inv = modular_inverse(b, n);
  return (inv == -1) ? -1 : (a * inv) % n;
}

int128_t modular_power(int128_t base, int128_t exponent, int128_t n) {
  if (exponent == 0) {
    return (base == 0) ? 0 : 1;
  }

  int128_t bit = 1;
  int128_t power = mod(base, n);
  int128_t out = 1;
  while (bit <= exponent) {
    if (exponent & bit) {
      out = mod(out * power, n);
    }
    power = mod(power * power, n);
    bit <<= 1;
  }

  return out;
}

int128_t atoi_128(const std::string& in) {
  int128_t res = 0;
  size_t i = 0;
  bool sign = false;
  while(in[i] == ' ') {
    i++;
  }
  if (in[i] == '-') {
      ++i;
      sign = true;
  }
  if (in[i] == '+') {
      ++i;
  }
  for (; i < in.size(); ++i) {
      const char c = in[i];
      res *= 10;
      res += c - '0';
  }
  if (sign) {
      res *= -1;
  }
  return res;
}

int main(int argc, char* argv[]) {
  // Get input
  std::string input = "../input/day_22_input";
  if (argc > 1) {
    input = argv[1];
  }
  std::ifstream file(input);
  std::string input_line;

  int128_t A = 1;
  int128_t B = 0;

  while(std::getline(file, input_line)) {
    int128_t a, b;
    if (input_line.substr(0,19) == "deal with increment") {
      a = atoi_128(input_line.substr(19, input_line.size() - 19));
      b = 0;
    } else if (input_line.substr(0,19) == "deal into new stack") {
      a = -1;
      b = n_cards - 1;
    } else if (input_line.substr(0,3) == "cut") {
      int128_t n_cut = atoi_128(input_line.substr(3, input_line.size() - 3));
      if (n_cut < 0) {
        n_cut += n_cards;
      }
      a = 1;
      b = n_cards - n_cut;
    }
    A = mod(a * A, n_cards);
    B = mod(a * B + b, n_cards);
  }

  int128_t fullA = modular_power(A, n_shuffles, n_cards);
  int128_t fullB = mod(B * modular_divide(modular_power(A, n_shuffles, n_cards) - 1, A - 1, n_cards), n_cards);
  int128_t startPos = mod(modular_divide(mod(final_position - fullB, n_cards), fullA, n_cards), n_cards);

  std::cout << startPos << '\n';
  return 0;  // main() cannot return int128_t, must return int
}
