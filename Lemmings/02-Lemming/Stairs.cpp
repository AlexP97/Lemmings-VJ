#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Stairs.h"
#include "Game.h"


enum StairsAnims
{
	NORMAL
};


void Stairs::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/stairs.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(4, 1), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(NORMAL, 0);
	sprite->addKeyframe(NORMAL, glm::vec2(0.0f, 0.f));

	sprite->changeAnimation(NORMAL);
	sprite->setPosition(initialPosition);

}

void Stairs::update(int deltaTime)
{
	if (sprite->update(deltaTime) == 0)
		return;
}

void Stairs::render()
{
	sprite->render();
}

void Stairs::displace(float d)
{
	glm::vec2 pos = sprite->position();
	pos.x += d;
	sprite->setPosition(pos);
}
