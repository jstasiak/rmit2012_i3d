#include "../engine/gameobject/basecomponent.h"

class Projectile : public BaseComponent {
public: virtual const char* className() const {
		return "Projectile";
	}

public: Projectile();

public: virtual void update(std::shared_ptr< FrameEventArgs > args);
public: virtual void onCollide(std::shared_ptr< BaseGameObject > collider);
};