/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ParameterDefaults.h"



//==============================================================================
TruSynth2AudioProcessor::TruSynth2AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
    , m_sinTable(WaveTable::createSineTable())
    , m_squareTable(WaveTable::createSquareTable())
    , m_sawtoothTable(WaveTable::createSawtoothTable())
    , m_osc1(&m_sawtoothTable)
    , m_osc2(&m_sawtoothTable)
    , m_currentMidiNote(-1)
    , m_amplitude(0.0f)
    , masterVolumeDb(new juce::AudioParameterFloat("masterVolume", "Master Volume", -100.0f, 0.0f, -12.0f))
    , filterFrequency(new juce::AudioParameterFloat("filterFrequency", "Filter Frequency", 10.0f, 20000.0f, 4000.0f))
    , filterResonance(new juce::AudioParameterFloat("filterResonance", "Filter Resonance", ParamDefaults::filterResonance, 10.0f, ParamDefaults::filterResonance))

    , osc1WaveType(new juce::AudioParameterChoice("osc1WaveType", "Osc1 Wave Type", { "Sine", "Square", "Sawtooth" }, 2))
    , osc1Level(new juce::AudioParameterFloat("osc1Level", "Osc1 Level", 0.0f, 1.0f, 0.5f))
    , osc1DetuneCents(new juce::AudioParameterInt("osc1DetuneCents", "Osc1 Detune Cents", -100, 100, 0))
    , osc1DetuneSemitones(new juce::AudioParameterInt("osc1DetuneSemitones", "Osc1 Detune Semitones", -36, 36, 0))

    , osc2WaveType(new juce::AudioParameterChoice("osc2WaveType", "Osc2 Wave Type", { "Sine", "Square", "Sawtooth" }, 2))
    , osc2Level(new juce::AudioParameterFloat("osc2Level", "Osc2 Level", 0.0f, 1.0f, 0.5f))
    , osc2DetuneCents(new juce::AudioParameterInt("osc2DetuneCents", "Osc2 Detune Cents", -100, 100, 10))
    , osc2DetuneSemitones(new juce::AudioParameterInt("osc2DetuneSemitones", "Osc2 Detune Semitones", -36, 36, 0))
{
    addParameter(masterVolumeDb);
    addParameter(filterFrequency);
    addParameter(filterResonance);

    addParameter(osc1WaveType);
    addParameter(osc1Level);
    addParameter(osc1DetuneCents);
    addParameter(osc1DetuneSemitones);

    addParameter(osc2WaveType);
    addParameter(osc2Level);
    addParameter(osc2DetuneCents);
    addParameter(osc2DetuneSemitones);
}

TruSynth2AudioProcessor::~TruSynth2AudioProcessor()
{
}

//==============================================================================
const juce::String TruSynth2AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TruSynth2AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TruSynth2AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TruSynth2AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TruSynth2AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TruSynth2AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TruSynth2AudioProcessor::getCurrentProgram()
{
    return 0;
}

void TruSynth2AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String TruSynth2AudioProcessor::getProgramName (int index)
{
    return {};
}

void TruSynth2AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void TruSynth2AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void TruSynth2AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TruSynth2AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void TruSynth2AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    m_amplitude = juce::Decibels::decibelsToGain(masterVolumeDb->get());
    m_filter.setFrequency(filterFrequency->get(), getSampleRate());
    m_filter.setQ(filterResonance->get());
    updateOscParams();
    auto nextMidiMessage = midiMessages.findNextSamplePosition(0);
    for (int i = 0; i < buffer.getNumSamples(); i++) {
        if (nextMidiMessage != midiMessages.cend() && (*nextMidiMessage).samplePosition == i) {
            auto midiMsg = (*nextMidiMessage).getMessage();
            processMidiMessage(midiMsg);
            nextMidiMessage = midiMessages.findNextSamplePosition(i+1);
        }

        float sample = getNextSample();
        for (int channel = 0; channel < totalNumOutputChannels; ++channel)
        {
            buffer.setSample(channel, i, sample);
        }
    }
}

float TruSynth2AudioProcessor::getNextSample()
{
    // TODO: implement amp envelope
    if (m_currentMidiNote < 0) {
        return 0.0f;
    }

    // sample the oscillators
    float osc1Sample = m_osc1.getNextSample();
    float osc2Sample = m_osc2.getNextSample();

    float result = (osc1Sample + osc2Sample) * m_amplitude;
    result = m_filter.processSample(result);
    return result;
}

void TruSynth2AudioProcessor::processMidiMessage(juce::MidiMessage& msg)
{
    if (msg.isNoteOn()) {
        if (m_currentMidiNote < 0) {
            // restart the wave position to avoid a pop
            m_osc1.resetCycle();
            m_osc2.resetCycle();
        }
        m_currentMidiNote = msg.getNoteNumber();
        updateOscParams();
    }
    else if (msg.isNoteOff()) {
        if (msg.getNoteNumber() == m_currentMidiNote) {
            // Only turn off the note if it was the most recently pressed one
            m_currentMidiNote = -1;
        }
    }
}

void TruSynth2AudioProcessor::updateOscParams()
{
    float noteFrequency = 440.0f * std::powf(2, ((float)m_currentMidiNote - 69) / 12);

    float osc1PitchShiftCents = (osc1DetuneSemitones->get() * 100) + osc1DetuneCents->get();
    m_osc1.setFrequency(noteFrequency, osc1PitchShiftCents, (float)getSampleRate());
    m_osc1.setLevel(osc1Level->get());
    setOscWaveType(m_osc1, (WaveType)osc1WaveType->getIndex());

    float osc2PitchShiftCents = (osc2DetuneSemitones->get() * 100) + osc2DetuneCents->get();
    m_osc2.setFrequency(noteFrequency, osc2PitchShiftCents, (float)getSampleRate());
    m_osc2.setLevel(osc2Level->get());
    setOscWaveType(m_osc2, (WaveType)osc2WaveType->getIndex());
}

//==============================================================================
bool TruSynth2AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* TruSynth2AudioProcessor::createEditor()
{
    return new TruSynth2AudioProcessorEditor (*this);
}

//==============================================================================
void TruSynth2AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void TruSynth2AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void TruSynth2AudioProcessor::setOscWaveType(Oscillator& osc, WaveType waveType)
{
    switch (waveType) {
    case WaveType::WAVE_SINE:
        osc.setWaveTable(&m_sinTable);
        break;
    case WaveType::WAVE_SQUARE:
        osc.setWaveTable(&m_squareTable);
        break;
    case WaveType::WAVE_SAWTOOTH:
        osc.setWaveTable(&m_sawtoothTable);
        break;
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TruSynth2AudioProcessor();
}
