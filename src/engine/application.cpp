#include "precompile.h"
#include "application.h"

#include "command.h"
#include "frameeventargs.h"
#include "utils.h"

#include "gameobject/gameobjectset.h"
#include "gameobject/basegameobject.h"
#include "gameobject/camera.h"

namespace po = boost::program_options;
using namespace std;

Application::Application()
	: commandSystem(new CommandSystem()),
	bindings(),
	gameObjectSet(),
	gameDir("data"),
	surface(0)
{
	setUpdateFps(77);
	setDrawFps(60);
}

Application::~Application() {
	SDL_Quit();
}

int Application::runWithCommandLineParameters(int argc, char** argv) {
	// Declare the supported options.
	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "shows help information")
		("gamedir", po::value<string>()->default_value(this->gameDir), "sets game data directory")
		;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm); 

	int returnCode = 0;

	if(vm.count("help")) {
		cout << desc << "\n\n";
	}
	else {
		this->gameDir = vm["gamedir"].as<string>();
		returnCode = this->run();
	}

	return returnCode;
}

int Application::run() {
	int result;

	result = SDL_Init(SDL_INIT_EVERYTHING);
	assert(result == 0);

	auto surface = SDL_SetVideoMode(1200, 600, 32, SDL_DOUBLEBUF | SDL_OPENGL | SDL_HWSURFACE);
	assert(surface);

	this->surface = surface;

	double last = millisecondsNow() / 1000.0f;
	float updateDt = 0.0f;
	float drawDt = 0.0f;
	double now = 0.0;
	double dt = 0.0;

	bool running = true;

	SDL_Event event;

	this->initialize();

	while(running) {
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYDOWN:
				this->onKeyDown(&event.key);
				break;
			case SDL_KEYUP:
				this->onKeyUp(&event.key);
				break;
			}
		}

		now = millisecondsNow() / 1000.0f;
		dt = now - last;
		last = now;
		updateDt += dt;
		drawDt += dt;

		while(updateDt >= this->updateEverySeconds) {
			//FIXME: HACK: develop smarter system of update managing
			updateDt -= this->updateEverySeconds;
			auto args = std::shared_ptr<FrameEventArgs>(
				FrameEventArgs::createFromSecondsAndTotalSeconds(this->updateEverySeconds, now));
			this->update(args);
		}

		if(drawDt >= this->drawEverySeconds) {
			auto args = std::shared_ptr<FrameEventArgs>(
				FrameEventArgs::createFromSecondsAndTotalSeconds(drawDt, now));
			this->draw(args);
			drawDt = 0.0;
		}
	}
	this->surface = 0;
	return 0;
}

void Application::setUpdateFps( int value )
{
	assert(value > 0);
	this->updateFps = value;
	this->updateEverySeconds = 1.0f / this->updateFps;
}

void Application::setDrawFps( int value )
{
	assert(value > 0);
	this->drawFps = value;
	this->drawEverySeconds = 1.0f / this->drawFps;
}

void Application::initialize() {
	this->gameObjectSet = std::make_shared<GameObjectSet>();
	this->gameObjectSet->setOwner(this->getSharedPointer());

	this->initializeCommands();
	this->initializeScene();
	this->startGameObjects();

	this->executeConfigFile("config.cfg");

	this->initializeGraphics();
}

void Application::initializeCommands() {
	this->commandSystem->registerCommand("quit", [this](command_parameters parameters) {
		this->quit();
	});

	this->commandSystem->registerCommand("echo", [](command_parameters parameters) {
		printf("[echo]");
		BOOST_FOREACH(const string& item, parameters) {
			printf(" %s", item.c_str());
		}
		printf("\n");
	});

	this->commandSystem->registerCommand("bind", [this](command_parameters parameters) {
		if(parameters.size() < 2) {
			printf("Bind error, usage: bind KEY COMMAND ...\n");
		}
		else {
			auto key = parameters.at(0);
			boost::algorithm::to_lower(key, locale());
			parameters.erase(parameters.begin());
			auto commandLine = boost::algorithm::join(parameters, " ");
			this->bindings[key] = commandLine;
		}
	});
}

void Application::initializeGraphics() {
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	float white[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);

	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_SCISSOR_TEST);
}

