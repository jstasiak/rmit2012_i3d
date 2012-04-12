#include "precompile.h"
#include "wave.h"

float Wave::getLength() const {
	assert(this->length > 0);
	return this->length;
}

Wave* Wave::setLength(float value) {
	assert(value > 0);
	this->length = value;
	return this;
}

float Wave::getFrequency() const {
	assert(this->frequency > 0);
	return this->frequency;
}

Wave* Wave::setFrequency(float value) {
	assert(value > 0);
	this->frequency = value;
	return this;
}

float Wave::getAmplitude() const {
	assert(this->amplitude > 0);
	return this->amplitude;
}

Wave* Wave::setAmplitude(float value) {
	assert(value > 0);
	this->amplitude = value;
	return this;
}

float Wave::getPhase() const {
	return this->phase;
}

Wave* Wave::setPhase(float value) {
	this->phase = value;
	return this;
}

/**
 * Calculates A * sin(kx - wt) value for given position (x) and time (t)
 */
float Wave::valueForPositionAndTime(float position, float time) const {
	float sineParameters = this->getK() * position - this->getRadiansPerSecond() * time + this->phase;
	float value = this->amplitude * sin(sineParameters);
	return value;
}

float Wave::derivativeForPositionAndTime(float position, float time) {
	// Add PI/2 phase difference so we get cos() instead of sin()
	float phase = M_PI / 2.0f;
	this->phase += phase;

	float derivative = this->getK() * this->valueForPositionAndTime(position, time);

	// Return to original state
	this->phase -= phase;

	return derivative;
}

/**
 * Gets wavenumber for current wavelength.
 */
float Wave::getK() const {
	return 2.0f * M_PI / this->length;
}

/**
 * Gets angular frequency (in radians per second) for current
 * wave frequency.
 */
float Wave::getRadiansPerSecond() const {
	return 2.0f * M_PI * this->frequency;
}

Wave::Wave()
	: length(1), frequency(1), amplitude(1), phase(0) {

}
