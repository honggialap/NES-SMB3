#pragma once
#ifndef __SUPER_MARIO_BROS_3_H__
#define __SUPER_MARIO_BROS_3_H__

#pragma region INCLUDE
#include "Engine/Game.h"
#pragma endregion

#pragma region ACTOR ID
/* Controller */
#define ACT_TITLE				10101
#define ACT_WORLDMAP			10201
#define ACT_WORLD				10301

/* Character */
#define ACT_MARIO				20101
#define ACT_MARIO_TAIL			20102
#define ACT_MARIO_FIREBALL		20103
#define ACT_GOOMBA				20201
#define ACT_GOOMBA_MICRO		20202
#define ACT_KOOPA				20301
#define ACT_KOOPA_SENSOR		20302
#define ACT_PLANT				20401
#define ACT_PLANT_SENSOR		20402
#define ACT_PLANT_FIREBALL		20403

/* Item */
#define ACT_COIN				30101
#define ACT_SUPER_MUSHROOM		30201
#define ACT_FIRE_FLOWER			30301
#define ACT_SUPER_LEAF			30401
#define ACT_STARMAN				30501
#define ACT_EXTRA_LIFE_MUSHROOM	30601


/* Prop */
#define ACT_PLATFORM			40101
#define ACT_BLOCK				40201
#define ACT_BRICK				40301
#define ACT_PIPE				40401
#define ACT_TRANSPORT_PIPE		40402
#define ACT_RELAY				40501
#define ACT_SPAWN_ZONE			40601
#define ACT_DEAD_ZONE			40701
#define ACT_GOAL				40801


/* Misc */
#define ACT_BACKGROUND			50101

#define ACT_HUD					60101

#define ACT_SCORE_EFFECT		70101
#define ACT_BRICK_FRAG_EFFECT	70201
#define ACT_HIT_EFFECT			70301
#define ACT_DEATH_EFFECT		70401

/* Title */
/* World Map */
#pragma endregion

class CSuperMarioBros3 : public CGame
{
public:
	pGameObject Create(
		unsigned int actor, std::string name, std::string source,
		float posX, float posY, int gridX, int gridY,
		unsigned int layer
	);
};
typedef CSuperMarioBros3* pSuperMarioBros3;

#endif // !__SUPER_MARIO_BROS_3_H__
