#pragma region INCLUDE
#include "Mario.h"
#include "../SuperMarioBros3.h"

#include "Goomba.h"
#include "GoombaMicro.h"

#include "Platform.h"
#include "Block.h"
#include "Brick.h"
#include "Pipe.h"

#include"../Engine/Framework/Debug.h"
#pragma endregion

void CMario::Load()
{
	CGameObject::Load();

	/* Read file */
	pugi::xml_document prefab;
	prefab.load_file(_source.c_str());

	/* Cheat */
	pugi::xml_node cheat = prefab.child("Prefab").child("Cheat");
	_cheat = cheat.attribute("cheat").as_bool();
	HOTKEY_SMALL = cheat.attribute("HOTKEY_SMALL").as_int();
	HOTKEY_LARGE = cheat.attribute("HOTKEY_LARGE").as_int();
	HOTKEY_FIRE = cheat.attribute("HOTKEY_FIRE").as_int();
	HOTKEY_RACCOON = cheat.attribute("HOTKEY_RACCOON").as_int();

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

	/* Gravity */
	pugi::xml_node gravity = prefab.child("Prefab").child("Gravity");
	GRAVITY = gravity.attribute("GRAVITY").as_float();
	DRAG = gravity.attribute("DRAG").as_float();

	/* Move */
	pugi::xml_node move = prefab.child("Prefab").child("Move");
	WALK_ACCELERATION = move.attribute("WALK_ACCELERATION").as_float();
	WALK_SPEED_LIMIT = move.attribute("WALK_SPEED_LIMIT").as_float();
	RUN_ACCELERATION = move.attribute("RUN_ACCELERATION").as_float();
	RUN_SPEED_LIMIT = move.attribute("RUN_SPEED_LIMIT").as_float();
	FULL_ACCELERATION = move.attribute("FULL_ACCELERATION").as_float();
	FULL_SPEED_LIMIT = move.attribute("FULL_SPEED_LIMIT").as_float();
	IDLE_THRESHOLD = move.attribute("IDLE_THRESHOLD").as_float();
	DRIFT_THRESHOLD = move.attribute("DRIFT_THRESHOLD").as_float();

	/* Momentum */
	pugi::xml_node momentum = prefab.child("Prefab").child("Momentum");
	MOMENTUM_INCREASE_RATE = momentum.attribute("MOMENTUM_INCREASE_RATE").as_float();
	MOMENTUM_DECREASE_RATE = momentum.attribute("MOMENTUM_DECREASE_RATE").as_float();
	MOMENTUM_THRESHOLD = momentum.attribute("MOMENTUM_THRESHOLD").as_float();
	MOMENTUM_LIMIT = momentum.attribute("MOMENTUM_LIMIT").as_float();

	/* Jump */
	pugi::xml_node jump = prefab.child("Prefab").child("Jump");
	JUMP_FORCE = jump.attribute("JUMP_FORCE").as_float();
	FULL_SPEED_JUMP_FORCE = jump.attribute("FULL_SPEED_JUMP_FORCE").as_float();
	JUMP_LIMIT = jump.attribute("JUMP_LIMIT").as_float();

	/* Hover */
	pugi::xml_node hover = prefab.child("Prefab").child("Hover");
	HOVER_GRAVITY = hover.attribute("HOVER_GRAVITY").as_float();
	HOVER_COUNTDOWN = hover.attribute("HOVER_COUNTDOWN").as_float();
	HOVER_ACCELERATION = hover.attribute("HOVER_ACCELERATION").as_float();
	HOVER_SPEED_LIMIT = hover.attribute("HOVER_SPEED_LIMIT").as_float();

	/* Fly */
	pugi::xml_node fly = prefab.child("Prefab").child("Fly");
	FLY_GRAVITY = fly.attribute("FLY_GRAVITY").as_float();
	FLY_COUNTDOWN = fly.attribute("FLY_COUNTDOWN").as_float();
	FLY_TIMEOUT_LIMIT = fly.attribute("FLY_TIMEOUT_LIMIT").as_float();
	FLY_ACCELERATION = fly.attribute("FLY_ACCELERATION").as_float();
	FLY_SPEED_LIMIT = fly.attribute("FLY_SPEED_LIMIT").as_float();

	/* Invincible */
	pugi::xml_node invincible = prefab.child("Prefab").child("Invincible");
	INVINCIBLE_TIMEOUT = invincible.attribute("INVINCIBLE_TIMEOUT").as_float();

	/* Hit */
	pugi::xml_node hit = prefab.child("Prefab").child("Hit");
	HIT_DEFLECT_HORIZONTAL_FORCE = hit.attribute("HIT_DEFLECT_HORIZONTAL_FORCE").as_float();
	HIT_DEFLECT_VERTICAL_FORCE = hit.attribute("HIT_DEFLECT_VERTICAL_FORCE").as_float();

	/* Tranformation */
	pugi::xml_node transformation = prefab.child("Prefab").child("Transformation");
	TRANSFORMATION_TIMEOUT = transformation.attribute("TRANSFORMATION_TIMEOUT").as_float();
}

void CMario::Start()
{
	CGameObject::Start();
}

void CMario::Update(float elapsedMs)
{
	if (!_start) Start();

	UpdateGravity(elapsedMs);
	UpdateMomentum(elapsedMs);
	UpdateFlyTimeout(elapsedMs);
	UpdateInvincible(elapsedMs);

	if (_vy < -BLOCK_PUSH_FACTOR * elapsedMs / 100) _ground = false;

	std::vector<pGameObject> collidables = _game->GetLocal(_id);
	_collider->Process(elapsedMs, &collidables);

	DebugOut(L"Grounded: %d | vy: %f \n", _ground, _vy);

	HandleAction(elapsedMs);

	/* Dev helper */
	//DebugOut(L"vx %f | momentum %f \n", _vx, _momentum);
	CameraControl();
	if (_cheat) PowerControlCheat();
}

