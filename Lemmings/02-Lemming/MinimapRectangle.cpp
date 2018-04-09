#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "MinimapRectangle.h"
#include "Game.h"

enum MinimapRectangleAnims
{
	NORMAL
};


void MinimapRectangle::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, int scene, int typeRectangle)
{
	spritesheet.loadFromFile("images/rectangle_minimap.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	if (scene == 0) {
		if (typeRectangle == 0) sprite = Sprite::createSprite(glm::ivec2(96.38f, 28.f), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
		else sprite = Sprite::createSprite(glm::ivec2(30.f, 28.f), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	}
	else if (scene == 1) {
		if (typeRectangle == 0) sprite = Sprite::createSprite(glm::ivec2(122.5f, 24.f), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
		else sprite = Sprite::createSprite(glm::ivec2(51.f, 24.f), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	}
	else if (scene == 2) {
		if (typeRectangle == 0) sprite = Sprite::createSprite(glm::ivec2(55.125f, 22.5f), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
		else sprite = Sprite::createSprite(glm::ivec2(30.f, 22.5f), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	}

	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(NORMAL, 0);
	sprite->addKeyframe(NORMAL, glm::vec2(0.0f, 0.0f));

	sprite->changeAnimation(NORMAL);
	sprite->setPosition(initialPosition);
}

void MinimapRectangle::update(int deltaTime)
{
	if (sprite->update(deltaTime) == 0)
		return;
}

void MinimapRectangle::render()
{
	sprite->render();
}

void MinimapRectangle::displace(float d)
{
	glm::vec2 pos = sprite->position();
	pos.x += d;
	sprite->setPosition(pos);
}