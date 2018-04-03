#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include <Windows.h>
#include <mmsystem.h>

void Game::init()
{
	bPlay = true;
	paused = false;
	doubleSpeed = false;
	bLeftMouse = bRightMouse = false;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	mciSendString(TEXT("open sound/BACKGROUND-2.mp3 alias background"), NULL, 0, NULL);
	mciSendString(TEXT("play background repeat"), NULL, 0, NULL);
	scene.init();
}

bool Game::update(int deltaTime)
{
	if (!paused) {
		if(!doubleSpeed) scene.update(deltaTime);
		else scene.update(deltaTime*2);
	}
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene.render();
}

void Game::keyPressed(int key)
{
	if(key == 27) // Escape code
		bPlay = false;
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
	mouseX = x;
	mouseY = y;
	scene.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse, paused);
}

void Game::mousePress(int button)
{
	if(button == GLUT_LEFT_BUTTON)
	{
		bLeftMouse = true;
		pair<bool, bool> speedOrPause = scene.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse, paused);
		if (speedOrPause.second) {
			if (paused) mciSendString(TEXT("play background repeat"), NULL, 0, NULL);
			else mciSendString(TEXT("stop background"), NULL, 0, NULL);
			paused = !paused;
		}
		if (speedOrPause.first) {
			if (doubleSpeed) mciSendString(TEXT("set background speed 1000"), NULL, 0, NULL);
			else mciSendString(TEXT("set background speed 1500"), NULL, 0, NULL);
			doubleSpeed = !doubleSpeed;
		}
	}
	else if(button == GLUT_RIGHT_BUTTON)
	{
		bRightMouse = true;
		scene.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse, paused);
	}
}

void Game::mouseRelease(int button)
{
	if(button == GLUT_LEFT_BUTTON)
		bLeftMouse = false;
	else if(button == GLUT_RIGHT_BUTTON)
		bRightMouse = false;
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}





