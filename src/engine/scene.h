#ifndef SCENE_H
#define SCENE_H

#include <boost/smart_ptr.hpp>

#include "object.h"

class Camera;
class Application;
class BaseGameObject;
class GameObjectSet;
class FrameEventArgs;
class Skybox;

class Scene : public Object {

private: std::weak_ptr<Application> application;
private: std::shared_ptr<GameObjectSet> gameObjects;
private: std::list< std::shared_ptr< BaseGameObject > > notStartedObjects;

private: std::shared_ptr< Camera > activeCamera;

private: std::shared_ptr< Skybox > skybox;

public: Scene();
public: void initialize();

public: std::shared_ptr<Application> getApplication();
public: void setApplication(std::weak_ptr<Application>);

private: std::shared_ptr<GameObjectSet> getGameObjects();

public: void startUnstartedGameObjects();
public: void fixedUpdateGameObjects(std::shared_ptr<FrameEventArgs> args);
public: void updateGameObjects(std::shared_ptr<FrameEventArgs> args);
private: void checkForCollisions();
public: void deleteDestroyedGameObjects();

public: void draw(std::shared_ptr<FrameEventArgs> args);
private: std::list < std::shared_ptr< Camera > > getSortedCameras() const;
private: void Scene::enableLights();

private: void drawGameObjects(std::shared_ptr<FrameEventArgs> args);
private: void drawGui();

private: void drawSkyboxForCamera(std::shared_ptr< Camera > camera);
private: void drawSkybox();

public: void add(std::shared_ptr<BaseGameObject> object);

public: std::shared_ptr< Camera > getActiveCamera();
private: void setActiveCamera(std::shared_ptr< Camera > value);
};

#endif