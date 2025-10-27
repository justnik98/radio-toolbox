//
// Created by yanni on 27.10.2025.
//

#ifndef RADIO_TOOLBOX_QAMMOD_H
#define RADIO_TOOLBOX_QAMMOD_H
#include "IModulator.h"


class QamMod : public IModulator {
public:
    explicit QamMod(uint32_t q, uint32_t A);

    std::vector<std::complex<float> > Mod(std::vector<bool> &bits) override;

    std::vector<bool> Demod(std::vector<std::complex<float> > &signals) override;

    ~QamMod() override = default;

private:
    void Preload();

    uint32_t A_;
};


#endif //RADIO_TOOLBOX_QAMMOD_H
