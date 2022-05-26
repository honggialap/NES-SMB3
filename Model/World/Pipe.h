#pragma once
#ifndef __PIPE_H__
#define __PIPE_H__

#pragma region INCLUDE
#include "../Engine/GameObject.h"
#pragma endregion

#pragma region DEFINE
/* SPRITE ID */
#define SPR_BBOX				999901

#define SPR_PIPE_GREEN_LEFT		100101
#define SPR_PIPE_GREEN_UP		100102
#define SPR_PIPE_GREEN_RIGHT	100103
#define SPR_PIPE_GREEN_DOWN		100104

#define SPR_PIPE_BLACK_LEFT		100201
#define SPR_PIPE_BLACK_UP		100202
#define SPR_PIPE_BLACK_RIGHT	100203
#define SPR_PIPE_BLACK_DOWN		100204

#pragma endregion

class CPipe : public CGameObject
{
public:
	CPipe(
		pGame game,
		unsigned int id, std::string name, std::string source,
		float x, float y, int gx, int gy, unsigned int layer
	) : CGameObject(game, id, name, source, x, y, gx, gy, layer) {};

public:
	virtual void Load();
	virtual void Start();
	virtual void Update(float elapsedMs);
	virtual void Render();

#pragma region LOGIC

	/* Body */
	bool _renderBody = false;
	float BODY_WIDTH = 0;
	float BODY_HEIGHT = 0;
	float BODY_OFFSETX = 0;
	float BODY_OFFSETY = 0;

	/* Type */
	enum class EType
	{
		GREEN,
		BLACK
	};
	EType _type = EType::GREEN;
	enum class EDirection
	{
		LEFT,
		UP,
		RIGHT,
		DOWN
	};
	EDirection _direction = EDirection::LEFT;

#pragma endregion

#pragma region COLLISION

	virtual int IsCollidable();
	virtual int IsBlocking();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void OnNoCollision(float elapsedMs);
	void OnCollisionWith(pCollision collision);

#pragma endregion

};
typedef CPipe* pPipe;

#endif // !__PIPE_H__
