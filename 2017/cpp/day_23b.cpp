#include <iostream>
#include <vector>

int main(int argc, char * argv[]) {
  constexpr int b = 108100;
  constexpr int c = 125100;
  std::vector<bool> primes(c, true);
  for (int i = 2; i * i <= c; i++) {
    if (primes[i]) {
      for (int j = i * i; j <= c; j+= i) {
        primes[j] = false;
      }
    }
  }

  int ans  = 0;
  for (int i = b; i <= c; i+=17) {
    if (!primes[i]) ans++;
  }
  std::cout << ans << '\n';
  return 0;
}


// Instructions converted to code:
// int main(int argc, char * argv[]) {
//   int count = 0;
//   int a = 1;
//   int b = 0;
//   int c = 0;
//   int d = 0;
//   int e = 0;
//   int f = 0;
//   int g = 0;
//   int h = 0;
//   b = 108100;
//   c = 125100;
// 
//   while(true) {
//     f=1;
//     d=2;
//     do  {
//       e = 2;
//       do {
//         g = d;
//         g = g * e;
//         g = g - b;
//         if (g == 0) {
//           f = 0;
//         }
//         e -= - 1;
//         g = e;
//         g = g - b;
// 
//       } while( g != 0);
//       d -= -1;
//       g = d;
//       g -= b;
//     } while (g != 0);
//     if (f == 0) {
//       h -= -1;
//     }
//     g = b;
//     g -= c;
//     if(g == 0) {
//       std::cout << h << '\n';
//       exit(0);
//     }
//     b -= -17;
//   }
//   std::cout << h << '\n';
// }

// Note that e and d both start of as 2 and the minimum value of b is 108100
// The while loops become for loops from 2 to b
// The outermost loop is indexed by b and runs from the initial value of b to c.