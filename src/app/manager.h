#ifndef MYAPP_H
#define MYAPP_H

#include "../engine/gameobject/basecomponent.h"

class Water;
class Ship;
class Font;

class Manager : public BaseComponent {
public: virtual const char* className() const {
		return "Manager";
	}

private: std::shared_ptr< Font> font;

public: enum DrawAxes {
		WorldOrigin,
		WaterOrigin,
		ShipOrigin,
		ShipOriginWithRotation,
	};

public: enum GameState {
		STATE_RUNNING,
		STATE_PLAYER1_WON,
		STATE_PLAYER2_WON,
		STATE_DRAW
	};

private: bool wireframe;
private: DrawAxes axes;

public: Manager();

public: virtual void start();

private: void createShips();

private: void resetGame();
private: void clearShips();

public: GameState getState();

public: virtual void update(std::shared_ptr<FrameEventArgs> args);
public: virtual void onGui();

private: std::list< std::shared_ptr< Ship > > getShips();

public: void toggleWireframe();
public: void toggleAxes();
};

#endif