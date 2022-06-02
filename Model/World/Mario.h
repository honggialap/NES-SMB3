#pragma once
#ifndef __MARIO_H__
#define __MARIO_H__

#pragma region INCLUDE
#include "../Engine/GameObject.h"

#include "MarioTail.h"
#include "MarioFireball.h"
#include "Koopa.h"
#include "TransportPipe.h"
#pragma endregion

#pragma region DEFINE
/* SPRITE ID */

// BBOX
#define SPR_MARIO_S_BBOX				999901
#define SPR_MARIO_L_BBOX				999902

// SMALL - LEFT
#define SPR_MARIO_S_IDLE_LEFT			110101
#define SPR_MARIO_S_WALK_LEFT			110201
#define SPR_MARIO_S_JUMP_LEFT			110301
#define SPR_MARIO_S_RUN1_LEFT			110401
#define SPR_MARIO_S_RUN2_LEFT			110402
#define SPR_MARIO_S_FLY_LEFT			110501
#define SPR_MARIO_S_DRIFT_LEFT			110601
#define SPR_MARIO_S_HOLD_IDLE_LEFT		110701
#define SPR_MARIO_S_HOLD_WALK_LEFT		110801
#define SPR_MARIO_S_KICK_LEFT			110901
#define SPR_MARIO_S_SLIDE_LEFT			111001
#define SPR_MARIO_S_SWIM1_LEFT			111101
#define SPR_MARIO_S_SWIM2_LEFT			111102
#define SPR_MARIO_S_SWIM3_LEFT			111103
#define SPR_MARIO_S_SWIM4_LEFT			111104
#define SPR_MARIO_S_CLIMB_LEFT			111201
// SMALL - RIGHT
#define SPR_MARIO_S_IDLE_RIGHT			120101
#define SPR_MARIO_S_WALK_RIGHT			120201
#define SPR_MARIO_S_JUMP_RIGHT			120301
#define SPR_MARIO_S_RUN1_RIGHT			120401
#define SPR_MARIO_S_RUN2_RIGHT			120402
#define SPR_MARIO_S_FLY_RIGHT			120501
#define SPR_MARIO_S_DRIFT_RIGHT			120601
#define SPR_MARIO_S_HOLD_IDLE_RIGHT		120701
#define SPR_MARIO_S_HOLD_WALK_RIGHT		120801
#define SPR_MARIO_S_KICK_RIGHT			120901
#define SPR_MARIO_S_SLIDE_RIGHT			121001
#define SPR_MARIO_S_SWIM1_RIGHT			121101
#define SPR_MARIO_S_SWIM2_RIGHT			121102
#define SPR_MARIO_S_SWIM3_RIGHT			121103
#define SPR_MARIO_S_SWIM4_RIGHT			121104
#define SPR_MARIO_S_CLIMB_RIGHT			121201
// SMALL - COMMON
#define SPR_MARIO_S_DIE					100101
#define SPR_MARIO_S_FRONT				100201

