#include "HackjobBulletManager.h"
#include "UIManager.h"
#include "TextComponent.h"
#include "SoundParams.h"

HackjobBulletManager::HackjobBulletManager(PhysicsManager *physics, Map* map) {
	this->physics = physics;
	this->map = map;
	bulletList = std::vector<HackjobBullet*>();
	timeSince = 0;
    EventManager::subscribe(BULLET_SPAWN, this);
}

HackjobBulletManager::~HackjobBulletManager()
{
	EventManager::unsubscribe(BULLET_SPAWN, this);
}

void HackjobBulletManager::SetInfoText(std::string info) {
    TextComponent *infoText = dynamic_cast<TextComponent*>(UIManager::GetComponentById("rInfoText"));
    if (infoText != nullptr) {
        infoText->SetText(info);
    }
}

void HackjobBulletManager::notify(EventName eventName, Param *params) {
    switch (eventName) {
    case BULLET_SPAWN: {
		Weapon* weap = PlayerManager::instance->GetCurrentPlayer()->getWeapons()->curWeapon();
        float bulletSpeedScalar = weap->_speed;
        int damage = weap->_damage;
        float explosionRadius = weap->_explosionRad;
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

		for (int j = 0; j < PlayerManager::instance->players.size(); j++) {

			for (int k = 0; k < PlayerManager::instance->players[j]->chars.size(); k++) {

				Character *character = PlayerManager::instance->players[j]->chars[k];

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
						PlayerManager::instance->players[j]->RemoveCharacter(k);
					}

					SoundParams * bulletNoise = new SoundParams();

					bulletNoise->sound = Damage;

					bulletNoise->x = 0;
					bulletNoise->y = 0;
					bulletNoise->z = 0;

					TypeParam<SoundParams*> *bulletSound = new TypeParam<SoundParams*>(bulletNoise);
					EventManager::notify(PLAY_SOUND, bulletSound);

					bullet->renderable = NULL; //delete bullet's renderable share pointer
					bulletList.erase(bulletList.begin() + i);

					PlayerManager::instance->NextPlayer();

					break;
				}
			}
		}
	}
}

void HackjobBulletManager::UpdateBullet(float dTime) {

	CheckIfPlayersDamaged();

	if (bulletList.size() != 0)
	{
		timeSince += dTime;
		if (timeSince >= waitTime)
		{
			timeSince = 0;
			bulletList[0]->renderable = NULL;
			bulletList.erase(bulletList.begin());

			PlayerManager::instance->NextPlayer();
		}
	}

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

			PlayerManager::instance->NextPlayer();

			break; //break in order to avoid vector iteration errors
		}
	} 


}

void HackjobBulletManager::SpawnBulleto(float speedScalar, int damage, float explodeRadius) { //spawns a projectile from where the player is located at

	if (PlayerManager::instance->GetCurrentPlayer()->getCurrentCharacter()->bulletoAmmo-- <= 0) return; //dont shoot if we don't have "bulleto ammo"

	std::string currentAmmoString = "P";
	currentAmmoString += std::to_string(PlayerManager::instance->GetCurrentPlayer()->playerID);
	currentAmmoString += " Ammo: ";
	currentAmmoString += std::to_string(PlayerManager::instance->GetCurrentPlayer()->getCurrentCharacter()->bulletoAmmo);
	SetInfoText(currentAmmoString);

	HackjobBullet *bullet = new HackjobBullet(damage, explodeRadius); //default dmg is float 100.0f, if no args are provided to the bullet, second argument = bullet's explode radius
	bullet->shooter_PlayerID = PlayerManager::instance->GetCurrentPlayer()->playerID;
	bullet->shooterCharacterID = PlayerManager::instance->GetCurrentPlayer()->getCurrentCharacter()->characterID;
	bullet->mass = 15;
	bullet->radius = 2.0f;
	bullet->set_position(PlayerManager::instance->GetCurrentPlayer()->getCurrentCharacter()->get_position()); 

	glm::vec3 rot = PlayerManager::instance->GetCurrentPlayer()->getCurrentCharacter()->get_rotation();
	rot.z += PlayerManager::instance->GetCurrentPlayer()->aim_angle;
	rot.z += (rot.y == 0.0f) ? -45 : 45;
	float x = -sin(rot.z * (float)M_PI / 180.0f);
	float y = cos(rot.z * (float)M_PI / 180.0f);
	bullet->velocity = glm::vec2(x, y) * speedScalar;

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
