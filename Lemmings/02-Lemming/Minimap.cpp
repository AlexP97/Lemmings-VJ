#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Minimap.h"
#include "Game.h"


enum MinimapAnims
{
	NORMAL
};


void Minimap::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, int scene)
{
	if (scene == 0) spritesheet.loadFromFile("images/fun1_mask2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	else if (scene == 1) spritesheet.loadFromFile("images/tricky9_mask2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	else if (scene == 2) spritesheet.loadFromFile("images/taxing5_mask2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);

	if (scene == 0) sprite = Sprite::createSprite(glm::ivec2(96.38f, 28.f), glm::vec2(0.99f, 0.625f), &spritesheet, &shaderProgram);
	else if (scene == 1) sprite = Sprite::createSprite(glm::ivec2(120.f, 22.5f), glm::vec2(0.78f, 0.625f), &spritesheet, &shaderProgram);
	else if (scene == 2) sprite = Sprite::createSprite(glm::ivec2(55.125f, 22.5f), glm::vec2(1.0f, 0.625f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(NORMAL, 0);
	if (scene == 0) sprite->addKeyframe(NORMAL, glm::vec2(0.0f / 1.0f, 0.0f / 1.0f));
	else if (scene == 1) sprite->addKeyframe(NORMAL, glm::vec2(0.13f / 1.0f, 0.f / 1.0f));
	else if (scene == 2) sprite->addKeyframe(NORMAL, glm::vec2(0.16f / 1.0f, 0.f / 1.0f));
	sprite->changeAnimation(NORMAL);

	sprite->setPosition(initialPosition);
}

void Minimap::update(int deltaTime)
{
	if (sprite->update(deltaTime) == 0)
		return;
}

void Minimap::render()
{
	sprite->render();
}