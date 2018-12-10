#include "Character.h"

Character::Character(int health, int startingAmmo, int maxBulletsPerTurn)
{
	controllable = false;
	left_input = false;
	right_input = false;
	jump_input = false;
	this->health = health;
	this->bulletoAmmo = startingAmmo; //hard coded 10 starting ammo
	this->maxBulletsPerTurn = maxBulletsPerTurn;

}

void Character::TakeDamage(int amount) {
	this->health -= amount;
}