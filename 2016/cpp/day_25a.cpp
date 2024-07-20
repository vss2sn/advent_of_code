// Translated the input into C++ code 
#include <iostream>

int main() {
    // bool works = false;
    for (int i = 0; ;i++) { // Have to find the first i that provides the pattern; no upper limit
        bool works = true; // Tracks if the 0,1,0,1 pattern is being followed
        int iterations = 0; // Number of times output is called with the given i value. 
        constexpr int threshold = 10; // If iterations exceeds this threshold, assume the pattern repeats forever
        bool val = 0; // Expected value of output
        // registers: a,b,c,d
        int a = i;
        int b = 0;
        int c = 0;
        int d = 0;
        d = a;
        c = 7;
        do{
            b = 365;
            do {
                d++;
                b--;
            } while(b!=0);
            c--;
        } while(c!=0);
        do{
            a = d;
            do {
                b = a;
                a = 0;
                while(true) {
                    c = 2;
                    if (b != 0) {
                        bool skip = true; 
                        // The b == 0 conditions on L43 needs to be checked only 
                        // when the loop is broken and the c == 0 condition 
                        // preceeding it has not been triggered. This can 
                        // probably be simplified.
                        while(b!=0) {
                            b--;
                            c--;
                            if (c == 0) {
                                a++;
                                skip = false;
                                break;
                            }
                        } 
                        if (b == 0 && skip) {
                            break;
                        }
                    } else {
                        break;
                    }
                }
                b = 2;
                while (c != 0) {
                    b--;
                    c--;
                }
                iterations++;
                if(iterations > threshold) { // Arbitrary number to ensure that the pattern is followed for at least n outputs
                  std::cout << i << '\n'; 
                  exit(0);
                }
                // std::cout << b << ' '; // debug
                if (b != (val ? 1 : 0)) {
                    // std::cout << '\n'; // debug
                    works = false; // pattern broken
                } else {
                    val = ! val; // Update expected val
                }
            } while (a != 0 && works);
        } while (works);
    }   
}