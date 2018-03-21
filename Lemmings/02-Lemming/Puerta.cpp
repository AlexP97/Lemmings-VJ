#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Puerta.h"
#include "Game.h"


enum PuertaAnims
{
	OPENING, OPENED
};


void Puerta::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram)
{
	state = OPENING;
	spritesheet.loadFromFile("images/miscelanea.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(2.0f/20.0f, 2.0f/32.0f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(OPENING, 3);
	for (int i = 0; i<10; i++)
		sprite->addKeyframe(OPENING, glm::vec2(12.0f/16.0f, float(i) / 18.));
	sprite->setAnimationSpeed(OPENED, 3);
	sprite->addKeyframe(OPENED, glm::vec2(12.0f / 16.0f, 9. / 18.));
	sprite->changeAnimation(OPENING);
	sprite->setPosition(initialPosition);

}

void Puerta::update(int deltaTime)
{
	int fall;

	if (sprite->update(deltaTime) == 0)
		return;

	switch (state)
	{
	case OPENING:
		sprite->changeAnimation(OPENED);
		break;
	
	}
}

void Puerta::render()
{
	sprite->render();
}