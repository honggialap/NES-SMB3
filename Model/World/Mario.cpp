#pragma region INCLUDE
#include "Mario.h"

#include "../SuperMarioBros3.h"

#include "Goomba.h"
#include "GoombaMicro.h"
#include "Koopa.h"
#include "Plant.h"
#include "PlantFireball.h"

#include "Platform.h"
#include "Block.h"
#include "Brick.h"
#include "Pipe.h"
#include "TransportPipe.h"
#include "Relay.h"
#include "DeadZone.h"
#include "Goal.h"

#include "Coin.h"
#include "SuperMushroom.h"
#include "SuperLeaf.h"
#include "FireFlower.h"
#include "ExtraLifeMushroom.h"

#include "../Engine/Framework/Debug.h"
#pragma endregion

void CMario::Load()
{
	CGameObject::Load();

	/* Read file */
	pugi::xml_document prefab;
	prefab.load_file(_source.c_str());

	/* Command */
	pugi::xml_node command = prefab.child("Prefab").child("Command");
	LEFT = command.attribute("LEFT").as_int();
	UP = command.attribute("UP").as_int();
	RIGHT = command.attribute("RIGHT").as_int();
	DOWN = command.attribute("DOWN").as_int();
	ACTION = command.attribute("ACTION").as_int();
	JUMP = command.attribute("JUMP").as_int();

	/* Small Body */
	pugi::xml_node smallBody = prefab.child("Prefab").child("SmallBody");
	_renderSmallBody = smallBody.attribute("render").as_bool();
	SMALL_BODY_WIDTH = smallBody.attribute("width").as_float();
	SMALL_BODY_HEIGHT = smallBody.attribute("height").as_float();
	SMALL_BODY_OFFSETX = smallBody.attribute("offsetX").as_float();
	SMALL_BODY_OFFSETY = smallBody.attribute("offsetY").as_float();

	/* Large Body */
	pugi::xml_node largeBody = prefab.child("Prefab").child("LargeBody");
	_renderLargeBody = largeBody.attribute("render").as_bool();
	LARGE_BODY_WIDTH = largeBody.attribute("width").as_float();
	LARGE_BODY_HEIGHT = largeBody.attribute("height").as_float();
	LARGE_BODY_OFFSETX = largeBody.attribute("offsetX").as_float();
	LARGE_BODY_OFFSETY = largeBody.attribute("offsetY").as_float();

	/* Cheat */
	auto cheat = prefab.child("Prefab").child("Cheat");
	HOT1 = cheat.attribute("HOT1").as_int();
	HOT2 = cheat.attribute("HOT2").as_int();
	HOT3 = cheat.attribute("HOT3").as_int();
	HOT4 = cheat.attribute("HOT4").as_int();

	/* Gravity */
	auto gravity = prefab.child("Prefab").child("Gravity");
	GRAVITY = gravity.attribute("GRAVITY").as_float();
	GROUND_DETECT_FACTOR = gravity.attribute("GROUND_DETECT_FACTOR").as_float();

	/* Move */
	auto move = prefab.child("Prefab").child("Move");
	DRAG = move.attribute("DRAG").as_float();
	IDLE_THRESHOLD = move.attribute("IDLE_THRESHOLD").as_float();
	WALK_ACCELERATION = move.attribute("WALK_ACCELERATION").as_float();
	WALK_SPEED_LIMIT = move.attribute("WALK_SPEED_LIMIT").as_float();
	RUN_ACCELERATION = move.attribute("RUN_ACCELERATION").as_float();
	RUN_SPEED_LIMIT = move.attribute("RUN_SPEED_LIMIT").as_float();
	FULLSPEED_ACCELERATION = move.attribute("FULLSPEED_ACCELERATION").as_float();
	FULLSPEED_SPEED_LIMIT = move.attribute("FULLSPEED_SPEED_LIMIT").as_float();
	MOMENTUM_INCREASE_RATE = move.attribute("MOMENTUM_INCREASE_RATE").as_float();
	MOMENTUM_DECREASE_RATE = move.attribute("MOMENTUM_DECREASE_RATE").as_float();
	MOMENTUM_LIMIT = move.attribute("MOMENTUM_LIMIT").as_float();
	MOMENTUM_THRESHOLD = move.attribute("MOMENTUM_THRESHOLD").as_float();
	WALK_ANIMATION_SPEED_FACTOR = move.attribute("WALK_ANIMATION_SPEED_FACTOR").as_float();
	DRIFT_THRESHOLD = move.attribute("DRIFT_THRESHOLD").as_float();

	/* Jump */
	auto jump = prefab.child("Prefab").child("Jump");
	JUMP_FORCE = jump.attribute("JUMP_FORCE").as_float();
	FULLSPEED_JUMP_FORCE = jump.attribute("FULLSPEED_JUMP_FORCE").as_float();
	JUMP_LIMIT = jump.attribute("JUMP_LIMIT").as_float();

	/* Hover */
	auto hover = prefab.child("Prefab").child("Hover");
	HOVER_COUNTDOWN = hover.attribute("HOVER_COUNTDOWN").as_float();
	HOVER_GRAVITY = hover.attribute("HOVER_GRAVITY").as_float();

	/* Fly */
	auto fly = prefab.child("Prefab").child("Fly");
	FLY_COUNTDOWN = fly.attribute("FLY_COUNTDOWN").as_float();
	FLY_GRAVITY = fly.attribute("FLY_GRAVITY").as_float();
	FLY_TIMEOUT_LIMIT = fly.attribute("FLY_TIMEOUT_LIMIT").as_float();

	/* Tranformation */
	auto transformation = prefab.child("Prefab").child("Transformation");
	TRANSFORMATION_TIMEOUT = transformation.attribute("TRANSFORMATION_TIMEOUT").as_float();

	/* Tail */
	auto tail = prefab.child("Prefab").child("Tail");
	_tail = dynamic_cast<pMarioTail>(
		_game->Create(
			tail.attribute("actor").as_uint(),
			_name + tail.attribute("name").as_string(),
			tail.attribute("source").as_string(),
			_x, _y, _gx, _gy, _layer
		)
		);

	auto tailAttack = prefab.child("Prefab").child("TailAttack");
	TAIL_ENTRY = tailAttack.attribute("TAIL_ENTRY").as_float();
	TAIL_PROGRESS = tailAttack.attribute("TAIL_PROGRESS").as_float();
	TAIL_RECOVER = tailAttack.attribute("TAIL_RECOVER").as_float();

	/* Fire */
	auto fireAttack = prefab.child("Prefab").child("FireAttack");
	FIRE_ENTRY = fireAttack.attribute("FIRE_ENTRY").as_float();
	FIRE_PROGRESS = fireAttack.attribute("FIRE_PROGRESS").as_float();
	FIRE_RECOVER = fireAttack.attribute("FIRE_RECOVER").as_float();
	FIRE_OFFSETX = fireAttack.attribute("FIRE_OFFSETX").as_float();
	FIRE_OFFSETY = fireAttack.attribute("FIRE_OFFSETY").as_float();

	/* Kick */
	auto kick = prefab.child("Prefab").child("Kick");
	KICK_INTERVAL = kick.attribute("KICK_INTERVAL").as_float();
	SHELL_OFFSET = kick.attribute("SHELL_OFFSET").as_float();

	/* Invincible */
	auto invincible = prefab.child("Prefab").child("Invincible");
	INVINCIBLE_TIMEOUT = invincible.attribute("INVINCIBLE_TIMEOUT").as_float();
	BLINK_INTERVAL = invincible.attribute("BLINK_INTERVAL").as_float();
}

void CMario::Start()
{
	CGameObject::Start();
}

void CMario::Update(float elapsedMs)
{
	if (!_start) Start();

	UpdateGravity(elapsedMs);
	UpdateFlying(elapsedMs);
	UpdateTail();
	UpdateShell();

	std::vector<pGameObject> collidables = _game->GetLocal(_id);
	_collider->Process(elapsedMs, &collidables);

	HandleAction(elapsedMs);

	CameraControl();
	PowerControl();
	DebugOut(
		L"Ground: %d| vx: %f| vy: %f| momentum: %f| fullspeed: %d| action: %d \n",
		_ground,
		_vx, _vy,
		_momentum, _fullspeed,
		_action
	);
}

