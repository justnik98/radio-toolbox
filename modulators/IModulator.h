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
    IModulator(uint32_t q, float f, float T);

    virtual std::vector<float> Mod(std::vector<bool> &bits) = 0;

    virtual std::vector<bool> Demod(const std::vector<float> &signals) = 0;

    virtual ~IModulator() = default;

protected:
    std::vector<std::vector<float> > signals_;

    uint32_t q_ = 0;

    uint32_t k_ = 0;

    float f_;

    float T_;

    float dt_;

    uint32_t n_samples_;
};
#endif //RADIO_TOOLBOX_IMODULATOR_H
