#pragma once
#ifndef __GOAL_H__
#define __GOAL_H__

#pragma region INCLUDE
#include "Engine/GameObject.h"
#pragma endregion

#pragma region DEFINE
/* SPRITE ID */
#define SPR_BBOX				999901

#define SPR_GOAL_FRAME			100001

#define SPR_MUSHROOM_SPIN1		100002
#define SPR_MUSHROOM_SPIN2		100003

#define SPR_FIRE_SPIN1			100004
#define SPR_FIRE_SPIN2			100005

#define SPR_STAR_SPIN1			100006
#define SPR_STAR_SPIN2			100007

#define SPR_END_SPIN1			100008
#define SPR_END_SPIN2			100009
#define SPR_END_SPIN3			100010

#define SPR_GOAL_ANNOUNCE		200001
#define SPR_MUSHROOM_ANNOUNCE	200002
#define SPR_FIRE_ANNOUNCE		200003
#define SPR_STAR_ANNOUNCE		200004

/* ANIMATION ID */
#define ANI_SPIN_IDLE			1001
#define ANI_SPIN_END			1002

/* SOUNDCLIP ID */
#pragma endregion

class CGoal : public CGameObject
{
public:
	CGoal(
		pGame game,
		unsigned int id, std::string name, std::string source,
		float x, float y, int gx, int gy, unsigned int layer
	) : CGameObject(game, id, name, source, x, y, gx, gy, layer) {};

	/* Body */
	bool _renderBody = false;
	float BODY_WIDTH = 0;
	float BODY_HEIGHT = 0;
	float BODY_OFFSETX = 0;
	float BODY_OFFSETY = 0;

	float _renderX = 0.0f;
	float _renderY = 0.0f;

	float ANNOUNCE_OFFSETX = 0.0f;
	float ANNOUNCE_OFFSETY = 0.0f;

	float _acquireCoundown = 0.0f;
	float ACQUIRE_COUNTDOWN = 0.0f;
	float POP_UP_SPEED = 0.0f;

	float _announceCountdown = 0.0f;
	float ANNOUNCE_COUNTDOWN = 0.0f;

	std::string _controllerName;

	enum class EReward 
	{
		MUSHROOM,
		FIRE,
		STAR
	};
	EReward _reward = EReward::MUSHROOM;
	float NEXT_REWARD_COUNTDOWN = 0.0f;
	float _nextRewardCountdown = 0.0f;
	void ChangeReward();

	enum class EState 
	{
		IDLE,
		ACQUIRED,
		ANNOUNCE
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
typedef CGoal* pGoal;

#endif // !__GOAL_H__
