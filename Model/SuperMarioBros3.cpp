#pragma region INCLUDE
/* Game */
#include "SuperMarioBros3.h"

/* Controller */
#include "Title.h"
#include "WorldMap.h"
#include "World.h"

/* Character */
#include "Mario.h"
#include "MarioTail.h"
#include "MarioFireball.h"
#include "Goomba.h"
#include "Koopa.h"
#include "KoopaSensor.h"
#include "Plant.h"
#include "PlantSensor.h"
#include "PlantFireball.h"

/* Item */
#include "Coin.h"
#include "SuperMushroom.h"
#include "FireFlower.h"
#include "SuperLeaf.h"
#include "ExtraLifeMushroom.h"

/* Prop */
#include "Platform.h"
#include "Block.h"
#include "Brick.h"
#include "Relay.h"
#include "Pipe.h"
#include "TransportPipe.h"
#include "SpawnZone.h"
#include "DeadZone.h"
#include "Goal.h"

/* Misc */
#include "Background.h"
#include "HUD.h"

#include "ScoreEffect.h"
#include "BrickFragEffect.h"
#include "HitEffect.h"
#include "DeathEffect.h"

#pragma endregion

pGameObject CSuperMarioBros3::Create(unsigned int actor, std::string name, std::string source, float x, float y, int gx, int gy, unsigned int layer)
{
	switch (actor)
	{

	// Title
	case ACT_TITLE:
	{
		auto gameObject = new CTitle(
			this, _nextGameObjectID++,
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
			this, _nextGameObjectID++,
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
			this, _nextGameObjectID++,
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
			this, _nextGameObjectID++,
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
			this, _nextGameObjectID++,
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
			this, _nextGameObjectID++,
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
			this, _nextGameObjectID++,
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
			this, _nextGameObjectID++,
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
			this, _nextGameObjectID++,
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
			this, _nextGameObjectID++,
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
			this, _nextGameObjectID++,
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
			this, _nextGameObjectID++,
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
			this, _nextGameObjectID++,
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
			this, _nextGameObjectID++,
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
			this, _nextGameObjectID++,
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
			this, _nextGameObjectID++,
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
			this, _nextGameObjectID++,
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
			this, _nextGameObjectID++,
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
			this, _nextGameObjectID++,
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
			this, _nextGameObjectID++,
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
			this, _nextGameObjectID++,
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
			this, _nextGameObjectID++,
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
			this, _nextGameObjectID++,
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
			this, _nextGameObjectID++,
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
			this, _nextGameObjectID++,
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
			this, _nextGameObjectID++,
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
			this, _nextGameObjectID++,
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
			this, _nextGameObjectID++,
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
			this, _nextGameObjectID++,
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
			this, _nextGameObjectID++,
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
			this, _nextGameObjectID++,
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
			this, _nextGameObjectID++,
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