void Application::initializeScene() {
	//TODO: load scene definition from file on demand
	auto r = Registry::getSharedInstance();

	this->gameObjectSet->add(r->create<BaseGameObject>("Water"));
	this->gameObjectSet->add(r->create<BaseGameObject>("Ship"));
	
	auto bo = r->create<BaseGameObject>();
	bo->initialize();
	bo->getComponents()->add(r->create<BaseComponent>("Manager"));
	this->gameObjectSet->add(bo);


	auto c1 = r->create<Camera>();
	auto c2 = r->create<Camera>();
	c1->setNormalizedRect(Rectf(0, 0, 0.5, 1));
	c2->setNormalizedRect(Rectf(0.5, 0.0, 0.5, 1));
	c2->setDepth(1);

	this->gameObjectSet->add(c1);
	this->gameObjectSet->add(c2);
}

void Application::startGameObjects() {
	auto objects = this->gameObjectSet->getList();
	for(auto i = objects.begin(); i != objects.end(); ++i) {
		auto o = *i;
		o->initialize();
		o->start();
	}
}

void Application::executeConfigFile(string configFile) {
	auto fileName = QString::fromUtf8(this->getDataDirectory().c_str());
	fileName += "/";
	fileName += configFile.c_str();

	QFile file(fileName);
	if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QTextStream stream((QIODevice*) &file);
		QString line = stream.readLine();
		while(!line.isNull()) {
			this->commandSystem->safeExecuteCommandLine(line.toStdString());
			line = stream.readLine();
		}
	}
	else {
		printf("Error opening config file %s\n", fileName.toStdString().c_str());
	}
}

void Application::update(std::shared_ptr<FrameEventArgs> args) {
	auto objects = this->gameObjectSet->getList();
	for(auto i = objects.begin(); i != objects.end(); ++i) {
		auto o = *i;
		o->update(args);
	}
}

void Application::draw(std::shared_ptr<FrameEventArgs> args) {
	auto cameras = this->getSortedCameras();
	BOOST_FOREACH(auto camera, cameras) {
		camera->applyCamera();

		// Enable lights in camera space only so light position is ok
		this->enableLights();

		this->drawGameObjects(args);
	}

	SDL_GL_SwapBuffers();
}

list < shared_ptr< Camera > > Application::getSortedCameras() const {
	list < shared_ptr< Camera> > cameras;
	auto all = this->gameObjectSet->getList();

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

void Application::drawGameObjects(std::shared_ptr<FrameEventArgs> args) {
	auto objects = this->gameObjectSet->getList();
	for(auto i = objects.begin(); i != objects.end(); ++i) {
		auto o = *i;
		o->draw(args);
	}
}

string sdlKeyToOurKey(SDLKey key) {
	string our(SDL_GetKeyName(key));
	if(our == string("-")) {
		our = "minus";
	}
	else if(our == string("=")) {
		our = "plus";
	}

	return our;
}


void Application::onKeyDown(const SDL_KeyboardEvent* event) {
	auto key = sdlKeyToOurKey(event->keysym.sym);
	auto iterator = this->bindings.find(key);
	if(iterator != this->bindings.end()) {
		auto commandLine = (*iterator).second;
		this->commandSystem->safeExecuteCommandLine(commandLine);
	}
}

void Application::onKeyUp(const SDL_KeyboardEvent* event) {
	auto key = sdlKeyToOurKey(event->keysym.sym);
	auto iterator = this->bindings.find(key);

	if(iterator != this->bindings.end()) {
		auto commandLine = (*iterator).second;
		if(commandLine.at(0) == '+') {
			commandLine.at(0) = '-';
			this->commandSystem->safeExecuteCommandLine(commandLine);
		}
	}
}


/**
 * Send SDL_QUIT event to message loop
 */
void Application::quit() {
	SDL_Event event;
	event.type = SDL_QUIT;
	SDL_PushEvent(&event);
}

std::shared_ptr<CommandSystem> Application::getCommandSystem() {
	return this->commandSystem;
}

std::shared_ptr<Application> Application::getSharedPointer() {
	return this->shared_from_this();
}

std::string Application::getDataDirectory() const {
	return this->gameDir;
}

glm::ivec2 Application::getScreenSize() const {
	return glm::ivec2(this->surface->w, this->surface->h);
}

void Application::enableLights() {
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