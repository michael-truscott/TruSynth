/*
  ==============================================================================

    Filter.cpp
    Created: 23 Aug 2021 2:45:43pm
    Author:  pickl

  ==============================================================================
*/

#define _USE_MATH_DEFINES

#include "Filter.h"
#include <cmath>
#include <JuceHeader.h>

Filter::Filter() :
    m_q(0.7071f),
    m_prevX(),
    m_prevY(),
    m_sampleRate(44100.0f),
    m_frequency(1000.0f),
    m_w0(0.0f),
    m_b0(0.0f), m_b1(0.0f), m_b2(0.0f),
    m_a0(0.0f), m_a1(0.0f), m_a2(0.0f)
{
    recalcCoefficients();
}

void Filter::setFrequency(float frequency, float sampleRate)
{
    if (m_frequency != frequency || m_sampleRate != sampleRate) {
        m_frequency = frequency;
        m_sampleRate = sampleRate;
        recalcCoefficients();
    }
}

void Filter::setQ(float q)
{
    if (m_q != q) {
        m_q = q;
        recalcCoefficients();
    }
}

float Filter::processSample(float sample)
{
    float result = sample;

    /*
    * From https://web.archive.org/web/20120204043306/https://www.musicdsp.org/files/Audio-EQ-Cookbook.txt
    * The most straight forward implementation would be the "Direct Form 1"

    y[n] = (b0/a0)*x[n] + (b1/a0)*x[n-1] + (b2/a0)*x[n-2]
                        - (a1/a0)*y[n-1] - (a2/a0)*y[n-2]

    */
    // m_prevX[0] is n-1
    // m_prevX[1] is n-2
    result = (m_b0 / m_a0) * sample
        + (m_b1 / m_a0) * m_prevX[0] + (m_b2 / m_a0) * m_prevX[1]
        - (m_a1 / m_a0) * m_prevY[0] - (m_a2 / m_a0) * m_prevY[1];

    m_prevX[1] = m_prevX[0];
    m_prevX[0] = sample;

    m_prevY[1] = m_prevY[0];
    m_prevY[0] = result;

    return result;
}

void Filter::recalcCoefficients()
{
    m_w0 = juce::MathConstants<float>::twoPi * m_frequency / m_sampleRate;
    
    float alpha = std::sinf(m_w0) / (2 * m_q);

    m_b0 = (1 - std::cosf(m_w0)) / 2;
    m_b1 = 1 - std::cosf(m_w0);
    m_b2 = (1 - std::cosf(m_w0)) / 2;
    m_a0 = 1 + alpha;
    m_a1 = -2 * std::cosf(m_w0);
    m_a2 = 1 - alpha;
}
