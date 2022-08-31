/*
  ==============================================================================

    Envelope.cpp
    Created: 31 Aug 2022 6:46:11pm
    Author:  Michael

  ==============================================================================
*/

#include "Envelope.h"

Envelope::Envelope(float a, float d, float s, float r) :
	a(a), d(d), s(s), r(r),
	value(0.0f),
	peak(0.0f),
	stateElapsed(0.0f),
	state(EnvelopeState::ENV_IDLE)
{
}

void Envelope::setParams(float _a, float _d, float _s, float _r) {
	a = _a; d = _d; s = _s; r = _r;
}

float Envelope::getValue() const { return value; }

float Envelope::interpolate(float f0, float f1, float t)
{
    return f0 + t * (f1 - f0);
}

void Envelope::update(double dt) {
	stateElapsed += dt;
	switch (state) {
	case EnvelopeState::ENV_ATTACK:
		if (a == 0.0f || stateElapsed > a) {
			state = EnvelopeState::ENV_DECAY;
			stateElapsed = 0.0f;
			value = 1.0f;
		}
		else {
			float t = stateElapsed / a;
			value = interpolate(0.0f, 1.0f, t);
		}
		break;
	case EnvelopeState::ENV_DECAY:
		if (d == 0.0f || stateElapsed > d) {
			state = EnvelopeState::ENV_SUSTAIN;
			stateElapsed = 0.0f;
			value = s;
		}
		else {
			float t = stateElapsed / d;
			value = interpolate(1.0f, s, t);
		}
		break;
	case EnvelopeState::ENV_SUSTAIN:
		value = s;
		break;
	case EnvelopeState::ENV_RELEASE:
		if (r == 0.0f || value <= 0.0f) {
			state = EnvelopeState::ENV_IDLE;
			value = 0.0f;
		}
		else {
			float t = stateElapsed / r;
			value = interpolate(peak, 0.0f, t);
		}
		break;
	}
}

void Envelope::keyDown() {
	value = 0.0f;
	stateElapsed = 0.0f;
	state = EnvelopeState::ENV_ATTACK;
}
void Envelope::keyUp() {
	switch (state) {
	case EnvelopeState::ENV_ATTACK:
	case EnvelopeState::ENV_DECAY:
	case EnvelopeState::ENV_SUSTAIN:
		state = EnvelopeState::ENV_RELEASE;
		peak = value;
		stateElapsed = 0.0f;
	}
}