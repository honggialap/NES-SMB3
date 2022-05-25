#pragma region INCLUDE
/* Game */
#include "SuperMarioBros3.h"


/* Controller */
#include "Title/Title.h"
#include "WorldMap/WorldMap.h"
#include "World/World.h"


/* Character */
#include "World/Mario.h"
#include "World/MarioTail.h"
#include "World/MarioFireball.h"
#include "World/Goomba.h"
#include "World/MicroGoomba.h"
#include "World/Koopa.h"
#include "World/KoopaSensor.h"
#include "World/Plant.h"
#include "World/PlantSensor.h"
#include "World/PlantFireball.h"


/* Item */
#include "World/Coin.h"
#include "World/SuperMushroom.h"
#include "World/FireFlower.h"
#include "World/SuperLeaf.h"
#include "World/Starman.h"
#include "World/ExtraLifeMushroom.h"


/* Prop */
#include "World/Platform.h"
#include "World/Block.h"
#include "World/Brick.h"
#include "World/Relay.h"
#include "World/Pipe.h"
#include "World/TransportPipe.h"
#include "World/SpawnZone.h"
#include "World/DeadZone.h"
#include "World/Goal.h"


/* Misc */
#include "World/Background.h"
#include "World/HUD.h"
#include "World/PowerBar.h"
#include "World/ScoreEffect.h"
#include "World/BrickFragEffect.h"
#include "World/HitEffect.h"
#include "World/DeathEffect.h"


/* Title */
/* World Map */
#pragma endregion

