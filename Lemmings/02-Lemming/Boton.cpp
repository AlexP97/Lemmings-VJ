#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Boton.h"
#include "Game.h"


enum BotonAnims
{
	OPENING
};


void Boton::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram)
{
	state = OPENING;
	spritesheet.loadFromFile("images/botonesSinFondo.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1.0f / 2.0f, 2.0f / 2.0f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(OPENING, 3);
	sprite->addKeyframe(OPENING, glm::vec2(0.5f / 1.0f, 0.0f / 1.0f));
	sprite->changeAnimation(OPENING);
	sprite->setPosition(initialPosition);

}

void Boton::update(int deltaTime)
{
	int fall;

	if (sprite->update(deltaTime) == 0)
		return;

	switch (state)
	{
	case OPENING:
		break;

	}
}

glm::vec2 Boton::position() {
	return sprite->position();
}

void Boton::render()
{
	sprite->render();
}