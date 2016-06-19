/* 

Amine Rehioui
Created: April 4th 2010

*/

#include "ShootTest.h"

// Game
#include "GameManager.h"
#include "AudioManager.h"
#include "Starfield.h"
#include "PlayerCamera.h"
#include "PlayerCollisionComponent.h"
#include "ConfigPoint.h"
#include "ItemManager.h"
#include "ConversationManager.h"
#include "Item.h"
#include "ConversationVisitor.h"
#include "GameData.h"
#include "UIContext.h"
#include "PlayerList.h"
#include "DamageComponent.h"
#include "VKButton.h"
#include "VKeyboard.h"

// AI
#include "AreaSpawner.h"
#include "TriggerSpawner.h"
#include "BasicEnemy.h"
#include "Asteroid.h"
#include "FallingAsteroid.h"
#include "BasicShooter.h"
#include "BasicTower.h"
#include "KamikazeTower.h"
#include "TightPassage.h"
#include "BasicFighter.h"
#include "AIManager.h"
#include "Boss1.h"
#include "Boss2.h"
#include "Boss3.h"
#include "Boss4.h"
#include "SpinShooter.h"
#include "Kamikaze.h"
#include "RadialShooter.h"
#include "RadialTower.h"
#include "SpinningLasers.h"
#include "Squad.h"
#include "RadialTank.h"
#include "LaserSwiper.h"
#include "Bomber.h"

// Weapon
#include "PlayerCannons.h"
#include "WeaponConfig.h"
#include "PulseManager.h"
#include "LaserManager.h"
#include "PelletManager.h"
#include "BulletManagerCollisionComponent.h"
#include "LaserManagerCollisionComponent.h"

using namespace shoot;

//! Object registration function
void RegisterObjects()
{
	ObjectFactory* pFactory = ObjectFactory::Instance();

	// Game
	pFactory->Register<Actor>();
	pFactory->Register<GameManager>();	
	pFactory->Register<AudioManager>();
	pFactory->Register<Starfield>();
	pFactory->Register<Player>();
	pFactory->Register<PlayerCamera>();
	pFactory->Register<PlayerCollisionComponent>();
	pFactory->Register<ConfigPoint>();
	pFactory->Register<ItemManager>();
	pFactory->Register<ConversationManager>();	
	pFactory->Register<Item>();
	pFactory->Register<SFXManager>();
	pFactory->Register<ConversationVisitor>();
	pFactory->Register<GameData>();
	pFactory->Register<UIContext>();
	pFactory->Register<PlayerList>();
	pFactory->Register<DamageComponent>();
	pFactory->Register<VKButton>();
	pFactory->Register<VKeyboard>();

	// AI
	pFactory->Register<AreaSpawner>();
	pFactory->Register<TriggerSpawner>();
	pFactory->Register<BasicEnemy>();
	pFactory->Register<BasicEnemySettings>();
	pFactory->Register<Asteroid>();
	pFactory->Register<AsteroidSettings>();
	pFactory->Register<FallingAsteroid>();
	pFactory->Register<FallingAsteroidSettings>();
	pFactory->Register<BasicShooter>();
	pFactory->Register<BasicShooterSettings>();
	pFactory->Register<BasicTower>();
	pFactory->Register<BasicTowerSettings>();
	pFactory->Register<KamikazeTower>();
	pFactory->Register<KamikazeTowerSettings>();
	pFactory->Register<TightPassage>();
	pFactory->Register<TightPassageSettings>();
	pFactory->Register<BasicFighter>();
	pFactory->Register<BasicFighterSettings>();	
	pFactory->Register<AIManager>();
	pFactory->Register<Boss1>();
	pFactory->Register<Boss2>();
	pFactory->Register<Boss3>();
	pFactory->Register<Boss4>();
	pFactory->Register<SpinShooter>();
	pFactory->Register<SpinShooterSettings>();
	pFactory->Register<RadialShooter>();
	pFactory->Register<RadialShooterSettings>();
	pFactory->Register<RadialTower>();
	pFactory->Register<RadialTowerSettings>();
	pFactory->Register<Kamikaze>();
	pFactory->Register<KamikazeSettings>();
	pFactory->Register<SpinningLasers>();
	pFactory->Register<SpinningLasersSettings>();
	pFactory->Register<Squad>();
	pFactory->Register<SquadSettings>();
	pFactory->Register<RadialTank>();
	pFactory->Register<RadialTankSettings>();
	pFactory->Register<LaserSwiper>();
	pFactory->Register<LaserSwiperSettings>();
	pFactory->Register<Bomber>();
	pFactory->Register<BomberSettings>();

	// Weapon
	pFactory->Register<PlayerCannons>();
	pFactory->Register<PulseConfig>();
	pFactory->Register<LaserConfig>();
	pFactory->Register<PelletConfig>();
	pFactory->Register<BulletManager>();
	pFactory->Register<PulseManager>();
	pFactory->Register<LaserManager>();
	pFactory->Register<PelletManager>();	
	pFactory->Register<BulletManagerCollisionComponent>();
	pFactory->Register<LaserManagerCollisionComponent>();	
}