// LARGE - LEFT
#define SPR_MARIO_L_IDLE_LEFT			210101
#define SPR_MARIO_L_WALK1_LEFT			210201
#define SPR_MARIO_L_WALK2_LEFT			210202
#define SPR_MARIO_L_CROUNCH_LEFT		210301
#define SPR_MARIO_L_JUMP_LEFT			210401
#define SPR_MARIO_L_RUN1_LEFT			210501
#define SPR_MARIO_L_RUN2_LEFT			210502
#define SPR_MARIO_L_RUN3_LEFT			210503
#define SPR_MARIO_L_FLY_LEFT			210601
#define SPR_MARIO_L_DRIFT_LEFT			210701
#define SPR_MARIO_L_HOLD_IDLE_LEFT		210801
#define SPR_MARIO_L_HOLD_WALK1_LEFT		210901
#define SPR_MARIO_L_HOLD_WALK2_LEFT		210902
#define SPR_MARIO_L_KICK_LEFT			211001
#define SPR_MARIO_L_SLIDE_LEFT			211101
#define SPR_MARIO_L_SWIM1_LEFT			211201
#define SPR_MARIO_L_SWIM2_LEFT			211202
#define SPR_MARIO_L_SWIM3_LEFT			211203
#define SPR_MARIO_L_SWIM4_LEFT			211204
#define SPR_MARIO_L_CLIMB_LEFT			211301
#define SPR_MARIO_L_SPINJUMP1_LEFT		211401
#define SPR_MARIO_L_SPINJUMP2_LEFT		211402
#define SPR_MARIO_L_SPINJUMP3_LEFT		211403
#define SPR_MARIO_L_SPINJUMP4_LEFT		211404
#define SPR_MARIO_L_SPINJUMP5_LEFT		211405
#define SPR_MARIO_L_SPINJUMP6_LEFT		211406
#define SPR_MARIO_L_SPINJUMP7_LEFT		211407
#define SPR_MARIO_L_SPINJUMP8_LEFT		211408
#define SPR_MARIO_L_SHRINK_LEFT			211501
// LARGE - RIGHT
#define SPR_MARIO_L_IDLE_RIGHT			220101
#define SPR_MARIO_L_WALK1_RIGHT			220201
#define SPR_MARIO_L_WALK2_RIGHT			220202
#define SPR_MARIO_L_CROUNCH_RIGHT		220301
#define SPR_MARIO_L_JUMP_RIGHT			220401
#define SPR_MARIO_L_RUN1_RIGHT			220501
#define SPR_MARIO_L_RUN2_RIGHT			220502
#define SPR_MARIO_L_RUN3_RIGHT			220503
#define SPR_MARIO_L_FLY_RIGHT			220601
#define SPR_MARIO_L_DRIFT_RIGHT			220701
#define SPR_MARIO_L_HOLD_IDLE_RIGHT		220801
#define SPR_MARIO_L_HOLD_WALK1_RIGHT	220901
#define SPR_MARIO_L_HOLD_WALK2_RIGHT	220902
#define SPR_MARIO_L_KICK_RIGHT			221001
#define SPR_MARIO_L_SLIDE_RIGHT			221101
#define SPR_MARIO_L_SWIM1_RIGHT			221201
#define SPR_MARIO_L_SWIM2_RIGHT			221202
#define SPR_MARIO_L_SWIM3_RIGHT			221203
#define SPR_MARIO_L_SWIM4_RIGHT			221204
#define SPR_MARIO_L_CLIMB_RIGHT			221301
#define SPR_MARIO_L_SPINJUMP1_RIGHT		221401
#define SPR_MARIO_L_SPINJUMP2_RIGHT		221402
#define SPR_MARIO_L_SPINJUMP3_RIGHT		221403
#define SPR_MARIO_L_SPINJUMP4_RIGHT		221404
#define SPR_MARIO_L_SPINJUMP5_RIGHT		221405
#define SPR_MARIO_L_SPINJUMP6_RIGHT		221406
#define SPR_MARIO_L_SPINJUMP7_RIGHT		221407
#define SPR_MARIO_L_SPINJUMP8_RIGHT		221408
#define SPR_MARIO_L_SHRINK_RIGHT		221501
//LARGE - COMMON
#define SPR_MARIO_L_FRONT				200101
#define SPR_MARIO_L_BACK				200102
#define SPR_MARIO_L_DEFLECT				200201
#define SPR_MARIO_L_LOOKUP				200301

