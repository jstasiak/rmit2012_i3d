#ifndef SKYBOX_H
#define SKYBOX_H

#include <string>

#include <boost/smart_ptr.hpp>

class Camera;
class Texture;

class Skybox {
private: std::shared_ptr< Texture > texture;

public: Skybox(std::string fileName);

public: void drawForCamera(std::shared_ptr< Camera > camera);
};

#endif