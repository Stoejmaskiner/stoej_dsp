#pragma once
#include <juce_core/juce_core.h>
#include <stoej_core/stoej_core.h>

#include "../generators/stoej_WhiteNoise.h"

namespace stoej {
    STOEJ_FT_
    class [[deprecated("split into smaller processors")]] MultiplicativeNoise {
    public:
        MultiplicativeNoise();
        void prepare(juce::dsp::ProcessSpec& spec);
        void reset();

        // process needs to be in header to improve optimization
        template <typename ProcessContext>
        void process(const ProcessContext& context) noexcept;
        void setNoiseWidth(FT_ val) { this->noise_generator_.setNoiseWidth(val); }
        FT_ getNoiseWidth() { return this->noise_generator_.getNoiseWidth(); }
        void setNoiseDenisty(FT_ val) { this->noise_density_ = val; }
        FT_ getNoiseDensity() { return this->noise_density_; }
        void setNoiseMix(FT_ val) { this->noise_mix_.setTargetValue(val); }
        FT_ getNoiseMix() { return this->noise_mix_.getTargetValue(); }
        void setFilterLPCutoff(FT_ val) { 
            this->filter_lp_cutoff_.setTargetValue(val); 
        }
		FT_ getFilterLPCutoff() { return this->filter_lp_cutoff_.getTargetValue(); }
		void setFilterHPCutoff(FT_ val) { 
            this->filter_hp_cutoff_.setTargetValue(val); 
        }
		FT_ getFilterHPCutoff() { return this->filter_hp_cutoff_.getTargetValue(); }
		void setGritEnable(bool enable) { 
            this->grit_enable_ = enable; 
        }
		bool getGritEnable() { return this->grit_enable_; }
		void setOutLevel(FT_ val) { this->out_level_.setTargetValue(val); }
		FT_ getOutLevel() { return this->out_level_.getTargetValue(); }

    private:

        // gain compensation for low density noise
        static constexpr FT_ DENSITY_GAIN_COMP_ = FT_(3.0);

        juce::SmoothedValue<FT_> filter_lp_cutoff_;
		juce::SmoothedValue<FT_> filter_hp_cutoff_;
		bool grit_enable_ = true;
		FT_ noise_density_ = FT_(1.0);
		juce::SmoothedValue<FT_> noise_mix_;
		juce::SmoothedValue<FT_> out_level_;

		juce::AudioBuffer<FT_> work_buffer_;
		juce::dsp::StateVariableTPTFilter<FT_> filter_lp_;
		juce::dsp::StateVariableTPTFilter<FT_> filter_hp_;

	    float sample_rate_ = 48000.f;

	    stoej::WhiteNoise<FT_> noise_generator_;
    };
}