// FIRE - LEFT
#define SPR_MARIO_F_IDLE_LEFT			310101
#define SPR_MARIO_F_WALK1_LEFT			310201
#define SPR_MARIO_F_WALK2_LEFT			310202
#define SPR_MARIO_F_CROUNCH_LEFT		310301
#define SPR_MARIO_F_JUMP_LEFT			310401
#define SPR_MARIO_F_RUN1_LEFT			310501
#define SPR_MARIO_F_RUN2_LEFT			310502
#define SPR_MARIO_F_RUN3_LEFT			310503
#define SPR_MARIO_F_FLY_LEFT			310601
#define SPR_MARIO_F_DRIFT_LEFT			310701
#define SPR_MARIO_F_HOLD_IDLE_LEFT		310801
#define SPR_MARIO_F_HOLD_WALK1_LEFT		310901
#define SPR_MARIO_F_HOLD_WALK2_LEFT		310902
#define SPR_MARIO_F_KICK_LEFT			311001
#define SPR_MARIO_F_SLIDE_LEFT			311101
#define SPR_MARIO_F_SWIM1_LEFT			311201
#define SPR_MARIO_F_SWIM2_LEFT			311202
#define SPR_MARIO_F_SWIM3_LEFT			311203
#define SPR_MARIO_F_SWIM4_LEFT			311204
#define SPR_MARIO_F_CLIMB_LEFT			311301
#define SPR_MARIO_F_SPINJUMP1_LEFT		311401
#define SPR_MARIO_F_SPINJUMP2_LEFT		311402
#define SPR_MARIO_F_SPINJUMP3_LEFT		311403
#define SPR_MARIO_F_SPINJUMP4_LEFT		311404
#define SPR_MARIO_F_SPINJUMP5_LEFT		311405
#define SPR_MARIO_F_SPINJUMP6_LEFT		311406
#define SPR_MARIO_F_SPINJUMP7_LEFT		311407
#define SPR_MARIO_F_SPINJUMP8_LEFT		311408
#define SPR_MARIO_F_ATTACK1_LEFT		311501
#define SPR_MARIO_F_ATTACK2_LEFT		311502
// FIRE - RIGHT
#define SPR_MARIO_F_IDLE_RIGHT			320101
#define SPR_MARIO_F_WALK1_RIGHT			320201
#define SPR_MARIO_F_WALK2_RIGHT			320202
#define SPR_MARIO_F_CROUNCH_RIGHT		320301
#define SPR_MARIO_F_JUMP_RIGHT			320401
#define SPR_MARIO_F_RUN1_RIGHT			320501
#define SPR_MARIO_F_RUN2_RIGHT			320502
#define SPR_MARIO_F_RUN3_RIGHT			320503
#define SPR_MARIO_F_FLY_RIGHT			320601
#define SPR_MARIO_F_DRIFT_RIGHT			320701
#define SPR_MARIO_F_HOLD_IDLE_RIGHT		320801
#define SPR_MARIO_F_HOLD_WALK1_RIGHT	320901
#define SPR_MARIO_F_HOLD_WALK2_RIGHT	320902
#define SPR_MARIO_F_KICK_RIGHT			321001
#define SPR_MARIO_F_SLIDE_RIGHT			321101
#define SPR_MARIO_F_SWIM1_RIGHT			321201
#define SPR_MARIO_F_SWIM2_RIGHT			321202
#define SPR_MARIO_F_SWIM3_RIGHT			321203
#define SPR_MARIO_F_SWIM4_RIGHT			321204
#define SPR_MARIO_F_CLIMB_RIGHT			321301
#define SPR_MARIO_F_SPINJUMP1_RIGHT		321401
#define SPR_MARIO_F_SPINJUMP2_RIGHT		321402
#define SPR_MARIO_F_SPINJUMP3_RIGHT		321403
#define SPR_MARIO_F_SPINJUMP4_RIGHT		321404
#define SPR_MARIO_F_SPINJUMP5_RIGHT		321405
#define SPR_MARIO_F_SPINJUMP6_RIGHT		321406
#define SPR_MARIO_F_SPINJUMP7_RIGHT		321407
#define SPR_MARIO_F_SPINJUMP8_RIGHT		321408
#define SPR_MARIO_F_ATTACK1_RIGHT		321501
#define SPR_MARIO_F_ATTACK2_RIGHT		321502
//FIRE - COMMON
#define SPR_MARIO_F_FRONT				300101
#define SPR_MARIO_F_BACK				300102

