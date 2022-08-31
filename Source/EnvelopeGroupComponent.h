/*
  ==============================================================================

    EnvelopeGroupComponent.h
    Created: 31 Aug 2022 7:58:57pm
    Author:  Michael

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class EnvelopeGroupComponent  : public juce::GroupComponent
{
public:
    EnvelopeGroupComponent(juce::AudioParameterFloat& attack, juce::AudioParameterFloat& decay, juce::AudioParameterFloat& sustain, juce::AudioParameterFloat& release) :
        m_attack(attack),
        m_decay(decay),
        m_sustain(sustain),
        m_release(release)
    {
        // Attack
        attackLabel.setText("Attack", juce::NotificationType::dontSendNotification);
        attackLabel.attachToComponent(&attackSlider, true);
        addAndMakeVisible(attackLabel);

        attackSlider.setRange(m_attack.range.start, m_attack.range.end, 0.01);
        attackSlider.setDoubleClickReturnValue(true, ParamDefaults::ampEnvAttack);
        attackSlider.setTextValueSuffix(" secs");
        attackSlider.setSkewFactorFromMidPoint(2.0);
        attackSlider.setValue(m_attack.get());
        attackSlider.onValueChange = [this] { m_attack = (float)attackSlider.getValue(); };
        addAndMakeVisible(attackSlider);

        // Decay
        decayLabel.setText("Decay", juce::NotificationType::dontSendNotification);
        decayLabel.attachToComponent(&decaySlider, true);
        addAndMakeVisible(decayLabel);

        decaySlider.setRange(m_decay.range.start, m_decay.range.end, 0.01);
        decaySlider.setDoubleClickReturnValue(true, ParamDefaults::ampEnvDecay);
        decaySlider.setTextValueSuffix(" secs");
        decaySlider.setSkewFactorFromMidPoint(2.0);
        decaySlider.setValue(m_decay.get());
        decaySlider.onValueChange = [this] { m_decay = (float)decaySlider.getValue(); };
        addAndMakeVisible(decaySlider);

        // Sustain
        sustainLabel.setText("Sustain", juce::NotificationType::dontSendNotification);
        sustainLabel.attachToComponent(&sustainSlider, true);
        addAndMakeVisible(sustainLabel);

        sustainSlider.setRange(m_sustain.range.start, m_sustain.range.end, 0.01);
        sustainSlider.setDoubleClickReturnValue(true, ParamDefaults::ampEnvSustain);
        sustainSlider.setValue(m_sustain.get());
        sustainSlider.onValueChange = [this] { m_sustain = (float)sustainSlider.getValue(); };
        addAndMakeVisible(sustainSlider);

        // Release
        releaseLabel.setText("Release", juce::NotificationType::dontSendNotification);
        releaseLabel.attachToComponent(&releaseSlider, true);
        addAndMakeVisible(releaseLabel);

        releaseSlider.setRange(m_release.range.start, m_release.range.end, 0.01);
        releaseSlider.setDoubleClickReturnValue(true, ParamDefaults::ampEnvRelease);
        releaseSlider.setTextValueSuffix(" secs");
        releaseSlider.setSkewFactorFromMidPoint(2.0);
        releaseSlider.setValue(m_release.get());
        releaseSlider.onValueChange = [this] { m_release = (float)releaseSlider.getValue(); };
        addAndMakeVisible(releaseSlider);
    }

    ~EnvelopeGroupComponent() override
    {
    }

    void resized() override
    {
        attackSlider.setBounds(100, 20, getWidth() - 120, 20);
        decaySlider.setBounds(100, 40, getWidth() - 120, 20);
        sustainSlider.setBounds(100, 60, getWidth() - 120, 20);
        releaseSlider.setBounds(100, 80, getWidth() - 120, 20);
    }

private:
    juce::Label attackLabel;
    juce::Slider attackSlider;

    juce::Label decayLabel;
    juce::Slider decaySlider;

    juce::Label sustainLabel;
    juce::Slider sustainSlider;

    juce::Label releaseLabel;
    juce::Slider releaseSlider;

    juce::AudioParameterFloat& m_attack;
    juce::AudioParameterFloat& m_decay;
    juce::AudioParameterFloat& m_sustain;
    juce::AudioParameterFloat& m_release;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EnvelopeGroupComponent)
};
