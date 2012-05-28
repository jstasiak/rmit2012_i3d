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
	surface(0),
	currentDrawFps(0)
{
	setDesiredFixedUpdateFps(77);
	setDesiredDrawFps(60);
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

	auto init = IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	assert(init);


	this->surface = surface;

	float last = millisecondsNow() / 1000.0f;
	float lastUpdate = last;
	float lastDraw = last;
	float fixedUpdates = 0.0f;

	float drawSecondsAccumulator = 0.0f;
	float drawFramesAccumulator = 0;

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

		this->scene->startUnstartedGameObjects();

		float now = millisecondsNow() / 1000.0f;
		auto updateDt = now - lastUpdate;
		lastUpdate = now;

		this->scene->updateGameObjects(std::shared_ptr<FrameEventArgs>(
				FrameEventArgs::createFromSecondsAndTotalSeconds(updateDt, now)
			)
		);

		this->scene->deleteDestroyedGameObjects();

		fixedUpdates += updateDt / this->updateEverySeconds;

		while(fixedUpdates >= 1) {
			fixedUpdates -= 1;
			auto args = std::shared_ptr<FrameEventArgs>(
				FrameEventArgs::createFromSecondsAndTotalSeconds(this->updateEverySeconds, now));
			this->scene->fixedUpdateGameObjects(args);

			this->scene->deleteDestroyedGameObjects();
		}


		now = millisecondsNow() / 1000.0f;
		float drawDt = now - lastDraw;

		if(drawDt >= this->drawEverySeconds) {
			auto args = std::shared_ptr<FrameEventArgs>(
				FrameEventArgs::createFromSecondsAndTotalSeconds(drawDt, now));
			this->scene->draw(args);
			lastDraw = now;

			drawSecondsAccumulator += drawDt;
			drawFramesAccumulator++;

			const float avgPeriod = 0.2f;
			if(drawSecondsAccumulator >= avgPeriod) {
				this->currentDrawFps = drawFramesAccumulator * 1.0f / drawSecondsAccumulator;
				drawSecondsAccumulator = 0.0f;
				drawFramesAccumulator = 0;
			}
		}
	}
	this->surface = 0;
	return 0;
}

void Application::setDesiredFixedUpdateFps( int value )
{
	assert(value > 0);
	this->desiredFixedUpdateFps = value;
	this->updateEverySeconds = 1.0f / this->desiredFixedUpdateFps;
}

void Application::setDesiredDrawFps( int value )
{
	assert(value > 0);
	this->desiredDrawFps = value;
	this->drawEverySeconds = 1.0f / this->desiredDrawFps;
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

	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

void Application::executeConfigFile(string configFile) {
	auto config = "\n\
echo config file begin\n\
\n\
bind ESCAPE quit\n\
bind Q quit\n\
\n\
bind A toggle_axes\n\
bind PLUS increase_water_tesselation\n\
bind MINUS decrease_water_tesselation\n\
bind N toggle_water_normals\n\
bind W toggle_wireframe\n\
\n\
\n\
bind UP +accel 1\n\
bind DOWN +deccel 1\n\
bind LEFT +left 1\n\
bind RIGHT +right 1\n\
bind DELETE fire 1 left\n\
bind PAGE_DOWN fire 1 right\n\
\n\
bind W +accel 2\n\
bind S +deccel 2\n\
bind A +left 2\n\
bind D +right 2\n\
bind Q fire 2 left\n\
bind E fire 2 right\n\
\n\
bind ENTER reset\n\
\n\
# TODO: implement scene command\n\
#scene start\n\
\n\
echo config file end\n\
	";

	std::vector< std::string > lines;
	boost::split(lines, config, boost::is_any_of("\n"));

	BOOST_FOREACH(auto line, lines) {
		this->commandSystem->safeExecuteCommandLine(line);
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
	else if(our == string("return")) {
		our = "enter";
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

float Application::getCurrentDrawFps() const {
	return this->currentDrawFps;
}