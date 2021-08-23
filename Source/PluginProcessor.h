/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "WaveTable.h"
#include "Oscillator.h"
#include "Filter.h"

//==============================================================================
/**
*/
class TruSynth2AudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    TruSynth2AudioProcessor();
    ~TruSynth2AudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // Parameters
    juce::AudioParameterFloat* masterVolumeDb;

    juce::AudioParameterChoice* osc1WaveType;
    juce::AudioParameterFloat* osc1Level;
    juce::AudioParameterInt* osc1DetuneCents;
    juce::AudioParameterInt* osc1DetuneSemitones;

    juce::AudioParameterChoice* osc2WaveType;
    juce::AudioParameterFloat* osc2Level;
    juce::AudioParameterInt* osc2DetuneCents;
    juce::AudioParameterInt* osc2DetuneSemitones;

    juce::AudioParameterFloat* filterFrequency;
    juce::AudioParameterFloat* filterResonance;

private:
    WaveTable m_sinTable;
    WaveTable m_squareTable;
    WaveTable m_sawtoothTable;
    Oscillator m_osc1, m_osc2;
    Filter m_filter;

    void setOscWaveType(Oscillator& osc, WaveType waveType);
    void processMidiMessage(juce::MidiMessage& msg);
    void updateOscParams();
    float getNextSample();

    int m_currentMidiNote;
    float m_amplitude;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TruSynth2AudioProcessor)
};
