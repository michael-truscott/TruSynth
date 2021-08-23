/*
  ==============================================================================

    Oscillator.h
    Created: 22 Aug 2021 2:45:36pm
    Author:  pickl

  ==============================================================================
*/

#pragma once

#include "WaveTable.h"

enum class WaveType {
    WAVE_SINE,
    WAVE_SQUARE,
    WAVE_SAWTOOTH,
};

class Oscillator {
public:
    Oscillator(WaveTable* table);

    float getNextSample();
    void setFrequency(float frequency, int detuneCents, float sampleRate);
    void setLevel(float level);
    void setWaveTable(WaveTable* table);
    void resetCycle();

private:
    float m_t;
    float m_deltaT;
    float m_level;

    WaveTable* m_waveTable;
};