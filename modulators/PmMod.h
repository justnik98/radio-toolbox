//
// Created by yanni on 27.10.2025.
//

#ifndef RADIO_TOOLBOX_PMMOD_H
#define RADIO_TOOLBOX_PMMOD_H

#include "IModulator.h"

class PmMod: public IModulator {
    public:
        PmMod(uint32_t q, float mean_energy, float frequency, float T);

        std::vector<std::complex<float> > ModComplex(std::vector<bool> &bits) const;

        [[nodiscard]] std::vector<bool> DemodComplex(const std::vector<std::complex<float> > &signals) const;

        std::vector<float> Mod(std::vector<bool> &bits) override;

        std::vector<bool> Demod(const std::vector<float> &signals) override;

        ~PmMod() override = default;

    private:
        void Preload();

        float E_{};

        float f_;

        float T_;

        float dt_;

        float n_samples_;

        std::vector<float> cos_;

        std::vector<float> sin_;

        std::vector<std::complex<float> > complex_signals_;

        static float CalcAvgEnergy(const std::vector<std::complex<float>> & signals) ;
    };


#endif //RADIO_TOOLBOX_PMMOD_H