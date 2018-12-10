#include "PlayerManager.h"
#include "UIManager.h"
#include "TextComponent.h"

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
	printf("next player index:%d\n", currentPlayerIndex);

	instance->players[instance->currentPlayerIndex]->getFirstCharacter()->bulletoAmmo = instance->players[instance->currentPlayerIndex]->getFirstCharacter()->maxBulletsPerTurn;
	//printf("max bullet per turn: %d", instance->players[instance->currentPlayerIndex]->getFirstCharacter()->maxBulletsPerTurn);

	actionsTaken = 0;
	UpdatePlayerUI();
}

void PlayerManager::AddPlayer(Player * player)
{
	players.push_back(player); 
}

void PlayerManager::RemovePlayer(int playerID) {

	for (int i = 0; i < instance->players.size(); i++) {	
		if (instance->players[i]->playerID == playerID) {
			instance->players.erase(instance->players.begin() + i);

			if (i == instance->currentPlayerIndex) {
				instance->currentPlayerIndex++;
				if (instance->currentPlayerIndex >= instance->players.size()) currentPlayerIndex = 0;
			}
			else if (i < instance->currentPlayerIndex) {
				//p1(index 0) p2(index 1) p3(index 2) p4(index 3) p5(index 4)  --> if we delete p2, while the current player is p3, then currentIndex should be 2
				//However, after p2 has been deleted, an index of 2 will point at p4, so we need to decrement by 1.
				instance->currentPlayerIndex--;
			}
			
			if(instance->currentPlayerIndex >= instance->players.size() || instance->currentPlayerIndex < 0){
				instance->currentPlayerIndex = 0; //dont want to access out of bounds element
			}

			break;
		}
	}

}

Player* PlayerManager::GetCurrentPlayer() {
	return instance->players[currentPlayerIndex];
}

void PlayerManager::UpdatePlayerUI() {

	if (instance->players[currentPlayerIndex] != NULL) { //update ui with info pertaining to whose turn it is, the team they belong to, and their hp

		std::vector<int> teams;
		teams.push_back(instance->players[currentPlayerIndex]->getFirstCharacter()->teamID);
		bool containsTeam = false;


		for (int i = 0;i < instance->players.size();i++) { //check how many teams are left

			for (int j = 0;j < teams.size();j++) {
				if (teams[j] == instance->players[i]->getFirstCharacter()->teamID) {
					containsTeam = true;
				}
			}

			if (!containsTeam) {
				teams.push_back(instance->players[i]->getFirstCharacter()->teamID);
			}

			containsTeam = false;
		}

		//printf("teams size: %d\n", teams.size());

		std::string info = "";

		if (teams.size() > 1) {
			info += "P:";
			info += std::to_string(instance->players[instance->currentPlayerIndex]->playerID);
			info += " Team:";
			info += std::to_string(instance->players[instance->currentPlayerIndex]->getFirstCharacter()->teamID);
			info += " HP:";
			info += std::to_string(instance->players[instance->currentPlayerIndex]->getFirstCharacter()->health);
		}
		else {
			info += "Team ";
			info += std::to_string(instance->players[instance->currentPlayerIndex]->getFirstCharacter()->teamID);
			info += " Has won!";
		}

        UIComponent *topRightInfo = UIManager::GetComponentById("rText");
        if (topRightInfo != nullptr) {
            TextComponent *trText = dynamic_cast<TextComponent*>(topRightInfo);
            trText->SetText(info);
        }
	}
}


void PlayerManager::notify(EventName eventName, Param *params)
{
	if (!players.size()) return;

	switch (eventName) {
	case PLAYER_LEFT: {
		TypeParam<bool> *p = dynamic_cast<TypeParam<bool> *>(params); // Safetly cast generic param pointer to a specific type
		if (p != nullptr && instance->turnStage == 0)
		{
			if (actionsTaken > maxActionsPerTurn && p->Param) return;
			players[currentPlayerIndex]->moveLeft(p->Param);
		}
		break;
	}
	case PLAYER_RIGHT: {
		TypeParam<bool> *p = dynamic_cast<TypeParam<bool> *>(params); // Safetly cast generic param pointer to a specific type
		if (p != nullptr && instance->turnStage == 0)
		{
			if (actionsTaken > maxActionsPerTurn && p->Param) return;
			players[currentPlayerIndex]->moveRight(p->Param);
		}
		break;
	}
	case PLAYER_JUMP: {
		TypeParam<bool> *p = dynamic_cast<TypeParam<bool> *>(params); // Safetly cast generic param pointer to a specific type
		if (p != nullptr)
		{
			if (actionsTaken > maxActionsPerTurn && p->Param) return;
			players[currentPlayerIndex]->jump(p->Param);
		}
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

	printf("Actions taken: %d, Max Actions Per Turn = %d\n", actionsTaken, maxActionsPerTurn);
}