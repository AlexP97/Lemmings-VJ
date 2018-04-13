#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include "Scene.h"
#include "Scene2.h"
#include "Scene3.h"
#include "Home.h"
#include "SimpleScene.h"


// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

public:
	Game() {}
	
	
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();
	
	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);
	
	bool getKey(int key) const;
	bool getSpecialKey(int key) const;

private:
	bool bPlay;                       // Continue to play game?
	Home home;						  // Home scene
	Scene scene;                      // Scene to render
	Scene2 scene2;
	Scene3 scene3;
	SimpleScene simpleScene;
	bool keys[256], specialKeys[256]; // Store key states so that 
	                                  // we can have access at any time
	int mouseX, mouseY;               // Mouse position
	bool bLeftMouse, bRightMouse;     // Mouse button states
	bool paused, doubleSpeed;
	int actualScene;
};


#endif // _GAME_INCLUDE


