#include "precompile.h"
#include "scene.h"

#include "application.h"

#include "utils.h"
#include "gameobject/basegameobject.h"
#include "gameobject/gameobjectset.h"
#include "gameobject/camera.h"
#include "gameobject/transform.h"
#include "gameobject/rigidbody.h"
#include "skybox.h"

#include "../app/terrain.h"

using namespace std;

Scene::Scene()
	: gameObjects(),
	notStartedObjects(),
	application(),
	activeCamera(),
	skybox()
{
}

void Scene::initialize() {
	auto r = Registry::getSharedInstance();

	auto app = this->getApplication();
	auto skyboxFile = app->getDataDirectory() + std::string("/textures/skybox/miramar_large.jpg");
	this->skybox = std::shared_ptr< Skybox >(new Skybox(skyboxFile));

	this->add(std::make_shared<Terrain>());
	this->add(r->create<BaseGameObject>("Water"));
	
		
	auto bo = r->create<BaseGameObject>();
	bo->getComponents()->add(r->create<BaseComponent>("Manager"));
	this->add(bo);
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

void Scene::fixedUpdateGameObjects(std::shared_ptr<FrameEventArgs> args) {
	auto objects = this->gameObjects->getList();
	for(auto i = objects.begin(); i != objects.end(); ++i) {
		auto o = *i;
		o->fixedUpdate(args);
	}

	this->checkForCollisions();
}

void Scene::updateGameObjects(std::shared_ptr<FrameEventArgs> args) {
	auto objects = this->gameObjects->getList();
	for(auto i = objects.begin(); i != objects.end(); ++i) {
		auto o = *i;
		if(o->isActive()) {
			o->update(args);
		}
	}
}

void Scene::checkForCollisions() {
	auto objects = this->gameObjects->getList();
	std::vector< std::shared_ptr< RigidBody > > bodies;

	for(auto i = objects.begin(); i != objects.end(); ++i) {
		auto o = *i;
		if(o->isActive() && !o->isDestroying()) {
			try {
				auto body = o->getComponents()->getSingleByClass< RigidBody >();
				bodies.push_back(body);
			}
			catch(const DoesNotExist& e) {}
		}
	}

	for(int i = 0; i < bodies.size(); ++i) {
		auto bodyA = bodies.at(i);
		for(int j = i + 1; j < bodies.size(); ++j) {
			auto bodyB = bodies.at(j);
			if(bodyA->collidesWith(bodyB)) {
				auto gameObjectA = bodyA->getGameObject();
				auto gameObjectB = bodyB->getGameObject();
				
				printf("collision %s vs %s\n", gameObjectA->metaObject()->className(),
					gameObjectB->metaObject()->className());


				gameObjectA->onCollide(gameObjectB);
				gameObjectB->onCollide(gameObjectA);
			}
		}
	}
}

void Scene::draw(std::shared_ptr<FrameEventArgs> args) {
	auto cameras = this->getSortedCameras();
	BOOST_FOREACH(auto camera, cameras) {
		this->setActiveCamera(camera);
		camera->applyCamera();

		this->drawSkyboxForCamera(camera);

		// Enable lights in camera space only so light position is ok
		this->enableLights();

		this->drawGameObjects(args);
		this->setActiveCamera(std::shared_ptr< Camera >());
	}

	this->drawGui();

	SDL_GL_SwapBuffers();
}

std::list < std::shared_ptr< Camera > > Scene::getSortedCameras() const {
	list < shared_ptr< Camera> > cameras;
	auto all = this->gameObjects->getList();

	BOOST_FOREACH(auto go, all) {
		if(strcmp(go->metaObject()->className(), "Camera") == 0
			&& go->isActive()) {
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
	float ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

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

		if(o->isActive()) {
			glPushMatrix();

			auto transform = o->getComponents()->getSingleByClass<Transform>();
			auto matrix = transform->getMatrix();
			auto matrixData = glm::value_ptr(matrix);
			glMultMatrixf(matrixData);

			o->draw(args);
		
			glPopMatrix();
		}
	}
}

void Scene::drawGui() {
	// Set right matrices so OpenGL coordinates will map directly
	// to screen coordinates


	auto ss = this->getApplication()->getScreenSize();
	int w = ss.x;
	int h = ss.y;
	glViewport(0, 0, w, h);
	glScissor(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, 0, h, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushAttrib(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);


	auto objects = this->gameObjects->getList();
	for(auto i = objects.begin(); i != objects.end(); ++i) {
		auto o = *i;

		if(o->isActive()) {
			try {
				o->onGui();
			}
			catch(const std::exception& e) {
				printf("exception: %s\n", e.what());
			}
		}
	}

	glPopAttrib();
}

void Scene::drawSkyboxForCamera(std::shared_ptr< Camera > camera) {
	if(this->skybox) {
		this->skybox->drawForCamera(camera);
	}
}

void Scene::add(std::shared_ptr<BaseGameObject> object) {
	auto objects = this->getGameObjects();
	objects->add(object);
	this->notStartedObjects.push_back(object);
}

std::shared_ptr< Camera > Scene::getActiveCamera() {
	return this->activeCamera;
}

void Scene::setActiveCamera(std::shared_ptr< Camera > value) {
	this->activeCamera = value;
}
