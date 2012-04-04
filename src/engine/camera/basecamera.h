
#ifndef BASECAMERA_H
#define BASECAMERA_H

class BaseCamera {
private:
	float n;
	float f;
public:
	virtual const glm::mat4x4* getProjectionMatrix() = 0;
	virtual const glm::mat4x4* getModelViewMatrix() = 0;

	float getNearClipDistance() const;
	void setNearClipDistance(float value);

	float getFarClipDistance() const;
	void setFarClipDistance(float value);
};

#endif // BASECAMERA_H