void CMario::Render()
{
	if (_renderSmallBody) _sprites[SPR_MARIO_S_BBOX]->Render(_x, _y);
	if (_renderLargeBody) _sprites[SPR_MARIO_L_BBOX]->Render(_x, _y);

	switch (_action)
	{
	case CMario::EAction::IDLE:
	{
		switch (_power)
		{
		case CMario::EPower::SMALL:
		{
			if (_hold)
			{
				if (_ground)
				{
					if (_left)	_sprites[SPR_MARIO_S_HOLD_IDLE_LEFT]->Render(_x, _y);
					else		_sprites[SPR_MARIO_S_HOLD_IDLE_RIGHT]->Render(_x, _y);
				}
				else
				{
					if (_left)	_sprites[SPR_MARIO_S_HOLD_WALK_LEFT]->Render(_x, _y);
					else		_sprites[SPR_MARIO_S_HOLD_WALK_RIGHT]->Render(_x, _y);
				}
			}
			else
			{
				if (_ground)
				{
					if (_left)	_sprites[SPR_MARIO_S_IDLE_LEFT]->Render(_x, _y);
					else		_sprites[SPR_MARIO_S_IDLE_RIGHT]->Render(_x, _y);
				}
				else
				{
					if (_left)	_sprites[SPR_MARIO_S_JUMP_LEFT]->Render(_x, _y);
					else		_sprites[SPR_MARIO_S_JUMP_RIGHT]->Render(_x, _y);
				}
			}
		}
		break;

		case CMario::EPower::LARGE:
		{
			if (_hold)
			{
				if (_ground)
				{
					if (_left)	_sprites[SPR_MARIO_L_HOLD_IDLE_LEFT]->Render(_x, _y);
					else		_sprites[SPR_MARIO_L_HOLD_IDLE_RIGHT]->Render(_x, _y);
				}
				else
				{
					if (_left)	_sprites[SPR_MARIO_L_HOLD_WALK2_LEFT]->Render(_x, _y);
					else		_sprites[SPR_MARIO_L_HOLD_WALK2_RIGHT]->Render(_x, _y);
				}
			}
			else
			{
				if (_ground)
				{
					if (_left)	_sprites[SPR_MARIO_L_IDLE_LEFT]->Render(_x, _y);
					else		_sprites[SPR_MARIO_L_IDLE_RIGHT]->Render(_x, _y);
				}
				else
				{
					if (_left)	_sprites[SPR_MARIO_L_WALK2_LEFT]->Render(_x, _y);
					else		_sprites[SPR_MARIO_L_WALK2_RIGHT]->Render(_x, _y);
				}
			}
		}
		break;

		case CMario::EPower::FIRE:
		{
			if (_hold)
			{
				if (_ground)
				{
					if (_left)	_sprites[SPR_MARIO_F_HOLD_IDLE_LEFT]->Render(_x, _y);
					else		_sprites[SPR_MARIO_F_HOLD_IDLE_RIGHT]->Render(_x, _y);
				}
				else
				{
					if (_left)	_sprites[SPR_MARIO_F_HOLD_WALK2_LEFT]->Render(_x, _y);
					else		_sprites[SPR_MARIO_F_HOLD_WALK2_RIGHT]->Render(_x, _y);
				}
			}
			else
			{
				if (_ground)
				{
					if (_left)	_sprites[SPR_MARIO_F_IDLE_LEFT]->Render(_x, _y);
					else		_sprites[SPR_MARIO_F_IDLE_RIGHT]->Render(_x, _y);
				}
				else
				{
					if (_left)	_sprites[SPR_MARIO_F_WALK2_LEFT]->Render(_x, _y);
					else		_sprites[SPR_MARIO_F_WALK2_RIGHT]->Render(_x, _y);
				}
			}

		}
		break;

		case CMario::EPower::RACCOON:
		{
			if (_hold)
			{
				if (_ground)
				{
					if (_left)	_sprites[SPR_MARIO_R_HOLD_IDLE_LEFT]->Render(_x, _y);
					else		_sprites[SPR_MARIO_R_HOLD_IDLE_RIGHT]->Render(_x, _y);
				}
				else
				{
					if (_left)	_sprites[SPR_MARIO_R_HOLD_WALK2_LEFT]->Render(_x, _y);
					else		_sprites[SPR_MARIO_R_HOLD_WALK2_RIGHT]->Render(_x, _y);
				}
			}
			else
			{
				if (_ground)
				{
					if (_left)	_sprites[SPR_MARIO_R_IDLE_LEFT]->Render(_x, _y);
					else		_sprites[SPR_MARIO_R_IDLE_RIGHT]->Render(_x, _y);
				}
				else
				{
					if (_left)	_sprites[SPR_MARIO_R_WALK2_LEFT]->Render(_x, _y);
					else		_sprites[SPR_MARIO_R_WALK2_RIGHT]->Render(_x, _y);
				}
			}
		}
		break;
		}
	}
	break;

	case CMario::EAction::CROUNCH:
	{
		switch (_power)
		{
		case CMario::EPower::LARGE:
		{
			if (_left)	_sprites[SPR_MARIO_L_CROUNCH_LEFT]->Render(_x, _y);
			else		_sprites[SPR_MARIO_L_CROUNCH_RIGHT]->Render(_x, _y);
		}
		break;

		case CMario::EPower::FIRE:
		{
			if (_left)	_sprites[SPR_MARIO_F_CROUNCH_LEFT]->Render(_x, _y);
			else		_sprites[SPR_MARIO_F_CROUNCH_RIGHT]->Render(_x, _y);
		}
		break;

		case CMario::EPower::RACCOON:
		{
			if (_left)	_sprites[SPR_MARIO_R_CROUNCH_LEFT]->Render(_x, _y);
			else		_sprites[SPR_MARIO_R_CROUNCH_RIGHT]->Render(_x, _y);
		}
		break;
		}
	}
	break;

	case CMario::EAction::MOVE:
	{
		switch (_power)
		{
		case CMario::EPower::SMALL:
		{
			if (_hold)
			{
				if (_ground)
				{
					if (_fullspeed)
					{
						if (_left)	_animations[ANI_MARIO_S_HOLD_WALK_LEFT]->Render(_x, _y);
						else		_animations[ANI_MARIO_S_HOLD_WALK_RIGHT]->Render(_x, _y);
					}
					else
					{
						if (_left)	_animations[ANI_MARIO_S_HOLD_WALK_LEFT]->Render(_x, _y);
						else		_animations[ANI_MARIO_S_HOLD_WALK_RIGHT]->Render(_x, _y);
					}
				}
				else
				{
					if (_fullspeed)
					{
						if (_left)	_sprites[SPR_MARIO_S_SWIM1_LEFT]->Render(_x, _y);
						else		_sprites[SPR_MARIO_S_SWIM1_RIGHT]->Render(_x, _y);
					}
					else
					{
						if (_left)	_sprites[SPR_MARIO_S_HOLD_WALK_LEFT]->Render(_x, _y);
						else		_sprites[SPR_MARIO_S_HOLD_WALK_RIGHT]->Render(_x, _y);
					}
				}
			}
			else
			{
				if (_ground)
				{
					if (_fullspeed)
					{
						if (_left)	_animations[ANI_MARIO_S_RUN_LEFT]->Render(_x, _y);
						else		_animations[ANI_MARIO_S_RUN_RIGHT]->Render(_x, _y);
					}
					else
					{
						if (_left)	_animations[ANI_MARIO_S_WALK_LEFT]->Render(_x, _y);
						else		_animations[ANI_MARIO_S_WALK_RIGHT]->Render(_x, _y);
					}
				}
				else
				{
					if (_fullspeed)
					{
						if (_left)	_sprites[SPR_MARIO_S_FLY_LEFT]->Render(_x, _y);
						else		_sprites[SPR_MARIO_S_FLY_RIGHT]->Render(_x, _y);
					}
					else
					{
						if (_left)	_sprites[SPR_MARIO_S_JUMP_LEFT]->Render(_x, _y);
						else		_sprites[SPR_MARIO_S_JUMP_RIGHT]->Render(_x, _y);
					}
				}
			}
		}
		break;

		case CMario::EPower::LARGE:
		{
			if (_hold)
			{
				if (_ground)
				{
					if (_fullspeed)
					{
						if (_left)	_animations[ANI_MARIO_L_HOLD_WALK_LEFT]->Render(_x, _y);
						else		_animations[ANI_MARIO_L_HOLD_WALK_RIGHT]->Render(_x, _y);
					}
					else
					{
						if (_left)	_animations[ANI_MARIO_L_RUN_LEFT]->Render(_x, _y);
						else		_animations[ANI_MARIO_L_RUN_RIGHT]->Render(_x, _y);
					}
				}
				else
				{
					if (_fullspeed)
					{
						if (_left)	_sprites[SPR_MARIO_L_SWIM1_LEFT]->Render(_x, _y);
						else		_sprites[SPR_MARIO_L_SWIM1_RIGHT]->Render(_x, _y);
					}
					else
					{
						if (_left)	_sprites[SPR_MARIO_L_HOLD_WALK2_LEFT]->Render(_x, _y);
						else		_sprites[SPR_MARIO_L_HOLD_WALK2_RIGHT]->Render(_x, _y);
					}
				}
			}
			else
			{
				if (_ground)
				{
					if (_fullspeed)
					{
						if (_left)	_animations[ANI_MARIO_L_RUN_LEFT]->Render(_x, _y);
						else		_animations[ANI_MARIO_L_RUN_RIGHT]->Render(_x, _y);
					}
					else
					{
						if (_left)	_animations[ANI_MARIO_L_WALK_LEFT]->Render(_x, _y);
						else		_animations[ANI_MARIO_L_WALK_RIGHT]->Render(_x, _y);
					}
				}
				else
				{
					if (_fullspeed)
					{
						if (_left)	_sprites[SPR_MARIO_L_FLY_LEFT]->Render(_x, _y);
						else		_sprites[SPR_MARIO_L_FLY_RIGHT]->Render(_x, _y);
					}
					else
					{
						if (_left)	_sprites[SPR_MARIO_L_WALK2_LEFT]->Render(_x, _y);
						else		_sprites[SPR_MARIO_L_WALK2_RIGHT]->Render(_x, _y);
					}
				}
			}

		}
		break;

		case CMario::EPower::FIRE:
		{
			if (_hold)
			{
				if (_ground)
				{
					if (_fullspeed)
					{
						if (_left)	_animations[ANI_MARIO_F_HOLD_WALK_LEFT]->Render(_x, _y);
						else		_animations[ANI_MARIO_F_HOLD_WALK_RIGHT]->Render(_x, _y);
					}
					else
					{
						if (_left)	_animations[ANI_MARIO_F_RUN_LEFT]->Render(_x, _y);
						else		_animations[ANI_MARIO_F_RUN_RIGHT]->Render(_x, _y);
					}
				}
				else
				{
					if (_fullspeed)
					{
						if (_left)	_sprites[SPR_MARIO_F_SWIM1_LEFT]->Render(_x, _y);
						else		_sprites[SPR_MARIO_F_SWIM1_RIGHT]->Render(_x, _y);
					}
					else
					{
						if (_left)	_sprites[SPR_MARIO_F_HOLD_WALK2_LEFT]->Render(_x, _y);
						else		_sprites[SPR_MARIO_F_HOLD_WALK2_RIGHT]->Render(_x, _y);
					}
				}
			}
			else
			{
				if (_ground)
				{
					if (_fullspeed)
					{
						if (_left)	_animations[ANI_MARIO_F_RUN_LEFT]->Render(_x, _y);
						else		_animations[ANI_MARIO_F_RUN_RIGHT]->Render(_x, _y);
					}
					else
					{
						if (_left)	_animations[ANI_MARIO_F_WALK_LEFT]->Render(_x, _y);
						else		_animations[ANI_MARIO_F_WALK_RIGHT]->Render(_x, _y);
					}
				}
				else
				{
					if (_fullspeed)
					{
						if (_left)	_sprites[SPR_MARIO_F_FLY_LEFT]->Render(_x, _y);
						else		_sprites[SPR_MARIO_F_FLY_RIGHT]->Render(_x, _y);
					}
					else
					{
						if (_left)	_sprites[SPR_MARIO_F_WALK2_LEFT]->Render(_x, _y);
						else		_sprites[SPR_MARIO_F_WALK2_RIGHT]->Render(_x, _y);
					}
				}
			}

		}
		break;

		case CMario::EPower::RACCOON:
		{
			if (_hold)
			{
				if (_ground)
				{
					if (_fullspeed)
					{
						if (_left)	_animations[ANI_MARIO_R_HOLD_WALK_LEFT]->Render(_x, _y);
						else		_animations[ANI_MARIO_R_HOLD_WALK_RIGHT]->Render(_x, _y);
					}
					else
					{
						if (_left)	_animations[ANI_MARIO_R_RUN_LEFT]->Render(_x, _y);
						else		_animations[ANI_MARIO_R_RUN_RIGHT]->Render(_x, _y);
					}
				}
				else
				{
					if (_fullspeed)
					{
						if (_left)	_sprites[SPR_MARIO_R_SWIM1_LEFT]->Render(_x, _y);
						else		_sprites[SPR_MARIO_R_SWIM1_RIGHT]->Render(_x, _y);
					}
					else
					{
						if (_left)	_sprites[SPR_MARIO_R_HOLD_WALK2_LEFT]->Render(_x, _y);
						else		_sprites[SPR_MARIO_R_HOLD_WALK2_RIGHT]->Render(_x, _y);
					}
				}
			}
			else
			{
				if (_ground)
				{
					if (_fullspeed)
					{
						if (_left)	_animations[ANI_MARIO_R_RUN_LEFT]->Render(_x, _y);
						else		_animations[ANI_MARIO_R_RUN_RIGHT]->Render(_x, _y);
					}
					else
					{
						if (_left)	_animations[ANI_MARIO_R_WALK_LEFT]->Render(_x, _y);
						else		_animations[ANI_MARIO_R_WALK_RIGHT]->Render(_x, _y);
					}
				}
				else
				{
					if (_fullspeed)
					{
						if (_left)	_sprites[SPR_MARIO_R_FLY1_LEFT]->Render(_x, _y);
						else		_sprites[SPR_MARIO_R_FLY1_RIGHT]->Render(_x, _y);
					}
					else
					{
						if (_left)	_sprites[SPR_MARIO_R_WALK2_LEFT]->Render(_x, _y);
						else		_sprites[SPR_MARIO_R_WALK2_RIGHT]->Render(_x, _y);
					}
				}
			}

		}
		break;
		}
	}
	break;

	case CMario::EAction::DRIFT:
	{
		switch (_power)
		{
		case CMario::EPower::SMALL:
		{
			if (_left)	_sprites[SPR_MARIO_S_DRIFT_LEFT]->Render(_x, _y);
			else		_sprites[SPR_MARIO_S_DRIFT_RIGHT]->Render(_x, _y);
		}
		break;

		case CMario::EPower::LARGE:
		{
			if (_left)	_sprites[SPR_MARIO_L_DRIFT_LEFT]->Render(_x, _y);
			else		_sprites[SPR_MARIO_L_DRIFT_RIGHT]->Render(_x, _y);
		}
		break;

		case CMario::EPower::FIRE:
		{
			if (_left)	_sprites[SPR_MARIO_F_DRIFT_LEFT]->Render(_x, _y);
			else		_sprites[SPR_MARIO_F_DRIFT_RIGHT]->Render(_x, _y);
		}
		break;

		case CMario::EPower::RACCOON:
		{
			if (_left)	_sprites[SPR_MARIO_R_DRIFT_LEFT]->Render(_x, _y);
			else		_sprites[SPR_MARIO_R_DRIFT_RIGHT]->Render(_x, _y);
		}
		break;
		}
	}
	break;

	case CMario::EAction::TURN:
	{
		switch (_power)
		{
		case CMario::EPower::SMALL:
			_sprites[SPR_MARIO_S_FRONT]->Render(_x, _y);
			break;

		case CMario::EPower::LARGE:
			_sprites[SPR_MARIO_L_FRONT]->Render(_x, _y);
			break;

		case CMario::EPower::FIRE:
			_sprites[SPR_MARIO_F_FRONT]->Render(_x, _y);
			break;

		case CMario::EPower::RACCOON:
			_sprites[SPR_MARIO_R_FRONT]->Render(_x, _y);
			break;
		}
	}
	break;

	case CMario::EAction::JUMP:
	{
		switch (_power)
		{
		case CMario::EPower::SMALL:
		{
			if (_hold)
			{
				if (_fullspeed)
				{
					if (_left)	_sprites[SPR_MARIO_S_SWIM1_LEFT]->Render(_x, _y);
					else		_sprites[SPR_MARIO_S_SWIM1_RIGHT]->Render(_x, _y);
				}
				else
				{
					if (_left)	_sprites[SPR_MARIO_S_HOLD_WALK_LEFT]->Render(_x, _y);
					else		_sprites[SPR_MARIO_S_HOLD_WALK_RIGHT]->Render(_x, _y);
				}
			}
			else
			{
				if (_fullspeed)
				{
					if (_left)	_sprites[SPR_MARIO_S_FLY_LEFT]->Render(_x, _y);
					else		_sprites[SPR_MARIO_S_FLY_RIGHT]->Render(_x, _y);
				}
				else
				{
					if (_left)	_sprites[SPR_MARIO_S_JUMP_LEFT]->Render(_x, _y);
					else		_sprites[SPR_MARIO_S_JUMP_RIGHT]->Render(_x, _y);
				}

			}
		}
		break;

		case CMario::EPower::LARGE:
		{
			if (_hold)
			{
				if (_fullspeed)
				{
					if (_left)	_sprites[SPR_MARIO_L_SWIM1_LEFT]->Render(_x, _y);
					else		_sprites[SPR_MARIO_L_SWIM1_RIGHT]->Render(_x, _y);
				}
				else
				{
					if (_left)	_sprites[SPR_MARIO_L_HOLD_WALK2_LEFT]->Render(_x, _y);
					else		_sprites[SPR_MARIO_L_HOLD_WALK2_RIGHT]->Render(_x, _y);
				}
			}
			else
			{
				if (_fall)
				{
					if (_fullspeed)
					{
						if (_left)	_sprites[SPR_MARIO_L_FLY_LEFT]->Render(_x, _y);
						else		_sprites[SPR_MARIO_L_FLY_RIGHT]->Render(_x, _y);
					}
					else
					{
						if (_left)	_sprites[SPR_MARIO_L_WALK2_LEFT]->Render(_x, _y);
						else		_sprites[SPR_MARIO_L_WALK2_RIGHT]->Render(_x, _y);
					}
				}
				else
				{
					if (_fullspeed)
					{
						if (_left)	_sprites[SPR_MARIO_L_FLY_LEFT]->Render(_x, _y);
						else		_sprites[SPR_MARIO_L_FLY_RIGHT]->Render(_x, _y);
					}
					else
					{
						if (_left)	_sprites[SPR_MARIO_L_JUMP_LEFT]->Render(_x, _y);
						else		_sprites[SPR_MARIO_L_JUMP_RIGHT]->Render(_x, _y);
					}
				}
			}
		}
		break;

		case CMario::EPower::FIRE:
		{
			if (_hold)
			{
				if (_fullspeed)
				{
					if (_left)	_sprites[SPR_MARIO_F_SWIM1_LEFT]->Render(_x, _y);
					else		_sprites[SPR_MARIO_F_SWIM1_RIGHT]->Render(_x, _y);
				}
				else
				{
					if (_left)	_sprites[SPR_MARIO_F_HOLD_WALK2_LEFT]->Render(_x, _y);
					else		_sprites[SPR_MARIO_F_HOLD_WALK2_RIGHT]->Render(_x, _y);
				}
			}
			else
			{
				if (_fall)
				{
					if (_fullspeed)
					{
						if (_left)	_sprites[SPR_MARIO_F_FLY_LEFT]->Render(_x, _y);
						else		_sprites[SPR_MARIO_F_FLY_RIGHT]->Render(_x, _y);
					}
					else
					{
						if (_left)	_sprites[SPR_MARIO_F_WALK2_LEFT]->Render(_x, _y);
						else		_sprites[SPR_MARIO_F_WALK2_RIGHT]->Render(_x, _y);
					}
				}
				else
				{
					if (_fullspeed)
					{
						if (_left)	_sprites[SPR_MARIO_F_FLY_LEFT]->Render(_x, _y);
						else		_sprites[SPR_MARIO_F_FLY_RIGHT]->Render(_x, _y);
					}
					else
					{
						if (_left)	_sprites[SPR_MARIO_F_JUMP_LEFT]->Render(_x, _y);
						else		_sprites[SPR_MARIO_F_JUMP_RIGHT]->Render(_x, _y);
					}
				}
			}
		}
		break;

		case CMario::EPower::RACCOON:
		{
			if (_hold)
			{
				if (_fullspeed)
				{
					if (_left)	_sprites[SPR_MARIO_R_SWIM1_LEFT]->Render(_x, _y);
					else		_sprites[SPR_MARIO_R_SWIM1_RIGHT]->Render(_x, _y);
				}
				else
				{
					if (_left)	_sprites[SPR_MARIO_R_HOLD_WALK2_LEFT]->Render(_x, _y);
					else		_sprites[SPR_MARIO_R_HOLD_WALK2_RIGHT]->Render(_x, _y);
				}
			}
			else
			{
				if (_fall)
				{
					if (_fullspeed)
					{
						if (_left)	_sprites[SPR_MARIO_R_FLY1_LEFT]->Render(_x, _y);
						else		_sprites[SPR_MARIO_R_FLY1_RIGHT]->Render(_x, _y);
					}
					else
					{
						if (_left)	_sprites[SPR_MARIO_R_WALK2_LEFT]->Render(_x, _y);
						else		_sprites[SPR_MARIO_R_WALK2_RIGHT]->Render(_x, _y);
					}
				}
				else
				{
					if (_fullspeed)
					{
						if (_left)	_sprites[SPR_MARIO_R_FLY1_LEFT]->Render(_x, _y);
						else		_sprites[SPR_MARIO_R_FLY1_RIGHT]->Render(_x, _y);
					}
					else
					{
						if (_left)	_sprites[SPR_MARIO_R_JUMP_LEFT]->Render(_x, _y);
						else		_sprites[SPR_MARIO_R_JUMP_RIGHT]->Render(_x, _y);
					}
				}
			}
		}
		break;
		}
	}
	break;

	case CMario::EAction::FLY:
	{
		switch (_power)
		{
		case CMario::EPower::RACCOON:
		{
			if (_fly)
			{
				if (_left)	_animations[ANI_MARIO_R_FLY_LEFT]->Render(_x, _y);
				else		_animations[ANI_MARIO_R_FLY_RIGHT]->Render(_x, _y);
			}
			else
			{
				if (_left)	_sprites[SPR_MARIO_R_FLY1_LEFT]->Render(_x, _y);
				else		_sprites[SPR_MARIO_R_FLY1_RIGHT]->Render(_x, _y);
			}
		}
		break;
		}
	}
	break;

	case CMario::EAction::HOVER:
	{
		switch (_power)
		{
		case CMario::EPower::RACCOON:
		{
			if (_hover)
			{
				if (_left)	_animations[ANI_MARIO_R_HOVER_LEFT]->Render(_x, _y);
				else		_animations[ANI_MARIO_R_HOVER_RIGHT]->Render(_x, _y);
			}
			else
			{
				if (_left)	_sprites[SPR_MARIO_R_WALK2_LEFT]->Render(_x, _y);
				else		_sprites[SPR_MARIO_R_WALK2_RIGHT]->Render(_x, _y);
			}
		}
		break;
		}
	}
	break;

	case CMario::EAction::KICK:
	{
		switch (_power)
		{
		case CMario::EPower::SMALL:
		{
			if (_left)	_sprites[SPR_MARIO_S_KICK_LEFT]->Render(_x, _y);
			else		_sprites[SPR_MARIO_S_KICK_RIGHT]->Render(_x, _y);
		}
		break;
		case CMario::EPower::LARGE:
		{
			if (_left)	_sprites[SPR_MARIO_L_KICK_LEFT]->Render(_x, _y);
			else		_sprites[SPR_MARIO_L_KICK_RIGHT]->Render(_x, _y);
		}
		break;
		case CMario::EPower::FIRE:
		{
			if (_left)	_sprites[SPR_MARIO_F_KICK_LEFT]->Render(_x, _y);
			else		_sprites[SPR_MARIO_F_KICK_RIGHT]->Render(_x, _y);
		}
		break;
		case CMario::EPower::RACCOON:
		{
			if (_left)	_sprites[SPR_MARIO_R_KICK_LEFT]->Render(_x, _y);
			else		_sprites[SPR_MARIO_R_KICK_RIGHT]->Render(_x, _y);
		}
		break;
		}
	}
	break;

	case CMario::EAction::SPIN:
	{
		if (_left)	_animations[ANI_MARIO_R_ATTACK_LEFT]->Render(_x, _y);
		else		_animations[ANI_MARIO_R_ATTACK_RIGHT]->Render(_x, _y);
	}
	break;

	case CMario::EAction::FIRE:
	{
		if (_ground)
		{
			if (_left)	_animations[ANI_MARIO_F_GROUND_ATTACK_LEFT]->Render(_x, _y);
			else		_animations[ANI_MARIO_F_GROUND_ATTACK_RIGHT]->Render(_x, _y);
		}
		else
		{
			if (_left)	_animations[ANI_MARIO_F_AIR_ATTACK_LEFT]->Render(_x, _y);
			else		_animations[ANI_MARIO_F_AIR_ATTACK_RIGHT]->Render(_x, _y);
		}
	}
	break;

	case CMario::EAction::GROW:
	{
		switch (_power)
		{
		case CMario::EPower::SMALL:
		{
			if (_left)	_animations[ANI_MARIO_GROW_LEFT]->Render(_x, _y);
			else		_animations[ANI_MARIO_GROW_RIGHT]->Render(_x, _y);
		}
		break;
		}
	}
	break;

	case CMario::EAction::SHRINK:
	{
		if (_left)	_animations[ANI_MARIO_SHRINK_LEFT]->Render(_x, _y);
		else		_animations[ANI_MARIO_SHRINK_RIGHT]->Render(_x, _y);
	}
	break;

	case CMario::EAction::POWER_TAIL:
	case CMario::EAction::POWER_FIRE:
	case CMario::EAction::POWER_DOWN:
	{
		if (_left)	_animations[ANI_MARIO_TRANSFORM_LEFT]->Render(_x, _y);
		else		_animations[ANI_MARIO_TRANSFORM_RIGHT]->Render(_x, _y);
	}
	break;

	case CMario::EAction::DIE:
	{
		_sprites[SPR_MARIO_S_DIE]->Render(_x, _y);
	}
	break;

	case CMario::EAction::ENTRY:
	case CMario::EAction::EXIT:
	{
		switch (_power)
		{
		case CMario::EPower::SMALL:
			_sprites[SPR_MARIO_S_FRONT]->Render(_x, _y);
			break;

		case CMario::EPower::LARGE:
			_sprites[SPR_MARIO_L_FRONT]->Render(_x, _y);
			break;

		case CMario::EPower::FIRE:
			_sprites[SPR_MARIO_F_FRONT]->Render(_x, _y);
			break;

		case CMario::EPower::RACCOON:
			_sprites[SPR_MARIO_R_FRONT]->Render(_x, _y);
			break;
		}
	}
	break;

	case CMario::EAction::CLEAR:
	{

	}
	break;
	}
}

