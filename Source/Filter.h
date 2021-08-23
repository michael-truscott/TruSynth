/*
  ==============================================================================

    Filter.h
    Created: 23 Aug 2021 2:45:43pm
    Author:  pickl

  ==============================================================================
*/

#pragma once
// Basic biquad low pass filter
class Filter {
public:
    Filter();

    void setFrequency(float frequency, float sampleRate);
    void setQ(float q);
    float processSample(float sample);

private:
    void recalcCoefficients();

    float m_sampleRate;
    float m_frequency;
    float m_q;
    float m_prevX[2], m_prevY[2];

    float m_w0;
    float m_b0, m_b1, m_b2;
    float m_a0, m_a1, m_a2;
};