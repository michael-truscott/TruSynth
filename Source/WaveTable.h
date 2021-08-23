/*
  ==============================================================================

    WaveTable.h
    Created: 22 Aug 2021 2:54:46pm
    Author:  pickl

  ==============================================================================
*/

#pragma once

#include <memory>

class WaveTable {
public:
    WaveTable(std::unique_ptr<float[]> data, int length);
    float getSample(float t);

    static WaveTable createSineTable();
    static WaveTable createSquareTable();
    static WaveTable createSawtoothTable();

private:
    std::unique_ptr<float[]> m_data;
    int m_length;
};