void CMario::HandleAction(float elapsedMs)
{
	switch (_action)
	{
	case CMario::EAction::IDLE:
		Idle(elapsedMs);
		break;

	case CMario::EAction::CROUNCH:
		Crounch(elapsedMs);
		break;

	case CMario::EAction::MOVE:
		Move(elapsedMs);
		break;

	case CMario::EAction::DRIFT:
		Drift(elapsedMs);
		break;

	case CMario::EAction::JUMP:
		Jump(elapsedMs);
		break;

	case CMario::EAction::FLY:
		Fly(elapsedMs);
		break;

	case CMario::EAction::HOVER:
		Hover(elapsedMs);
		break;

	case CMario::EAction::KICK:
		Kick(elapsedMs);
		break;

	case CMario::EAction::SPIN:
		Spin(elapsedMs);
		break;

	case CMario::EAction::FIRE:
		Fire(elapsedMs);
		break;

	case CMario::EAction::GROW:
		Grow(elapsedMs);
		break;

	case CMario::EAction::SHRINK:
		Shrink(elapsedMs);
		break;

	case CMario::EAction::POWER_FIRE:
		PowerFire(elapsedMs);
		break;

	case CMario::EAction::POWER_TAIL:
		PowerTail(elapsedMs);
		break;

	case CMario::EAction::POWER_DOWN:
		PowerDown(elapsedMs);
		break;

	case CMario::EAction::DIE:
		Die(elapsedMs);
		break;

	case CMario::EAction::ENTRY:
		Entry(elapsedMs);
		break;

	case CMario::EAction::EXIT:
		Exit(elapsedMs);
		break;

	case CMario::EAction::CLEAR:
		Clear(elapsedMs);
		break;
	}
}

