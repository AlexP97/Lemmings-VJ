#include <cmath>
#include <algorithm>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Lemming.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum LemmingAnims
{
	WALKING_LEFT, WALKING_RIGHT, COMING_OUT, FALLING_LEFT, FALLING_RIGHT, DIG, BLOCKING, DIG_LEFT, DIG_RIGHT, EXPLODING
};


void Lemming::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram)
{
	eliminado = false;
	come_Out = false;
	state = FALLING_RIGHT_STATE;
	spritesheet.loadFromFile("images/lemming4_sinfondo.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.125, 1 / 16.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(10);
	
		sprite->setAnimationSpeed(WALKING_RIGHT, 12);
		for(int i=0; i<8; i++)
			sprite->addKeyframe(WALKING_RIGHT, glm::vec2(float(i) / 8, 0.f / 16.f));
		
		sprite->setAnimationSpeed(WALKING_LEFT, 12);
		for(int i=0; i<8; i++)
			sprite->addKeyframe(WALKING_LEFT, glm::vec2(float(i) / 8, 1.f / 16.f));

		sprite->setAnimationSpeed(COMING_OUT, 9);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(COMING_OUT, glm::vec2(float(i) / 8, 2.f / 16.f));

		sprite->setAnimationSpeed(FALLING_LEFT, 12);
		for (int i = 0; i<4; i++)
			sprite->addKeyframe(FALLING_LEFT, glm::vec2(float(i)+4 / 8, 3.f / 16.f));

		sprite->setAnimationSpeed(FALLING_RIGHT, 12);
		for (int i = 0; i<4; i++)
			sprite->addKeyframe(FALLING_RIGHT, glm::vec2(float(i) / 8, 3.f / 16.f));

		sprite->setAnimationSpeed(DIG, 9);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(DIG, glm::vec2(float(i) / 8, 6.f / 16.f));

		sprite->setAnimationSpeed(BLOCKING, 9);
		for (int j = 0; j < 2; j++) {
			for (int i = 0; i < 8; i++) {
				sprite->addKeyframe(BLOCKING, glm::vec2(float(i) / 8, (4.f+float(j)) / 16.f));
			}
		}

		sprite->setAnimationSpeed(EXPLODING, 3);
		for (int i = 0; i < 8; i++) {
				sprite->addKeyframe(EXPLODING, glm::vec2(float(i) / 8, 15.f / 16.f));
		}
		
		
	sprite->changeAnimation(FALLING_RIGHT);
	sprite->setPosition(initialPosition);
}

void Lemming::update(int deltaTime)
{
	int fall;

	if(sprite->update(deltaTime) == 0)
		return;

	switch (state) {

	case FALLING_LEFT_STATE:
		fall = collisionFloor(2);
		if (fall > 0)
			sprite->position() += glm::vec2(0, fall);
		else {
			sprite->changeAnimation(WALKING_LEFT);
			state = WALKING_LEFT_STATE;
		}
		break;

	case FALLING_RIGHT_STATE:
		fall = collisionFloor(2);
		if (fall > 0)
			sprite->position() += glm::vec2(0, fall);
		else {
			sprite->changeAnimation(WALKING_RIGHT);
			state = WALKING_RIGHT_STATE;
		}
		break;

	case WALKING_LEFT_STATE:
		sprite->position() += glm::vec2(-1, -1);
		if (collision())
		{
			sprite->position() -= glm::vec2(-1, -1);
			sprite->changeAnimation(WALKING_RIGHT);
			state = WALKING_RIGHT_STATE;
		}
		else
		{
			fall = collisionFloor(3);
			if (fall > 0)
				sprite->position() += glm::vec2(0, 1);
			if (fall > 1)
				sprite->position() += glm::vec2(0, 1);
			if (fall > 2) {
				sprite->changeAnimation(FALLING_LEFT);
				state = FALLING_LEFT_STATE;
			}
		}
		break;

	case WALKING_RIGHT_STATE:
		sprite->position() += glm::vec2(1, -1);
		if (collision())
		{
			sprite->position() -= glm::vec2(1, -1);
			sprite->changeAnimation(WALKING_LEFT);
			state = WALKING_LEFT_STATE;
		}
		else
		{
			fall = collisionFloor(3);
			if (fall < 3)
				sprite->position() += glm::vec2(0, fall);
			else {
				sprite->changeAnimation(FALLING_RIGHT);
				state = FALLING_RIGHT_STATE;
			}
		}
		break;

	case COMING_OUT_STATE:
		cout << sprite->currentKeyFrame();
		if (sprite->currentKeyFrame() == 7)
			eliminado = true;
		break;

	case DIG_STATE:
		fall = collisionFloor(3);
		if (fall > 0)
			sprite->position() += glm::vec2(0, 1);
		if (fall > 1)
			sprite->position() += glm::vec2(0, 1);
		if (fall > 2) {
			sprite->changeAnimation(FALLING_RIGHT);
			state = FALLING_RIGHT_STATE;
		}
		if (state != FALLING_RIGHT_STATE) {
			eraseMask(position()[0] + 8 + 120, position()[1] + 16, 0, 0.25,-3,3);
			sprite->position() += glm::vec2(0, 0.25);
		}
		break;

	case BLOCKING_STATE:
		break;

	case DIG_LEFT_STATE:
		if(collision()) {
			eraseMask(position()[0] + 120, position()[1] + 8, -3, 3, 0, -0.25);
			sprite->position() += glm::vec2(-0.25, 0);
		}
		else {
			sprite->changeAnimation(WALKING_LEFT);
			state = WALKING_LEFT_STATE;
		}
		break;

	case DIG_RIGHT_STATE:
		
		if (mask->pixel(position()[0]+16+1,position()[1]+8) == 255) {
			eraseMask(position()[0] + 16 + 120, position()[1] + 8, -8, 8, 0, 0.25);
			sprite->position() += glm::vec2(0.25, 0);
		}
		else {
			sprite->changeAnimation(WALKING_RIGHT);
			state = WALKING_RIGHT_STATE;
		}
		break;
	case EXPLODE_STATE:
		if (sprite->currentKeyFrame() == 7)
			eliminado = true;
		break;
	}
}

void Lemming::render()
{
	sprite->render();
}

glm::vec2 Lemming::position() {
	return sprite->position();
}

void Lemming::setMapMask(VariableTexture *mapMask)
{
	mask = mapMask;
}

bool Lemming::eliminar() 
{
	return eliminado;
}

void Lemming::setComeOut(bool b) {
	come_Out = b;
	if (b) {
		sprite->changeAnimation(COMING_OUT);
		state = COMING_OUT_STATE;
	}
}

bool Lemming::goOut() {
	return come_Out;
}

int Lemming::collisionFloor(int maxFall)
{
	bool bContact = false;
	int fall = 0;
	glm::ivec2 posBase = sprite->position() + glm::vec2(120, 0); // Add the map displacement
	
	posBase += glm::ivec2(7, 16);
	while((fall < maxFall) && !bContact)
	{
		if((mask->pixel(posBase.x, posBase.y+fall) == 0) && (mask->pixel(posBase.x+1, posBase.y+fall) == 0))
			fall += 1;
		else
			bContact = true;
	}

	return fall;
}

bool Lemming::collision()
{
	glm::ivec2 posBase = sprite->position() + glm::vec2(120, 0); // Add the map displacement
	
	posBase += glm::ivec2(7, 15);
	if((mask->pixel(posBase.x, posBase.y) == 0) && (mask->pixel(posBase.x+1, posBase.y) == 0))
		return false;

	return true;
}

void Lemming::setAbility(int ability) {
	if (ability == 4) {
		state = DIG_STATE;
		sprite->changeAnimation(DIG);
	}
	else if (ability == 3) {
		state = BLOCKING_STATE;
		sprite->changeAnimation(BLOCKING);
		applyMask();
	}
	else if (ability == 5) {
		if (state == WALKING_LEFT_STATE) {
			state = DIG_LEFT_STATE;
			sprite->changeAnimation(DIG_LEFT);
		}
		if (state == WALKING_RIGHT_STATE) {
			state = DIG_RIGHT_STATE;
			sprite->changeAnimation(DIG_RIGHT);
		}
	}
	else if (ability == 8) {
		state = EXPLODE_STATE;
		sprite->changeAnimation(EXPLODING);
	}
}

void Lemming::eraseMask(int posX, int posY, float ymin, float ymax, float xmin, float xmax) {
	for (int y = max(0.f, posY+ymin); y <= min((float)mask->height() - 1, posY + ymax); y++) {
		for (int x = max(0.f, posX + xmin); x <= min((float)mask->width() - 1, posX + xmax); x++) {
			mask->setPixel(x, y, 0);
		}
	}
}

void Lemming::applyMask() {
	int posX, posY;
	posX = position()[0] + 120;
	posY = position()[1];

	for (int y = max(0, posY); y <= min(mask->height() - 1, posY + 16); y++) {
		for (int x = max(0, posX); x <= min(mask->width() - 1, posX + 16); x++) {
			mask->setPixel(x, y, 255);
		}
	}
}








