/*
  ==============================================================================

    Oscillator.cpp
    Created: 22 Aug 2021 2:45:36pm
    Author:  pickl

  ==============================================================================
*/

#include "Oscillator.h"
#include <cmath>

Oscillator::Oscillator(WaveTable* table) :
    m_waveTable(table),
    m_t(0.0f),
    m_deltaT(0.0f),
    m_level(1.0f)
{
}

float Oscillator::getNextSample()
{
    float result = m_waveTable->getSample(m_t);
    m_t += m_deltaT;
    if (m_t >= 1.0f) {
        m_t -= 1.0f;
    }
    return result * m_level;
}

void Oscillator::setFrequency(float frequency, int detuneCents, float sampleRate)
{
    float decimalCents = (float)detuneCents / 100;
    float detuneRatio = std::powf(2, decimalCents / 12);
    m_deltaT = (frequency * detuneRatio) / sampleRate;
}

void Oscillator::setLevel(float level)
{
    m_level = level;
}

void Oscillator::setWaveTable(WaveTable* table)
{
    m_waveTable = table;
}

void Oscillator::resetCycle()
{
    m_t = 0.0f;
}
