#pragma once
#ifndef __RELAY_H__
#define __RELAY_H__

#pragma region INCLUDE
#include "../Engine/GameObject.h"
#pragma endregion

#pragma region DEFINE
/* SPRITE ID */
#define SPR_BBOX				999901

#define SPR_RELAY1				100101
#define SPR_RELAY2				100102
#define SPR_RELAY_TRIGGERED		100201

/* ANIMATION ID */
#define ANI_RELAY				1001

#pragma endregion

class CRelay : public CGameObject
{
public:
	CRelay(
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

	/* Tranform */
	float SPAWN_OFFSETY = 0;
	float TRANSFORM_TIMEOUT = 0;
	float _transformTimeout = 0;
	bool _timeOut = false;
	bool _triggered = false;

#pragma endregion

#pragma region STATE MACHINE

	enum class EAction
	{
		SPAWN,
		IDLE,
		TRIGGERED
	};
	EAction _action = EAction::SPAWN;
	EAction _nextAction = EAction::SPAWN;

	enum class EActionStage
	{
		ENTRY,
		PROGRESS,
		EXIT
	};
	EActionStage _actionStage = EActionStage::ENTRY;

	void SetAction(EAction action)
	{
		_action = action;
		_actionStage = EActionStage::ENTRY;
	}
	void SetNextAction(EAction action)
	{
		_nextAction = action;
		_actionStage = EActionStage::EXIT;
	}
	void NextAction()
	{
		_action = _nextAction;
		_actionStage = EActionStage::ENTRY;
	}

	void HandleAction(float elapsedMs);
	void Spawn(float elapsedMs);
	void Idle(float elapsedMs);
	void Triggered(float elapsedMs);

	void Transform();

#pragma endregion

#pragma region COLLISION

	virtual int IsCollidable();
	virtual int IsBlocking();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

#pragma endregion
};
typedef CRelay* pRelay;

#endif // !__RELAY_H__
