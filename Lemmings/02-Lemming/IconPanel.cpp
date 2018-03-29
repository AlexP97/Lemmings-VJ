#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "IconPanel.h"
#include "Game.h"

enum IconPanelAnims {
	NORMAL
};

void IconPanel::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/icon_panel_sinfondo.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(530.f / 3.f, 135.f / 3.f), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(NORMAL, 0);

	for (int i = 0; i<1; i++)
		sprite->addKeyframe(NORMAL, glm::vec2(0.f, 0.f));

	sprite->changeAnimation(NORMAL);

	sprite->setPosition(initialPosition);

}

void IconPanel::update(int deltaTime)
{
	if (sprite->update(deltaTime) == 0)
		return;
}

void IconPanel::render()
{
	sprite->render();
}

glm::vec2 IconPanel::position() {
	return sprite->position();
}