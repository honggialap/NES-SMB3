#pragma once
#ifndef __TRANSPORT_PIPE_H__
#define __TRANSPORT_PIPE_H__

#pragma region INCLUDE
#include "../Engine/GameObject.h"
#pragma endregion

#pragma region DEFINE
/* SPRITE ID */
#define SPR_BBOX				999901

#pragma endregion

class CTransportPipe : public CGameObject
{
public:
	CTransportPipe(
		pGame game, pScene scene,
		unsigned int id, std::string name, std::string source,
		float x, float y, int gx, int gy, unsigned int layer
	) : CGameObject(game, scene, id, name, source, x, y, gx, gy, layer) {};

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
		UP,
		DOWN
	};
	EType _type = EType::UP;
	float DESTINATION_X = 0;
	float DESTINATION_Y = 0;
	int CAMERA_SWITCH = 0;

#pragma endregion

#pragma region COLLISION

	virtual int IsCollidable();
	virtual int IsBlocking();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void OnNoCollision(float elapsedMs);
	void OnCollisionWith(pCollision collision);

#pragma endregion
};
typedef CTransportPipe* pTransportPipe;

#endif // !__TRANSPORT_PIPE_H__
