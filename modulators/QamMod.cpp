//
// Created by yanni on 27.10.2025.
//

#include "QamMod.h"

QamMod::QamMod(uint32_t q, uint32_t A) : IModulator(q), A_(A) {
    Preload();
}

std::vector<std::complex<float> > QamMod::Mod(std::vector<bool> &bits) {
    std::vector<std::complex<float> > result(bits.size() / k_);
    size_t num = 0;
    size_t index = 0;
    size_t i = 0;
    for (auto &&b: bits) {
        num <<= 1;
        num |= b;
        index++;
        if (index == k_) {
            index = 0;
            result[i++] = signals_[num];
            num = 0;
        }
    }
    return result;
}

std::vector<bool> QamMod::Demod(std::vector<std::complex<float> > &signals) {
    std::vector<bool> result;
    auto res = 0;
    for (auto signal: signals) {
        auto dmin = std::numeric_limits<double>::infinity();
        auto ind = 0;
        for (auto i = 0; i < signals_.size(); ++i) {
            const double d = sqrt(pow(signals_[i].real() - signal.real(), 2) +
                                  pow(signals_[i].imag() - signal.imag(), 2));
            if (d < dmin) {
                dmin = d;
                res = i;
            }
        }
        for (auto i = 0; i < k_; ++i) {
            bool b = res & 1 << (k_ - i-1);
            result.push_back(b);
        }
    }
    return result;
}


void QamMod::Preload() {
    for (auto i = 0; i < q_; i++) {
        const int i1 = i / static_cast<int>(sqrt(q_));
        const int i2 = i % static_cast<int>(round(sqrt(q_)));
        const float si1 = A_ * (1 - 2 * i1 / (sqrt(q_) - 1));
        const float si2 = A_ * (1 - 2 * i2 / (sqrt(q_) - 1));
        signals_[i] = std::complex(si1, si2);
    }
}
