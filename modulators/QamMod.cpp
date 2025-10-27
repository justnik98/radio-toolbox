//
// Created by yanni on 27.10.2025.
//

#include "QamMod.h"

#include <cmath>

QamMod::QamMod(uint32_t q, float mean_energy, float frequency, float T) : IModulator(q), A_(mean_energy), f_(frequency),
                                                                          T_(T) {
    dt_ = 1.0f / (20.0f * f_);
    n_samples_ = T / dt_;
    complex_signals_.resize(q_);
    signals_.resize(q_);
    Preload();
}

std::vector<std::complex<float> > QamMod::ModComplex(std::vector<bool> &bits) const {
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
            result[i++] = complex_signals_[num];
            num = 0;
        }
    }
    return result;
}

std::vector<bool> QamMod::DemodComplex(std::vector<std::complex<float> > &signals) const {
    std::vector<bool> result;
    auto res = 0;
    for (auto signal: signals) {
        auto dmin = std::numeric_limits<double>::infinity();
        auto ind = 0;
        for (auto i = 0; i < complex_signals_.size(); ++i) {
            const double d = sqrt(pow(complex_signals_[i].real() - signal.real(), 2) +
                                  pow(complex_signals_[i].imag() - signal.imag(), 2));
            if (d < dmin) {
                dmin = d;
                res = i;
            }
        }
        for (auto i = 0; i < k_; ++i) {
            bool b = res & 1 << (k_ - i - 1);
            result.push_back(b);
        }
    }
    return result;
}

std::vector<float> QamMod::Mod(std::vector<bool> &bits) {
    std::vector<float> result;
    size_t num = 0;
    size_t index = 0;
    size_t i = 0;
    for (auto &&b: bits) {
        num <<= 1;
        num |= b;
        index++;
        if (index == k_) {
            index = 0;
            result.insert(result.end(), signals_[num].begin(), signals_[num].end());
            num = 0;
        }
    }
    return result;
}

std::vector<bool> QamMod::Demod(std::vector<float> &signals) {
    auto t = 0.0f;
    auto r = 0.0f;
    auto i = 0.0f;
    std::vector<std::complex<float>> res;
    for (auto &&s: signals) {
        r += s * cos(2 * pi * f_ * t);
        i += s * sin(2 * pi * f_ * t);
        t+=dt_;
        if (t >= T_) {
            res.emplace_back(r*T_,i*T_);
            t = 0.0;
            r = 0.0;
            i = 0.0;
        }
    }
    return DemodComplex(res);
}

void QamMod::Preload() {
    for (auto i = 0; i < q_; i++) {
        const int i1 = i / static_cast<int>(sqrt(q_));
        const int i2 = i % static_cast<int>(round(sqrt(q_)));
        const float si1 = A_ * (1 - 2 * i1 / (sqrt(q_) - 1));
        const float si2 = A_ * (1 - 2 * i2 / (sqrt(q_) - 1));
        complex_signals_[i] = std::complex(si1, si2);
        for (auto t = 0.0; t < T_; t += dt_) {
            auto s = si1 * cos(2 * pi * f_ * t) + si2 * sin(2 * pi * f_ * t);
            cos_.emplace_back(cos(2 * pi * f_ * t));
            sin_.emplace_back(sin(2 * pi * f_ * t));
            signals_[i].push_back(s);
        }
    }
}
