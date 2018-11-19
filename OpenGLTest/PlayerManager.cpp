#include "PlayerManager.h"

PlayerManager * PlayerManager::instance = NULL;

PlayerManager::PlayerManager()
{
	instance = this;
	currentPlayerIndex = 0;
	turnStage = 0; //moving, aiming, firing
}


PlayerManager::~PlayerManager()
{

}

void PlayerManager::prevWeapon()
{
	if (instance->currentPlayerIndex < instance->players.size())
	{
		instance->players[instance->currentPlayerIndex]->prevWeapon();
	}
}

void PlayerManager::nextWeapon()
{
	if (instance->currentPlayerIndex < instance->players.size())
	{
		instance->players[instance->currentPlayerIndex]->nextWeapon();
	}
}

void PlayerManager::aimWeapon()
{
	if (instance->currentPlayerIndex < instance->players.size())
	{
		instance->turnStage = 1;
		instance->players[instance->currentPlayerIndex]->setControllable(false);
	}
}

void PlayerManager::handlePlayers(float dTime)
{
	if (instance->currentPlayerIndex < instance->players.size())
	{
		if (instance->turnStage == 1)
			instance->players[instance->currentPlayerIndex]->adjustAim(dTime);
	}
}

void PlayerManager::fireWeapon()
{
	if (instance->currentPlayerIndex < instance->players.size())
	{
		instance->players[instance->currentPlayerIndex]->fireWeapon();

		instance->turnStage = 2;
	}
	
	instance->NextPlayer();
}

void PlayerManager::NextPlayer()
{
	instance->turnStage = 0;
	instance->players[instance->currentPlayerIndex]->setControllable(true);

	if (players.size())
	{
		players[currentPlayerIndex]->clearInput();
		currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
	}
	printf("next player indx:%d\n", currentPlayerIndex);
}

void PlayerManager::AddPlayer(Player * player)
{
	players.push_back(player);
}

void PlayerManager::notify(EventName eventName, Param *params)
{
	if (!players.size()) return;
	switch (eventName) {
	case PLAYER_LEFT: {
		TypeParam<bool> *p = dynamic_cast<TypeParam<bool> *>(params); // Safetly cast generic param pointer to a specific type
		if (p != nullptr && instance->turnStage == 0)
			players[currentPlayerIndex]->moveLeft(p->Param);
		break;
	}
	case PLAYER_RIGHT: {
		TypeParam<bool> *p = dynamic_cast<TypeParam<bool> *>(params); // Safetly cast generic param pointer to a specific type
		if (p != nullptr && instance->turnStage == 0)
			players[currentPlayerIndex]->moveRight(p->Param);
		break;
	}
	case PLAYER_JUMP: {
		printf("jump!\n");
		TypeParam<bool> *p = dynamic_cast<TypeParam<bool> *>(params); // Safetly cast generic param pointer to a specific type
		if (p != nullptr)
			players[currentPlayerIndex]->jump(p->Param);
		break;
	}
	case AIM_LEFT: {
		TypeParam<bool> *p = dynamic_cast<TypeParam<bool> *>(params); // Safetly cast generic param pointer to a specific type
		if (p != nullptr && instance->turnStage == 1)
			players[currentPlayerIndex]->setAimLeft(p->Param);
		break;
	}
	case AIM_RIGHT: {
		TypeParam<bool> *p = dynamic_cast<TypeParam<bool> *>(params); // Safetly cast generic param pointer to a specific type
		if (p != nullptr && instance->turnStage == 1)
			players[currentPlayerIndex]->setAimRight(p->Param);
		break;
	}
	case PLAYER_FIRE: {
		if (instance->turnStage == 0)
			aimWeapon();
		else if (instance->turnStage == 1)
			fireWeapon();
		break;
	}
	default:
		break;
	}
}