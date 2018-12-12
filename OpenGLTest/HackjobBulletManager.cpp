#include "HackjobBulletManager.h"
#include "UIManager.h"
#include "TextComponent.h"
#include "SoundParams.h"

HackjobBulletManager::HackjobBulletManager(PlayerManager *playerManager, PhysicsManager *physics, Map* map) {
	this->physics = physics;
	this->playerManager = playerManager;
	this->map = map;
	bulletList = std::vector<HackjobBullet*>();

    EventManager::subscribe(BULLET_SPAWN, this);
}

void HackjobBulletManager::SetInfoText(std::string info) { //this doesn't work (crashes) if called in Main
    TextComponent *infoText = dynamic_cast<TextComponent*>(UIManager::GetComponentById("rInfoText"));
    if (infoText != nullptr) {
        infoText->SetText(info);
    }
}

void HackjobBulletManager::notify(EventName eventName, Param *params) {
    switch (eventName) {
    case BULLET_SPAWN: {
        float bulletSpeedScalar = 42.069f;
        int damage = 200;
        float explosionRadius = 5.0f;
        SpawnBulleto(bulletSpeedScalar, damage, explosionRadius); //speed scalar, int damage, float explosionRadius
        printf("Firing bullet with speedScalar %lf, damage %d, explode radius %lf\n", bulletSpeedScalar, damage, explosionRadius);
        break;
    }
    default:
        break;
    }
}

void HackjobBulletManager::CheckIfPlayersDamaged() {

	for (int i = 0; i < bulletList.size(); i++) {	//For every bullet in the game, check if they have hit any of the players 

		HackjobBullet *bullet = bulletList[i];

		for (int j = 0; j < playerManager->instance->players.size(); j++) {

			for (int k = 0; k < playerManager->instance->players[j]->chars.size(); k++) {

				Character *character = playerManager->instance->players[j]->chars[k];

				if (bullet->colliding_with_player(character->get_position())
					&& character->playerID != bullet->shooter_PlayerID && character->characterID != bullet->shooterCharacterID) {

					character->TakeDamage(bullet->damage);
					//printf("Player ID:%d got hit! health remaining: %d\n", character->playerID, character->health);
					std::string hitInfo = "P";
					hitInfo += std::to_string(character->playerID);
					hitInfo += "C:";
					hitInfo += std::to_string(character->characterID);
					hitInfo += " hit hp:";
					hitInfo += std::to_string(character->health);
					SetInfoText(hitInfo);

					map->explosion(Planetoid(bullet->get_position().x, bullet->get_position().y, bullet->explosionRadius));

					if (character->health <= 0) { //kills character and removes the dead dude from player manager
						character->renderable = NULL;
						int playerToBeRemovedID = character->playerID;
						playerManager->instance->players[j]->RemoveCharacter(k);

						if (playerManager->instance->players[j]->chars.size() == 0) {
							playerManager->RemovePlayer(playerToBeRemovedID);
						}
					}

					bullet->renderable = NULL; //delete bullet's renderable share pointer
					bulletList.erase(bulletList.begin() + i);
					break;
					
				}
			}

		}
	}
}

void HackjobBulletManager::UpdateBullet() {

	CheckIfPlayersDamaged();

	for (int i = 0; i < bulletList.size(); i++)
	{

		if (bulletList[i]->grounded && bulletList[i]->bulletMaxAliveTicks++ > 50) { //check if alive ticks greater than 10 so bullets dont immediately explode as we're shooting 
			map->explosion(Planetoid(bulletList[i]->get_position().x, bulletList[i]->get_position().y, bulletList[i]->explosionRadius));
			SoundParams * bulletNoise = new SoundParams();

			bulletNoise->sound = Damage;

			bulletNoise->x = 0;
			bulletNoise->y = 0;
			bulletNoise->z = 0;

			TypeParam<SoundParams*> *bulletSound = new TypeParam<SoundParams*>(bulletNoise);
			EventManager::notify(PLAY_SOUND, bulletSound);
			bulletList[i]->renderable = NULL;
			bulletList.erase(bulletList.begin() + i);
			break; //break in order to avoid vector iteration errors
		}
	} 


}

float xBaseMultiplier = 9.0f, yBaseMultiplier = 1.0f;

void HackjobBulletManager::SpawnBulleto(float speedScalar, int damage, float explodeRadius) { //spawns a projectile from where the player is located at

	if (playerManager->GetCurrentPlayer()->getCurrentCharacter()->bulletoAmmo-- <= 0) return; //dont shoot if we don't have "bulleto ammo"

	std::string currentAmmoString = "P";
	currentAmmoString += std::to_string( playerManager->GetCurrentPlayer()->playerID);
	currentAmmoString += " Ammo: ";
	currentAmmoString += std::to_string(playerManager->GetCurrentPlayer()->getCurrentCharacter()->bulletoAmmo);
	SetInfoText(currentAmmoString);

	HackjobBullet *bullet = new HackjobBullet(damage, explodeRadius); //default dmg is float 100.0f, if no args are provided to the bullet, second argument = bullet's explode radius
	bullet->shooter_PlayerID = playerManager->GetCurrentPlayer()->playerID;
	bullet->shooterCharacterID = playerManager->GetCurrentPlayer()->getCurrentCharacter()->characterID;
	bullet->mass = 15;
	bullet->radius = 2.0f;
	bullet->set_position(playerManager->GetCurrentPlayer()->getCurrentCharacter()->get_position()); 

	int zOrientation = (int)playerManager->GetCurrentPlayer()->getCurrentCharacter()->get_rotation().z % 360; //set projectile angle based on player  orientation
	if (zOrientation < 0) zOrientation += 360; //normalize z rotation to a value between 0 - 360
	

	float dir = playerManager->GetCurrentPlayer()->getCurrentCharacter()->get_rotation().y;
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


	Renderable *pSkin = new Renderable();
	pSkin->z = 0;
	pSkin->model = AssetLoader::loadModel("../Models/sphere.obj");
	pSkin->color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	bullet->setRenderable(pSkin);
	pSkin->scale = glm::vec3(0.1);
	pSkin->metallic = 1.0;
	physics->addObject(bullet);
	bulletList.push_back(bullet);

	EventManager::notify(RENDERER_ADD_TO_RENDERABLES, &TypeParam<std::shared_ptr<Renderable>>(bullet->renderable), false);
}
