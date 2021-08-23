/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "OscillatorGroupComponent.h"
#include "FilterGroupComponent.h"

//==============================================================================
/**
*/
class TruSynth2AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    TruSynth2AudioProcessorEditor (TruSynth2AudioProcessor&);
    ~TruSynth2AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TruSynth2AudioProcessor& audioProcessor;

    juce::Label masterVolumeLabel;
    juce::Slider masterVolumeSlider;

    OscillatorGroupComponent osc1Group, osc2Group;
    FilterGroupComponent filterGroup;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TruSynth2AudioProcessorEditor)
};
