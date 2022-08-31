/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ParameterDefaults.h"

//==============================================================================
TruSynth2AudioProcessorEditor::TruSynth2AudioProcessorEditor (TruSynth2AudioProcessor& p) : AudioProcessorEditor (&p),
    audioProcessor (p),
    osc1Group(audioProcessor.osc1Level, audioProcessor.osc1DetuneSemitones, audioProcessor.osc1DetuneCents, audioProcessor.osc1WaveType),
    osc2Group(audioProcessor.osc2Level, audioProcessor.osc2DetuneSemitones, audioProcessor.osc2DetuneCents, audioProcessor.osc2WaveType),
    filterGroup(*audioProcessor.filterFrequency, *audioProcessor.filterResonance),
    ampEnvGroup(*audioProcessor.ampEnvAttack, *audioProcessor.ampEnvDecay, *audioProcessor.ampEnvSustain, *audioProcessor.ampEnvRelease)
{
    setSize (800, 600);

    masterVolumeLabel.setText("Master Volume", juce::NotificationType::dontSendNotification);
    masterVolumeLabel.attachToComponent(&masterVolumeSlider, true);
    addAndMakeVisible(masterVolumeLabel);

    masterVolumeSlider.setRange(audioProcessor.masterVolumeDb->range.start, audioProcessor.masterVolumeDb->range.end, 0.1f);
    masterVolumeSlider.setValue(audioProcessor.masterVolumeDb->get());
    masterVolumeSlider.setDoubleClickReturnValue(true, ParamDefaults::masterVolume);
    masterVolumeSlider.setSkewFactorFromMidPoint(-24.0f);
    masterVolumeSlider.setTextValueSuffix(" dB");
    masterVolumeSlider.onValueChange = [this] { *audioProcessor.masterVolumeDb = (float)masterVolumeSlider.getValue(); };
    addAndMakeVisible(masterVolumeSlider);

    osc1Group.setText("Oscillator 1");
    addAndMakeVisible(osc1Group);

    osc2Group.setText("Oscillator 2");
    addAndMakeVisible(osc2Group);

    filterGroup.setText("Filter");
    addAndMakeVisible(filterGroup);

    ampEnvGroup.setText("Amp Envelope");
    addAndMakeVisible(ampEnvGroup);
}

TruSynth2AudioProcessorEditor::~TruSynth2AudioProcessorEditor()
{
}

//==============================================================================
void TruSynth2AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void TruSynth2AudioProcessorEditor::resized()
{
    masterVolumeSlider.setBounds(140, 20, getWidth() - 180, 20);

    osc1Group.setBounds(20, 60, getWidth() - 40, 120);
    osc2Group.setBounds(20, 180, getWidth() - 40, 120);
    filterGroup.setBounds(20, 300, getWidth() - 40, 80);
    ampEnvGroup.setBounds(20, 380, getWidth() - 40, 120);
}
