/*
  ==============================================================================

    ParameterDefaults.h
    Created: 23 Aug 2021 9:18:39pm
    Author:  pickl

  ==============================================================================
*/

#pragma once

#include "Oscillator.h"

namespace ParamDefaults {
    const float root2Over2 = juce::MathConstants<float>::sqrt2 / 2;

    const float masterVolume = -12.0f;
    const float filterFrequency = 4000.0f;
    const float filterResonance = root2Over2;

    const WaveType oscWaveType = WaveType::WAVE_SAWTOOTH;
    const float oscLevel = 0.5f;
    const int oscDetuneCents = 0;
    const int oscDetuneSemitones = 0;

    const float ampEnvAttack = 0;
    const float ampEnvDecay = 0;
    const float ampEnvSustain = 1;
    const float ampEnvRelease = 0;
}