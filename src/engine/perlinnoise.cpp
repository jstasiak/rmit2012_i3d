#include "precompile.h"

#include "perlinnoise.h"

PerlinNoise::PerlinNoise()
{
}

float PerlinNoise::getValue(float x, float y) {
	int bx = abs(x);
	int by = abs(y);

	float s = this->getValueAtGridPoint(bx, by);
	float t = this->getValueAtGridPoint(bx + 1, by);
	float u = this->getValueAtGridPoint(bx, by + 1);
	float v = this->getValueAtGridPoint(bx + 1, by + 1);

	float sAndT = this->interpolate(s, t, 1.0f - (x - bx));
	float uAndV = this->interpolate(u, v, 1.0f - (x - bx));

	float final = this->interpolate(sAndT, uAndV, 1.0f - (y - by));
	return final;
}

float PerlinNoise::getValueAtGridPoint(int x, int y) {
	int n = x + y * 57;
	n = (n << 13) ^ n;
	int nn = (n * (n * n * 60493 + 19990303) + 1376312589) & 0x7fffffff;
	return 1.0f - ( (float) nn / 1073741824.0f);
}

float PerlinNoise::interpolate(float a, float b, float shareOfA) {
	float shareOfB = 1.0f - shareOfA;
	shareOfB = (1.0f - cos(shareOfB * 3.141592f)) * 0.5f;
	return a * (1.0f - shareOfB) + b * shareOfB;
}