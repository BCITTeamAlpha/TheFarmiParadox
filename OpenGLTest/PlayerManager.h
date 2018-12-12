#pragma once
#include "Player.h"
#include <vector>
#include "ISubscriber.h"
#include "EventManager.h"
#include "UIManager.h"
#include "TextComponent.h"

class PlayerManager : public ISubscriber
{
public:
	static PlayerManager *instance;
	PlayerManager();
	~PlayerManager();

	static void prevWeapon();
	static void nextWeapon();
	static void aimWeapon();
	static void fireWeapon();

	int handlePlayers(float dTime);

	void NextPlayer();

	void AddPlayer(Player *player);

	void RemovePlayer(int playerID);

	float SecondsRemaining();

	Player* GetCurrentPlayer();

	void UpdatePlayerUI();

	virtual void notify(EventName eventName, Param *params) override;
	std::vector<Player*> players;

	int moveTime = 20;
	int aimTime = 10;
private:
    std::string getTimeString();
	int currentPlayerIndex;
	size_t turnStage;
	float timeElapsed;
};