// RACCOON - LEFT
#define SPR_MARIO_R_IDLE_LEFT			410101
#define SPR_MARIO_R_WALK1_LEFT			410201
#define SPR_MARIO_R_WALK2_LEFT			410202
#define SPR_MARIO_R_CROUNCH_LEFT		410301
#define SPR_MARIO_R_JUMP_LEFT			410401
#define SPR_MARIO_R_HOVER1_LEFT			410501
#define SPR_MARIO_R_HOVER2_LEFT			410502
#define SPR_MARIO_R_RUN1_LEFT			410601
#define SPR_MARIO_R_RUN2_LEFT			410602
#define SPR_MARIO_R_RUN3_LEFT			410603
#define SPR_MARIO_R_FLY1_LEFT			410701
#define SPR_MARIO_R_FLY2_LEFT			410702
#define SPR_MARIO_R_FLY3_LEFT			410703
#define SPR_MARIO_R_DRIFT_LEFT			410801
#define SPR_MARIO_R_HOLD_IDLE_LEFT		410901
#define SPR_MARIO_R_HOLD_WALK1_LEFT		411001
#define SPR_MARIO_R_HOLD_WALK2_LEFT		411002
#define SPR_MARIO_R_KICK_LEFT			411101
#define SPR_MARIO_R_SLIDE_LEFT			411201
#define SPR_MARIO_R_SWIM1_LEFT			411301
#define SPR_MARIO_R_SWIM2_LEFT			411302
#define SPR_MARIO_R_SWIM3_LEFT			411303
#define SPR_MARIO_R_SWIM4_LEFT			411304
#define SPR_MARIO_R_CLIMB_LEFT			411401
#define SPR_MARIO_R_SPINJUMP1_LEFT		411501
#define SPR_MARIO_R_SPINJUMP2_LEFT		411502
#define SPR_MARIO_R_SPINJUMP3_LEFT		411503
#define SPR_MARIO_R_SPINJUMP4_LEFT		411504
#define SPR_MARIO_R_SPINJUMP5_LEFT		411505
#define SPR_MARIO_R_SPINJUMP6_LEFT		411506
#define SPR_MARIO_R_SPINJUMP7_LEFT		411507
#define SPR_MARIO_R_SPINJUMP8_LEFT		411508
// RACCOON - RIGHT
#define SPR_MARIO_R_IDLE_RIGHT			420101
#define SPR_MARIO_R_WALK1_RIGHT			420201
#define SPR_MARIO_R_WALK2_RIGHT			420202
#define SPR_MARIO_R_CROUNCH_RIGHT		420301
#define SPR_MARIO_R_JUMP_RIGHT			420401
#define SPR_MARIO_R_HOVER1_RIGHT		420501
#define SPR_MARIO_R_HOVER2_RIGHT		420502
#define SPR_MARIO_R_RUN1_RIGHT			420601
#define SPR_MARIO_R_RUN2_RIGHT			420602
#define SPR_MARIO_R_RUN3_RIGHT			420603
#define SPR_MARIO_R_FLY1_RIGHT			420701
#define SPR_MARIO_R_FLY2_RIGHT			420702
#define SPR_MARIO_R_FLY3_RIGHT			420703
#define SPR_MARIO_R_DRIFT_RIGHT			420801
#define SPR_MARIO_R_HOLD_IDLE_RIGHT		420901
#define SPR_MARIO_R_HOLD_WALK1_RIGHT	421001
#define SPR_MARIO_R_HOLD_WALK2_RIGHT	421002
#define SPR_MARIO_R_KICK_RIGHT			421101
#define SPR_MARIO_R_SLIDE_RIGHT			421201
#define SPR_MARIO_R_SWIM1_RIGHT			421301
#define SPR_MARIO_R_SWIM2_RIGHT			421302
#define SPR_MARIO_R_SWIM3_RIGHT			421303
#define SPR_MARIO_R_SWIM4_RIGHT			421304
#define SPR_MARIO_R_CLIMB_RIGHT			421401
#define SPR_MARIO_R_SPINJUMP1_RIGHT		421501
#define SPR_MARIO_R_SPINJUMP2_RIGHT		421502
#define SPR_MARIO_R_SPINJUMP3_RIGHT		421503
#define SPR_MARIO_R_SPINJUMP4_RIGHT		421504
#define SPR_MARIO_R_SPINJUMP5_RIGHT		421505
#define SPR_MARIO_R_SPINJUMP6_RIGHT		421506
#define SPR_MARIO_R_SPINJUMP7_RIGHT		421507
#define SPR_MARIO_R_SPINJUMP8_RIGHT		421508
// RACCOON - COMMON
#define SPR_MARIO_R_FRONT				400101
#define SPR_MARIO_R_LEFT				400102
#define SPR_MARIO_R_BACK				400103
#define SPR_MARIO_R_RIGHT				400104

