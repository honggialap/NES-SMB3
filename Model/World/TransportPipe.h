#pragma once
#ifndef __TRANSPORT_PIPE_H__
#define __TRANSPORT_PIPE_H__

#pragma region INCLUDE
#include "../Engine/GameObject.h"
#pragma endregion

#pragma region COMPONENT ID
#pragma endregion

class CTransportPipe : public CGameObject {
public:
	CTransportPipe(
		pGame game, 
		unsigned int id, std::string name, std::string source,
		float x, float y, int gx, int gy, unsigned int layer
	) : CGameObject(game, id, name, source, x, y, gx, gy, layer) {};

public:
	virtual void Load();
	virtual void Start();
	virtual void Update(float elapsedMs);
	virtual void Render();

	virtual int IsCollidable();
	virtual int IsBlocking();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
typedef CTransportPipe* pTransportPipe;

#endif // !__TRANSPORT_PIPE_H__
