#ifndef BASECAMERA_H
#define BASECAMERA_H

namespace glm {
	class mat4x4;
}

class BaseCamera {
private:
	float near;
	float far;
public:
	virtual const glm::mat4x4* getProjectionMatrix() = 0;
	virtual const glm::mat4x4* getModelViewMatrix() = 0;

	float getNearClipDistance() const;
	void setNearClipDistance(float value);

	float getFarClipDistance() const;
	void setFarClipDistance(float value);
};

#endif // BASECAMERA_H