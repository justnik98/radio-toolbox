#include <chrono>
#include <iostream>

#include "modulators/PmMod.h"
#include "modulators/QamMod.h"

using namespace std;
int main() {
    auto num = 360;
    vector<bool> bits(num);
    for (auto && bit : bits) {
        bit = rand() % 2;
    }

    for (auto && bit : bits) {
        cout << bit;
    }
    cout << "\n !!!! "<< endl;

    PmMod qam_mod(16, 2, 800e6, 2e-8);
    auto start = chrono::high_resolution_clock::now();
    auto signals = qam_mod.Mod(bits);
    auto start2 = chrono::high_resolution_clock::now();
    bits = qam_mod.Demod(signals);
    auto stop = chrono::high_resolution_clock::now();

    auto duration = duration_cast<chrono::microseconds>(start2 - start);
    auto duration2 = duration_cast<chrono::microseconds>(stop - start2);

    for (auto && bit : bits) {
        cout << bit;
    }
    cout << endl << duration.count()<< endl << duration2.count() << endl;
    cout << endl << "speed bits/sec: \n" << num*1e6/duration.count() << endl << num*1e6/duration2.count() << endl;
    return 0;
}