//
// Created by yanni on 09.11.2025.
//

#include "Fft.h"
#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <numbers>

using namespace std;
using complexd = complex<double>;


void bit_reverse(vector<complexd>& a) {
    int n = a.size();
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1)
            j ^= bit;
        j ^= bit;
        if (i < j)
            swap(a[i], a[j]);
    }
}


void fft_inplace(vector<complexd>& a, bool invert) {
    int n = a.size();
    bit_reverse(a);

    for (int len = 2; len <= n; len <<= 1) {
        double angle = 2 * numbers::pi / len * (invert ? -1 : 1);
        complexd wlen(cos(angle), sin(angle));
        for (int i = 0; i < n; i += len) {
            complexd w(1);
            for (int j = 0; j < len / 2; j++) {
                complexd u = a[i + j];
                complexd v = a[i + j + len / 2] * w;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }

    if (invert) {
        for (complexd& x : a)
            x /= n;
    }
}

vector<complexd> fft(const vector<complexd>& a) {
    vector<complexd> result = a;
    fft_inplace(result, false);
    return result;
}

vector<complexd> ifft(const vector<complexd>& a) {
    vector<complexd> result = a;
    fft_inplace(result, true);
    return result;
}

vector<complexd> fft(const vector<double>& a) {
    vector<complexd> complex_a(a.begin(), a.end());
    return fft(complex_a);
}

vector<complexd> ifft(const vector<double>& a) {
    vector<complexd> complex_a(a.begin(), a.end());
    return ifft(complex_a);
}