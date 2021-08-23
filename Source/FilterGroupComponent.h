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
    FilterGroupComponent(juce::AudioParameterFloat& frequency) :
        m_frequency(frequency)
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
    }

    ~FilterGroupComponent() override
    {
    }

    void resized() override
    {
        frequencySlider.setBounds(100, 20, getWidth() - 120, 20);
    }

private:
    juce::Label frequencyLabel;
    juce::Slider frequencySlider;

    juce::AudioParameterFloat& m_frequency;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterGroupComponent)
};
