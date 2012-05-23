#include "precompile.h"
#include "application.h"

#include "command.h"
#include "frameeventargs.h"
#include "utils.h"

#include "gameobject/gameobjectset.h"
#include "gameobject/basegameobject.h"
#include "gameobject/camera.h"
#include "gameobject/transform.h"
#include "scene.h"

namespace po = boost::program_options;
using namespace std;

Application::Application()
	: commandSystem(new CommandSystem()),
	bindings(),
	scene(),
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

		this->scene->startUnstartedGameObjects();

		while(updateDt >= this->updateEverySeconds) {
			//FIXME: HACK: develop smarter system of update managing
			updateDt -= this->updateEverySeconds;
			auto args = std::shared_ptr<FrameEventArgs>(
				FrameEventArgs::createFromSecondsAndTotalSeconds(this->updateEverySeconds, now));
			this->scene->updateGameObjects(args);
		}

		this->scene->deleteDestroyedGameObjects();

		if(drawDt >= this->drawEverySeconds) {
			auto args = std::shared_ptr<FrameEventArgs>(
				FrameEventArgs::createFromSecondsAndTotalSeconds(drawDt, now));
			this->scene->draw(args);
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
	this->scene = make_shared<Scene>();
	this->scene->setApplication(this->getSharedPointer());
	this->scene->initialize();

	this->initializeCommands();

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

string sdlKeyToOurKey(SDLKey key) {
	string our(SDL_GetKeyName(key));
	if(our == string("-")) {
		our = "minus";
	}
	else if(our == string("=")) {
		our = "plus";
	}
	std::replace(our.begin(), our.end(), ' ', '_');

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