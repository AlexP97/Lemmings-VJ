#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Lava.h"
#include "Game.h"


enum LavaAnims
{
	NORMAL
};


void Lava::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/lava.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);

	sprite = Sprite::createSprite(glm::ivec2(134, 14), glm::vec2(1.0f, 1.0f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(NORMAL, 0);
	sprite->addKeyframe(NORMAL, glm::vec2(0.0f / 1.0f, 0.0f / 1.0f));

	sprite->changeAnimation(NORMAL);

	sprite->setPosition(initialPosition);
}

void Lava::update(int deltaTime)
{
	if (sprite->update(deltaTime) == 0)
		return;
}

glm::vec2 Lava::position() {
	return sprite->position();
}

void Lava::displace(float d)
{
	glm::vec2 pos = sprite->position();
	pos.x += d;
	sprite->setPosition(pos);
}

void Lava::render()
{
	sprite->render();
}