//
// Created by yanni on 09.11.2025.
//

#ifndef RADIO_TOOLBOX_FFT_H
#define RADIO_TOOLBOX_FFT_H

#include <iostream>
#include <vector>
#include <complex>
#include <cmath>

std::vector<std::complex<double>> fft(const std::vector<double>& a);
std::vector<std::complex<double>> ifft(const std::vector<double>& a);
std::vector<std::complex<double>> fft(const std::vector<std::complex<double>>& a);
std::vector<std::complex<double>> ifft(const std::vector<std::complex<double>>& a);
#endif //RADIO_TOOLBOX_FFT_H