// TRANFORMATION - LEFT
#define SPR_MARIO_TRANSFORM1_LEFT		510101
#define SPR_MARIO_TRANSFORM2_LEFT		510102
#define SPR_MARIO_TRANSFORM3_LEFT		510103
#define SPR_MARIO_TRANSFORM4_LEFT		510104
// TRANFORMATION - RIGHT
#define SPR_MARIO_TRANSFORM1_RIGHT		520101
#define SPR_MARIO_TRANSFORM2_RIGHT		520102
#define SPR_MARIO_TRANSFORM3_RIGHT		520103
#define SPR_MARIO_TRANSFORM4_RIGHT		520104

/* ANIMATION ID - {state - direction - action} */
// SMALL - LEFT
#define ANI_MARIO_S_WALK_LEFT			1101
#define ANI_MARIO_S_RUN_LEFT			1102
#define ANI_MARIO_S_HOLD_WALK_LEFT		1103
#define ANI_MARIO_S_SWIM_LEFT			1104
// SMALL - RIGHT
#define ANI_MARIO_S_WALK_RIGHT			1201
#define ANI_MARIO_S_RUN_RIGHT			1202
#define ANI_MARIO_S_HOLD_WALK_RIGHT		1203
#define ANI_MARIO_S_SWIM_RIGHT			1204

// LARGE - LEFT
#define ANI_MARIO_L_WALK_LEFT			2101
#define ANI_MARIO_L_RUN_LEFT			2102
#define ANI_MARIO_L_HOLD_WALK_LEFT		2103
#define ANI_MARIO_L_SWIM_LEFT			2104
#define ANI_MARIO_L_SPINJUMP_LEFT		2105
// LARGE - RIGHT
#define ANI_MARIO_L_WALK_RIGHT			2201
#define ANI_MARIO_L_RUN_RIGHT			2202
#define ANI_MARIO_L_HOLD_WALK_RIGHT		2203
#define ANI_MARIO_L_SWIM_RIGHT			2204
#define ANI_MARIO_L_SPINJUMP_RIGHT		2205

// FIRE - LEFT
#define ANI_MARIO_F_WALK_LEFT			3101
#define ANI_MARIO_F_RUN_LEFT			3102
#define ANI_MARIO_F_HOLD_WALK_LEFT		3103
#define ANI_MARIO_F_SWIM_LEFT			3104
#define ANI_MARIO_F_SPINJUMP_LEFT		3105
#define ANI_MARIO_F_GROUND_ATTACK_LEFT	3106
#define ANI_MARIO_F_AIR_ATTACK_LEFT		3107
// FIRE - RIGHT
#define ANI_MARIO_F_WALK_RIGHT			3201
#define ANI_MARIO_F_RUN_RIGHT			3202
#define ANI_MARIO_F_HOLD_WALK_RIGHT		3203
#define ANI_MARIO_F_SWIM_RIGHT			3204
#define ANI_MARIO_F_SPINJUMP_RIGHT		3205
#define ANI_MARIO_F_GROUND_ATTACK_RIGHT	3206
#define ANI_MARIO_F_AIR_ATTACK_RIGHT	3207

// RACCOON - LEFT
#define ANI_MARIO_R_WALK_LEFT			4101
#define ANI_MARIO_R_HOVER_LEFT			4102
#define ANI_MARIO_R_RUN_LEFT			4103
#define ANI_MARIO_R_FLY_LEFT			4104
#define ANI_MARIO_R_HOLD_WALK_LEFT		4105
#define ANI_MARIO_R_SWIM_LEFT			4106
#define ANI_MARIO_R_SPINJUMP_LEFT		4107
#define ANI_MARIO_R_ATTACK_LEFT			4108
// RACCOON - RIGHT
#define ANI_MARIO_R_WALK_RIGHT			4201
#define ANI_MARIO_R_HOVER_RIGHT			4202
#define ANI_MARIO_R_RUN_RIGHT			4203
#define ANI_MARIO_R_FLY_RIGHT			4204
#define ANI_MARIO_R_HOLD_WALK_RIGHT		4205
#define ANI_MARIO_R_SWIM_RIGHT			4206
#define ANI_MARIO_R_SPINJUMP_RIGHT		4207
#define ANI_MARIO_R_ATTACK_RIGHT		4208

