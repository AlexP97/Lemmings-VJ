#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Explosion.h"
#include "Game.h"


enum ExplosionAnims
{
	NORMAL
};


void Explosion::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/explosion_sinfondo.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);

	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1.0f, 1.0f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(NORMAL, 0);
	sprite->addKeyframe(NORMAL, glm::vec2(0.0f, 0.0f));

	sprite->changeAnimation(NORMAL);

	sprite->setPosition(initialPosition);
}

void Explosion::update(int deltaTime)
{
	if (sprite->update(deltaTime) == 0)
		return;
}

void Explosion::render()
{
	sprite->render();
}