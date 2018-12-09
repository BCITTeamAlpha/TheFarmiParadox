#include "BulletoManager.h"


BulletoManager::BulletoManager(PlayerManager *playerManager, PhysicsManager *physics) {
	this->physics = physics;
	this->playerManager = playerManager;
	bulletList = std::vector<Bulleto*>();
}

void BulletoManager::SetInfoText(std::string info) {
	TypeParam<std::string*> param(&info);
	EventManager::notify(RENDERER_SET_INFOTEXT, &param, false);

}

void BulletoManager::UpdateBullet() {

	for (int i = 0; i < bulletList.size(); i++)
	{
		if (bulletList[i]->bulletAliveTicks++ > bulletList[i]->bulletMaxAliveTicks) { //delete bullets once they've existed for longer than the maximum allowed ticks
			bulletList[i]->renderable = NULL;
			bulletList.erase(bulletList.begin() + i);
			break; //break in order to avoid vector iteration errors
		}
	}

	for (int i = 0; i < bulletList.size(); i++) {	//For every bullet in the game, check if they have hit any of the players 

		Bulleto *bullet = bulletList[i]; 

		for (int j = 0; j < playerManager->instance->players.size();j++) {

			Character *character = playerManager->instance->players[j]->getFirstCharacter();

			if (bullet->colliding_with_player(character->get_position()) 
				&& character->playerID != bullet->shooter_PlayerID
				&& character->teamID != bullet->shooter_teamID) {
				//character->health -= bullet->damage;
				character->TakeDamage(bullet->damage);
				printf("Player ID:%d got hit! health remaining: %d\n", character->playerID, character->health);
				if (character->health <= 0) { //kills character and removes the dead dude from player manager
					character->renderable = NULL;
					playerManager->RemovePlayer(character->playerID);
				}
				bullet->renderable = NULL; //delete bullet's renderable share pointer
				bulletList.erase(bulletList.begin() + i);
				break;
			}
		}
	}

}

float xBaseMultiplier = 8.0f, yBaseMultiplier = 1.0f;

void BulletoManager::SpawnBulleto(float speedScalar) { //spawns a projectile from where the player is located at
	//printf("current char ammo: %d\n" , playerManager->GetCurrentPlayer()->getFirstCharacter()->bulletoAmmo);
	if (playerManager->GetCurrentPlayer()->getFirstCharacter()->bulletoAmmo-- <= 0) return; //dont shoot if we don't have "bulleto ammo"
	std::string currentAmmoString = "P";
	currentAmmoString += std::to_string( playerManager->GetCurrentPlayer()->playerID);
	currentAmmoString += " Ammo: ";
	currentAmmoString += std::to_string(playerManager->GetCurrentPlayer()->getFirstCharacter()->bulletoAmmo);
	SetInfoText(currentAmmoString);

	Bulleto *bullet = new Bulleto(); //default dmg is float 100.0f, if no args are provided to the bullet
	bullet->shooter_PlayerID = playerManager->GetCurrentPlayer()->playerID;
	bullet->shooter_teamID = playerManager->GetCurrentPlayer()->getFirstCharacter()->teamID;
	bullet->mass = 15;
	bullet->radius = 2.0f;
	bullet->set_position(playerManager->GetCurrentPlayer()->getFirstCharacter()->get_position()); 

	int zOrientation = (int)playerManager->GetCurrentPlayer()->getFirstCharacter()->get_rotation().z % 360; //set projectile angle based on player  orientation
	if (zOrientation < 0) zOrientation += 360; //normalize z rotation to a value between 0 - 360
	

	float dir = playerManager->GetCurrentPlayer()->getFirstCharacter()->get_rotation().y; 
	if (dir != 0) {
		if(zOrientation > 90 && zOrientation < 270) //between 90 and 270 exclusive
			bullet->velocity = glm::vec2(xBaseMultiplier, yBaseMultiplier) * speedScalar;
		if ((zOrientation > 0 && zOrientation < 90) || (zOrientation > 270 && zOrientation < 360))
			bullet->velocity =  glm::vec2(-xBaseMultiplier, yBaseMultiplier) * speedScalar;
	}
	else {
		if (zOrientation > 90 && zOrientation < 270)
			bullet->velocity = glm::vec2(-xBaseMultiplier, yBaseMultiplier) * speedScalar;
		if ((zOrientation > 0 && zOrientation < 90) || (zOrientation > 270 && zOrientation < 360))
			bullet->velocity = glm::vec2(xBaseMultiplier, yBaseMultiplier) * speedScalar;
	}
	//std::cout << "current player rot: " << zOrientation << " , " << playerManager->GetCurrentPlayer()->getFirstCharacter()->get_rotation().y << std::endl;

	Renderable *pSkin = new Renderable();
	pSkin->z = 0;
	pSkin->model = AssetLoader::loadModel("../Models/sphere.obj");
	pSkin->color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	bullet->setRenderable(pSkin);
	pSkin->scale = glm::vec3(0.1);
	physics->addObject(bullet);
	bulletList.push_back(bullet);
	EventManager::notify(RENDERER_ADD_TO_RENDERABLES, &TypeParam<std::shared_ptr<Renderable>>(bullet->renderable), false);
}
