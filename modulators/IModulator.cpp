//
// Created by yanni on 02.11.2025.
//


#include "IModulator.h"

IModulator::IModulator(uint32_t q, float f, float T) : q_(q), f_(f), T_(T) {
    if (q < 2 || (q & (q - 1)) != 0) {
        throw std::invalid_argument("M must be a power of 2 and >= 4");
    }
    k_ = round(std::log2(q));
    signals_.resize(q_);
    dt_ = 1.0f / (7.0f * f_);
    n_samples_ = round(T / dt_);
}
