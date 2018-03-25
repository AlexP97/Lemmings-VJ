#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Boton.h"
#include "Game.h"


enum BotonAnims
{
	RUNNING, PAUSED
};


void Boton::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/botones3.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1.0f / 2.0f, 2.0f / 2.0f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(RUNNING, 0);
	sprite->addKeyframe(RUNNING, glm::vec2(0.0f / 1.0f, 0.0f / 1.0f));

	sprite->setAnimationSpeed(PAUSED, 0);
	sprite->addKeyframe(PAUSED, glm::vec2(0.5f / 1.0f, 0.0f / 1.0f));

	sprite->changeAnimation(RUNNING);
	sprite->setPosition(initialPosition);

}

void Boton::update(int deltaTime)
{
	int fall;

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