void CMario::Render()
{
	if (_renderSmallBody) _sprites[SPR_MARIO_S_BBOX]->Render(_x, _y);
	if (_renderLargeBody) _sprites[SPR_MARIO_L_BBOX]->Render(_x, _y);

	if (_blink)
	{
		return;
	}

	switch (_action)
	{
	case CMario::EAction::IDLE:
	{
		switch (_power)
		{
		case CMario::EPower::SMALL:
		{
			if (_left)	_sprites[SPR_MARIO_S_IDLE_LEFT]->Render(_x, _y);
			else		_sprites[SPR_MARIO_S_IDLE_RIGHT]->Render(_x, _y);
		}
		break;
		case CMario::EPower::LARGE:
		{
			if (_left)	_sprites[SPR_MARIO_L_IDLE_LEFT]->Render(_x, _y);
			else		_sprites[SPR_MARIO_L_IDLE_RIGHT]->Render(_x, _y);
		}
		break;
		case CMario::EPower::FIRE:
		{
			if (_left)	_sprites[SPR_MARIO_F_IDLE_LEFT]->Render(_x, _y);
			else		_sprites[SPR_MARIO_F_IDLE_RIGHT]->Render(_x, _y);
		}
		break;
		case CMario::EPower::RACCOON:
		{
			if (_left)	_sprites[SPR_MARIO_R_IDLE_LEFT]->Render(_x, _y);
			else		_sprites[SPR_MARIO_R_IDLE_RIGHT]->Render(_x, _y);
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

	case CMario::EAction::WALK:
	{
		switch (_power)
		{
		case CMario::EPower::SMALL:
		{
			if (_left)	_animations[ANI_MARIO_S_WALK_LEFT]->Render(_x, _y);
			else		_animations[ANI_MARIO_S_WALK_RIGHT]->Render(_x, _y);
		}
		break;
		case CMario::EPower::LARGE:
		{
			if (_left)	_animations[ANI_MARIO_L_WALK_LEFT]->Render(_x, _y);
			else		_animations[ANI_MARIO_L_WALK_RIGHT]->Render(_x, _y);
		}
		break;
		case CMario::EPower::FIRE:
		{
			if (_left)	_animations[ANI_MARIO_F_WALK_LEFT]->Render(_x, _y);
			else		_animations[ANI_MARIO_F_WALK_RIGHT]->Render(_x, _y);
		}
		break;
		case CMario::EPower::RACCOON:
		{
			if (_left)	_animations[ANI_MARIO_R_WALK_LEFT]->Render(_x, _y);
			else		_animations[ANI_MARIO_R_WALK_RIGHT]->Render(_x, _y);
		}
		break;
		}
	}
	break;

	case CMario::EAction::RUN:
	{
		switch (_power)
		{
		case CMario::EPower::SMALL:
		{
			if (_fullSpeed)
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
		break;
		case CMario::EPower::LARGE:
		{
			if (_fullSpeed)
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
		break;
		case CMario::EPower::FIRE:
		{
			if (_fullSpeed)
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
		break;
		case CMario::EPower::RACCOON:
		{
			if (_fullSpeed)
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

	case CMario::EAction::JUMP:
	{
		switch (_power)
		{
		case CMario::EPower::SMALL:
		{
			if (_fullSpeed)
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
		break;
		case CMario::EPower::LARGE:
		{
			if (_fullSpeed)
			{
				if (_left)	_sprites[SPR_MARIO_L_FLY_LEFT]->Render(_x, _y);
				else		_sprites[SPR_MARIO_L_FLY_RIGHT]->Render(_x, _y);
			}
			else
			{
				if (_falling)
				{
					if (_left)	_sprites[SPR_MARIO_L_WALK2_LEFT]->Render(_x, _y);
					else		_sprites[SPR_MARIO_L_WALK2_RIGHT]->Render(_x, _y);
				}
				else
				{
					if (_left)	_sprites[SPR_MARIO_L_JUMP_LEFT]->Render(_x, _y);
					else		_sprites[SPR_MARIO_L_JUMP_RIGHT]->Render(_x, _y);
				}
			}
		}
		break;
		case CMario::EPower::FIRE:
		{
			if (_fullSpeed)
			{
				if (_left)	_sprites[SPR_MARIO_F_FLY_LEFT]->Render(_x, _y);
				else		_sprites[SPR_MARIO_F_FLY_RIGHT]->Render(_x, _y);
			}
			else
			{
				if (_falling)
				{
					if (_left)	_sprites[SPR_MARIO_F_WALK2_LEFT]->Render(_x, _y);
					else		_sprites[SPR_MARIO_F_WALK2_RIGHT]->Render(_x, _y);
				}
				else
				{
					if (_left)	_sprites[SPR_MARIO_F_JUMP_LEFT]->Render(_x, _y);
					else		_sprites[SPR_MARIO_F_JUMP_RIGHT]->Render(_x, _y);
				}
			}
		}
		break;
		case CMario::EPower::RACCOON:
		{
			if (_fullSpeed)
			{
				if (_left)	_sprites[SPR_MARIO_R_FLY1_LEFT]->Render(_x, _y);
				else		_sprites[SPR_MARIO_R_FLY1_RIGHT]->Render(_x, _y);
			}
			else
			{
				if (_falling)
				{
					if (_left)	_sprites[SPR_MARIO_R_WALK2_LEFT]->Render(_x, _y);
					else		_sprites[SPR_MARIO_R_WALK2_RIGHT]->Render(_x, _y);
				}
				else
				{
					if (_left)	_sprites[SPR_MARIO_R_JUMP_LEFT]->Render(_x, _y);
					else		_sprites[SPR_MARIO_R_JUMP_RIGHT]->Render(_x, _y);
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
		switch (_power)
		{
		case CMario::EPower::RACCOON:
		{
			if (_left)	_animations[ANI_MARIO_R_ATTACK_LEFT]->Render(_x, _y);
			else		_animations[ANI_MARIO_R_ATTACK_RIGHT]->Render(_x, _y);
		}
		break;
		}
	}
	break;

	case CMario::EAction::FIRE:
	{
		switch (_power)
		{
		case CMario::EPower::FIRE:
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
		switch (_power)
		{
		case CMario::EPower::LARGE:
		{
			if (_left)	_animations[ANI_MARIO_SHRINK_LEFT]->Render(_x, _y);
			else		_animations[ANI_MARIO_SHRINK_RIGHT]->Render(_x, _y);

		}
		break;
		}
	}
	break;

	case CMario::EAction::POWER_FIRE:
	{
		switch (_power)
		{
		case CMario::EPower::LARGE:
		case CMario::EPower::RACCOON:
		{
			if (_left)	_animations[ANI_MARIO_TRANSFORM_LEFT]->Render(_x, _y);
			else		_animations[ANI_MARIO_TRANSFORM_RIGHT]->Render(_x, _y);
		}
		break;
		}
	}
	break;

	case CMario::EAction::POWER_TAIL:
	{
		switch (_power)
		{
		case CMario::EPower::LARGE:
		case CMario::EPower::FIRE:
		{
			if (_left)	_animations[ANI_MARIO_TRANSFORM_LEFT]->Render(_x, _y);
			else		_animations[ANI_MARIO_TRANSFORM_RIGHT]->Render(_x, _y);
		}
		break;
		}
	}
	break;

	case CMario::EAction::POWER_DOWN:
	{
		switch (_power)
		{
		case CMario::EPower::RACCOON:
		case CMario::EPower::FIRE:
		{
			if (_left)	_animations[ANI_MARIO_TRANSFORM_LEFT]->Render(_x, _y);
			else		_animations[ANI_MARIO_TRANSFORM_RIGHT]->Render(_x, _y);
		}
		break;
		}
	}
	break;

	case CMario::EAction::DIE:
	{
		switch (_power)
		{
		case CMario::EPower::SMALL:
		{
			_sprites[SPR_MARIO_S_DIE]->Render(_x, _y);
		}
		break;
		}
	}
	break;

	}

}

#pragma region STATE MACHINE

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

	case CMario::EAction::WALK:
		Walk(elapsedMs);
		break;

	case CMario::EAction::RUN:
		Run(elapsedMs);
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

	case CMario::EAction::FALL:
		Fall(elapsedMs);
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
		if (_game->IsKeyDown(DOWN) && _power != EPower::SMALL)
		{
			SetNextAction(EAction::CROUNCH);
			break;
		}

		if (_game->IsKeyDown(LEFT) && !_game->IsKeyDown(RIGHT))
		{
			_left = true;
			if (_game->IsKeyDown(ACTION))
			{
				SetNextAction(EAction::RUN);
				break;
			}
			else
			{
				SetNextAction(EAction::WALK);
				break;
			}
		}
		else if (_game->IsKeyDown(RIGHT) && !_game->IsKeyDown(LEFT))
		{
			_left = false;
			if (_game->IsKeyDown(ACTION))
			{
				SetNextAction(EAction::RUN);
				break;
			}
			else
			{
				SetNextAction(EAction::WALK);
				break;
			}
		}

		if (_ground && _game->IsKeyPressed(JUMP))
		{
			SetNextAction(EAction::JUMP);
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
		if (!_game->IsKeyDown(DOWN) && _ground)
		{
			SetNextAction(EAction::IDLE);
			break;
		}

		if (_game->IsKeyPressed(JUMP) && _ground)
		{
			_crounchJump = true;
			_fall = false;
			_ground = false;
			_jumpLimit = JUMP_LIMIT;
			_vy = JUMP_FORCE;
		}

		if (_crounchJump)
		{
			if (!_fall)
			{
				if (_game->IsKeyReleased(JUMP)) _fall = true;

				if (_game->IsKeyDown(JUMP) && _jumpLimit > 0)
				{
					_jumpLimit -= elapsedMs;
					_vy = JUMP_FORCE;
				}
				else _fall = true;
			}
		}

		if (_ground) _crounchJump = false;
	}
	break;

	case CMario::EActionStage::EXIT:
	{

	}
	NextAction();
	break;
	}
}

void CMario::Walk(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMario::EActionStage::ENTRY:
	{
		/* Animation Start */
		switch (_power)
		{
		case CMario::EPower::SMALL:
		{
			_animations[ANI_MARIO_S_WALK_LEFT]->Play(true);
			_animations[ANI_MARIO_S_WALK_RIGHT]->Play(true);
		}
		break;

		case CMario::EPower::LARGE:
		{
			_animations[ANI_MARIO_L_WALK_LEFT]->Play(true);
			_animations[ANI_MARIO_L_WALK_RIGHT]->Play(true);
		}
		break;

		case CMario::EPower::FIRE:
		{
			_animations[ANI_MARIO_F_WALK_LEFT]->Play(true);
			_animations[ANI_MARIO_F_WALK_RIGHT]->Play(true);
		}
		break;

		case CMario::EPower::RACCOON:
		{
			_animations[ANI_MARIO_R_WALK_LEFT]->Play(true);
			_animations[ANI_MARIO_R_WALK_RIGHT]->Play(true);
		}
		break;
		}
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{

		if (_game->IsKeyDown(DOWN) && _power != EPower::SMALL)
		{
			SetNextAction(EAction::CROUNCH);
			break;
		}

		if (_left && _game->IsKeyDown(RIGHT) && _vx < -DRIFT_THRESHOLD)
		{
			_left = false;
			SetNextAction(EAction::DRIFT);
			break;
		}
		else if (!_left && _game->IsKeyDown(LEFT) && _vx > DRIFT_THRESHOLD)
		{
			_left = true;
			SetNextAction(EAction::DRIFT);
			break;
		}

		if (_game->IsKeyDown(LEFT) && !_game->IsKeyDown(RIGHT))
		{
			_left = true;
			if (_game->IsKeyDown(ACTION))
			{
				SetNextAction(EAction::RUN);
				break;
			}
			else
			{
				if (_vx > -WALK_SPEED_LIMIT)
				{
					_vx -= WALK_ACCELERATION * elapsedMs;
					if (_vx < -WALK_SPEED_LIMIT) _vx = -WALK_SPEED_LIMIT;
				}
				else _vx -= WALK_ACCELERATION * elapsedMs;
				_walkSpeedFactor = abs(_vx / RUN_SPEED_LIMIT);
			}
		}
		else if (_game->IsKeyDown(RIGHT) && !_game->IsKeyDown(LEFT))
		{
			_left = false;
			if (_game->IsKeyDown(ACTION))
			{
				SetNextAction(EAction::RUN);
				break;
			}
			else
			{
				if (_vx < WALK_SPEED_LIMIT)
				{
					_vx += WALK_ACCELERATION * elapsedMs;
					if (_vx > WALK_SPEED_LIMIT) _vx = WALK_SPEED_LIMIT;
				}
				else _vx += WALK_ACCELERATION * elapsedMs;
				_walkSpeedFactor = abs(_vx / RUN_SPEED_LIMIT);
			}
		}

		if ((_left && _vx > -IDLE_THRESHOLD) || (!_left && _vx < IDLE_THRESHOLD))
		{
			SetNextAction(EAction::IDLE);
			break;
		}

		if (_ground && _game->IsKeyPressed(JUMP))
		{
			SetNextAction(EAction::JUMP);
			break;
		}

		/* Animation Update */
		switch (_power)
		{
		case CMario::EPower::SMALL:
		{
			_animations[ANI_MARIO_S_WALK_LEFT]->Update(elapsedMs * _walkSpeedFactor);
			_animations[ANI_MARIO_S_WALK_RIGHT]->Update(elapsedMs * _walkSpeedFactor);
		}
		break;

		case CMario::EPower::LARGE:
		{
			_animations[ANI_MARIO_L_WALK_LEFT]->Update(elapsedMs * _walkSpeedFactor);
			_animations[ANI_MARIO_L_WALK_RIGHT]->Update(elapsedMs * _walkSpeedFactor);
		}
		break;

		case CMario::EPower::FIRE:
		{
			_animations[ANI_MARIO_F_WALK_LEFT]->Update(elapsedMs * _walkSpeedFactor);
			_animations[ANI_MARIO_F_WALK_RIGHT]->Update(elapsedMs * _walkSpeedFactor);
		}
		break;

		case CMario::EPower::RACCOON:
		{
			_animations[ANI_MARIO_R_WALK_LEFT]->Update(elapsedMs * _walkSpeedFactor);
			_animations[ANI_MARIO_R_WALK_RIGHT]->Update(elapsedMs * _walkSpeedFactor);
		}
		break;
		}
	}
	break;

	case CMario::EActionStage::EXIT:
	{
		/* Animation Stop */
		switch (_power)
		{
		case CMario::EPower::SMALL:
		{
			_animations[ANI_MARIO_S_WALK_LEFT]->Stop();
			_animations[ANI_MARIO_S_WALK_RIGHT]->Stop();
		}
		break;

		case CMario::EPower::LARGE:
		{
			_animations[ANI_MARIO_L_WALK_LEFT]->Stop();
			_animations[ANI_MARIO_L_WALK_RIGHT]->Stop();
		}
		break;

		case CMario::EPower::FIRE:
		{
			_animations[ANI_MARIO_F_WALK_LEFT]->Stop();
			_animations[ANI_MARIO_F_WALK_RIGHT]->Stop();
		}
		break;

		case CMario::EPower::RACCOON:
		{
			_animations[ANI_MARIO_R_WALK_LEFT]->Stop();
			_animations[ANI_MARIO_R_WALK_RIGHT]->Stop();
		}
		break;
		}
	}
	NextAction();
	break;
	}
}

void CMario::Run(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMario::EActionStage::ENTRY:
	{
		/* Animation Start */
		switch (_power)
		{
		case CMario::EPower::SMALL:
		{
			_animations[ANI_MARIO_S_WALK_LEFT]->Play(true);
			_animations[ANI_MARIO_S_WALK_RIGHT]->Play(true);
			_animations[ANI_MARIO_S_RUN_LEFT]->Play(true);
			_animations[ANI_MARIO_S_RUN_RIGHT]->Play(true);
		}
		break;

		case CMario::EPower::LARGE:
		{
			_animations[ANI_MARIO_L_WALK_LEFT]->Play(true);
			_animations[ANI_MARIO_L_WALK_RIGHT]->Play(true);
			_animations[ANI_MARIO_L_RUN_LEFT]->Play(true);
			_animations[ANI_MARIO_L_RUN_RIGHT]->Play(true);
		}
		break;

		case CMario::EPower::FIRE:
		{
			_animations[ANI_MARIO_F_WALK_LEFT]->Play(true);
			_animations[ANI_MARIO_F_WALK_RIGHT]->Play(true);
			_animations[ANI_MARIO_F_RUN_LEFT]->Play(true);
			_animations[ANI_MARIO_F_RUN_RIGHT]->Play(true);
		}
		break;

		case CMario::EPower::RACCOON:
		{
			_animations[ANI_MARIO_R_WALK_LEFT]->Play(true);
			_animations[ANI_MARIO_R_WALK_RIGHT]->Play(true);
			_animations[ANI_MARIO_R_RUN_LEFT]->Play(true);
			_animations[ANI_MARIO_R_RUN_RIGHT]->Play(true);
		}
		break;
		}
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{
		if (_game->IsKeyDown(DOWN) && _power != EPower::SMALL)
		{
			SetNextAction(EAction::CROUNCH);
			break;
		}

		if (_left && _game->IsKeyDown(RIGHT) && _vx < -DRIFT_THRESHOLD)
		{
			_left = false;
			SetNextAction(EAction::DRIFT);
			break;
		}
		else if (!_left && _game->IsKeyDown(LEFT) && _vx > DRIFT_THRESHOLD)
		{
			_left = true;
			SetNextAction(EAction::DRIFT);
			break;
		}

		if (_game->IsKeyDown(LEFT) && !_game->IsKeyDown(RIGHT))
		{
			_left = true;
			if (!_game->IsKeyDown(ACTION))
			{
				SetNextAction(EAction::WALK);
				break;
			}
			else
			{
				if (_fullSpeed)
				{
					if (_vx > -FULL_SPEED_LIMIT)
					{
						_vx -= FULL_ACCELERATION * elapsedMs;
						if (_vx < -FULL_SPEED_LIMIT) _vx = -FULL_SPEED_LIMIT;
					}
					else _vx -= FULL_ACCELERATION * elapsedMs;
				}
				else
				{
					if (_vx > -RUN_SPEED_LIMIT)
					{
						_vx -= RUN_ACCELERATION * elapsedMs;
						if (_vx < -RUN_SPEED_LIMIT) _vx = -RUN_SPEED_LIMIT;
					}
					else _vx -= RUN_ACCELERATION * elapsedMs;
				}
				_walkSpeedFactor = abs(_vx / WALK_SPEED_LIMIT);
			}
		}
		else if (_game->IsKeyDown(RIGHT) && !_game->IsKeyDown(LEFT))
		{
			_left = false;
			if (!_game->IsKeyDown(ACTION))
			{
				SetNextAction(EAction::WALK);
				break;
			}
			else
			{
				if (_fullSpeed)
				{
					if (_vx < FULL_SPEED_LIMIT)
					{
						_vx += FULL_ACCELERATION * elapsedMs;
						if (_vx > FULL_SPEED_LIMIT) _vx = FULL_SPEED_LIMIT;
					}
					else _vx += FULL_ACCELERATION * elapsedMs;
				}
				else
				{
					if (_vx < RUN_SPEED_LIMIT)
					{
						_vx += RUN_ACCELERATION * elapsedMs;
						if (_vx > RUN_SPEED_LIMIT) _vx = RUN_SPEED_LIMIT;
					}
					else _vx += RUN_ACCELERATION * elapsedMs;
				}
				_walkSpeedFactor = abs(_vx / WALK_SPEED_LIMIT);
			}
		}

		if ((_left && _vx > -IDLE_THRESHOLD) || (!_left && _vx < IDLE_THRESHOLD))
		{
			SetNextAction(EAction::IDLE);
			break;
		}

		if (_ground && _game->IsKeyPressed(JUMP))
		{
			SetNextAction(EAction::JUMP);
			break;
		}

		/* Animation Update */
		switch (_power)
		{
		case CMario::EPower::SMALL:
		{
			_animations[ANI_MARIO_S_WALK_LEFT]->Update(elapsedMs * _walkSpeedFactor);
			_animations[ANI_MARIO_S_WALK_RIGHT]->Update(elapsedMs * _walkSpeedFactor);
			_animations[ANI_MARIO_S_RUN_LEFT]->Update(elapsedMs * _walkSpeedFactor);
			_animations[ANI_MARIO_S_RUN_RIGHT]->Update(elapsedMs * _walkSpeedFactor);
		}
		break;

		case CMario::EPower::LARGE:
		{
			_animations[ANI_MARIO_L_WALK_LEFT]->Update(elapsedMs * _walkSpeedFactor);
			_animations[ANI_MARIO_L_WALK_RIGHT]->Update(elapsedMs * _walkSpeedFactor);
			_animations[ANI_MARIO_L_RUN_LEFT]->Update(elapsedMs * _walkSpeedFactor);
			_animations[ANI_MARIO_L_RUN_RIGHT]->Update(elapsedMs * _walkSpeedFactor);
		}
		break;

		case CMario::EPower::FIRE:
		{
			_animations[ANI_MARIO_F_WALK_LEFT]->Update(elapsedMs * _walkSpeedFactor);
			_animations[ANI_MARIO_F_WALK_RIGHT]->Update(elapsedMs * _walkSpeedFactor);
			_animations[ANI_MARIO_F_RUN_LEFT]->Update(elapsedMs * _walkSpeedFactor);
			_animations[ANI_MARIO_F_RUN_RIGHT]->Update(elapsedMs * _walkSpeedFactor);
		}
		break;

		case CMario::EPower::RACCOON:
		{
			_animations[ANI_MARIO_R_WALK_LEFT]->Update(elapsedMs * _walkSpeedFactor);
			_animations[ANI_MARIO_R_WALK_RIGHT]->Update(elapsedMs * _walkSpeedFactor);
			_animations[ANI_MARIO_R_RUN_LEFT]->Update(elapsedMs * _walkSpeedFactor);
			_animations[ANI_MARIO_R_RUN_RIGHT]->Update(elapsedMs * _walkSpeedFactor);
		}
		break;
		}
	}
	break;

	case CMario::EActionStage::EXIT:
	{
		/* Animation Stop */
		switch (_power)
		{
		case CMario::EPower::SMALL:
		{
			_animations[ANI_MARIO_S_WALK_LEFT]->Stop();
			_animations[ANI_MARIO_S_WALK_RIGHT]->Stop();
			_animations[ANI_MARIO_S_RUN_LEFT]->Stop();
			_animations[ANI_MARIO_S_RUN_RIGHT]->Stop();
		}
		break;

		case CMario::EPower::LARGE:
		{
			_animations[ANI_MARIO_L_WALK_LEFT]->Stop();
			_animations[ANI_MARIO_L_WALK_RIGHT]->Stop();
			_animations[ANI_MARIO_L_RUN_LEFT]->Stop();
			_animations[ANI_MARIO_L_RUN_RIGHT]->Stop();
		}
		break;

		case CMario::EPower::FIRE:
		{
			_animations[ANI_MARIO_F_WALK_LEFT]->Stop();
			_animations[ANI_MARIO_F_WALK_RIGHT]->Stop();
			_animations[ANI_MARIO_F_RUN_LEFT]->Stop();
			_animations[ANI_MARIO_F_RUN_RIGHT]->Stop();
		}
		break;

		case CMario::EPower::RACCOON:
		{
			_animations[ANI_MARIO_R_WALK_LEFT]->Stop();
			_animations[ANI_MARIO_R_WALK_RIGHT]->Stop();
			_animations[ANI_MARIO_R_RUN_LEFT]->Stop();
			_animations[ANI_MARIO_R_RUN_RIGHT]->Stop();
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

		if ((_left && _vx < IDLE_THRESHOLD) || (!_left && _vx > -IDLE_THRESHOLD))
		{
			SetNextAction(EAction::IDLE);
			break;
		}

		if (!_left && _game->IsKeyDown(LEFT) && !_game->IsKeyDown(RIGHT))
		{
			_left = true;
			if (_game->IsKeyDown(ACTION))
			{
				SetNextAction(EAction::RUN);
				break;
			}
			else
			{
				SetNextAction(EAction::WALK);
				break;
			}
		}
		else if (_left && _game->IsKeyDown(RIGHT) && !_game->IsKeyDown(LEFT))
		{
			_left = false;
			if (_game->IsKeyDown(ACTION))
			{
				SetNextAction(EAction::RUN);
				break;
			}
			else
			{
				SetNextAction(EAction::WALK);
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

void CMario::Jump(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMario::EActionStage::ENTRY:
	{
		_fall = false;
		_falling = false;
		_ground = false;
		_jumpLimit = JUMP_LIMIT;
		if (_fullSpeed) _vy = FULL_SPEED_JUMP_FORCE;
		else _vy = JUMP_FORCE;

		/* Audio Play */
		//_soundClips[SC_MARIO_JUMP]->Play();
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{
		if (_vy < 0) _falling = true;

		if (!_fall)
		{
			if (_game->IsKeyReleased(JUMP)) _fall = true;

			if (_game->IsKeyDown(JUMP) && _jumpLimit > 0)
			{
				_jumpLimit -= elapsedMs;
				if (_fullSpeed) _vy = FULL_SPEED_JUMP_FORCE;
				else _vy = JUMP_FORCE;
			}
			else _fall = true;
		}

		if (_fullSpeed && _game->IsKeyPressed(JUMP) && _power == EPower::RACCOON)
		{
			_flyTimeout = FLY_TIMEOUT_LIMIT;
			SetNextAction(EAction::FLY);
			break;
		}

		if (_fall && _game->IsKeyPressed(JUMP) && _power == EPower::RACCOON)
		{
			SetNextAction(EAction::HOVER);
			break;
		}

		if (_game->IsKeyDown(LEFT) && !_game->IsKeyDown(RIGHT))
		{
			_left = true;

			if (_vx >= -WALK_SPEED_LIMIT)
			{
				_vx -= WALK_ACCELERATION * elapsedMs;
				if (_vx < -WALK_SPEED_LIMIT) _vx = -WALK_SPEED_LIMIT;
			}
			else if (_vx >= -RUN_SPEED_LIMIT)
			{
				_vx -= WALK_ACCELERATION * elapsedMs;
				if (_vx < -RUN_SPEED_LIMIT) _vx = -RUN_SPEED_LIMIT;
			}
			else if (_vx >= -FULL_SPEED_LIMIT)
			{
				_vx -= WALK_ACCELERATION * elapsedMs;
				if (_vx < -FULL_SPEED_LIMIT) _vx = -FULL_SPEED_LIMIT;
			}
		}
		else if (_game->IsKeyDown(RIGHT) && !_game->IsKeyDown(LEFT))
		{
			_left = false;

			if (_vx <= WALK_SPEED_LIMIT)
			{
				_vx += WALK_ACCELERATION * elapsedMs;
				if (_vx > WALK_SPEED_LIMIT) _vx = WALK_SPEED_LIMIT;
			}
			else if (_vx <= RUN_SPEED_LIMIT)
			{
				_vx += WALK_ACCELERATION * elapsedMs;
				if (_vx > RUN_SPEED_LIMIT) _vx = RUN_SPEED_LIMIT;
			}
			else if (_vx <= FULL_SPEED_LIMIT)
			{
				_vx += WALK_ACCELERATION * elapsedMs;
				if (_vx > FULL_SPEED_LIMIT) _vx = FULL_SPEED_LIMIT;
			}
		}

		if (_ground)
		{
			SetNextAction(EAction::IDLE);
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

void CMario::Fly(float elapsedMs)
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
		if (_ground)
		{
			SetNextAction(EAction::IDLE);
			break;
		}

		if (_flyTimeout > 0)
		{
			if (_game->IsKeyPressed(JUMP))
			{
				_fly = true;
				_flyCountdown = FLY_COUNTDOWN;

				/*Animation Start*/
				{
					_animations[ANI_MARIO_R_FLY_LEFT]->Play();
					_animations[ANI_MARIO_R_FLY_RIGHT]->Play();
				}
			}

			if (_flyCountdown > 0)
			{
				_flyCountdown -= elapsedMs;
			}
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

		if (_game->IsKeyDown(LEFT) && !_game->IsKeyDown(RIGHT))
		{
			_left = true;

			if (_vx >= -FLY_SPEED_LIMIT)
			{
				_vx -= FLY_ACCELERATION * elapsedMs;
				if (_vx < -FLY_SPEED_LIMIT) _vx = -FLY_SPEED_LIMIT;
			}
		}
		else if (_game->IsKeyDown(RIGHT) && !_game->IsKeyDown(LEFT))
		{
			_left = false;

			if (_vx <= FLY_SPEED_LIMIT)
			{
				_vx += FLY_ACCELERATION * elapsedMs;
				if (_vx > FLY_SPEED_LIMIT) _vx = FLY_SPEED_LIMIT;
			}
		}

		/* Animation Update */
		{
			_animations[ANI_MARIO_R_FLY_LEFT]->Update(elapsedMs);
			_animations[ANI_MARIO_R_FLY_RIGHT]->Update(elapsedMs);
		}
	}
	break;

	case CMario::EActionStage::EXIT:
	{
		/* Animation Update */
		{
			_animations[ANI_MARIO_R_FLY_LEFT]->Stop();
			_animations[ANI_MARIO_R_FLY_RIGHT]->Stop();
		}

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
		if (_ground)
		{
			_hover = false;
			SetNextAction(EAction::IDLE);
			break;
		}

		if (_game->IsKeyPressed(JUMP))
		{
			_hover = true;
			_hoverCountdown = HOVER_COUNTDOWN;

			/*Animation Start*/
			{
				_animations[ANI_MARIO_R_HOVER_LEFT]->Play();
				_animations[ANI_MARIO_R_HOVER_RIGHT]->Play();
			}
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

		if (_game->IsKeyDown(LEFT) && !_game->IsKeyDown(RIGHT))
		{
			_left = true;

			if (_vx >= -HOVER_SPEED_LIMIT)
			{
				_vx -= HOVER_ACCELERATION * elapsedMs;
				if (_vx < -HOVER_SPEED_LIMIT) _vx = -HOVER_SPEED_LIMIT;
			}
		}
		else if (_game->IsKeyDown(RIGHT) && !_game->IsKeyDown(LEFT))
		{
			_left = false;

			if (_vx <= HOVER_SPEED_LIMIT)
			{
				_vx += HOVER_ACCELERATION * elapsedMs;
				if (_vx > HOVER_SPEED_LIMIT) _vx = HOVER_SPEED_LIMIT;
			}
		}

		/*Animation Update */
		{
			_animations[ANI_MARIO_R_HOVER_LEFT]->Update(elapsedMs);
			_animations[ANI_MARIO_R_HOVER_RIGHT]->Update(elapsedMs);
		}
	}
	break;

	case CMario::EActionStage::EXIT:
	{
		/*Animation Stop */
		{
			_animations[ANI_MARIO_R_HOVER_LEFT]->Stop();
			_animations[ANI_MARIO_R_HOVER_RIGHT]->Stop();
		}
	}
	NextAction();
	break;
	}
}

void CMario::Fall(float elapsedMs)
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

void CMario::Kick(float elapsedMs)
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

void CMario::Spin(float elapsedMs)
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

void CMario::Fire(float elapsedMs)
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
			SetNextAction(EAction::IDLE);
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
			SetNextAction(EAction::IDLE);
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
			SetNextAction(EAction::IDLE);
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
			SetNextAction(EAction::IDLE);
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
			SetNextAction(EAction::IDLE);
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

void CMario::UpdateGravity(float elapsedMs)
{
	if (_fly) _vy = FLY_GRAVITY;
	else if (_hover) _vy = -HOVER_GRAVITY;
	else _vy -= GRAVITY * elapsedMs;

	if (_ground) _vx *= DRAG;
}

void CMario::UpdateMomentum(float elapsedMs)
{
	if (_action != EAction::JUMP || !_fullSpeed)
	{
		_momentum -= MOMENTUM_DECREASE_RATE * elapsedMs;
		if (_momentum < 0) _momentum = 0;
	}

	if (_game->IsKeyDown(ACTION) && _ground && (_vx < -MOMENTUM_THRESHOLD || _vx > MOMENTUM_THRESHOLD))
	{
		_momentum += MOMENTUM_INCREASE_RATE * elapsedMs;
		if (_momentum > MOMENTUM_LIMIT)
		{
			_momentum = MOMENTUM_LIMIT;
			_fullSpeed = true;
		}
	}

	if (_momentum < MOMENTUM_LIMIT) _fullSpeed = false;
}

void CMario::UpdateFlyTimeout(float elapsedMs)
{
	if (_flyTimeout > 0) _flyTimeout -= elapsedMs;
	else _flyTimeout = 0;
}

void CMario::UpdateInvincible(float elapsedMs)
{
	if (_invincible)
	{
		_blink = !_blink;
		if (_invincibleTimeout > 0) _invincibleTimeout -= elapsedMs;
		else
		{
			_invincibleTimeout = 0;
			_invincible = false;
			_blink = false;
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

	if (_left) _vx = HIT_DEFLECT_HORIZONTAL_FORCE;
	else _vx = -HIT_DEFLECT_HORIZONTAL_FORCE;

	_vy = HIT_DEFLECT_VERTICAL_FORCE;
}

void CMario::CameraControl()
{
	_game->MoveCameraTo(
		_x - _game->GetGraphics()->GetBackBufferWidth() / 2,
		_y + _game->GetGraphics()->GetBackBufferHeight() / 2
	);
}

void CMario::PowerControlCheat()
{
	if (_game->IsKeyPressed(HOTKEY_SMALL))
	{
		if (_power == EPower::LARGE)
			SetNextAction(EAction::SHRINK);
	}
	else if (_game->IsKeyPressed(HOTKEY_LARGE))
	{
		if (_power == EPower::SMALL)
			SetNextAction(EAction::GROW);
		else if (_power == EPower::FIRE || _power == EPower::RACCOON)
			SetNextAction(EAction::POWER_DOWN);
	}
	else if (_game->IsKeyPressed(HOTKEY_FIRE))
	{
		if (_power == EPower::LARGE || _power == EPower::RACCOON)
			SetNextAction(EAction::POWER_FIRE);
	}
	else if (_game->IsKeyPressed(HOTKEY_RACCOON))
	{
		if (_power == EPower::LARGE || _power == EPower::FIRE)
			SetNextAction(EAction::POWER_TAIL);
	}
}

#pragma endregion

#pragma region COLLISION

int CMario::IsCollidable()
{
	switch (_action)
	{
	case CMario::EAction::IDLE:
	case CMario::EAction::CROUNCH:
	case CMario::EAction::WALK:
	case CMario::EAction::RUN:
	case CMario::EAction::DRIFT:
	case CMario::EAction::JUMP:
	case CMario::EAction::FLY:
	case CMario::EAction::HOVER:
	case CMario::EAction::FALL:
	case CMario::EAction::KICK:
	case CMario::EAction::SPIN:
	case CMario::EAction::FIRE:
	case CMario::EAction::GROW:
	case CMario::EAction::SHRINK:
	case CMario::EAction::POWER_FIRE:
	case CMario::EAction::POWER_TAIL:
	case CMario::EAction::POWER_DOWN:
		return 1;
		break;

	case CMario::EAction::DIE:
		return 0;
		break;

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
		switch (_action)
		{
		case CMario::EAction::IDLE:
		case CMario::EAction::CROUNCH:
		case CMario::EAction::WALK:
		case CMario::EAction::RUN:
		case CMario::EAction::DRIFT:
		case CMario::EAction::JUMP:
		case CMario::EAction::FLY:
		case CMario::EAction::HOVER:
		case CMario::EAction::FALL:
		case CMario::EAction::KICK:
		case CMario::EAction::SPIN:
		case CMario::EAction::FIRE:
		case CMario::EAction::GROW:
		case CMario::EAction::SHRINK:
		case CMario::EAction::POWER_FIRE:
		case CMario::EAction::POWER_TAIL:
		case CMario::EAction::POWER_DOWN:
		{
			left = _x + SMALL_BODY_OFFSETX - (SMALL_BODY_WIDTH / 2);
			right = _x + SMALL_BODY_OFFSETX + (SMALL_BODY_WIDTH / 2);
			top = _y + SMALL_BODY_OFFSETY + (SMALL_BODY_HEIGHT / 2);
			bottom = _y + SMALL_BODY_OFFSETY - (SMALL_BODY_HEIGHT / 2);
		}
		break;

		}
		break;

	case CMario::EPower::LARGE:
	case CMario::EPower::FIRE:
	case CMario::EPower::RACCOON:
		switch (_action)
		{
		case CMario::EAction::CROUNCH:
		{
			left = _x + SMALL_BODY_OFFSETX - (SMALL_BODY_WIDTH / 2);
			right = _x + SMALL_BODY_OFFSETX + (SMALL_BODY_WIDTH / 2);
			top = _y + SMALL_BODY_OFFSETY + (SMALL_BODY_HEIGHT / 2);
			bottom = _y + SMALL_BODY_OFFSETY - (SMALL_BODY_HEIGHT / 2);
		}
		break;

		case CMario::EAction::IDLE:
		case CMario::EAction::WALK:
		case CMario::EAction::RUN:
		case CMario::EAction::DRIFT:
		case CMario::EAction::JUMP:
		case CMario::EAction::FLY:
		case CMario::EAction::HOVER:
		case CMario::EAction::FALL:
		case CMario::EAction::KICK:
		case CMario::EAction::SPIN:
		case CMario::EAction::FIRE:
		case CMario::EAction::GROW:
		case CMario::EAction::SHRINK:
		case CMario::EAction::POWER_FIRE:
		case CMario::EAction::POWER_TAIL:
		case CMario::EAction::POWER_DOWN:
		{
			left = _x + LARGE_BODY_OFFSETX - (LARGE_BODY_WIDTH / 2);
			right = _x + LARGE_BODY_OFFSETX + (LARGE_BODY_WIDTH / 2);
			top = _y + LARGE_BODY_OFFSETY + (LARGE_BODY_HEIGHT / 2);
			bottom = _y + LARGE_BODY_OFFSETY - (LARGE_BODY_HEIGHT / 2);
		}
		break;

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
	if (dynamic_cast<pGoomba>(collision->_target))
		OnCollisionWithGoomba(collision);

	else if (dynamic_cast<pPlatform>(collision->_target))
		OnCollisionWithPlatform(collision);

	else if (dynamic_cast<pBlock>(collision->_target))
		OnCollisionWithBlock(collision);

	else if (dynamic_cast<pBrick>(collision->_target))
		OnCollisionWithBrick(collision);

	else if (dynamic_cast<pPipe>(collision->_target))
		OnCollisionWithPipe(collision);

}

void CMario::OnCollisionWithGoomba(pCollision collision)
{
	auto goomba = dynamic_cast<pGoomba>(collision->_target);
	if (collision->_ny > 0)
	{
		goomba->HitTop();
		_vy = HIT_DEFLECT_VERTICAL_FORCE;
	}
	else if (collision->_nx != 0)
	{
		if (!_invincible) Hit();
	}
}

void CMario::OnCollisionWithPlatform(pCollision collision)
{
	auto platform = dynamic_cast<pPlatform>(collision->_target);
	if (platform->_solid)
	{
		if (collision->_ny != 0 && collision->_target->IsBlocking())
		{
			_vy = 0;
			if (collision->_ny > 0) _ground = true;
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
	if (collision->_ny != 0 && collision->_target->IsBlocking())
	{
		_vy = 0;
		if (collision->_ny > 0) _ground = true;
		else if (collision->_ny < 0)
		{
			block->HitBottom();
		}
	}

	if (collision->_nx != 0 && collision->_target->IsBlocking())
	{
		_vx = 0;
	}
}

void CMario::OnCollisionWithBrick(pCollision collision)
{
	auto brick = dynamic_cast<pBrick>(collision->_target);
	if (collision->_ny != 0 && collision->_target->IsBlocking())
	{
		_vy = 0;
		if (collision->_ny > 0) _ground = true;
		else if (collision->_ny < 0)
		{
			if (_power == EPower::SMALL)
			{
				brick->HitBottom();
			}
			else
			{
				brick->Brake();
			}
		}
	}

	if (collision->_nx != 0 && collision->_target->IsBlocking())
	{
		_vx = 0;
	}
}

void CMario::OnCollisionWithPipe(pCollision collision)
{
	float pipeLeft = 0;
	float pipeTop = 0;
	float pipeRight = 0;
	float pipeBottom = 0;
	collision->_target->GetBoundingBox(pipeLeft, pipeTop, pipeRight, pipeBottom);

	float left = 0;
	float top = 0;
	float right = 0;
	float bottom = 0;
	GetBoundingBox(left, top, right, bottom);

	if (collision->_ny == 0 && collision->_nx != 0)
	{
		if (collision->_nx > 0)
		{
			_vx = 0;
			_x = pipeRight + ((right - left) / 2) + BLOCK_PUSH_FACTOR;
		}
		else
		{
			_vx = 0;
			_x = pipeLeft - ((right - left) / 2) - BLOCK_PUSH_FACTOR;
		}
	}
	else if (collision->_ny != 0 && collision->_nx == 0)
	{
		if (collision->_ny > 0)
		{
			_vy = 0;
			_ground = true;
			_y = pipeTop + BLOCK_PUSH_FACTOR;
		}
		else
		{
			_vy = 0;
			_y = pipeBottom - (top - bottom) - BLOCK_PUSH_FACTOR;
		}
	}
}

#pragma endregion