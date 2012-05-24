#ifndef SCENE_H
#define SCENE_H

#include <boost/smart_ptr.hpp>

#include "object.h"

class Camera;
class Application;
class BaseGameObject;
class GameObjectSet;
class FrameEventArgs;

class Scene : public Object {
	Q_OBJECT

private: std::weak_ptr<Application> application;
private: std::shared_ptr<GameObjectSet> gameObjects;
private: std::list< std::shared_ptr< BaseGameObject > > notStartedObjects;

public: Scene();
public: void initialize();

public: std::shared_ptr<Application> getApplication();
public: void setApplication(std::weak_ptr<Application>);

private: std::shared_ptr<GameObjectSet> getGameObjects();

public: void startUnstartedGameObjects();
public: void fixedUpdateGameObjects(std::shared_ptr<FrameEventArgs> args);
public: void updateGameObjects(std::shared_ptr<FrameEventArgs> args);
public: void deleteDestroyedGameObjects();

public: void draw(std::shared_ptr<FrameEventArgs> args);
private: std::list < std::shared_ptr< Camera > > getSortedCameras() const;
private: void Scene::enableLights();
private: void drawGameObjects(std::shared_ptr<FrameEventArgs> args);

public: void add(std::shared_ptr<BaseGameObject> object);
};

#endif