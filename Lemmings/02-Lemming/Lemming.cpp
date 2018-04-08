#include <cmath>
#include <algorithm>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Lemming.h"
#include "Game.h"
#include <Windows.h>
#include <mmsystem.h>


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum LemmingAnims
{
	WALKING_LEFT, WALKING_RIGHT, COMING_OUT, FALLING_LEFT, FALLING_RIGHT, DIG, BLOCKING, DIG_LEFT, DIG_RIGHT, EXPLODING, CLIMBING_LEFT,
	CLIMBING_RIGHT, STOPPING_CLIMB_LEFT, STOPPING_CLIMB_RIGHT, BUILDING_LEFT, BUILDING_RIGHT
};


void Lemming::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, int desplazamiento)
{
	eliminado = false;
	climber = false;
	come_Out = false;
	primeraPasada = false;
	numberOfStairs = 0;
	displacement = desplazamiento;
	state = FALLING_RIGHT_STATE;
	spritesheet.loadFromFile("images/lemming2_sinfondo.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.125, 1 / 23.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(16);
	
		sprite->setAnimationSpeed(WALKING_RIGHT, 12);
		for(int i=0; i<8; i++)
			sprite->addKeyframe(WALKING_RIGHT, glm::vec2(float(i) / 8, 0.f / 23.f));
		
		sprite->setAnimationSpeed(WALKING_LEFT, 12);
		for(int i=0; i<8; i++)
			sprite->addKeyframe(WALKING_LEFT, glm::vec2(float(i) / 8, 1.f / 23.f));

		sprite->setAnimationSpeed(COMING_OUT, 9);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(COMING_OUT, glm::vec2(float(i) / 8, 2.f / 23.f));

		sprite->setAnimationSpeed(FALLING_LEFT, 12);
		for (int i = 7; i>3; i--)
			sprite->addKeyframe(FALLING_LEFT, glm::vec2(float(i) / 8, 3.f / 23.f));

		sprite->setAnimationSpeed(FALLING_RIGHT, 12);
		for (int i = 0; i<4; i++)
			sprite->addKeyframe(FALLING_RIGHT, glm::vec2(float(i) / 8, 3.f / 23.f));

		sprite->setAnimationSpeed(DIG, 9);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(DIG, glm::vec2(float(i) / 8, 6.f / 23.f));

		sprite->setAnimationSpeed(DIG_RIGHT, 9);
		for (int j = 0; j < 2; j++) {
			for (int i = 0; i < 8; i++) {
				sprite->addKeyframe(DIG_RIGHT, glm::vec2(float(i) / 8, (7.f + float(j)) / 23.f));
			}
		}

		sprite->setAnimationSpeed(DIG_LEFT, 9);
		for (int j = 0; j < 2; j++) {
			for (int i = 0; i < 8; i++) {
				sprite->addKeyframe(DIG_LEFT, glm::vec2(float(i) / 8, (9.f + float(j)) / 23.f));
			}
		}




		sprite->setAnimationSpeed(BLOCKING, 9);
		for (int j = 0; j < 2; j++) {
			for (int i = 0; i < 8; i++) {
				sprite->addKeyframe(BLOCKING, glm::vec2(float(i) / 8, (4.f+float(j)) / 23.f));
			}
		}

		sprite->setAnimationSpeed(EXPLODING, 12);
		for (int j = 0; j < 4; j++) {
			for (int i = 0; i < 8; i++) {
				sprite->addKeyframe(EXPLODING, glm::vec2(float(i) / 8, (15.f + float(j)) / 23.f));
			}
		}


		sprite->setAnimationSpeed(CLIMBING_LEFT, 9);
		for (int i = 0; i < 8; i++) {
			sprite->addKeyframe(CLIMBING_LEFT, glm::vec2(float(i) / 8, 13.f / 23.f));
		}

		sprite->setAnimationSpeed(CLIMBING_RIGHT, 9);
		for (int i = 0; i < 8; i++) {
			sprite->addKeyframe(CLIMBING_RIGHT, glm::vec2(float(i) / 8, 11.f / 23.f));
		}

		sprite->setAnimationSpeed(STOPPING_CLIMB_LEFT, 12);
		for (int i = 0; i < 8; i++) {
			sprite->addKeyframe(STOPPING_CLIMB_LEFT, glm::vec2(float(i) / 8, 14.f / 23.f));
		}

		sprite->setAnimationSpeed(STOPPING_CLIMB_RIGHT, 12);
		for (int i = 0; i < 8; i++) {
			sprite->addKeyframe(STOPPING_CLIMB_RIGHT, glm::vec2(float(i) / 8, 12.f / 23.f));
		}

		sprite->setAnimationSpeed(BUILDING_LEFT, 9);
		for (int j = 0; j < 2; j++) {
			for (int i = 0; i < 8; i++) {
				sprite->addKeyframe(BUILDING_LEFT, glm::vec2(float(i) / 8, (21.f + float(j)) / 23.f));
			}
		}

		sprite->setAnimationSpeed(BUILDING_RIGHT, 9);
		for (int j = 0; j < 2; j++) {
			for (int i = 0; i < 8; i++) {
				sprite->addKeyframe(BUILDING_RIGHT, glm::vec2(float(i) / 8, (19.f + float(j)) / 23.f));
			}
		}
		
	sprite->changeAnimation(FALLING_RIGHT);
	sprite->setPosition(initialPosition);
}

bool Lemming::update(int deltaTime)
{
	int fall;

	if(sprite->update(deltaTime) == 0)
		return true;

	switch (state) {

	case FALLING_LEFT_STATE:
		primeraPasada = false;
		fall = collisionFloor(2);
		if (fall > 0)
			sprite->position() += glm::vec2(0, fall);
		else {
			sprite->changeAnimation(WALKING_LEFT);
			state = WALKING_LEFT_STATE;
		}
		break;

	case FALLING_RIGHT_STATE:
		primeraPasada = false;
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
		if (collision() || hayParado() && primeraPasada)
		{
			primeraPasada = true;
			if (!climber) {
				sprite->position() -= glm::vec2(-1, -1);
				sprite->changeAnimation(WALKING_RIGHT);
				state = WALKING_RIGHT_STATE;
			}
			else {
				sprite->position() -= glm::vec2(-1, -1);
				sprite->changeAnimation(CLIMBING_LEFT);
				state = CLIMBING_LEFT_STATE;
			}
		}
		else
		{
			if (!hayParado())
				primeraPasada = true;
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
		if (collision() || (hayParado() && primeraPasada))
		{
			primeraPasada = true;
			if (!climber) {
				sprite->position() -= glm::vec2(1, -1);
				sprite->changeAnimation(WALKING_LEFT);
				state = WALKING_LEFT_STATE;
			}
			else {
				sprite->position() -= glm::vec2(1, -1);
				sprite->changeAnimation(CLIMBING_RIGHT);
				state = CLIMBING_RIGHT_STATE;
			}
		}
		else
		{
			if (!hayParado())
				primeraPasada = true;
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
		if (sprite->currentKeyFrame() == 7) {
			eliminado = true;
		}
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
			eraseMask(position()[0] + 8 + displacement, position()[1] + 16, 0, 0.25,-3,3, mask);
			sprite->position() += glm::vec2(0, 0.25);
		}
		break;

	case BLOCKING_STATE:
		break;

	case DIG_LEFT_STATE:
		if(collision()) {
			eraseMask(position()[0] + displacement, position()[1] + 8, -3, 3, -0.25, 0, mask);
			sprite->position() += glm::vec2(-0.25, 0);
		}
		else {
			sprite->changeAnimation(WALKING_LEFT);
			state = WALKING_LEFT_STATE;
		}
		break;

	case DIG_RIGHT_STATE:
		sprite->position() += glm::vec2(1.0, 0);
		if (collision()) {
			eraseMask(position()[0] + 12 + displacement, position()[1] + 8, -8, 8, 0, 0.25, mask);
			sprite->position() += glm::vec2(0.25, 0);
		}
		else {
			sprite->changeAnimation(WALKING_RIGHT);
			state = WALKING_RIGHT_STATE;
		}
		sprite->position() += glm::vec2(-1.0, 0);;
		break;
	case EXPLODE_STATE:
		if (sprite->currentKeyFrame() == 20) {
			mciSendString(TEXT("play sound/BANG.WAV"), NULL, 0, NULL);
		}
		else if (sprite->currentKeyFrame() == 31) {
			eliminado = true;
			return false;
		}
		break;
	case CLIMBING_LEFT_STATE:
		sprite->position() += glm::vec2(-1, -1);
		if (collision()) {
			sprite->position() += glm::vec2(1, 0.5);
		}
		else {
			sprite->changeAnimation(STOPPING_CLIMB_LEFT);
			state = STOPPING_CLIMB_LEFT_STATE;
		}
		break;
	case CLIMBING_RIGHT_STATE:
		sprite->position() += glm::vec2(1, -1);
		if (collision()) {
			sprite->position() += glm::vec2(-1, 0.5);
		}
		else {
			sprite->changeAnimation(STOPPING_CLIMB_RIGHT);
			state = STOPPING_CLIMB_RIGHT_STATE;
		}
		break;
	case STOPPING_CLIMB_LEFT_STATE:
		if (sprite->animation() == STOPPING_CLIMB_LEFT && sprite->currentKeyFrame() == 7) {
			sprite->changeAnimation(WALKING_LEFT);
			state = WALKING_LEFT_STATE;
		}
		break;
	case STOPPING_CLIMB_RIGHT_STATE:
		if (sprite->animation() == STOPPING_CLIMB_RIGHT && sprite->currentKeyFrame() == 7) {
			sprite->changeAnimation(WALKING_RIGHT);
			state = WALKING_RIGHT_STATE;
		}
		break;
	case BUILDING_LEFT_STATE:
		sprite->position() += glm::vec2(-3, -1);
		if (numberOfStairs == 12) {
			sprite->position() -= glm::vec2(-3, -1);
			sprite->changeAnimation(WALKING_LEFT);
			state = WALKING_LEFT_STATE;
		}
		else if (collision()) {
			sprite->position() -= glm::vec2(-3, -1);
			sprite->changeAnimation(WALKING_RIGHT);
			state = WALKING_RIGHT_STATE;
		}
		else {
			sprite->position() -= glm::vec2(-3, 0);
			fall = collisionFloor(3);
			sprite->position() -= glm::vec2(0, -1);
			if (fall < 3) {
				if (sprite->currentKeyFrame() == 0) {
					sprite->position() += glm::vec2(-2.0f, -1.0f);
					numberOfStairs++;
				}
			}
			else {
				sprite->changeAnimation(FALLING_RIGHT);
				state = FALLING_RIGHT_STATE;
			}
		}
		break;
	case BUILDING_RIGHT_STATE:
		sprite->position() += glm::vec2(3, -1);
		if (numberOfStairs == 12) {
			sprite->position() -= glm::vec2(3, -1);
			sprite->changeAnimation(WALKING_RIGHT);
			state = WALKING_RIGHT_STATE;
		}
		else if (collision()) {
			sprite->position() -= glm::vec2(3, -1);
			sprite->changeAnimation(WALKING_LEFT);
			state = WALKING_LEFT_STATE;
		}
		else {
			sprite->position() -= glm::vec2(3, 0);
			fall = collisionFloor(3);
			sprite->position() -= glm::vec2(0, -1);
			if (fall < 3) {
				if (sprite->currentKeyFrame() == 0) {
					sprite->position() += glm::vec2(2.0f, -1.0f);
					numberOfStairs++;
				}
			}
			else {
				sprite->changeAnimation(FALLING_RIGHT);
				state = FALLING_RIGHT_STATE;
			}
		}
		break;
	}
	return true;
}

void Lemming::render()
{
	sprite->render();
}

glm::vec2 Lemming::position() {
	return sprite->position();
}

void Lemming::setMapMask(VariableTexture *mapMask, VariableTexture *lemmingMask)
{
	mask = mapMask;
	parados = lemmingMask;
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

void Lemming::displace(float d)
{
	glm::vec2 pos = sprite->position();
	pos.x += d;
	sprite->setPosition(pos);
}

bool Lemming::goOut() {
	return come_Out;
}

pair<bool, int> Lemming::putStair() {
	pair<bool, int> ret;
	if (state == BUILDING_LEFT_STATE && sprite->currentKeyFrame() == 9) {
		ret.first = true;
		ret.second = 0;
	}
	else if (state == BUILDING_RIGHT_STATE && sprite->currentKeyFrame() == 9) {
		ret.first = true;
		ret.second = 1;
	}
	return ret;
}

int Lemming::collisionFloor(int maxFall)
{
	bool bContact = false;
	int fall = 0;
	glm::ivec2 posBase = sprite->position() + glm::vec2(displacement, 0); // Add the map displacement
	
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
	glm::ivec2 posBase = sprite->position() + glm::vec2(displacement, 0); // Add the map displacement
	
	posBase += glm::ivec2(7, 15);
	if((mask->pixel(posBase.x, posBase.y) == 0) && (mask->pixel(posBase.x+1, posBase.y) == 0))
		return false;

	return true;
}

bool Lemming::hayParado()
{
	glm::ivec2 posBase = sprite->position() + glm::vec2(displacement, 0); // Add the map displacement

	posBase += glm::ivec2(7, 15);
	if ((parados->pixel(posBase.x, posBase.y) == 0) && (parados->pixel(posBase.x + 1, posBase.y) == 0))
		return false;

	return true;
}

void Lemming::setAbility(int ability) {
	if (state != BLOCKING_STATE) {
		if (ability == 2) {
			state = BLOCKING_STATE;
			sprite->changeAnimation(BLOCKING);
			applyMask();
		}
		else if (ability == 3) {
			state = DIG_STATE;
			sprite->changeAnimation(DIG);
		}
		else if (ability == 4) {
			if (state == WALKING_LEFT_STATE) {
				state = DIG_LEFT_STATE;
				sprite->changeAnimation(DIG_LEFT);
			}
			if (state == WALKING_RIGHT_STATE) {
				state = DIG_RIGHT_STATE;
				sprite->changeAnimation(DIG_RIGHT);
			}
		}
		else if (ability == 5) climber = true;
		else if (ability == 6) {
			if (state == WALKING_LEFT_STATE) {
				state = BUILDING_LEFT_STATE;
				sprite->changeAnimation(BUILDING_LEFT);
				numberOfStairs = 0;
			}
			else if (state == WALKING_RIGHT_STATE) {
				state = BUILDING_RIGHT_STATE;
				sprite->changeAnimation(BUILDING_RIGHT);
				numberOfStairs = 0;
			}
			else if (state == BUILDING_LEFT_STATE || state == BUILDING_RIGHT_STATE) numberOfStairs = 0;
		}
	}
	if (ability == 7 || ability == 1) {
		if(ability == 1 && state == BLOCKING_STATE) 
			eraseMask(position()[0] + displacement, position()[1], 0, 16, 0, 16, parados);
		state = EXPLODE_STATE;
		sprite->changeAnimation(EXPLODING);
	}
}

void Lemming::eraseMask(int posX, int posY, float ymin, float ymax, float xmin, float xmax, VariableTexture* mascara) {
	for (int y = max(0.f, posY+ymin); y <= min((float)mask->height() - 1, posY + ymax); y++) {
		for (int x = max(0.f, posX + xmin); x <= min((float)mask->width() - 1, posX + xmax); x++) {
			mascara->setPixel(x, y, 0);
		}
	}
}



void Lemming::applyMask() {
	int posX, posY;
	posX = position()[0] + displacement;
	posY = position()[1];

	for (int y = max(0, posY); y <= min(mask->height() - 1, posY + 16); y++) {
		for (int x = max(0, posX); x <= min(mask->width() - 1, posX + 16); x++) {
			parados->setPixel(x, y, 255);
		}
	}
}








