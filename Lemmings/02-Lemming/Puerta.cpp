#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Puerta.h"
#include "Game.h"


enum PuertaAnims
{
	OPENING, OPENED, CLOSED
};


void Puerta::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, int escena)
{
	state = CLOSED_STATE;
	if (escena == 0) spritesheet.loadFromFile("images/puertas2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	else if (escena == 1) spritesheet.loadFromFile("images/puertas2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	else if (escena == 2) spritesheet.loadFromFile("images/puertas6_sinfondo.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(33, 20), glm::vec2(1.f, 1.f/10.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(3);

	sprite->setAnimationSpeed(OPENING, 15);
	for (int i = 0; i<10; i++)
		sprite->addKeyframe(OPENING, glm::vec2(0.f, float(i) / 10.f));

	sprite->setAnimationSpeed(OPENED, 0);
	sprite->addKeyframe(OPENED, glm::vec2(0.0f , 9.f / 10.f));

	sprite->setAnimationSpeed(CLOSED, 0);
	sprite->addKeyframe(CLOSED, glm::vec2(0.0f, 0.f / 10.f));

	sprite->changeAnimation(CLOSED);
	sprite->setPosition(initialPosition);

}

void Puerta::update(int deltaTime)
{
	if (sprite->update(deltaTime) == 0)
		return;

	switch (state)
	{
	case OPENING_STATE:
		if(sprite->currentKeyFrame() == sprite->sizeCurrentAnimation()-1)
			sprite->changeAnimation(OPENED);
		break;
	
	}
}

void Puerta::open() {
	sprite->changeAnimation(OPENING);
	state = OPENING_STATE;
}

void Puerta::render()
{
	sprite->render();
}