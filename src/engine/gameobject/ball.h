#include "basegameobject.h"

#include <GL/glu.h>

class Ball : public BaseGameObject {
private: float radius;
private: GLUquadric* quadric;

public: Ball();
public: ~Ball();

public: float getRadius() const;
public: void setRadius(float value);

public: virtual void start();
public: virtual void draw(std::shared_ptr< FrameEventArgs > args);
};
