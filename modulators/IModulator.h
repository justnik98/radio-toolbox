//
// Created by yanni on 27.10.2025.
//

#ifndef RADIO_TOOLBOX_IMODULATOR_H
#define RADIO_TOOLBOX_IMODULATOR_H
#include <complex>
#include <cstdint>
#include <vector>
#include <complex>

using std::numbers::pi;

class IModulator {
public:
    explicit IModulator(uint32_t q) : q_(q) {
        if (q < 4 || (q & (q - 1)) != 0) {
            throw std::invalid_argument("M must be a power of 2 and >= 4");
        }
        k_ = round(std::log2(q));
        signals_.resize(q_);

    }

    virtual std::vector<float> Mod(std::vector<bool> &bits) = 0;

    virtual std::vector<bool> Demod(const std::vector<float> &signals) = 0;

    virtual ~IModulator() = default;

protected:
    uint32_t q_ = 0;
    uint32_t k_ = 0;
    std::vector<std::vector<float> > signals_;
};


#endif //RADIO_TOOLBOX_IMODULATOR_H
