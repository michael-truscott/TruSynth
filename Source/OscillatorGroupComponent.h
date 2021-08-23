/*
  ==============================================================================

    OscillatorGroupComponent.h
    Created: 22 Aug 2021 11:32:56pm
    Author:  pickl

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class OscillatorGroupComponent  : public juce::GroupComponent
{
public:
    OscillatorGroupComponent(juce::AudioParameterFloat* level,
        juce::AudioParameterInt* detuneSemitones,
        juce::AudioParameterInt* detuneCents,
        juce::AudioParameterChoice* waveType) :
        m_level(level),
        m_detuneSemitones(detuneSemitones),
        m_detuneCents(detuneCents),
        m_waveType(waveType)
    {
        // Wave type radio buttons
        waveTypeLabel.setText("Wave Type", juce::NotificationType::dontSendNotification);
        addAndMakeVisible(waveTypeLabel);

        waveTypeSine.setClickingTogglesState(true);
        waveTypeSine.setRadioGroupId(1);
        waveTypeSine.setToggleState(waveType->getIndex() == (int)WaveType::WAVE_SINE, juce::NotificationType::dontSendNotification);
        waveTypeSine.onClick = [this] { *m_waveType = (int)WaveType::WAVE_SINE; };
        addAndMakeVisible(waveTypeSine);

        waveTypeSquare.setClickingTogglesState(true);
        waveTypeSquare.setRadioGroupId(1);
        waveTypeSquare.setToggleState(waveType->getIndex() == (int)WaveType::WAVE_SQUARE, juce::NotificationType::dontSendNotification);
        waveTypeSquare.onClick = [this] { *m_waveType = (int)WaveType::WAVE_SQUARE; };
        addAndMakeVisible(waveTypeSquare);

        waveTypeSawtooth.setClickingTogglesState(true);
        waveTypeSawtooth.setRadioGroupId(1);
        waveTypeSawtooth.setToggleState(waveType->getIndex() == (int)WaveType::WAVE_SAWTOOTH, juce::NotificationType::dontSendNotification);
        waveTypeSawtooth.onClick = [this] { *m_waveType = (int)WaveType::WAVE_SAWTOOTH; };
        addAndMakeVisible(waveTypeSawtooth);

        // Sliders
        levelLabel.setText("Level", juce::NotificationType::dontSendNotification);
        levelLabel.attachToComponent(&levelSlider, true);
        addAndMakeVisible(levelLabel);

        levelSlider.setRange(m_level->range.start, m_level->range.end, 0.01f);
        levelSlider.setValue(m_level->get());
        levelSlider.onValueChange = [this] { *m_level = levelSlider.getValue(); };
        addAndMakeVisible(levelSlider);

        detuneLabel.setText("Detune", juce::NotificationType::dontSendNotification);
        detuneLabel.attachToComponent(&detuneSlider, true);
        addAndMakeVisible(detuneLabel);

        detuneSlider.setRange(detuneCents->getRange().getStart(), detuneCents->getRange().getEnd(), 1.0f);
        detuneSlider.setValue(detuneCents->get());
        detuneSlider.setTextValueSuffix(" cents");
        detuneSlider.onValueChange = [this] { *m_detuneCents = detuneSlider.getValue(); };
        addAndMakeVisible(detuneSlider);

        semitoneLabel.setText("Pitch", juce::NotificationType::dontSendNotification);
        semitoneLabel.attachToComponent(&semitoneSlider, true);
        addAndMakeVisible(semitoneLabel);

        semitoneSlider.setRange(m_detuneSemitones->getRange().getStart(), m_detuneSemitones->getRange().getEnd(), 1.0f);
        semitoneSlider.setValue(m_detuneSemitones->get());
        semitoneSlider.setTextValueSuffix(" semitones");
        semitoneSlider.onValueChange = [this] { *m_detuneSemitones = semitoneSlider.getValue(); };
        addAndMakeVisible(semitoneSlider);
    }

    ~OscillatorGroupComponent() override
    {
    }

    void resized() override
    {
        waveTypeLabel.setBounds(20, 20, 100, 20);
        waveTypeSine.setBounds(120, 20, 60, 20);
        waveTypeSquare.setBounds(180, 20, 60, 20);
        waveTypeSawtooth.setBounds(240, 20, 60, 20);
        
        levelSlider.setBounds(100, 50, getWidth() - 120, 20);
        semitoneSlider.setBounds(100, 70, getWidth() - 120, 20);
        detuneSlider.setBounds(100, 90, getWidth() - 120, 20);
    }

private:
    juce::AudioParameterFloat* m_level;
    juce::AudioParameterInt* m_detuneSemitones;
    juce::AudioParameterInt* m_detuneCents;
    juce::AudioParameterChoice* m_waveType;

    juce::Label waveTypeLabel;
    juce::TextButton waveTypeSine{ "Sine" };
    juce::TextButton waveTypeSquare{ "Square" };
    juce::TextButton waveTypeSawtooth{ "Sawtooth" };

    juce::Label levelLabel;
    juce::Slider levelSlider;
    juce::Label detuneLabel;
    juce::Slider detuneSlider;
    juce::Label semitoneLabel;
    juce::Slider semitoneSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscillatorGroupComponent)
};
