#include "lab/tema/enemy.h"

#include <vector>
#include <string>
#include <iostream>

#include "glm/gtx/string_cast.hpp"
#include <glm/gtc/random.hpp>

using namespace std;
using namespace lab;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Enemy::Enemy(glm::vec3 position, glm::vec3 color)
{
	this->position = position;
	this->color = color;
	this->rotation = glm::vec3(0);
	this->speed = 2.5f;
	this->limbRotation1 = 0.0f;
	this->limbRotation2 = 0.0f;
	this->limbMaxRotation = 45.0f;
	this->limbMinRotation = -45.0f;
	this->limbDirection1 = 0;
	this->limbDirection2 = 1;
	this->limbRotationSpeed = 200.0f;
	this->isMoving = false;
}


Enemy::~Enemy()
{

}

// Getters
glm::vec3 Enemy::GetPosition()
{
	return this->position;
}

glm::vec3 Enemy::GetRotation()
{
	return this->rotation;
}

glm::vec3 Enemy::GetColor()
{
	return this->color;
}

float Enemy::GetLimbRotation1()
{
	return this->limbRotation1;
}

float Enemy::GetLimbRotation2()
{
	return this->limbRotation2;
}

float Enemy::GetLimbMaxRotation()
{
	return this->limbMaxRotation;
}

float Enemy::GetLimbMinRotation()
{
	return this->limbMinRotation;
}

int Enemy::GetLimbDirection1()
{
	return this->limbDirection1;
}

int Enemy::GetLimbDirection2()
{
	return this->limbDirection2;
}

float Enemy::GetLimbRotationSpeed()
{
	return this->limbRotationSpeed;
}

// Setters

void Enemy::SetPosition(glm::vec3 newPos)
{
    this->position = newPos;
}



void Enemy::Follow(glm::vec3 target, float deltaTime)
{
    if (this->position == target)
    {
        this->isMoving = false;
    }
    else
    {
        this->isMoving = true;
    }
	glm::vec3 direction = glm::normalize(target - this->position);

	float angle = glm::degrees(atan2(direction.x, direction.z));
	this->rotation = glm::vec3(0, angle, 0);

	this->position += direction * this->speed * deltaTime;


}

void Enemy::Animate(float deltaTime)
{
    if (this->isMoving)
    {
        // Calculate limb rotation for walking animation
        if (limbDirection1 == 0)
        {
            limbRotation1 += deltaTime * limbRotationSpeed;
        }
        else
        {
            limbRotation1 -= deltaTime * limbRotationSpeed;
        }
        if (limbDirection2 == 0)
        {
            limbRotation2 += deltaTime * limbRotationSpeed;
        }
        else
        {
            limbRotation2 -= deltaTime * limbRotationSpeed;
        }

        if (limbRotation1 >= limbMaxRotation)
        {
            limbDirection1 = 1;
        }
        if (limbRotation1 <= limbMinRotation)
        {
            limbDirection1 = 0;
        }
        if (limbRotation2 >= limbMaxRotation)
        {
            limbDirection2 = 1;
        }
        if (limbRotation2 <= limbMinRotation)
        {
            limbDirection2 = 0;
        }
    }
    else
    {
        // Calculate limb rotation for idle player
        const float resetSpeed = 5.0f;
        if (limbRotation1 != 0 && limbRotation2 != 0)
        {
            limbRotation1 = glm::mix(limbRotation1, 0.0f, deltaTime * resetSpeed);
            limbRotation2 = glm::mix(limbRotation2, 0.0f, deltaTime * resetSpeed);
        }
    }
}

