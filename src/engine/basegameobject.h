#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "frameeventargs.h"

class BaseGameObject {
public: void update(FrameEventArgs* args) {}
public: void draw(FrameEventArgs* args) {}
};

#endif //GAMEOBJECT_H