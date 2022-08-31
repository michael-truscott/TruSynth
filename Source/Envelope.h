/*
  ==============================================================================

    Envelope.h
    Created: 31 Aug 2022 6:46:11pm
    Author:  Michael

  ==============================================================================
*/

#pragma once

enum class EnvelopeState {
	ENV_IDLE,
	ENV_ATTACK,
	ENV_DECAY,
	ENV_SUSTAIN,
	ENV_RELEASE,
};

class Envelope {
public:
	Envelope(float a, float d, float s, float r);
	void setParams(float a, float d, float s, float r);
	
	float a, d, s, r;

	float getValue() const;
	void update(double dt);

	void keyDown();
	void keyUp();

private:
	float interpolate(float f0, float f1, float t);

	float peak;
	float value;
	double stateElapsed;
	EnvelopeState state;
};