pGameObject CSuperMarioBros3::Create(pScene scene, unsigned int actor, std::string name, std::string source, float x, float y, int gx, int gy, unsigned int layer)
{
	switch (actor)
	{

	// Title
	case ACT_TITLE:
	{
		auto gameObject = new CTitle(
			this, scene, nextId++,
			name, source,
			x, y, gx, gy, layer
		);
		Add(gameObject);
		gameObject->Load();
		return gameObject;
	}
	break;

	// World Map
	case ACT_WORLDMAP:
	{
		auto gameObject = new CWorldMap(
			this, scene, nextId++,
			name, source,
			x, y, gx, gy, layer
		);
		Add(gameObject);
		gameObject->Load();
		return gameObject;
	}
	break;

	// World
	case ACT_WORLD:
	{
		auto gameObject = new CWorld(
			this, scene, nextId++,
			name, source,
			x, y, gx, gy, layer
		);
		Add(gameObject);
		gameObject->Load();
		return gameObject;
	}
	break;


	/* Character */
	// Mario
	case ACT_MARIO:
	{
		auto gameObject = new CMario(
			this, scene, nextId++,
			name, source,
			x, y, gx, gy, layer
		);
		Add(gameObject);
		gameObject->Load();
		return gameObject;
	}
	break;

	case ACT_MARIO_TAIL:
	{
		auto gameObject = new CMarioTail(
			this, scene, nextId++,
			name, source,
			x, y, gx, gy, layer
		);
		Add(gameObject);
		gameObject->Load();
		return gameObject;
	}
	break;

	case ACT_MARIO_FIREBALL:
	{
		auto gameObject = new CMarioFireball(
			this, scene, nextId++,
			name, source,
			x, y, gx, gy, layer
		);
		Add(gameObject);
		gameObject->Load();
		return gameObject;
	}
	break;

	// Goomba
	case ACT_GOOMBA:
	{
		auto gameObject = new CGoomba(
			this, scene, nextId++,
			name, source,
			x, y, gx, gy, layer
		);
		Add(gameObject);
		gameObject->Load();
		return gameObject;
	}
	break;

	case ACT_GOOMBA_MICRO:
	{
		auto gameObject = new CMicroGoomba(
			this, scene, nextId++,
			name, source,
			x, y, gx, gy, layer
		);
		Add(gameObject);
		gameObject->Load();
		return gameObject;
	}
	break;

	// Koopa
	case ACT_KOOPA:
	{
		auto gameObject = new CKoopa(
			this, scene, nextId++,
			name, source,
			x, y, gx, gy, layer
		);
		Add(gameObject);
		gameObject->Load();
		return gameObject;
	}
	break;

	case ACT_KOOPA_SENSOR:
	{
		auto gameObject = new CKoopaSensor(
			this, scene, nextId++,
			name, source,
			x, y, gx, gy, layer
		);
		Add(gameObject);
		gameObject->Load();
		return gameObject;
	}
	break;

	// Plant
	case ACT_PLANT:
	{
		auto gameObject = new CPlant(
			this, scene, nextId++,
			name, source,
			x, y, gx, gy, layer
		);
		Add(gameObject);
		gameObject->Load();
		return gameObject;
	}
	break;

	case ACT_PLANT_SENSOR:
	{
		auto gameObject = new CPlantSensor(
			this, scene, nextId++,
			name, source,
			x, y, gx, gy, layer
		);
		Add(gameObject);
		gameObject->Load();
		return gameObject;
	}
	break;

	case ACT_PLANT_FIREBALL:
	{
		auto gameObject = new CPlantFireball(
			this, scene, nextId++,
			name, source,
			x, y, gx, gy, layer
		);
		Add(gameObject);
		gameObject->Load();
		return gameObject;
	}
	break;


	/* Item */
	// Coin
	case ACT_COIN:
	{
		auto gameObject = new CCoin(
			this, scene, nextId++,
			name, source,
			x, y, gx, gy, layer
		);
		Add(gameObject);
		gameObject->Load();
		return gameObject;
	}
	break;

	// Super Mushroom
	case ACT_SUPER_MUSHROOM:
	{
		auto gameObject = new CSuperMushroom(
			this, scene, nextId++,
			name, source,
			x, y, gx, gy, layer
		);
		Add(gameObject);
		gameObject->Load();
		return gameObject;
	}
	break;

	// Fire Flower
	case ACT_FIRE_FLOWER:
	{
		auto gameObject = new CFireFlower(
			this, scene, nextId++,
			name, source,
			x, y, gx, gy, layer
		);
		Add(gameObject);
		gameObject->Load();
		return gameObject;
	}
	break;

	// Super Leaf
	case ACT_SUPER_LEAF:
	{
		auto gameObject = new CSuperLeaf(
			this, scene, nextId++,
			name, source,
			x, y, gx, gy, layer
		);
		Add(gameObject);
		gameObject->Load();
		return gameObject;
	}
	break;

	// Starman
	case ACT_STARMAN:
	{
		auto gameObject = new CStarman(
			this, scene, nextId++,
			name, source,
			x, y, gx, gy, layer
		);
		Add(gameObject);
		gameObject->Load();
		return gameObject;
	}
	break;

	// Extra Life Mushroom
	case ACT_EXTRA_LIFE_MUSHROOM:
	{
		auto gameObject = new CExtraLifeMushroom(
			this, scene, nextId++,
			name, source,
			x, y, gx, gy, layer
		);
		Add(gameObject);
		gameObject->Load();
		return gameObject;
	}
	break;


	/* Prop */
	// Block
	case ACT_BLOCK:
	{
		auto gameObject = new CBlock(
			this, scene, nextId++,
			name, source,
			x, y, gx, gy, layer
		);
		Add(gameObject);
		gameObject->Load();
		return gameObject;
	}
	break;

	// Brick
	case ACT_BRICK:
	{
		auto gameObject = new CBrick(
			this, scene, nextId++,
			name, source,
			x, y, gx, gy, layer
		);
		Add(gameObject);
		gameObject->Load();
		return gameObject;
	}
	break;

	// Platform
	case ACT_PLATFORM:
	{
		auto gameObject = new CPlatform(
			this, scene, nextId++,
			name, source,
			x, y, gx, gy, layer
		);
		Add(gameObject);
		gameObject->Load();
		return gameObject;
	}
	break;

	// Relay
	case ACT_RELAY:
	{
		auto gameObject = new CRelay(
			this, scene, nextId++,
			name, source,
			x, y, gx, gy, layer
		);
		Add(gameObject);
		gameObject->Load();
		return gameObject;
	}
	break;

	// Pipe
	case ACT_PIPE:
	{
		auto gameObject = new CPipe(
			this, scene, nextId++,
			name, source,
			x, y, gx, gy, layer
		);
		Add(gameObject);
		gameObject->Load();
		return gameObject;
	}
	break;

	case ACT_TRANSPORT_PIPE:
	{
		auto gameObject = new CTransportPipe(
			this, scene, nextId++,
			name, source,
			x, y, gx, gy, layer
		);
		Add(gameObject);
		gameObject->Load();
		return gameObject;
	}
	break;

	// Spawm Zone
	case ACT_SPAWN_ZONE:
	{
		auto gameObject = new CSpawnZone(
			this, scene, nextId++,
			name, source,
			x, y, gx, gy, layer
		);
		Add(gameObject);
		gameObject->Load();
		return gameObject;
	}
	break;

	// Dead Zone
	case ACT_DEAD_ZONE:
	{
		auto gameObject = new CDeadZone(
			this, scene, nextId++,
			name, source,
			x, y, gx, gy, layer
		);
		Add(gameObject);
		gameObject->Load();
		return gameObject;
	}
	break;

	// Goal
	case ACT_GOAL:
	{
		auto gameObject = new CGoal(
			this, scene, nextId++,
			name, source,
			x, y, gx, gy, layer
		);
		Add(gameObject);
		gameObject->Load();
		return gameObject;
	}
	break;


	/* Misc */
	// Background
	case ACT_BACKGROUND:
	{
		auto gameObject = new CBackground(
			this, scene, nextId++,
			name, source,
			x, y, gx, gy, layer
		);
		Add(gameObject);
		gameObject->Load();
		return gameObject;
	}
	break;

	// HUD
	case ACT_HUD:
	{
		auto gameObject = new CHUD(
			this, scene, nextId++,
			name, source,
			x, y, gx, gy, layer
		);
		Add(gameObject);
		gameObject->Load();
		return gameObject;
	}
	break;

	case ACT_POWER_BAR:
	{
		auto gameObject = new CPowerBar(
			this, scene, nextId++,
			name, source,
			x, y, gx, gy, layer
		);
		Add(gameObject);
		gameObject->Load();
		return gameObject;
	}
	break;

	// Score Effect
	case ACT_SCORE_EFFECT:
	{
		auto gameObject = new CScoreEffect(
			this, scene, nextId++,
			name, source,
			x, y, gx, gy, layer
		);
		Add(gameObject);
		gameObject->Load();
		return gameObject;
	}
	break;

	// Brick Frag Effect
	case ACT_BRICK_FRAG_EFFECT:
	{
		auto gameObject = new CBrickFragEffect(
			this, scene, nextId++,
			name, source,
			x, y, gx, gy, layer
		);
		Add(gameObject);
		gameObject->Load();
		return gameObject;
	}
	break;

	// Hit Effect
	case ACT_HIT_EFFECT:
	{
		auto gameObject = new CHitEffect(
			this, scene, nextId++,
			name, source,
			x, y, gx, gy, layer
		);
		Add(gameObject);
		gameObject->Load();
		return gameObject;
	}
	break;

	// Death Effect
	case ACT_DEATH_EFFECT:
	{
		auto gameObject = new CDeathEffect(
			this, scene, nextId++,
			name, source,
			x, y, gx, gy, layer
		);
		Add(gameObject);
		gameObject->Load();
		return gameObject;
	}
	break;

	default:
		return nullptr;
		break;
	}
}
