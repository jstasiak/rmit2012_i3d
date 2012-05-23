#include "precompile.h"
#include "scene.h"

#include "utils.h"
#include "gameobject/basegameobject.h"
#include "gameobject/gameobjectset.h"
#include "gameobject/camera.h"
#include "gameobject/transform.h"

using namespace std;

Scene::Scene()
	: gameObjects(),
	notStartedObjects(),
	application()
{
}

void Scene::initialize() {
	//TODO: load scene definition from file on demand
	auto r = Registry::getSharedInstance();

	auto ship1 = r->create<BaseGameObject>("Ship");
	ship1->setName("ship1");

	auto ship2 = r->create<BaseGameObject>("Ship");
	ship2->setName("ship2");

	this->add(r->create<BaseGameObject>("Water"));
	this->add(ship1);
	this->add(ship2);
	
	auto bo = r->create<BaseGameObject>();
	bo->getComponents()->add(r->create<BaseComponent>("Manager"));
	this->add(bo);


	auto c1 = r->create<Camera>();
	auto c2 = r->create<Camera>();
	c1->setNormalizedRect(Rectf(0, 0, 0.5, 1));
	c2->setNormalizedRect(Rectf(0.5, 0.0, 0.5, 1));
	c2->setDepth(1);

	c1->setTrackedObject(ship1);
	c2->setTrackedObject(ship2);

	this->add(c1);
	this->add(c2);
}


std::shared_ptr<Application> Scene::getApplication() {
	return this->application.lock();
}

void Scene::setApplication(std::weak_ptr<Application> value) {
	this->application = value;
}


std::shared_ptr<GameObjectSet> Scene::getGameObjects() {
	if(!this->gameObjects) {
		this->gameObjects = make_shared<GameObjectSet>();
		this->gameObjects->setOwner(this->getSharedPointer<Scene>());
	}

	return this->gameObjects;
}

void Scene::deleteDestroyedGameObjects() {
	this->getGameObjects()->deleteDestroyed();
}

void Scene::startUnstartedGameObjects() {
	auto& objects = this->notStartedObjects;
	for(auto i = objects.begin(); i != objects.end(); ++i) {
		auto o = *i;
		o->start();
	}

	objects.clear();
}

void Scene::updateGameObjects(std::shared_ptr<FrameEventArgs> args) {
	auto objects = this->gameObjects->getList();
	for(auto i = objects.begin(); i != objects.end(); ++i) {
		auto o = *i;
		o->update(args);
	}
}

void Scene::draw(std::shared_ptr<FrameEventArgs> args) {
	auto cameras = this->getSortedCameras();
	BOOST_FOREACH(auto camera, cameras) {
		camera->applyCamera();

		// Enable lights in camera space only so light position is ok
		this->enableLights();

		this->drawGameObjects(args);
	}

	SDL_GL_SwapBuffers();
}

std::list < std::shared_ptr< Camera > > Scene::getSortedCameras() const {
	list < shared_ptr< Camera> > cameras;
	auto all = this->gameObjects->getList();

	BOOST_FOREACH(auto go, all) {
		if(strcmp(go->metaObject()->className(), "Camera") == 0) {
			cameras.push_back(go->getSharedPointer<Camera>());
		}
	}

	auto cameraSorter = [](shared_ptr<Camera> c1, shared_ptr<Camera> c2) -> bool {
		return c1->getDepth() < c2->getDepth();
	};

	cameras.sort(cameraSorter);

	return cameras;
}

void Scene::enableLights() {
	//TODO: refactor it to separate light object
	glEnable(GL_LIGHTING);

	float white[] = {1.0f, 1.0f, 1.0f, 1.0f};
	float black[] = {0.0f, 0.0f, 0.0f, 1.0f};
	float position[] = {0.0f, 30.0f, -100.0f, 1.0f};

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, black);

	float shininess[] = { 64.0f };

	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, black);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);

	glEnable(GL_LIGHT0);
}

void Scene::drawGameObjects(std::shared_ptr<FrameEventArgs> args) {
	auto objects = this->gameObjects->getList();
	for(auto i = objects.begin(); i != objects.end(); ++i) {
		auto o = *i;
		auto transform = o->getComponents()->getSingleByClass<Transform>();
		auto pos = transform->getPosition();
		glPushMatrix();
		glTranslatef(pos.x, pos.y, pos.z);
		o->draw(args);
		glPopMatrix();
	}
}

void Scene::add(std::shared_ptr<BaseGameObject> object) {
	auto objects = this->getGameObjects();
	objects->add(object);
	this->notStartedObjects.push_back(object);
}