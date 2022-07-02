#pragma once
#ifndef __WORLDMAP_H__
#define __WORLDMAP_H__

#pragma region INCLUDE
#include "Engine/GameObject.h"
#include <map>
#include <tuple>
#pragma endregion

#pragma region DEFINE
/* SPRITE ID */
#define SPR_WORLDMAP1_BACKGROUND	100001

#define SPR_WORLDMAP_MARIO_SMALL1	200001
#define SPR_WORLDMAP_MARIO_SMALL2	200002

#define SPR_WORLDMAP_MARIO_LARGE1	300001
#define SPR_WORLDMAP_MARIO_LARGE2	300002

#define SPR_WORLDMAP_MARIO_FIRE1	400001
#define SPR_WORLDMAP_MARIO_FIRE2	400002

#define SPR_WORLDMAP_MARIO_RACOON1	500001
#define SPR_WORLDMAP_MARIO_RACOON2	500002

#define SPR_WORLDMAP_KACTUS1		600001
#define SPR_WORLDMAP_KACTUS2		600002
#define SPR_WORLDMAP_KACTUS3		600003

/* ANIMATION ID */
#define ANI_MARIO_SMALL				2000
#define ANI_MARIO_LARGE				3000
#define ANI_MARIO_FIRE				4000
#define ANI_MARIO_RACOON			5000
#define ANI_KACTUS					6000

/* SOUNDCLIP ID */
#pragma endregion

class CWorldMap : public CGameObject
{
public:
	CWorldMap(
		pGame game,
		unsigned int id, std::string name, std::string source,
		float x, float y, int gx, int gy, unsigned int layer
	) : CGameObject(game, id, name, source, x, y, gx, gy, layer) {};

	// Command
	unsigned int LEFT = 0;
	unsigned int UP = 0;
	unsigned int RIGHT = 0;
	unsigned int DOWN = 0;
	unsigned int SELECT = 0;

	// Camera origin
	float CAMERA_ORIGIN_X = 0.0;
	float CAMERA_ORIGIN_Y = 0.0;

	// Background position
	float _backgroundX = 0.0;
	float _backgroundY = 0.0;

	// Kactus position vector
	std::vector<
		std::pair<
		float,	// positionX
		float	// positionY
		>
	> _kactus;

	// Nodes vector
	std::map<
		unsigned int,	// node ID
		std::tuple<
		float,	// positionX
		float,	// positionY
		unsigned int,	// left node
		unsigned int,	// top node
		unsigned int,	// right node
		unsigned int,	// bottom node
		bool,	// gate flag
		unsigned int	// nextScene
		>
	> _nodes;

	unsigned int _startNode = 0;

	unsigned int _currentNode = 0;
	float _currentX = 0.0;
	float _currentY = 0.0;

	unsigned int _targetNode = 0;
	float _targetX = 0.0;
	float _targetY = 0.0;

	float MOVE_SPEED = 0.0;

	enum class EMarioPower {
		SMALL,
		LARGE,
		FIRE,
		RACOON
	};
	EMarioPower _marioPower = EMarioPower::SMALL;

	enum class EState {
		IDLE,
		MOVE,
	};
	EState _state = EState::IDLE;
	void SetState(EState state);

public:
	virtual void Load();
	virtual void Start();
	virtual void Update(float elapsedMs);
	virtual void Render();

	virtual int IsCollidable();
	virtual int IsBlocking();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
typedef CWorldMap* pWorldMap;

#endif // !__WORLDMAP_H__
