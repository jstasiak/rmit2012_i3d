#ifndef WAVE_H
#define WAVE_H

class Wave {
private:
	float amplitude;
	float length;
	float frequency;
public:
	Wave();

	float getAmplitude() const;
	Wave* setAmplitude(float value);

	float getLength() const;
	Wave* setLength(float value);

	float getFrequency() const;
	Wave* setFrequency(float value);

	float valueForPositionAndTime(float position, float time) const;

private:
	float getK() const;
	float getRadiansPerSecond() const;
};

#endif // WAVE_H