// TRANSFORMATION - LEFT
#define ANI_MARIO_SHRINK_LEFT			5101
#define ANI_MARIO_GROW_LEFT				5102
#define ANI_MARIO_TRANSFORM_LEFT		5103
// TRANSFORMATION - RIGHT
#define ANI_MARIO_SHRINK_RIGHT			5201
#define ANI_MARIO_GROW_RIGHT			5202
#define ANI_MARIO_TRANSFORM_RIGHT		5203

#pragma endregion

class CMario : public CGameObject
{
public:
	CMario(
		pGame game,
		unsigned int id, std::string name, std::string source,
		float x, float y, int gx, int gy, unsigned int layer
	) : CGameObject(game, id, name, source, x, y, gx, gy, layer) {};

public:
	virtual void Load();
	virtual void Start();
	virtual void Update(float elapsedMs);
	virtual void Render();

	/* Command */
	int LEFT = 0;
	int UP = 0;
	int RIGHT = 0;
	int DOWN = 0;
	int ACTION = 0;
	int JUMP = 0;

	/* CHEAT */
	int HOT1 = 0;
	int HOT2 = 0;
	int HOT3 = 0;
	int HOT4 = 0;

	/* Small Body */
	bool _renderSmallBody = false;
	float SMALL_BODY_WIDTH = 0;
	float SMALL_BODY_HEIGHT = 0;
	float SMALL_BODY_OFFSETX = 0;
	float SMALL_BODY_OFFSETY = 0;

	/* Large Body */
	bool _renderLargeBody = false;
	float LARGE_BODY_WIDTH = 0;
	float LARGE_BODY_HEIGHT = 0;
	float LARGE_BODY_OFFSETX = 0;
	float LARGE_BODY_OFFSETY = 0;

	/* Gravity */
	bool _left = false;
	bool _ground = false;
	float GRAVITY = 0.0f;
	float GROUND_DETECT_FACTOR = 0.0f;
	
	/* Move */
	bool _fullspeed = false;
	float _momentum = 0.0f;
	float DRAG = 0.0f;
	float IDLE_THRESHOLD = 0.0f;
	float WALK_ACCELERATION = 0.0f;
	float WALK_SPEED_LIMIT = 0.0f;
	float RUN_ACCELERATION = 0.0f;
	float RUN_SPEED_LIMIT = 0.0f;
	float FULLSPEED_ACCELERATION = 0.0f;
	float FULLSPEED_SPEED_LIMIT = 0.0f;
	float MOMENTUM_INCREASE_RATE = 0.0f;
	float MOMENTUM_DECREASE_RATE = 0.0f;
	float MOMENTUM_LIMIT = 0.0f;
	float MOMENTUM_THRESHOLD = 0.0f;
	float WALK_ANIMATION_SPEED_FACTOR = 0.0f;
	float DRIFT_THRESHOLD = 0.0f;

	/* Jump */
	float _jumpLimit = 0.0f;
	bool _fall = false;
	float JUMP_FORCE = 0.0f;
	float FULLSPEED_JUMP_FORCE = 0.0f;
	float JUMP_LIMIT = 0.0f;
	float JUMP_ACCELERATION = 0.0f;

	/* Hover */
	bool _hover = false;
	float _hoverCountdown = 0.0f;
	float HOVER_COUNTDOWN = 0.0f;
	float HOVER_GRAVITY = 0.0f;

	/* Fly */
	bool _fly = false;
	float _flyCountdown = 0.0f;
	bool _flying = false;
	float _flyTimeout = 0.0f;
	float FLY_COUNTDOWN = 0.0f;
	float FLY_GRAVITY = 0.0f;
	float FLY_TIMEOUT_LIMIT = 0.0f;

	/* Transformation */
	float _transformationTimeout = 0.0f;
	float TRANSFORMATION_TIMEOUT = 0.0f;

	/* Invincible */
	bool _invincible = false;
	bool _blink = false;
	float _blinkInterval = 0;
	float _invincibleTimeout = 0;
	float INVINCIBLE_TIMEOUT = 0;
	float BLINK_INTERVAL = 0;

