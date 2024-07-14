//David Pérez Sampedro - 48774775D
#include <iostream>
#include <math.h>
#include <iomanip>

using namespace std;

double STEPS;

// coste: O(log2(n))
unsigned long pow2_1(unsigned n) {
    STEPS++;
    if (n != 0) {
        if (n % 2 == 0) {
            unsigned long half_pow2_1 = pow2_1(n / 2);
            return half_pow2_1 * half_pow2_1;
        }
        unsigned long half_pow_2_1 = pow2_1((n - 1) / 2);
        return 2 * half_pow_2_1 * half_pow_2_1;
    }
    return 1;
}

// coste: O(n)
unsigned long pow2_2(unsigned n) {
    STEPS++;
    if(n != 0) {
        return 2 * pow2_2(n - 1);
    }
    return 1;
}

// coste: O(2^n)
unsigned long pow2_3(unsigned n) {
    STEPS++;
    if(n != 0) {
        return pow2_3(n - 1) + pow2_3(n - 1);
    }
    return 1;
}

int main(void) {
    cout << "#POW FUNCTION COMPARATION."
        << endl
        << "#Tsteps (Thousands of programs steps):"
        << endl << endl
        << "# Unsigned n    pow2_1(n)  pow2_2(n)  pow2_3(n)"
        << endl
        << "#==============================================="
        << endl;

        for(int n = 0; n <= 10; n++) {
            cout << std::setw(8) << n << std::flush;

            unsigned long pow_result = pow(2, n);

            STEPS = 0;
            unsigned long pow1_result = pow2_1(n);
            double pow1 = STEPS / 1000;

            if(pow_result != pow1_result) {
                cerr << endl << "Error, pow2_1 it is worng!" << endl;
                exit(EXIT_FAILURE);
            }

            STEPS = 0;
            unsigned long pow2_result = pow2_2(n);
            double pow2 = STEPS / 1000;

            if(pow_result != pow2_result) {
                cerr << endl << "Error, pow2_2 it is worng!" << endl;
                exit(EXIT_FAILURE);
            }

            STEPS = 0;
            unsigned long pow3_result = pow2_3(n);
            double pow3 = STEPS / 1000;

            if(pow_result != pow3_result) {
                cerr << endl << "Error, pow2_3 it is wrong!" << endl;
                exit(EXIT_FAILURE);
            }

            std::cout << std::fixed << std::setprecision(3);
            cout << std::setw(15) << pow1 << std::setw(11) << pow2 << std::setw(11) << pow3 << endl;
        }
    
    return 0;
}