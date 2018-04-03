#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Boton.h"
#include "Game.h"


enum BotonAnims
{
	NORMAL, CLICKED
};


void Boton::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, int typeOfButton)
{
	if (typeOfButton == 0) spritesheet.loadFromFile("images/botones.png", TEXTURE_PIXEL_FORMAT_RGBA);
	else if (typeOfButton == 1) spritesheet.loadFromFile("images/1x-2x_sinfondo.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);

		sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1.0f / 2.0f, 2.0f / 2.0f), &spritesheet, &shaderProgram);
		sprite->setNumberAnimations(2);

		sprite->setAnimationSpeed(NORMAL, 0);
		sprite->addKeyframe(NORMAL, glm::vec2(0.0f / 1.0f, 0.0f / 1.0f));

		sprite->setAnimationSpeed(CLICKED, 0);
		sprite->addKeyframe(CLICKED, glm::vec2(0.5f / 1.0f, 0.0f / 1.0f));

		sprite->changeAnimation(NORMAL);

	sprite->setPosition(initialPosition);
}

void Boton::update(int deltaTime)
{
	if (sprite->update(deltaTime) == 0)
		return;
}

glm::vec2 Boton::centerPosition() {
	glm::vec2 spritePos = sprite->position();
	return glm::vec2(spritePos.x + 5.5f, spritePos.y + 6.5f);
}

void Boton::render()
{
	sprite->render();
}

void Boton::changeAnimation() {
	int currentAnim = sprite->animation();
	if(currentAnim == 0) sprite->changeAnimation(1);
	else sprite->changeAnimation(0);
}