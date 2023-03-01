#pragma once
#include <juce_core/juce_core.h>
#include <stoej_core/stoej_core.h>

namespace stoej {
    STOEJ_FT_
    class WhiteNoise {
    public:

        /// initialize with random seed
        WhiteNoise();

        /// initialize with given seed
        WhiteNoise(const juce::int64 seed);

        void prepare(juce::dsp::ProcessSpec& spec);

        void reset();

        // must be in header to maximize chance of good optimization
        template <typename ProcessContext>
        void process(const ProcessContext& context) noexcept;

        /// process a single sample instead of a buffer, note that the width
        /// parameter does nothing when processing single samples as it
        /// requires multi-channel awareness
        FT_ processSample() {
            if consteval (std::is_same_v<FT_, float>) {
                return this->random_.nextFloat() * 2.0f - 1.0f;
            }
            return this->random_.nextDouble() * 2.0 - 1.0;
        }

        void setNoiseWidth(FT_ val) { this->noise_width_ = val; }
        FT_ getNoiseWidth() { return this->noise_width_; }

        /// Enables more expensive computation of noise width that guarantees uniform
        /// distribution for all intermediate values of noise width.
        ///
        /// Normally, intermediate widths are obtained by power preserving crossfade
        /// of a mono buffer and a stereo buffer. Perceptually, this sounds very
        /// smooth, however the statistical distribution of the noise changes with
        /// the value of width, which would affect any non-linear post processing
        /// done to the noise.
        ///
        /// To counter this, enabling HQ width guarantees that the distribution
        /// will remain uniform for all values of noise_width_, by adding an
        /// additional bernoulli random variable that chooses whether each sample
        /// is taken from the stereo buffer or the mono buffer.
        ///
        /// <b>Caveats</b>\n
        /// While this method is good enough for sound processing it has some
        /// strange undesired statistical side effects, so it should not be used as
        /// for any precise statistical purposes.
        ///
        /// Specifically, the individual samples in one frame are always either fully
        /// correlated or fully uncorrelated, but the probability of them being correlated
        /// or not determines their long-term average correlation.
        ///
        /// Acoustically this has no or very subtle effect, but it might have subtle sub-liminal
        /// or psychoacustic effects.
        void setEnableHQWidth(bool enable) { this->enable_hq_width_ = enable; }
        FT_ getEnableHQWidth() { return this->enable_hq_width_; }

    private:
        FT_ noise_width_ = FT_(.5);
        bool enable_hq_width_ = false;
        size_t max_size_;
        juce::int64 initial_seed_;
        juce::Random random_;
    }
}
