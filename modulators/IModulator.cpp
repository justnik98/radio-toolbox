//
// Created by yanni on 02.11.2025.
//


#include "IModulator.h"

IModulator::IModulator(uint32_t q): q_(q) {
    if (q < 2 || (q & (q - 1)) != 0) {
        throw std::invalid_argument("M must be a power of 2 and >= 4");
    }
    k_ = round(std::log2(q));
    signals_.resize(q_);

}
