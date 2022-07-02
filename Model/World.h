#pragma once
#ifndef __WORLD_H__
#define __WORLD_H__

#pragma region INCLUDE
#include "Engine/GameObject.h"
#include <map>
#include <tuple>
#pragma endregion

#pragma region DEFINE
/* SPRITE ID */
/* ANIMATION ID */
/* SOUNDCLIP ID */
#pragma endregion

class CWorld : public CGameObject
{
public:
	CWorld(
		pGame game,
		unsigned int id, std::string name, std::string source,
		float x, float y, int gx, int gy, unsigned int layer
	) : CGameObject(game, id, name, source, x, y, gx, gy, layer) {};

	/* Logic */
	float _camBoundLeft = 0;
	float _camBoundTop = 0;
	float _camBoundRight = 0;
	float _camBoundBottom = 0;
	float _camTrackWidth = 0;
	float _camTrackHeight = 0;
	float _camTrackOffsetX = 0;
	float _camTrackOffsetY = 0;
	pGameObject _camTarget = nullptr;

	void SetCamera(unsigned int cameraID);

	unsigned int WORLD_MAP_ID = 0;

public:
	void BackToWorldMap();

	virtual void Load();
	virtual void Start();
	virtual void Update(float elapsedMs);
	virtual void Render();

	virtual int IsCollidable();
	virtual int IsBlocking();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
typedef CWorld* pWorld;

#endif // !__WORLD_H__
