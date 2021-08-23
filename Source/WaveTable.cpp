/*
  ==============================================================================

    WaveTable.cpp
    Created: 22 Aug 2021 2:54:46pm
    Author:  pickl

  ==============================================================================
*/

#define _USE_MATH_DEFINES

#include "WaveTable.h"
#include <stdexcept>
#include <cmath>

WaveTable::WaveTable(std::unique_ptr<float[]> data, int length) :
    m_data(std::move(data)),
    m_length(length)
{
}

/// <summary>
/// Return the sample at the point in the waveform specified by t.
/// </summary>
/// <param name="t">The point between 0 (start) and 1 (end) to sample the waveform from.</param>
/// <returns>The sample at point t</returns>
float WaveTable::getSample(float t)
{
    if (t < 0.0f || t >= 1.0f)
        throw std::invalid_argument("t out of bounds");
    int sampleIndex = (int)(t * m_length);
    return m_data[sampleIndex];
}


//===================================================
// Factory Methods
//===================================================

WaveTable WaveTable::createSineTable()
{
    auto data = std::make_unique<float[]>(128);
    for (int i = 0; i < 128; i++) {
        float t = (float)i / 128 * (2 * M_PI);
        data[i] = std::sinf(t);
    }
    return WaveTable(std::move(data), 128);
}

WaveTable WaveTable::createSquareTable()
{
    auto data = std::make_unique<float[]>(128);
    for (int i = 0; i < 128; i++) {
        data[i] = (i < 128 / 2) ? 1.0f : -1.0f;
    }
    return WaveTable(std::move(data), 128);
}

WaveTable WaveTable::createSawtoothTable()
{
    auto data = std::make_unique<float[]>(128);
    for (int i = 0; i < 128; i++) {
        float t = (float)i / 128;
        data[i] = 1.0f - (2 * t);
    }
    return WaveTable(std::move(data), 128);
}
