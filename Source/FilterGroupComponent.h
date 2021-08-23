/*
  ==============================================================================

    FilterGroupComponent.h
    Created: 23 Aug 2021 2:45:29pm
    Author:  pickl

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class FilterGroupComponent  : public juce::GroupComponent
{
public:
    FilterGroupComponent(juce::AudioParameterFloat& frequency, juce::AudioParameterFloat& resonance) :
        m_frequency(frequency),
        m_resonance(resonance)
    {
        // Sliders
        frequencyLabel.setText("Cutoff", juce::NotificationType::dontSendNotification);
        frequencyLabel.attachToComponent(&frequencySlider, true);
        addAndMakeVisible(frequencyLabel);

        frequencySlider.setRange(m_frequency.range.start, m_frequency.range.end, 1.0f);
        frequencySlider.setSkewFactorFromMidPoint(2000.0f);
        frequencySlider.setTextValueSuffix(" Hz");
        frequencySlider.setValue(m_frequency.get());
        frequencySlider.onValueChange = [this] { m_frequency = frequencySlider.getValue(); };
        addAndMakeVisible(frequencySlider);

        resonanceLabel.setText("Resonance (Q)", juce::NotificationType::dontSendNotification);
        resonanceLabel.attachToComponent(&resonanceSlider, true);
        addAndMakeVisible(resonanceLabel);

        resonanceSlider.setRange(m_resonance.range.start, m_resonance.range.end, 0.01f);
        resonanceSlider.setValue(m_resonance.get());
        resonanceSlider.onValueChange = [this] {m_resonance = resonanceSlider.getValue(); };
        addAndMakeVisible(resonanceSlider);
    }

    ~FilterGroupComponent() override
    {
    }

    void resized() override
    {
        frequencySlider.setBounds(100, 20, getWidth() - 120, 20);
        resonanceSlider.setBounds(100, 40, getWidth() - 120, 20);
    }

private:
    juce::Label frequencyLabel;
    juce::Slider frequencySlider;

    juce::Label resonanceLabel;
    juce::Slider resonanceSlider;

    juce::AudioParameterFloat& m_frequency;
    juce::AudioParameterFloat& m_resonance;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterGroupComponent)
};
