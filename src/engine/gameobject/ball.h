#include "basegameobject.h"

class Ball : public BaseGameObject {
	Q_OBJECT

private: float radius;
private: GLUquadric* quadric;

public: Q_INVOKABLE Ball();
public: ~Ball();

public: float getRadius() const;
public: void setRadius(float value);

public: virtual void start();
public: virtual void draw(std::shared_ptr< FrameEventArgs > args);
};