void CMario::Idle(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMario::EActionStage::ENTRY:
	{
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{
		_vx -= _vx * DRAG;
		DecreaseMomentum(elapsedMs);

		if (AttackTrigger())
			break;

		if (FlyTrigger())
			break;

		if (_game->IsKeyDown(LEFT) && !_game->IsKeyDown(RIGHT))
		{
			_left = true;
			SetNextAction(EAction::MOVE);
			break;
		}
		else if (!_game->IsKeyDown(LEFT) && _game->IsKeyDown(RIGHT))
		{
			_left = false;
			SetNextAction(EAction::MOVE);
			break;
		}

		if (_ground)
		{
			if (_power != EPower::SMALL
				&& _game->IsKeyDown(DOWN))
			{
				SetNextAction(EAction::CROUNCH);
				break;
			}

			if (_game->IsKeyPressed(JUMP))
			{
				SetNextAction(EAction::JUMP);
				break;
			}
		}
	}
	break;

	case CMario::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

void CMario::Crounch(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMario::EActionStage::ENTRY:
	{
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{
		_vx -= _vx * DRAG;
		DecreaseMomentum(elapsedMs);

		if (!_game->IsKeyDown(LEFT) && _game->IsKeyDown(RIGHT))
		{
			_left = false;
		}
		else if (_game->IsKeyDown(LEFT) && !_game->IsKeyDown(RIGHT))
		{
			_left = true;
		}

		if (_ground && _game->IsKeyPressed(JUMP))
		{
			_ground = false;
			_jumpLimit = JUMP_LIMIT;
			_vy = JUMP_FORCE;
		}
		else if (_jumpLimit > 0)
		{
			if (_game->IsKeyDown(JUMP)) _vy = JUMP_FORCE;
			_jumpLimit -= elapsedMs;
			if (_jumpLimit < 0
				|| _game->IsKeyReleased(JUMP))
				_jumpLimit = 0;
		}

		if (!_game->IsKeyDown(DOWN))
			IdleTrigger();
	}
	break;

	case CMario::EActionStage::EXIT:
	{

	}
	NextAction();
	break;
	}
}

void CMario::Move(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMario::EActionStage::ENTRY:
	{
		switch (_power)
		{
		case CMario::EPower::SMALL:
		{
			_animations[ANI_MARIO_S_WALK_LEFT]->Play(true);
			_animations[ANI_MARIO_S_WALK_RIGHT]->Play(true);
			_animations[ANI_MARIO_S_RUN_LEFT]->Play(true);
			_animations[ANI_MARIO_S_RUN_RIGHT]->Play(true);
			_animations[ANI_MARIO_S_HOLD_WALK_LEFT]->Play(true);
			_animations[ANI_MARIO_S_HOLD_WALK_RIGHT]->Play(true);
		}
		break;

		case CMario::EPower::LARGE:
		{
			_animations[ANI_MARIO_L_WALK_LEFT]->Play(true);
			_animations[ANI_MARIO_L_WALK_RIGHT]->Play(true);
			_animations[ANI_MARIO_L_RUN_LEFT]->Play(true);
			_animations[ANI_MARIO_L_RUN_RIGHT]->Play(true);
			_animations[ANI_MARIO_L_HOLD_WALK_LEFT]->Play(true);
			_animations[ANI_MARIO_L_HOLD_WALK_RIGHT]->Play(true);
		}
		break;

		case CMario::EPower::FIRE:
		{
			_animations[ANI_MARIO_F_WALK_LEFT]->Play(true);
			_animations[ANI_MARIO_F_WALK_RIGHT]->Play(true);
			_animations[ANI_MARIO_F_RUN_LEFT]->Play(true);
			_animations[ANI_MARIO_F_RUN_RIGHT]->Play(true);
			_animations[ANI_MARIO_F_HOLD_WALK_LEFT]->Play(true);
			_animations[ANI_MARIO_F_HOLD_WALK_RIGHT]->Play(true);
		}
		break;

		case CMario::EPower::RACCOON:
		{
			_animations[ANI_MARIO_R_WALK_LEFT]->Play(true);
			_animations[ANI_MARIO_R_WALK_RIGHT]->Play(true);
			_animations[ANI_MARIO_R_RUN_LEFT]->Play(true);
			_animations[ANI_MARIO_R_RUN_RIGHT]->Play(true);
			_animations[ANI_MARIO_R_HOLD_WALK_LEFT]->Play(true);
			_animations[ANI_MARIO_R_HOLD_WALK_RIGHT]->Play(true);
		}
		break;
		}

	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{
		if (AttackTrigger())
			break;

		if (_ground)
		{
			if (_left
				&& _vx < -DRIFT_THRESHOLD
				&& !_game->IsKeyDown(LEFT) && _game->IsKeyDown(RIGHT))
			{
				_left = false;
				SetNextAction(EAction::DRIFT);
				break;
			}
			else if (!_left
				&& _vx > DRIFT_THRESHOLD
				&& _game->IsKeyDown(LEFT) && !_game->IsKeyDown(RIGHT))
			{
				_left = true;
				SetNextAction(EAction::DRIFT);
				break;
			}

			MovementControl(elapsedMs);

			if (_game->IsKeyPressed(JUMP))
			{
				SetNextAction(EAction::JUMP);
				break;
			}
		}
		else
		{
			AerialControl(elapsedMs);

			if (FlyTrigger())
				break;

			if (_power == EPower::RACCOON
				&& _game->IsKeyPressed(JUMP))
			{
				SetNextAction(EAction::HOVER);
				break;
			}
		}

		if (_vx > -IDLE_THRESHOLD && _vx < IDLE_THRESHOLD)
		{
			_vx = 0;
			SetNextAction(EAction::IDLE);
			break;
		}

		auto animationSpeedFactor = (abs(_vx / FULLSPEED_SPEED_LIMIT) + WALK_ANIMATION_SPEED_FACTOR);
		switch (_power)
		{
		case CMario::EPower::SMALL:
		{
			_animations[ANI_MARIO_S_WALK_LEFT]->Update(elapsedMs * animationSpeedFactor);
			_animations[ANI_MARIO_S_WALK_RIGHT]->Update(elapsedMs * animationSpeedFactor);
			_animations[ANI_MARIO_S_RUN_LEFT]->Update(elapsedMs * animationSpeedFactor);
			_animations[ANI_MARIO_S_RUN_RIGHT]->Update(elapsedMs * animationSpeedFactor);
			_animations[ANI_MARIO_S_HOLD_WALK_LEFT]->Update(elapsedMs * animationSpeedFactor);
			_animations[ANI_MARIO_S_HOLD_WALK_RIGHT]->Update(elapsedMs * animationSpeedFactor);
		}
		break;

		case CMario::EPower::LARGE:
		{
			_animations[ANI_MARIO_L_WALK_LEFT]->Update(elapsedMs * animationSpeedFactor);
			_animations[ANI_MARIO_L_WALK_RIGHT]->Update(elapsedMs * animationSpeedFactor);
			_animations[ANI_MARIO_L_RUN_LEFT]->Update(elapsedMs * animationSpeedFactor);
			_animations[ANI_MARIO_L_RUN_RIGHT]->Update(elapsedMs * animationSpeedFactor);
			_animations[ANI_MARIO_L_HOLD_WALK_LEFT]->Update(elapsedMs * animationSpeedFactor);
			_animations[ANI_MARIO_L_HOLD_WALK_RIGHT]->Update(elapsedMs * animationSpeedFactor);
		}
		break;

		case CMario::EPower::FIRE:
		{
			_animations[ANI_MARIO_F_WALK_LEFT]->Update(elapsedMs * animationSpeedFactor);
			_animations[ANI_MARIO_F_WALK_RIGHT]->Update(elapsedMs * animationSpeedFactor);
			_animations[ANI_MARIO_F_RUN_LEFT]->Update(elapsedMs * animationSpeedFactor);
			_animations[ANI_MARIO_F_RUN_RIGHT]->Update(elapsedMs * animationSpeedFactor);
			_animations[ANI_MARIO_F_HOLD_WALK_LEFT]->Update(elapsedMs * animationSpeedFactor);
			_animations[ANI_MARIO_F_HOLD_WALK_RIGHT]->Update(elapsedMs * animationSpeedFactor);
		}
		break;

		case CMario::EPower::RACCOON:
		{
			_animations[ANI_MARIO_R_WALK_LEFT]->Update(elapsedMs * animationSpeedFactor);
			_animations[ANI_MARIO_R_WALK_RIGHT]->Update(elapsedMs * animationSpeedFactor);
			_animations[ANI_MARIO_R_RUN_LEFT]->Update(elapsedMs * animationSpeedFactor);
			_animations[ANI_MARIO_R_RUN_RIGHT]->Update(elapsedMs * animationSpeedFactor);
			_animations[ANI_MARIO_R_HOLD_WALK_LEFT]->Update(elapsedMs * animationSpeedFactor);
			_animations[ANI_MARIO_R_HOLD_WALK_RIGHT]->Update(elapsedMs * animationSpeedFactor);
		}
		break;
		}
	}
	break;

	case CMario::EActionStage::EXIT:
	{
		switch (_power)
		{
		case CMario::EPower::SMALL:
		{
			_animations[ANI_MARIO_S_WALK_LEFT]->Stop();
			_animations[ANI_MARIO_S_WALK_RIGHT]->Stop();
			_animations[ANI_MARIO_S_RUN_LEFT]->Stop();
			_animations[ANI_MARIO_S_RUN_RIGHT]->Stop();
			_animations[ANI_MARIO_S_HOLD_WALK_LEFT]->Stop();
			_animations[ANI_MARIO_S_HOLD_WALK_RIGHT]->Stop();
		}
		break;

		case CMario::EPower::LARGE:
		{
			_animations[ANI_MARIO_L_WALK_LEFT]->Stop();
			_animations[ANI_MARIO_L_WALK_RIGHT]->Stop();
			_animations[ANI_MARIO_L_RUN_LEFT]->Stop();
			_animations[ANI_MARIO_L_RUN_RIGHT]->Stop();
			_animations[ANI_MARIO_L_HOLD_WALK_LEFT]->Stop();
			_animations[ANI_MARIO_L_HOLD_WALK_RIGHT]->Stop();
		}
		break;

		case CMario::EPower::FIRE:
		{
			_animations[ANI_MARIO_F_WALK_LEFT]->Stop();
			_animations[ANI_MARIO_F_WALK_RIGHT]->Stop();
			_animations[ANI_MARIO_F_RUN_LEFT]->Stop();
			_animations[ANI_MARIO_F_RUN_RIGHT]->Stop();
			_animations[ANI_MARIO_F_HOLD_WALK_LEFT]->Stop();
			_animations[ANI_MARIO_F_HOLD_WALK_RIGHT]->Stop();
		}
		break;

		case CMario::EPower::RACCOON:
		{
			_animations[ANI_MARIO_R_WALK_LEFT]->Stop();
			_animations[ANI_MARIO_R_WALK_RIGHT]->Stop();
			_animations[ANI_MARIO_R_RUN_LEFT]->Stop();
			_animations[ANI_MARIO_R_RUN_RIGHT]->Stop();
			_animations[ANI_MARIO_R_HOLD_WALK_LEFT]->Stop();
			_animations[ANI_MARIO_R_HOLD_WALK_RIGHT]->Stop();
		}
		break;
		}
	}
	NextAction();
	break;
	}
}

void CMario::Drift(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMario::EActionStage::ENTRY:
	{

	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{
		_vx -= _vx * DRAG;
		DecreaseMomentum(elapsedMs);

		if (AttackTrigger())
			break;

		if (_ground)
		{
			if (_power != EPower::SMALL
				&& _game->IsKeyDown(DOWN))
			{
				SetNextAction(EAction::CROUNCH);
				break;
			}

			if (_game->IsKeyPressed(JUMP))
			{
				SetNextAction(EAction::JUMP);
				break;
			}
		}
		else
		{
			if (FlyTrigger())
				break;

			if (_power == EPower::RACCOON
				&& _game->IsKeyPressed(JUMP))
			{
				SetNextAction(EAction::HOVER);
				break;
			}
		}

		if (_left
			&& !_game->IsKeyDown(LEFT) && _game->IsKeyDown(RIGHT))
		{
			_left = false;
			SetNextAction(EAction::MOVE);
			break;
		}
		else if (
			!_left
			&& _game->IsKeyDown(LEFT) && !_game->IsKeyDown(RIGHT))
		{
			_left = true;
			SetNextAction(EAction::MOVE);
			break;
		}

		if (_vx > -IDLE_THRESHOLD && _vx < IDLE_THRESHOLD)
		{
			_vx = 0;
			SetNextAction(EAction::IDLE);
			break;
		}
	}
	break;

	case CMario::EActionStage::EXIT:
	{

	}
	NextAction();
	break;
	}
}

void CMario::Turn(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMario::EActionStage::ENTRY:
	{

	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{

	}
	break;

	case CMario::EActionStage::EXIT:
	{

	}
	NextAction();
	break;
	}
}

void CMario::Jump(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMario::EActionStage::ENTRY:
	{
		_ground = false;
		_fall = false;
		_jumpLimit = JUMP_LIMIT;
		_vy = JUMP_FORCE;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{
		if (AttackTrigger())
			break;

		if (!_fullspeed)
			DecreaseMomentum(elapsedMs);

		if (_jumpLimit > 0)
		{
			if (_game->IsKeyDown(JUMP)) _vy = JUMP_FORCE;
			_jumpLimit -= elapsedMs;
			if (_jumpLimit < 0
				|| _game->IsKeyReleased(JUMP))
				_jumpLimit = 0;
		}

		if (_vy < 0) _fall = true;

		AerialControl(elapsedMs);

		if (FlyTrigger())
			break;

		if (_fall && _power == EPower::RACCOON
			&& _game->IsKeyPressed(JUMP))
		{
			SetNextAction(EAction::HOVER);
			break;
		}

		if (_ground)
			IdleTrigger();
	}
	break;

	case CMario::EActionStage::EXIT:
	{

	}
	NextAction();
	break;
	}
}

void CMario::Fly(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMario::EActionStage::ENTRY:
	{
		_flyTimeout = FLY_TIMEOUT_LIMIT;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{
		if (AttackTrigger())
			break;

		if (_ground)
		{
			_fly = false;
			if (IdleTrigger())
				break;
		}

		if (_flyTimeout > 0)
		{
			if (_game->IsKeyPressed(JUMP))
			{
				_fly = true;
				_flyCountdown = FLY_COUNTDOWN;

				_animations[ANI_MARIO_R_FLY_LEFT]->Play();
				_animations[ANI_MARIO_R_FLY_RIGHT]->Play();
			}

			if (_flyCountdown > 0) _flyCountdown -= elapsedMs;
			else
			{
				_flyCountdown = 0;
				_fly = false;
			}
		}
		else
		{
			_fly = false;
			SetNextAction(EAction::HOVER);
			break;
		}

		AerialControl(elapsedMs);

		_animations[ANI_MARIO_R_FLY_LEFT]->Update(elapsedMs);
		_animations[ANI_MARIO_R_FLY_RIGHT]->Update(elapsedMs);
	}
	break;

	case CMario::EActionStage::EXIT:
	{
		_animations[ANI_MARIO_R_FLY_LEFT]->Stop();
		_animations[ANI_MARIO_R_FLY_RIGHT]->Stop();
	}
	NextAction();
	break;
	}
}

void CMario::Hover(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMario::EActionStage::ENTRY:
	{

	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{
		if (AttackTrigger())
		{
			_hover = false;
			break;
		}

		DecreaseMomentum(elapsedMs);

		if (_ground)
		{
			_hover = false;
			if (IdleTrigger())
				break;
		}

		if (_game->IsKeyPressed(JUMP))
		{
			_hover = true;
			_hoverCountdown = HOVER_COUNTDOWN;

			_animations[ANI_MARIO_R_HOVER_LEFT]->Play();
			_animations[ANI_MARIO_R_HOVER_RIGHT]->Play();
		}

		if (_hoverCountdown > 0)
		{
			_hoverCountdown -= elapsedMs;
		}
		else
		{
			_hoverCountdown = 0;
			_hover = false;
		}

		AerialControl(elapsedMs);

		_animations[ANI_MARIO_R_HOVER_LEFT]->Update(elapsedMs);
		_animations[ANI_MARIO_R_HOVER_RIGHT]->Update(elapsedMs);
	}
	break;

	case CMario::EActionStage::EXIT:
	{
		_animations[ANI_MARIO_R_HOVER_LEFT]->Stop();
		_animations[ANI_MARIO_R_HOVER_RIGHT]->Stop();
	}
	NextAction();
	break;
	}
}

void CMario::Kick(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMario::EActionStage::ENTRY:
	{
		if (_shell != nullptr && _hold)
		{
			_hold = false;
			_shell->Kicked(_left);
			_shell = nullptr;
		}
		_kickInterval = KICK_INTERVAL;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{
		if (_kickInterval > 0)
		{
			_kickInterval -= elapsedMs;
		}
		else
		{
			if (IdleTrigger())
				break;
		}
	}
	break;

	case CMario::EActionStage::EXIT:
	{

	}
	NextAction();
	break;
	}
}

void CMario::Spin(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMario::EActionStage::ENTRY:
	{
		_animations[ANI_MARIO_R_ATTACK_LEFT]->Play(false);
		_animations[ANI_MARIO_R_ATTACK_RIGHT]->Play(false);

		_tailEntry = TAIL_ENTRY;
		_tailProgress = TAIL_PROGRESS;
		_tailRecover = TAIL_RECOVER;
		_tail->_activate = false;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{
		if (_ground)
		{
			if (_game->IsKeyPressed(JUMP))
			{
				SetNextAction(EAction::JUMP);
				break;
			}

			MovementControl(elapsedMs);
		}
		else
		{
			if (FlyTrigger())
				break;

			AerialControl(elapsedMs);
		}

		if (_tailEntry > 0)
			_tailEntry -= elapsedMs;
		else
		{
			if (_tailProgress > 0)
			{
				_tailProgress -= elapsedMs;
				_tail->_activate = true;
			}
			else
			{
				_tail->_activate = false;
				if (_tailRecover > 0)
					_tailRecover -= elapsedMs;
				else
					IdleTrigger();
			}
		}

		_animations[ANI_MARIO_R_ATTACK_LEFT]->Update(elapsedMs);
		_animations[ANI_MARIO_R_ATTACK_RIGHT]->Update(elapsedMs);
	}
	break;

	case CMario::EActionStage::EXIT:
	{
		_animations[ANI_MARIO_R_ATTACK_LEFT]->Stop();
		_animations[ANI_MARIO_R_ATTACK_RIGHT]->Stop();
	}
	NextAction();
	break;
	}
}

void CMario::Fire(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMario::EActionStage::ENTRY:
	{
		_animations[ANI_MARIO_F_GROUND_ATTACK_LEFT]->Play(false);
		_animations[ANI_MARIO_F_GROUND_ATTACK_RIGHT]->Play(false);
		_animations[ANI_MARIO_F_AIR_ATTACK_LEFT]->Play(false);
		_animations[ANI_MARIO_F_AIR_ATTACK_RIGHT]->Play(false);

		_fireEntry = FIRE_ENTRY;
		_fireProgress = FIRE_PROGRESS;
		_fireRecover = FIRE_RECOVER;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{
		_animations[ANI_MARIO_F_GROUND_ATTACK_LEFT]->Update(elapsedMs);
		_animations[ANI_MARIO_F_GROUND_ATTACK_RIGHT]->Update(elapsedMs);
		_animations[ANI_MARIO_F_AIR_ATTACK_LEFT]->Update(elapsedMs);
		_animations[ANI_MARIO_F_AIR_ATTACK_RIGHT]->Update(elapsedMs);

		if (_ground)
		{
			if (_game->IsKeyPressed(JUMP))
			{
				SetNextAction(EAction::JUMP);
				break;
			}

			MovementControl(elapsedMs);
		}
		else
		{
			if (FlyTrigger())
				break;

			AerialControl(elapsedMs);
		}

		if (_fireEntry > 0)
			_fireEntry -= elapsedMs;
		else
		{
			if (_fireProgress > 0)
			{
				_fireProgress -= elapsedMs;
				if (!_shot)
				{
					_shot = true;
					ShootFireball();
				}
			}
			else
			{
				if (_fireProgress > 0)
					_fireProgress -= elapsedMs;
				else
				{
					_shot = false;
					IdleTrigger();
				}
			}
		}
	}
	break;

	case CMario::EActionStage::EXIT:
	{
		_animations[ANI_MARIO_F_GROUND_ATTACK_LEFT]->Stop();
		_animations[ANI_MARIO_F_GROUND_ATTACK_RIGHT]->Stop();
		_animations[ANI_MARIO_F_AIR_ATTACK_LEFT]->Stop();
		_animations[ANI_MARIO_F_AIR_ATTACK_RIGHT]->Stop();
	}
	NextAction();
	break;
	}
}

void CMario::Grow(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMario::EActionStage::ENTRY:
	{
		_transformationTimeout = TRANSFORMATION_TIMEOUT;
		/* Animation Start */
		{
			_animations[ANI_MARIO_GROW_LEFT]->Play();
			_animations[ANI_MARIO_GROW_RIGHT]->Play();
		}
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{
		/* Animation Update */
		{
			_animations[ANI_MARIO_GROW_LEFT]->Update(elapsedMs);
			_animations[ANI_MARIO_GROW_RIGHT]->Update(elapsedMs);
		}

		if (_transformationTimeout > 0) _transformationTimeout -= elapsedMs;
		else
		{
			_transformationTimeout = 0;
			IdleTrigger();
			break;
		}
	}
	break;

	case CMario::EActionStage::EXIT:
	{
		_power = EPower::LARGE;
		/* Animation Stop */
		{
			_animations[ANI_MARIO_GROW_LEFT]->Stop();
			_animations[ANI_MARIO_GROW_RIGHT]->Stop();
		}
	}
	NextAction();
	break;
	}
}

void CMario::Shrink(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMario::EActionStage::ENTRY:
	{
		_transformationTimeout = TRANSFORMATION_TIMEOUT;
		/* Animation Start */
		{
			_animations[ANI_MARIO_SHRINK_LEFT]->Play();
			_animations[ANI_MARIO_SHRINK_RIGHT]->Play();
		}
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{
		/* Animation Update */
		{
			_animations[ANI_MARIO_SHRINK_LEFT]->Update(elapsedMs);
			_animations[ANI_MARIO_SHRINK_RIGHT]->Update(elapsedMs);
		}

		if (_transformationTimeout > 0) _transformationTimeout -= elapsedMs;
		else
		{
			_transformationTimeout = 0;
			IdleTrigger();
			break;
		}
	}
	break;

	case CMario::EActionStage::EXIT:
	{
		_power = EPower::SMALL;
		/* Animation Stop */
		{
			_animations[ANI_MARIO_SHRINK_LEFT]->Stop();
			_animations[ANI_MARIO_SHRINK_RIGHT]->Stop();
		}
	}
	NextAction();
	break;
	}
}

void CMario::PowerFire(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMario::EActionStage::ENTRY:
	{
		_transformationTimeout = TRANSFORMATION_TIMEOUT;
		/* Animation Start */
		{
			_animations[ANI_MARIO_TRANSFORM_LEFT]->Play();
			_animations[ANI_MARIO_TRANSFORM_RIGHT]->Play();
		}
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{
		/* Animation Update */
		{
			_animations[ANI_MARIO_TRANSFORM_LEFT]->Update(elapsedMs);
			_animations[ANI_MARIO_TRANSFORM_RIGHT]->Update(elapsedMs);
		}

		if (_transformationTimeout > 0) _transformationTimeout -= elapsedMs;
		else
		{
			_transformationTimeout = 0;
			IdleTrigger();
			break;
		}
	}
	break;

	case CMario::EActionStage::EXIT:
	{
		_power = EPower::FIRE;
		/* Animation Stop */
		{
			_animations[ANI_MARIO_TRANSFORM_LEFT]->Stop();
			_animations[ANI_MARIO_TRANSFORM_RIGHT]->Stop();
		}
	}
	NextAction();
	break;
	}
}

void CMario::PowerTail(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMario::EActionStage::ENTRY:
	{
		_transformationTimeout = TRANSFORMATION_TIMEOUT;
		/* Animation Start */
		{
			_animations[ANI_MARIO_TRANSFORM_LEFT]->Play();
			_animations[ANI_MARIO_TRANSFORM_RIGHT]->Play();
		}
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{
		/* Animation Update */
		{
			_animations[ANI_MARIO_TRANSFORM_LEFT]->Update(elapsedMs);
			_animations[ANI_MARIO_TRANSFORM_RIGHT]->Update(elapsedMs);
		}

		if (_transformationTimeout > 0) _transformationTimeout -= elapsedMs;
		else
		{
			_transformationTimeout = 0;
			IdleTrigger();
			break;
		}
	}
	break;

	case CMario::EActionStage::EXIT:
	{
		_power = EPower::RACCOON;
		/* Animation Stop */
		{
			_animations[ANI_MARIO_TRANSFORM_LEFT]->Stop();
			_animations[ANI_MARIO_TRANSFORM_RIGHT]->Stop();
		}
	}
	NextAction();
	break;
	}
}


void CMario::PowerDown(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMario::EActionStage::ENTRY:
	{
		_transformationTimeout = TRANSFORMATION_TIMEOUT;
		/* Animation Start */
		{
			_animations[ANI_MARIO_TRANSFORM_LEFT]->Play();
			_animations[ANI_MARIO_TRANSFORM_RIGHT]->Play();
		}
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{
		/* Animation Update */
		{
			_animations[ANI_MARIO_TRANSFORM_LEFT]->Update(elapsedMs);
			_animations[ANI_MARIO_TRANSFORM_RIGHT]->Update(elapsedMs);
		}

		if (_transformationTimeout > 0) _transformationTimeout -= elapsedMs;
		else
		{
			_transformationTimeout = 0;
			IdleTrigger();
			break;
		}
	}
	break;

	case CMario::EActionStage::EXIT:
	{
		_power = EPower::LARGE;
		/* Animation Stop */
		{
			_animations[ANI_MARIO_TRANSFORM_LEFT]->Stop();
			_animations[ANI_MARIO_TRANSFORM_RIGHT]->Stop();
		}
	}
	NextAction();
	break;
	}
}

void CMario::Die(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMario::EActionStage::ENTRY:
	{

	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{

	}
	break;

	case CMario::EActionStage::EXIT:
	{

	}
	NextAction();
	break;
	}
}

void CMario::Entry(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMario::EActionStage::ENTRY:
	{

	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{

	}
	break;

	case CMario::EActionStage::EXIT:
	{

	}
	NextAction();
	break;
	}
}

void CMario::Exit(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMario::EActionStage::ENTRY:
	{

	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{

	}
	break;

	case CMario::EActionStage::EXIT:
	{

	}
	NextAction();
	break;
	}
}

void CMario::Clear(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMario::EActionStage::ENTRY:
	{

	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{

	}
	break;

	case CMario::EActionStage::EXIT:
	{

	}
	NextAction();
	break;
	}
}

void CMario::UpdateGravity(float elapsedMs)
{
	if (_action == EAction::FLY)
	{
		if (_fly) _vy = FLY_GRAVITY;
		else	  _vy -= GRAVITY * elapsedMs;
	}
	else if (_action == EAction::HOVER)
	{
		if (_hover) _vy = -HOVER_GRAVITY;
		else		_vy -= GRAVITY * elapsedMs;
	}
	else
	{
		_vy -= GRAVITY * elapsedMs;
	}

	if (_vy < -GROUND_DETECT_FACTOR) _ground = false;
}

void CMario::IncreaseMomentum(float elapsedMs)
{
	_momentum += MOMENTUM_INCREASE_RATE * elapsedMs;
	if (_momentum >= MOMENTUM_LIMIT)
	{
		_fullspeed = true;
		_momentum = MOMENTUM_LIMIT;
	}
}

void CMario::DecreaseMomentum(float elapsedMs)
{
	_momentum -= MOMENTUM_DECREASE_RATE * elapsedMs;
	if (_momentum < MOMENTUM_LIMIT) _fullspeed = false;
	if (_momentum < 0) _momentum = 0;
}

void CMario::UpdateFlying(float elapsedMs)
{
	if (_flying)
	{
		_flyTimeout -= elapsedMs;
		if (_flyTimeout < 0)
		{
			_flyTimeout = 0;
			_flying = false;
		}
	}
}

void CMario::UpdateInvincibility(float elapsedMs)
{
	if (_invincible)
	{
		if (_invincibleTimeout > 0)
		{
			_invincibleTimeout -= elapsedMs;
			if (_blinkInterval > 0)
			{
				_blinkInterval -= elapsedMs;
			}
			else
			{
				_blinkInterval = BLINK_INTERVAL;
				_blink = !_blink;
			}
		}
		else
		{
			_invincibleTimeout = 0;
			_invincible = false;
			_blink = false;
		}
	}
}

void CMario::MovementControl(float elapsedMs)
{
	if (_game->IsKeyDown(LEFT) && !_game->IsKeyDown(RIGHT))
	{
		_left = true;

		if (_game->IsKeyDown(ACTION))
		{
			if (_fullspeed)
			{
				if (_vx < -FULLSPEED_SPEED_LIMIT) _vx -= _vx * DRAG;
				else
				{
					_vx += -FULLSPEED_ACCELERATION * elapsedMs;
					if (_vx < -FULLSPEED_SPEED_LIMIT) _vx = -FULLSPEED_SPEED_LIMIT;
				}

				if (_vx < -MOMENTUM_THRESHOLD) IncreaseMomentum(elapsedMs);
				else DecreaseMomentum(elapsedMs);
			}
			else
			{
				if (_vx < -RUN_SPEED_LIMIT) _vx -= _vx * DRAG;
				else
				{
					_vx += -RUN_ACCELERATION * elapsedMs;
					if (_vx < -RUN_SPEED_LIMIT) _vx = -RUN_SPEED_LIMIT;
				}

				if (_vx < -MOMENTUM_THRESHOLD) IncreaseMomentum(elapsedMs);
				else DecreaseMomentum(elapsedMs);
			}
		}
		else
		{
			if (_vx < -WALK_SPEED_LIMIT) _vx -= _vx * DRAG;
			else
			{
				_vx += -WALK_ACCELERATION * elapsedMs;
				if (_vx < -WALK_SPEED_LIMIT) _vx = -WALK_SPEED_LIMIT;
			}

			DecreaseMomentum(elapsedMs);
		}
	}
	else if (!_game->IsKeyDown(LEFT) && _game->IsKeyDown(RIGHT))
	{
		_left = false;

		if (_game->IsKeyDown(ACTION))
		{
			if (_fullspeed)
			{
				if (_vx > FULLSPEED_SPEED_LIMIT) _vx -= _vx * DRAG;
				else
				{
					_vx += FULLSPEED_ACCELERATION * elapsedMs;
					if (_vx > FULLSPEED_SPEED_LIMIT) _vx = FULLSPEED_SPEED_LIMIT;
				}

				if (_vx > MOMENTUM_THRESHOLD) IncreaseMomentum(elapsedMs);
				else DecreaseMomentum(elapsedMs);
			}
			else
			{
				if (_vx > RUN_SPEED_LIMIT) _vx -= _vx * DRAG;
				{
					_vx += RUN_ACCELERATION * elapsedMs;
					if (_vx > RUN_SPEED_LIMIT) _vx = RUN_SPEED_LIMIT;
				}

				if (_vx > MOMENTUM_THRESHOLD) IncreaseMomentum(elapsedMs);
				else DecreaseMomentum(elapsedMs);
			}
		}
		else
		{
			if (_vx > WALK_SPEED_LIMIT) _vx -= _vx * DRAG;
			else
			{
				_vx += WALK_ACCELERATION * elapsedMs;
				if (_vx > WALK_SPEED_LIMIT) _vx = WALK_SPEED_LIMIT;
			}

			DecreaseMomentum(elapsedMs);
		}
	}

	if ((_game->IsKeyDown(LEFT) && _game->IsKeyDown(RIGHT))
		|| (!_game->IsKeyDown(LEFT) && !_game->IsKeyDown(RIGHT)))
	{
		_vx -= _vx * DRAG;
		DecreaseMomentum(elapsedMs);
	}
}

bool CMario::AttackTrigger()
{
	if (_power == EPower::FIRE
		&& _game->IsKeyPressed(ACTION))
	{
		SetNextAction(EAction::FIRE);
		return true;
	}

	else if (_power == EPower::RACCOON
		&& _game->IsKeyPressed(ACTION))
	{
		SetNextAction(EAction::SPIN);
		return true;
	}

	else if (_hold
		&& _game->IsKeyReleased(ACTION))
	{
		SetNextAction(EAction::KICK);
		return true;
	}

	return false;
}

bool CMario::IdleTrigger()
{
	if (_vx > -IDLE_THRESHOLD && _vx < IDLE_THRESHOLD)
	{
		SetNextAction(EAction::IDLE);
		return true;
	}

	else
	{
		SetNextAction(EAction::MOVE);
		return true;
	}

	return false;
}

bool CMario::FlyTrigger()
{
	if (_flying)
	{
		if (_power == EPower::RACCOON
			&& _game->IsKeyPressed(JUMP))
		{
			SetNextAction(EAction::FLY);
			return true;
		}
	}
	else
	{
		if (_fullspeed && _power == EPower::RACCOON
			&& _game->IsKeyPressed(JUMP))
		{
			SetNextAction(EAction::FLY);
			_flying = true;
			return true;;
		}
	}

	return false;
}

void CMario::AerialControl(float elapsedMs)
{
	if (_left
		&& !_game->IsKeyDown(LEFT) && _game->IsKeyDown(RIGHT))
	{
		_left = false;
		if (_vx < -WALK_SPEED_LIMIT) _vx = -WALK_SPEED_LIMIT;
	}
	else if (!_left
		&& _game->IsKeyDown(LEFT) && !_game->IsKeyDown(RIGHT))
	{
		_left = true;
		if (_vx > WALK_SPEED_LIMIT) _vx = WALK_SPEED_LIMIT;
	}
	else if (_left
		&& _game->IsKeyDown(LEFT) && !_game->IsKeyDown(RIGHT))
	{
		if (_vx > -WALK_SPEED_LIMIT)
		{
			_vx += -WALK_ACCELERATION * elapsedMs;
			if (_vx < -WALK_SPEED_LIMIT) _vx = -WALK_SPEED_LIMIT;
		}
		else if (_vx < -WALK_SPEED_LIMIT && _vx > -RUN_SPEED_LIMIT)
		{
			_vx += -RUN_ACCELERATION * elapsedMs;
			if (_vx < -RUN_SPEED_LIMIT) _vx = -RUN_SPEED_LIMIT;
		}
		else if (_vx < -RUN_SPEED_LIMIT && _vx > -FULLSPEED_SPEED_LIMIT)
		{
			_vx += -FULLSPEED_ACCELERATION * elapsedMs;
			if (_vx < -FULLSPEED_SPEED_LIMIT) _vx = -FULLSPEED_SPEED_LIMIT;
		}
	}
	else if (!_left
		&& !_game->IsKeyDown(LEFT) && _game->IsKeyDown(RIGHT))
	{
		if (_vx < WALK_SPEED_LIMIT)
		{
			_vx += WALK_ACCELERATION * elapsedMs;
			if (_vx > WALK_SPEED_LIMIT) _vx = WALK_SPEED_LIMIT;
		}
		else if (_vx > WALK_SPEED_LIMIT && _vx < RUN_SPEED_LIMIT)
		{
			_vx += RUN_ACCELERATION * elapsedMs;
			if (_vx > RUN_SPEED_LIMIT) _vx = RUN_SPEED_LIMIT;
		}
		else if (_vx > RUN_SPEED_LIMIT && _vx < FULLSPEED_SPEED_LIMIT)
		{
			_vx += FULLSPEED_ACCELERATION * elapsedMs;
			if (_vx > FULLSPEED_SPEED_LIMIT) _vx = FULLSPEED_SPEED_LIMIT;
		}
	}
}

void CMario::UpdateTail()
{
	_tail->_left = _left;
	_tail->SetPosition(_x, _y);
}

void CMario::UpdateShell()
{
	if (_hold && _shell != nullptr)
	{
		if (_left) _shell->SetPosition(_x - SHELL_OFFSET, _y);
		else _shell->SetPosition(_x + SHELL_OFFSET, _y);
		_shell->SetVelocity(0, 0);
		if (_shell->_action == CKoopa::EAction::MOVE)
		{
			Hit();
			_hold = false;
			_shell = nullptr;
		}
	}
}

void CMario::ShootFireball()
{
	/* Read file */
	pugi::xml_document prefab;
	prefab.load_file(_source.c_str());

	/* Fireball */
	for (auto fireballNode = prefab.child("Prefab").child("Fire");
		fireballNode;
		fireballNode = fireballNode.next_sibling("Fire"))
	{
		std::string fireballName = _name + fireballNode.attribute("name").as_string();
		if (_game->Get(fireballName) == nullptr)
		{
			float fireOffsetX = 0;
			if (_left) fireOffsetX = -FIRE_OFFSETX;
			else fireOffsetX = FIRE_OFFSETX;

			pMarioFireball fireball = dynamic_cast<pMarioFireball>(
				_game->Create(
					fireballNode.attribute("actor").as_uint(),
					fireballName,
					fireballNode.attribute("source").as_string(),
					_x + fireOffsetX, _y + FIRE_OFFSETY, _gx, _gy, _layer
				)
				);

			if (_left) fireball->SetVelocity(-fireball->INITIAL_FORCE, 0);
			else fireball->SetVelocity(fireball->INITIAL_FORCE, 0);

			break;
		}
	}
}

void CMario::Hit()
{
	if (_power == EPower::SMALL)
	{
		SetNextAction(EAction::DIE);
	}
	else if (_power == EPower::LARGE)
	{
		SetNextAction(EAction::SHRINK);
	}
	else if (_power == EPower::FIRE || _power == EPower::RACCOON)
	{
		SetNextAction(EAction::POWER_DOWN);
	}

	_invincible = true;
	_invincibleTimeout = INVINCIBLE_TIMEOUT;
}

void CMario::CameraControl()
{
	_game->MoveCameraTo(
		_x - _game->GetGraphics()->GetBackBufferWidth() / 2,
		_y + _game->GetGraphics()->GetBackBufferHeight() / 2
	);
}

void CMario::PowerControl()
{
	if (_game->IsKeyPressed(HOT1))
		SetNextAction(EAction::SHRINK);
	else if (_game->IsKeyPressed(HOT2))
		SetNextAction(EAction::GROW);
	else if (_game->IsKeyPressed(HOT3))
		SetNextAction(EAction::POWER_FIRE);
	else if (_game->IsKeyPressed(HOT4))
		SetNextAction(EAction::POWER_TAIL);
}

int CMario::IsCollidable()
{
	switch (_action)
	{
	case CMario::EAction::IDLE:
	case CMario::EAction::CROUNCH:
	case CMario::EAction::MOVE:
	case CMario::EAction::DRIFT:
	case CMario::EAction::TURN:
	case CMario::EAction::JUMP:
	case CMario::EAction::FLY:
	case CMario::EAction::HOVER:
	case CMario::EAction::KICK:
	case CMario::EAction::SPIN:
	case CMario::EAction::FIRE:
	case CMario::EAction::GROW:
	case CMario::EAction::SHRINK:
	case CMario::EAction::POWER_FIRE:
	case CMario::EAction::POWER_TAIL:
	case CMario::EAction::POWER_DOWN:
	case CMario::EAction::CLEAR:
		return 1;
		break;

	case CMario::EAction::ENTRY:
	case CMario::EAction::EXIT:
	case CMario::EAction::DIE:
	default:
		return 0;
		break;
	}
}

int CMario::IsBlocking()
{
	return 0;
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	switch (_power)
	{
	case CMario::EPower::SMALL:
	{
		switch (_action)
		{
		case CMario::EAction::IDLE:
		case CMario::EAction::CROUNCH:
		case CMario::EAction::MOVE:
		case CMario::EAction::DRIFT:
		case CMario::EAction::TURN:
		case CMario::EAction::JUMP:
		case CMario::EAction::FLY:
		case CMario::EAction::HOVER:
		case CMario::EAction::KICK:
		case CMario::EAction::SPIN:
		case CMario::EAction::FIRE:
		case CMario::EAction::GROW:
		case CMario::EAction::SHRINK:
		case CMario::EAction::POWER_FIRE:
		case CMario::EAction::POWER_TAIL:
		case CMario::EAction::POWER_DOWN:
		case CMario::EAction::CLEAR:
		{
			left = _x + SMALL_BODY_OFFSETX - (SMALL_BODY_WIDTH / 2);
			right = _x + SMALL_BODY_OFFSETX + (SMALL_BODY_WIDTH / 2);
			top = _y + SMALL_BODY_OFFSETY + (SMALL_BODY_HEIGHT / 2);
			bottom = _y + SMALL_BODY_OFFSETY - (SMALL_BODY_HEIGHT / 2);
		}
		break;
		}
	}
	break;

	case CMario::EPower::LARGE:
	case CMario::EPower::FIRE:
	case CMario::EPower::RACCOON:
	{
		switch (_action)
		{
		case CMario::EAction::IDLE:
		case CMario::EAction::MOVE:
		case CMario::EAction::JUMP:
		case CMario::EAction::DRIFT:
		case CMario::EAction::TURN:
		case CMario::EAction::FLY:
		case CMario::EAction::HOVER:
		case CMario::EAction::KICK:
		case CMario::EAction::SPIN:
		case CMario::EAction::FIRE:
		case CMario::EAction::GROW:
		case CMario::EAction::SHRINK:
		case CMario::EAction::POWER_FIRE:
		case CMario::EAction::POWER_TAIL:
		case CMario::EAction::POWER_DOWN:
		case CMario::EAction::CLEAR:
		{
			left = _x + LARGE_BODY_OFFSETX - (LARGE_BODY_WIDTH / 2);
			right = _x + LARGE_BODY_OFFSETX + (LARGE_BODY_WIDTH / 2);
			top = _y + LARGE_BODY_OFFSETY + (LARGE_BODY_HEIGHT / 2);
			bottom = _y + LARGE_BODY_OFFSETY - (LARGE_BODY_HEIGHT / 2);
		}
		break;

		case CMario::EAction::CROUNCH:
			left = _x + SMALL_BODY_OFFSETX - (SMALL_BODY_WIDTH / 2);
			right = _x + SMALL_BODY_OFFSETX + (SMALL_BODY_WIDTH / 2);
			top = _y + SMALL_BODY_OFFSETY + (SMALL_BODY_HEIGHT / 2);
			bottom = _y + SMALL_BODY_OFFSETY - (SMALL_BODY_HEIGHT / 2);
			break;
		}
	}
	break;
	}
}

void CMario::OnNoCollision(float elapsedMs)
{
	_x += _vx * elapsedMs;
	_y += _vy * elapsedMs;
}

void CMario::OnCollisionWith(pCollision collision)
{
	/* Enemies */
	if (dynamic_cast<pGoomba>(collision->_target))
		OnCollisionWithGoomba(collision);

	else if (dynamic_cast<pGoombaMicro>(collision->_target))
		OnCollisionWithGoombaMicro(collision);

	else if (dynamic_cast<pKoopa>(collision->_target))
		OnCollisionWithKoopa(collision);

	else if (dynamic_cast<pPlant>(collision->_target))
		OnCollisionWithPlant(collision);

	else if (dynamic_cast<pPlantFireball>(collision->_target))
		OnCollisionWithPlantFireball(collision);

	/* Props */
	else if (dynamic_cast<pPlatform>(collision->_target))
		OnCollisionWithPlatform(collision);

	else if (dynamic_cast<pBlock>(collision->_target))
		OnCollisionWithBlock(collision);

	else if (dynamic_cast<pBrick>(collision->_target))
		OnCollisionWithBrick(collision);

	else if (dynamic_cast<pPipe>(collision->_target))
		OnCollisionWithPipe(collision);

	else if (dynamic_cast<pTransportPipe>(collision->_target))
		OnCollisionWithTransportPipe(collision);

	else if (dynamic_cast<pRelay>(collision->_target))
		OnCollisionWithRelay(collision);

	else if (dynamic_cast<pDeadZone>(collision->_target))
		OnCollisionWithDeadZone(collision);

	else if (dynamic_cast<pGoal>(collision->_target))
		OnCollisionWithGoal(collision);

	/* Items */
	else if (dynamic_cast<pCoin>(collision->_target))
		OnCollisionWithCoin(collision);

	else if (dynamic_cast<pSuperMushroom>(collision->_target))
		OnCollisionWithSuperMushroom(collision);

	else if (dynamic_cast<pSuperLeaf>(collision->_target))
		OnCollisionWithSuperLeaf(collision);

	else if (dynamic_cast<pFireFlower>(collision->_target))
		OnCollisionWithFireFlower(collision);

	else if (dynamic_cast<pExtraLifeMushroom>(collision->_target))
		OnCollisionWithExtraLifeMushroom(collision);
}

void CMario::OnCollisionWithGoomba(pCollision collision)
{
	auto goomba = dynamic_cast<pGoomba>(collision->_target);
	if (collision->_ny > 0)
	{
		goomba->Stomped();
		SetNextAction(EAction::JUMP);
	}
	else if (collision->_nx != 0 || collision->_ny <= 0)
	{
		if (!_invincible) Hit();
	}
}

void CMario::OnCollisionWithGoombaMicro(pCollision collision)
{

}

void CMario::OnCollisionWithKoopa(pCollision collision)
{
	auto koopa = dynamic_cast<pKoopa>(collision->_target);

}

void CMario::OnCollisionWithPlant(pCollision collision)
{
	auto plant = dynamic_cast<pPlant>(collision->_target);

}

void CMario::OnCollisionWithPlantFireball(pCollision collision)
{
	auto plantFireball = dynamic_cast<pPlantFireball>(collision->_target);

}

void CMario::OnCollisionWithPlatform(pCollision collision)
{
	auto platform = dynamic_cast<pPlatform>(collision->_target);
	if (platform->_solid)
	{
		if (collision->_ny != 0 && collision->_target->IsBlocking())
		{
			_vy = 0;
			if (collision->_ny > 0)
				_ground = true;
		}

		if (collision->_nx != 0 && collision->_target->IsBlocking())
		{
			_vx = 0;
		}
	}
	else
	{
		if (collision->_ny > 0 && !collision->_target->IsBlocking())
		{
			float top = 0;
			float temp = 0;
			collision->_target->GetBoundingBox(temp, top, temp, temp);

			_y = top + BLOCK_PUSH_FACTOR;
			_vy = 0;
			_ground = true;
		}
	}
}

void CMario::OnCollisionWithBlock(pCollision collision)
{
	auto block = dynamic_cast<pBlock>(collision->_target);
	if (collision->_ny > 0 && collision->_target->IsBlocking())
	{
		_vy = 0;
		_ground = true;
	}
	else if (collision->_ny < 0 && collision->_target->IsBlocking())
	{
		_jumpLimit = 0;
		_fall = true;
		_vy = -_vy;

		block->HitBottom();
	}

	if (collision->_nx != 0 && collision->_target->IsBlocking())
	{
		_vx = 0;
	}
}

void CMario::OnCollisionWithBrick(pCollision collision)
{
	auto brick = dynamic_cast<pBrick>(collision->_target);

}

void CMario::OnCollisionWithPipe(pCollision collision)
{
	auto pipe = dynamic_cast<pPipe>(collision->_target);

}

void CMario::OnCollisionWithTransportPipe(pCollision collision)
{
	auto transportPipe = dynamic_cast<pTransportPipe>(collision->_target);

}

void CMario::OnCollisionWithRelay(pCollision collision)
{
	auto relay = dynamic_cast<pRelay>(collision->_target);

}

void CMario::OnCollisionWithDeadZone(pCollision collision)
{
	auto deadZone = dynamic_cast<pDeadZone>(collision->_target);

}

void CMario::OnCollisionWithGoal(pCollision collision)
{
	auto goal = dynamic_cast<pGoal>(collision->_target);

}

void CMario::OnCollisionWithCoin(pCollision collision)
{
	auto coin = dynamic_cast<pCoin>(collision->_target);

}

void CMario::OnCollisionWithSuperMushroom(pCollision collision)
{
	auto superMushroom = dynamic_cast<pSuperMushroom>(collision->_target);

}

void CMario::OnCollisionWithSuperLeaf(pCollision collision)
{
	auto superLeaf = dynamic_cast<pSuperLeaf>(collision->_target);

}

void CMario::OnCollisionWithFireFlower(pCollision collision)
{
	auto fireFlower = dynamic_cast<pFireFlower>(collision->_target);

}

void CMario::OnCollisionWithExtraLifeMushroom(pCollision collision)
{
	auto extraLifeMushroom = dynamic_cast<pExtraLifeMushroom>(collision->_target);

}
