#pragma once
#include "Player.h"
#include <vector>
#include "ISubscriber.h"

class PlayerManager : public ISubscriber
{
public:
	static PlayerManager *instance;
	PlayerManager();
	~PlayerManager();

	static void prevWeapon();
	static void nextWeapon();
	static void fireWeapon();

	void NextPlayer();

	void AddPlayer(Player *player);

	virtual void notify(EventName eventName, Param *params) override;

private:
	std::vector<Player*> players;
	int currentPlayerIndex;
};