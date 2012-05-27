#ifndef PERLINNOISE_H
#define PERLINNOISE_H

class PerlinNoise {
public: PerlinNoise();

public: float getValue(float x, float y);

private: float getValueAtGridPoint(int x, int y);
private: float interpolate(float a, float b, float shareOfA);
};

#endif