#include "Character.h"

Character::Character(int health, int startingAmmo, int maxBulletsPerTurn)
{
	controllable = false;
	left_input = false;
	right_input = false;
	jump_input = false;
	this->health = health;
	this->bulletoAmmo = startingAmmo; 
	this->maxBulletsPerTurn = maxBulletsPerTurn;
}

void Character::TakeDamage(int amount) {
	this->health -= amount;
}