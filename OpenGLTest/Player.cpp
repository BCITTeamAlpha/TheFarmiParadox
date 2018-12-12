#include "Player.h"

Player::Player()
{
	currentCharacterIndex = 0;
	//chars = std::vector<Character *>();
	weaps = new Inventory();
}

void Player::addCharacter(Character *c)
{
	chars.push_back(c);
}

Character* Player::getFirstCharacter() {
	return chars[0];
}

Character* Player::getCurrentCharacter() {
	return chars[currentCharacterIndex];
}

void Player::addItem(Pickup item)
{
	weaps->addItem(item);
}

void Player::prevWeapon()
{
	weaps->selectSlot(weaps->currentSlot - 1);
}

void Player::nextWeapon()
{
	weaps->selectSlot(weaps->currentSlot + 1);
}

void Player::setControllable(bool c)
{
	chars[currentCharacterIndex]->controllable = c;
}

void Player::setNextCharacter() {
	clearInput(); //sets previous to not controllable
	currentCharacterIndex++;
	if (currentCharacterIndex >= chars.size()) currentCharacterIndex = 0; //checks for wrap around
	setControllable(true); //sets current to controllable
}

void Player::fireWeapon()
{
	weaps->useWeapon(chars[currentCharacterIndex]->get_position());
}

void Player::setAimLeft(bool b)
{
	aimLeft = b;
}

void Player::setAimRight(bool b)
{
	aimRight = b;
}

void Player::adjustAim(float dTime)
{
	float dAngle;

	if (aimLeft && !aimRight)
		dAngle = -dTime;
	else if (!aimLeft && aimRight)
		dAngle = dTime;
	else
		dAngle = 0;

	weaps->angleWeapon(dAngle);
}

void Player::clearInput()
{
	for (auto &c : chars)
	{
		c->left_input = false;
		c->right_input = false;
		c->jump_input= false;
	}
}

void Player::moveLeft(bool v)
{
	for (auto &c : chars)
	{
		c->left_input = v;
	}
}

void Player::moveRight(bool v)
{
	for (auto &c : chars)
	{
		c->right_input = v;
	}
}

void Player::jump(bool v)
{
	for (auto &c : chars)
	{
		c->jump_input = v;
	}
}
void Player::RemoveCharacter(int index) {
	chars.erase(chars.begin() + index);
	currentCharacterIndex = currentCharacterIndex % chars.size();
}

Inventory Player::getWeapons() {
	return *weaps;
}