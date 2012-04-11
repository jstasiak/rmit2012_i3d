#include "precompile.h"
#include "wave.h"

float Wave::getLength() const {
	return this->length;
}

Wave* Wave::setLength(float value){
	this->length = value;
	return this;
}

float Wave::getFrequency() const {
	return this->frequency;
}

Wave* Wave::setFrequency(float value) {
	this->frequency = value;
	return this;
}

float Wave::getAmplitude() const {
	return this->amplitude;
}

Wave* Wave::setAmplitude(float value) {
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
	: length(1), frequency(1), amplitude(1) {

}
