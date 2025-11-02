//
// Created by yanni on 27.10.2025.
//

#include "PmMod.h"

PmMod::PmMod(uint32_t q, float mean_energy, float frequency, float T) : IModulator(q, frequency, T) {
    complex_signals_.resize(q_);
    signals_.resize(q_);
    E_ = std::sqrt(mean_energy);
    Preload();
}

std::vector<std::complex<float> > PmMod::ModComplex(std::vector<bool> &bits) const {
    std::vector<std::complex<float> > result(bits.size() / k_);
    size_t num = 0;
    size_t index = 0;
    size_t i = 0;
    for (const auto &&b: bits) {
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

std::vector<bool> PmMod::DemodComplex(const std::vector<std::complex<float> > &signals) const {
    std::vector<bool> result;

    const float scale = std::sqrt(1.0f / CalcAvgEnergy(signals));
    result.reserve(signals.size() * k_);
    auto res = 0;
    for (const auto signal: signals) {
        auto dmin = std::numeric_limits<float>::infinity();
        for (auto i = 0; i < complex_signals_.size(); ++i) {
            if (const float d = std::norm(complex_signals_[i] - signal * scale); d < dmin) {
                dmin = d;
                res = i;
            }
        }
        for (int i = k_ - 1; i >= 0; --i) {
            bool b = res & (1 << i);
            result.emplace_back(b);
        }
    }
    return result;
}

std::vector<float> PmMod::Mod(std::vector<bool> &bits) {
    std::vector<float> result;
    size_t num = 0;
    size_t index = 0;
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

std::vector<bool> PmMod::Demod(const std::vector<float> &signals) {
    auto t = 0.0f;
    auto r = 0.0f;
    auto i = 0.0f;
    auto ind = 0;
    std::vector<std::complex<float> > res;
    res.reserve(signals.size() / n_samples_); // Оценка количества символов
    for (auto &&s: signals) {
        r += s * cos_[ind];
        i += s * sin_[ind];
        ind = (ind + 1) % cos_.size();
        t += dt_;
        if (t >= T_) {
            res.emplace_back(2 * r / T_ * dt_, 2 * i / T_ * dt_);
            t = 0.0;
            r = 0.0;
            i = 0.0;
        }
    }
    return DemodComplex(res);
}

void PmMod::Preload() {
    for (auto i = 0; i < q_; i++) {
        const auto theta = 2.0f * M_PI * static_cast<float>(i) / static_cast<float>(q_);
        const auto i_part = cos(theta);
        const auto q_part = -sin(theta);
        complex_signals_[i] = std::complex<float>(i_part, q_part);
        for (auto t = 0.0; t < T_; t += dt_) {
            auto s = std::sqrt(E_)*cos(2 * pi * f_ * t + theta);
            if (i == 0) {
                cos_.emplace_back(cos(2 * pi * f_ * t));
                sin_.emplace_back(sin(2 * pi * f_ * t));
            }
            signals_[i].push_back(s);
        }
    }
}

float PmMod::CalcAvgEnergy(const std::vector<std::complex<float> > &signals) {
    auto avg = 0.0;
    for (auto &&s: signals) {
        avg += std::norm(s);
    }
    return avg / signals.size();
}