	/* Tail */
	pMarioTail _tail = nullptr;
	float _tailEntry = 0.0f;
	float _tailProgress = 0.0f;
	float _tailRecover = 0.0f;
	float TAIL_ENTRY = 0.0f;
	float TAIL_PROGRESS = 0.0f;
	float TAIL_RECOVER = 0.0f;

	/* Fire */
	bool _shot = false;
	float _fireEntry = 0.0f;
	float _fireProgress = 0.0f;
	float _fireRecover = 0.0f;
	float FIRE_RECOVER = 0.0f;
	float FIRE_PROGRESS = 0.0f;
	float FIRE_ENTRY = 0.0f;
	float FIRE_OFFSETX = 0.0f;
	float FIRE_OFFSETY = 0.0f;

	/* Shell */
	pKoopa _shell = nullptr;
	bool _hold = false;
	float _kickInterval = 0.0f;
	float KICK_INTERVAL = 0.0f;
	float SHELL_OFFSET = 0.0f;

	/* Power */
	enum class EPower
	{
		SMALL,
		LARGE,
		FIRE,
		RACCOON
	};
	EPower _power = EPower::SMALL;

	enum class EAction
	{
		IDLE,
		CROUNCH,
		MOVE,
		DRIFT,
		TURN,
		JUMP,
		FLY,
		HOVER,
		KICK,
		SPIN,
		FIRE,
		GROW,
		SHRINK,
		POWER_FIRE,
		POWER_TAIL,
		POWER_DOWN,
		DIE,
		ENTRY,
		EXIT,
		CLEAR
	};
	EAction _action = EAction::IDLE;
	EAction _nextAction = EAction::IDLE;

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
	void Idle(float elapsedMs);
	void Crounch(float elapsedMs);
	void Move(float elapsedMs);
	void Drift(float elapsedMs);
	void Turn(float elapsedMs);
	void Jump(float elapsedMs);
	void Fly(float elapsedMs);
	void Hover(float elapsedMs);
	void Kick(float elapsedMs);
	void Spin(float elapsedMs);
	void Fire(float elapsedMs);
	void Grow(float elapsedMs);
	void Shrink(float elapsedMs);
	void PowerFire(float elapsedMs);
	void PowerTail(float elapsedMs);
	void PowerDown(float elapsedMs);
	void Die(float elapsedMs);
	void Entry(float elapsedMs);
	void Exit(float elapsedMs);
	void Clear(float elapsedMs);

	void UpdateGravity(float elapsedMs);
	void IncreaseMomentum(float elapsedMs);
	void DecreaseMomentum(float elapsedMs);
	void UpdateFlying(float elapsedMs);
	void UpdateInvincibility(float elapsedMs);

	void AerialControl(float elapsedMs);
	void MovementControl(float elapsedMs);

	bool AttackTrigger();
	bool IdleTrigger();
	bool FlyTrigger();

	void UpdateTail();
	void UpdateShell();
	void ShootFireball();
	void Hit();

	void CameraControl();
	void PowerControl();

	virtual int IsCollidable();
	virtual int IsBlocking();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void OnNoCollision(float elapsedMs);
	void OnCollisionWith(pCollision collision);

	void OnCollisionWithGoomba(pCollision collision);
	void OnCollisionWithGoombaMicro(pCollision collision);
	void OnCollisionWithKoopa(pCollision collision);
	void OnCollisionWithPlant(pCollision collision);
	void OnCollisionWithPlantFireball(pCollision collision);

	void OnCollisionWithPlatform(pCollision collision);
	void OnCollisionWithBlock(pCollision collision);
	void OnCollisionWithBrick(pCollision collision);
	void OnCollisionWithPipe(pCollision collision);
	void OnCollisionWithTransportPipe(pCollision collision);
	void OnCollisionWithRelay(pCollision collision);
	void OnCollisionWithDeadZone(pCollision collision);
	void OnCollisionWithGoal(pCollision collision);

	void OnCollisionWithCoin(pCollision collision);
	void OnCollisionWithSuperMushroom(pCollision collision);
	void OnCollisionWithSuperLeaf(pCollision collision);
	void OnCollisionWithFireFlower(pCollision collision);
	void OnCollisionWithExtraLifeMushroom(pCollision collision);
};
typedef CMario* pMario;

#endif // !__MARIO_H__
