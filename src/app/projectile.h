#include "../engine/gameobject/basecomponent.h"

class Projectile : public BaseComponent {
	Q_OBJECT

public: Q_INVOKABLE Projectile();

public: virtual void update(std::shared_ptr< FrameEventArgs > args);
public: virtual void onCollide(std::shared_ptr< BaseGameObject > collider);
};