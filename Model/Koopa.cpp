#pragma region INCLUDE
#include "Koopa.h"
#include "SuperMarioBros3.h"

#include "Goomba.h"
#include "Plant.h"

#include "Platform.h"
#include "Block.h"
#include "Brick.h"
#include "Pipe.h"
#include "DeadZone.h"

#pragma endregion

void CKoopa::Load()
{
	CGameObject::Load();

	/* Read file */
	pugi::xml_document prefab;
	prefab.load_file(_source.c_str());

	/* Body */
	pugi::xml_node body = prefab.child("Prefab").child("Body");
	_renderBody = body.attribute("render").as_bool();
	BODY_WIDTH = body.attribute("width").as_float();
	BODY_HEIGHT = body.attribute("height").as_float();
	BODY_OFFSETX = body.attribute("offsetX").as_float();
	BODY_OFFSETY = body.attribute("offsetY").as_float();

	/* Type */
	pugi::xml_node type = prefab.child("Prefab").child("Type");
	_type = EType(type.attribute("type").as_int());
	_wing = type.attribute("wing").as_bool();

	/* Sensor */
	pugi::xml_node sensor = prefab.child("Prefab").child("Sensor");
	SENSOR_OFFSETX = sensor.attribute("OffsetX").as_float();
	SENSOR_OFFSETY = sensor.attribute("OffsetY").as_float();
	_sensor = dynamic_cast<pKoopaSensor>(
		_game->Create(
			sensor.attribute("actor").as_int(),
			_name + sensor.attribute("name").as_string(),
			sensor.attribute("source").as_string(),
			_x, _y, _gx, _gy, _layer
		)
		);

	/* Gravity */
	pugi::xml_node gravity = prefab.child("Prefab").child("Gravity");
	GRAVITY = gravity.attribute("GRAVITY").as_float();

	/* Move */
	pugi::xml_node move = prefab.child("Prefab").child("Move");
	WALK_SPEED = move.attribute("WALK_SPEED").as_float();

	/* Jump */
	pugi::xml_node jump = prefab.child("Prefab").child("Jump");
	JUMP_FORCE = jump.attribute("JUMP_FORCE").as_float();

	/* Fly */
	pugi::xml_node fly = prefab.child("Prefab").child("Fly");
	FLY_LIMIT = fly.attribute("FLY_LIMIT").as_float();
	FLY_ACCELERATION = fly.attribute("FLY_ACCELERATION").as_float();
	FLY_MAX_SPEED = fly.attribute("FLY_MAX_SPEED").as_float();

	/* Hit */
	pugi::xml_node hit = prefab.child("Prefab").child("Hit");
	HORIZONTAL_DEFLECT_FORCE = hit.attribute("HORIZONTAL_DEFLECT_FORCE").as_float();
	VERTICAL_DEFLECT_FORCE = hit.attribute("VERTICAL_DEFLECT_FORCE").as_float();
	DECAY_TIMEOUT = hit.attribute("DECAY_TIMEOUT").as_float();
	SPIN_SPEED = hit.attribute("SPIN_SPEED").as_float();
	RETRACT_COUNTDOWN = hit.attribute("RETRACT_COUNTDOWN").as_float();
	RECOVER_COUNTDOWN = hit.attribute("RECOVER_COUNTDOWN").as_float();
	_targetName = hit.attribute("targetName").as_string();
}

void CKoopa::Start()
{
	CGameObject::Start();
	if (_wing)
	{
		switch (_type)
		{
		case CKoopa::EType::GREEN:
			SetAction(EAction::JUMP);
			break;

		case CKoopa::EType::RED:
			SetAction(EAction::FLY);
			break;
		}
	}

	AcquireTarget();
}

void CKoopa::Update(float elapsedMs)
{
	if (!_start) Start();

	UpdateGravity(elapsedMs);

	std::vector<pGameObject> collidables = _game->GetLocal(_id);
	_collider->Process(elapsedMs, &collidables);

	HandleAction(elapsedMs);
	UpdateSensor(elapsedMs);
}

void CKoopa::Render()
{
	if (_renderBody) _sprites[SPR_BBOX]->Render(_x, _y);

	switch (_action)
	{
	case CKoopa::EAction::MOVE:
	{
		switch (_type)
		{
		case CKoopa::EType::GREEN:
		{
			if (_left)	_animations[ANI_KOOPA_WALK_UP_LEFT]->Render(_x, _y);
			else		_animations[ANI_KOOPA_WALK_UP_RIGHT]->Render(_x, _y);
		}
		break;

		case CKoopa::EType::RED:
		{
			if (_left)	_animations[ANI_RED_KOOPA_WALK_UP_LEFT]->Render(_x, _y);
			else		_animations[ANI_RED_KOOPA_WALK_UP_RIGHT]->Render(_x, _y);
		}
		break;
		}
	}
	break;

	case CKoopa::EAction::JUMP:
	{
		switch (_type)
		{
		case CKoopa::EType::GREEN:
		{
			if (_left)	_animations[ANI_KOOPA_WALK_UP_LEFT]->Render(_x, _y);
			else		_animations[ANI_KOOPA_WALK_UP_RIGHT]->Render(_x, _y);
		}
		break;
		}

		if (_wing)
		{
			if (_left)	_animations[ANI_KOOPA_WING_FLAP_SLOW_LEFT]->Render(_x, _y);
			else		_animations[ANI_KOOPA_WING_FLAP_SLOW_RIGHT]->Render(_x, _y);
		}
	}
	break;

	case CKoopa::EAction::FLY:
	{
		switch (_type)
		{
		case CKoopa::EType::RED:
		{
			if (_left)	_animations[ANI_RED_KOOPA_WALK_UP_LEFT]->Render(_x, _y);
			else		_animations[ANI_RED_KOOPA_WALK_UP_RIGHT]->Render(_x, _y);
		}
		break;
		}

		if (_wing)
		{
			if (_vy > 0)
			{
				if (_left)	_animations[ANI_KOOPA_WING_FLAP_FAST_LEFT]->Render(_x, _y);
				else		_animations[ANI_KOOPA_WING_FLAP_FAST_RIGHT]->Render(_x, _y);
			}
			else
			{
				if (_left)	_animations[ANI_KOOPA_WING_FLAP_SLOW_LEFT]->Render(_x, _y);
				else		_animations[ANI_KOOPA_WING_FLAP_SLOW_RIGHT]->Render(_x, _y);
			}
		}
	}
	break;

	case CKoopa::EAction::RETRACT:
	{
		switch (_type)
		{
		case CKoopa::EType::GREEN:
		{
			if (!_flip)	_sprites[SPR_KOOPA_SHELL1_UP]->Render(_x, _y);
			else		_sprites[SPR_KOOPA_SHELL1_DOWN]->Render(_x, _y);
		}
		break;

		case CKoopa::EType::RED:
		{
			if (!_flip)	_sprites[SPR_RED_KOOPA_SHELL1_UP]->Render(_x, _y);
			else		_sprites[SPR_RED_KOOPA_SHELL1_DOWN]->Render(_x, _y);
		}
		break;
		}
	}
	break;

	case CKoopa::EAction::SPIN:
	{
		switch (_type)
		{
		case CKoopa::EType::GREEN:
		{
			if (!_flip)
			{
				if (_left)	_animations[ANI_KOOPA_SHELL_SPIN_UP_LEFT]->Render(_x, _y);
				else		_animations[ANI_KOOPA_SHELL_SPIN_UP_RIGHT]->Render(_x, _y);
			}
			else
			{
				if (_left)	_animations[ANI_KOOPA_SHELL_SPIN_DOWN_LEFT]->Render(_x, _y);
				else		_animations[ANI_KOOPA_SHELL_SPIN_DOWN_RIGHT]->Render(_x, _y);
			}
		}
		break;

		case CKoopa::EType::RED:
		{
			if (!_flip)
			{
				if (_left)	_animations[ANI_RED_KOOPA_SHELL_SPIN_UP_LEFT]->Render(_x, _y);
				else		_animations[ANI_RED_KOOPA_SHELL_SPIN_UP_RIGHT]->Render(_x, _y);
			}
			else
			{
				if (_left)	_animations[ANI_RED_KOOPA_SHELL_SPIN_DOWN_LEFT]->Render(_x, _y);
				else		_animations[ANI_RED_KOOPA_SHELL_SPIN_DOWN_RIGHT]->Render(_x, _y);
			}
		}
		break;
		}
	}
	break;

	case CKoopa::EAction::RECOVER:
	{
		switch (_type)
		{
		case CKoopa::EType::GREEN:
		{
			if (!_flip)	_animations[ANI_KOOPA_SHELL_RECOVER_UP]->Render(_x, _y);
			else		_animations[ANI_KOOPA_SHELL_RECOVER_DOWN]->Render(_x, _y);
		}
		break;

		case CKoopa::EType::RED:
		{
			if (!_flip)	_animations[ANI_RED_KOOPA_SHELL_RECOVER_UP]->Render(_x, _y);
			else		_animations[ANI_RED_KOOPA_SHELL_RECOVER_DOWN]->Render(_x, _y);
		}
		break;
		}
	}
	break;

	case CKoopa::EAction::THROWN:
	{
		switch (_type)
		{
		case CKoopa::EType::GREEN:
		{
			_sprites[SPR_KOOPA_SHELL1_DOWN]->Render(_x, _y);
		}
		break;

		case CKoopa::EType::RED:
		{
			_sprites[SPR_RED_KOOPA_SHELL1_DOWN]->Render(_x, _y);
		}
		break;
		}
	}
	break;

	case CKoopa::EAction::DEADZONED:
	{
	}
	break;

	}
}

#pragma region STATE MACHINE

void CKoopa::HandleAction(float elapsedMs)
{
	switch (_action)
	{
	case CKoopa::EAction::MOVE:
		Move(elapsedMs);
		break;

	case CKoopa::EAction::JUMP:
		Jump(elapsedMs);
		break;

	case CKoopa::EAction::FLY:
		Fly(elapsedMs);
		break;

	case CKoopa::EAction::RETRACT:
		Retract(elapsedMs);
		break;

	case CKoopa::EAction::SPIN:
		Spin(elapsedMs);
		break;

	case CKoopa::EAction::RECOVER:
		Recover(elapsedMs);
		break;

	case CKoopa::EAction::THROWN:
		Thrown(elapsedMs);
		break;

	case CKoopa::EAction::DEADZONED:
		DeadZoned(elapsedMs);
		break;
	}
}

void CKoopa::Move(float elapsedMs)
{
	switch (_actionStage)
	{
	case CKoopa::EActionStage::ENTRY:
	{
		/* Animation Start */
		{
			switch (_type)
			{
			case CKoopa::EType::GREEN:
			{
				_animations[ANI_KOOPA_WALK_UP_LEFT]->Play(true);
				_animations[ANI_KOOPA_WALK_UP_RIGHT]->Play(true);
			}
			break;

			case CKoopa::EType::RED:
			{
				_animations[ANI_RED_KOOPA_WALK_UP_LEFT]->Play(true);
				_animations[ANI_RED_KOOPA_WALK_UP_RIGHT]->Play(true);
			}
			break;
			}
		}
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CKoopa::EActionStage::PROGRESS:
	{
		/* Animation Update */
		{
			switch (_type)
			{
			case CKoopa::EType::GREEN:
			{
				_animations[ANI_KOOPA_WALK_UP_LEFT]->Update(elapsedMs);
				_animations[ANI_KOOPA_WALK_UP_RIGHT]->Update(elapsedMs);
			}
			break;

			case CKoopa::EType::RED:
			{
				_animations[ANI_RED_KOOPA_WALK_UP_LEFT]->Update(elapsedMs);
				_animations[ANI_RED_KOOPA_WALK_UP_RIGHT]->Update(elapsedMs);
			}
			break;
			}
		}

		if (_type == EType::RED
			&& _sensor->_triggered
			&& _ground)
		{
			_left = !_left;
		}

		if (_left)	_vx = -WALK_SPEED;
		else		_vx = WALK_SPEED;
	}
	break;

	case CKoopa::EActionStage::EXIT:
	{
		/* Animation Stop */
		{
			switch (_type)
			{
			case CKoopa::EType::GREEN:
			{
				_animations[ANI_KOOPA_WALK_UP_LEFT]->Stop();
				_animations[ANI_KOOPA_WALK_UP_RIGHT]->Stop();
			}
			break;

			case CKoopa::EType::RED:
			{
				_animations[ANI_RED_KOOPA_WALK_UP_LEFT]->Stop();
				_animations[ANI_RED_KOOPA_WALK_UP_RIGHT]->Stop();
			}
			break;
			}
		}
	}
	NextAction();
	break;
	}
}

void CKoopa::Jump(float elapsedMs)
{
	switch (_actionStage)
	{
	case CKoopa::EActionStage::ENTRY:
	{
		/* Animation Start */
		{
			switch (_type)
			{
			case CKoopa::EType::GREEN:
			{
				_animations[ANI_KOOPA_WALK_UP_LEFT]->Play(true);
				_animations[ANI_KOOPA_WALK_UP_RIGHT]->Play(true);
				_animations[ANI_KOOPA_WING_FLAP_SLOW_LEFT]->Play(true);
				_animations[ANI_KOOPA_WING_FLAP_SLOW_RIGHT]->Play(true);
			}
			break;
			}
		}
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CKoopa::EActionStage::PROGRESS:
	{
		if (_ground)
		{
			_vy = JUMP_FORCE;
			_ground = false;
		}

		if (_left)	_vx = -WALK_SPEED;
		else		_vx = WALK_SPEED;

		/* Animation Update */
		_animations[ANI_KOOPA_WALK_UP_LEFT]->Update(elapsedMs);
		_animations[ANI_KOOPA_WALK_UP_RIGHT]->Update(elapsedMs);
		_animations[ANI_KOOPA_WING_FLAP_SLOW_LEFT]->Update(elapsedMs);
		_animations[ANI_KOOPA_WING_FLAP_SLOW_RIGHT]->Update(elapsedMs);
	}
	break;

	case CKoopa::EActionStage::EXIT:
	{
		/* Animation Stop */
		{
			switch (_type)
			{
			case CKoopa::EType::GREEN:
			{
				_animations[ANI_KOOPA_WALK_UP_LEFT]->Stop();
				_animations[ANI_KOOPA_WALK_UP_RIGHT]->Stop();
				_animations[ANI_KOOPA_WING_FLAP_SLOW_LEFT]->Stop();
				_animations[ANI_KOOPA_WING_FLAP_SLOW_RIGHT]->Stop();
			}
			break;
			}
		}
	}
	NextAction();
	break;
	}
}

void CKoopa::Fly(float elapsedMs)
{
	switch (_actionStage)
	{
	case CKoopa::EActionStage::ENTRY:
	{
		/* Animation Start */
		{
			switch (_type)
			{
			case CKoopa::EType::RED:
			{
				_animations[ANI_RED_KOOPA_WALK_UP_LEFT]->Play(true);
				_animations[ANI_RED_KOOPA_WALK_UP_RIGHT]->Play(true);
				_animations[ANI_KOOPA_WING_FLAP_SLOW_LEFT]->Play(true);
				_animations[ANI_KOOPA_WING_FLAP_SLOW_RIGHT]->Play(true);
				_animations[ANI_KOOPA_WING_FLAP_FAST_LEFT]->Play(true);
				_animations[ANI_KOOPA_WING_FLAP_FAST_RIGHT]->Play(true);
			}
			break;
			}
		}

		_flyOrigin = _y;
		_flyLimit = _y + FLY_LIMIT;
		_flyUp = true;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CKoopa::EActionStage::PROGRESS:
	{
		/* Animation Update */
		{
			switch (_type)
			{
			case CKoopa::EType::RED:
			{
				_animations[ANI_RED_KOOPA_WALK_UP_LEFT]->Update(elapsedMs);
				_animations[ANI_RED_KOOPA_WALK_UP_RIGHT]->Update(elapsedMs);
				_animations[ANI_KOOPA_WING_FLAP_SLOW_LEFT]->Update(elapsedMs);
				_animations[ANI_KOOPA_WING_FLAP_SLOW_RIGHT]->Update(elapsedMs);
				_animations[ANI_KOOPA_WING_FLAP_FAST_LEFT]->Update(elapsedMs);
				_animations[ANI_KOOPA_WING_FLAP_FAST_RIGHT]->Update(elapsedMs);
			}
			break;
			}
		}

		if (_flyUp)
		{
			if (_y <= _flyLimit)
			{
				_vy += FLY_ACCELERATION * elapsedMs;
				if (_vy > FLY_MAX_SPEED) _vy = FLY_MAX_SPEED;
			}
			else
			{
				_flyUp = false;
			}
		}
		else
		{
			if (_y >= _flyOrigin)
			{
				_vy -= FLY_ACCELERATION * elapsedMs;
				if (_vy < -FLY_MAX_SPEED) _vy = -FLY_MAX_SPEED;
			}
			else
			{
				_flyUp = true;
			}
		}
	}
	break;

	case CKoopa::EActionStage::EXIT:
	{
		/* Animation Stop */
		{
			switch (_type)
			{
			case CKoopa::EType::RED:
			{
				_animations[ANI_RED_KOOPA_WALK_UP_LEFT]->Stop();
				_animations[ANI_RED_KOOPA_WALK_UP_RIGHT]->Stop();
				_animations[ANI_KOOPA_WING_FLAP_SLOW_LEFT]->Stop();
				_animations[ANI_KOOPA_WING_FLAP_SLOW_RIGHT]->Stop();
				_animations[ANI_KOOPA_WING_FLAP_FAST_LEFT]->Stop();
				_animations[ANI_KOOPA_WING_FLAP_FAST_RIGHT]->Stop();
			}
			break;
			}
		}
	}
	NextAction();
	break;
	}
}

void CKoopa::Retract(float elapsedMs)
{
	switch (_actionStage)
	{
	case CKoopa::EActionStage::ENTRY:
	{
		_vx = 0;
		_retractCountdown = RETRACT_COUNTDOWN;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CKoopa::EActionStage::PROGRESS:
	{
		if (_retractCountdown > 0)
		{
			_retractCountdown -= elapsedMs;
		}
		else
		{
			SetNextAction(EAction::RECOVER);
		}
	}
	break;

	case CKoopa::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

void CKoopa::Spin(float elapsedMs)
{
	switch (_actionStage)
	{
	case CKoopa::EActionStage::ENTRY:
	{
		/* Animation Start */
		{
			switch (_type)
			{
			case CKoopa::EType::GREEN:
			{
				_animations[ANI_KOOPA_SHELL_SPIN_UP_LEFT]->Play(true);
				_animations[ANI_KOOPA_SHELL_SPIN_UP_RIGHT]->Play(true);
				_animations[ANI_KOOPA_SHELL_SPIN_DOWN_LEFT]->Play(true);
				_animations[ANI_KOOPA_SHELL_SPIN_DOWN_RIGHT]->Play(true);
			}
			break;

			case CKoopa::EType::RED:
			{
				_animations[ANI_RED_KOOPA_SHELL_SPIN_UP_LEFT]->Play(true);
				_animations[ANI_RED_KOOPA_SHELL_SPIN_UP_RIGHT]->Play(true);
				_animations[ANI_RED_KOOPA_SHELL_SPIN_DOWN_LEFT]->Play(true);
				_animations[ANI_RED_KOOPA_SHELL_SPIN_DOWN_RIGHT]->Play(true);
			}
			break;
			}
		}
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CKoopa::EActionStage::PROGRESS:
	{
		/* Animation Update */
		{
			switch (_type)
			{
			case CKoopa::EType::GREEN:
			{
				_animations[ANI_KOOPA_SHELL_SPIN_UP_LEFT]->Update(elapsedMs);
				_animations[ANI_KOOPA_SHELL_SPIN_UP_RIGHT]->Update(elapsedMs);
				_animations[ANI_KOOPA_SHELL_SPIN_DOWN_LEFT]->Update(elapsedMs);
				_animations[ANI_KOOPA_SHELL_SPIN_DOWN_RIGHT]->Update(elapsedMs);
			}
			break;

			case CKoopa::EType::RED:
			{
				_animations[ANI_RED_KOOPA_SHELL_SPIN_UP_LEFT]->Update(elapsedMs);
				_animations[ANI_RED_KOOPA_SHELL_SPIN_UP_RIGHT]->Update(elapsedMs);
				_animations[ANI_RED_KOOPA_SHELL_SPIN_DOWN_LEFT]->Update(elapsedMs);
				_animations[ANI_RED_KOOPA_SHELL_SPIN_DOWN_RIGHT]->Update(elapsedMs);
			}
			break;
			}
		}


		if (_left) _vx = -SPIN_SPEED;
		else _vx = SPIN_SPEED;
	}
	break;

	case CKoopa::EActionStage::EXIT:
	{
		/* Animation Stop */
		{
			switch (_type)
			{
			case CKoopa::EType::GREEN:
			{
				_animations[ANI_KOOPA_SHELL_SPIN_UP_LEFT]->Stop();
				_animations[ANI_KOOPA_SHELL_SPIN_UP_RIGHT]->Stop();
				_animations[ANI_KOOPA_SHELL_SPIN_DOWN_LEFT]->Stop();
				_animations[ANI_KOOPA_SHELL_SPIN_DOWN_RIGHT]->Stop();
			}
			break;

			case CKoopa::EType::RED:
			{
				_animations[ANI_RED_KOOPA_SHELL_SPIN_UP_LEFT]->Stop();
				_animations[ANI_RED_KOOPA_SHELL_SPIN_UP_RIGHT]->Stop();
				_animations[ANI_RED_KOOPA_SHELL_SPIN_DOWN_LEFT]->Stop();
				_animations[ANI_RED_KOOPA_SHELL_SPIN_DOWN_RIGHT]->Stop();
			}
			break;
			}
		}
	}
	NextAction();
	break;
	}
}

void CKoopa::Recover(float elapsedMs)
{
	switch (_actionStage)
	{
	case CKoopa::EActionStage::ENTRY:
	{
		/* Animation Start */
		{
			switch (_type)
			{
			case CKoopa::EType::GREEN:
			{
				_animations[ANI_KOOPA_SHELL_RECOVER_UP]->Play(true);
				_animations[ANI_KOOPA_SHELL_RECOVER_DOWN]->Play(true);
			}
			break;

			case CKoopa::EType::RED:
			{
				_animations[ANI_RED_KOOPA_SHELL_RECOVER_UP]->Play(true);
				_animations[ANI_RED_KOOPA_SHELL_RECOVER_DOWN]->Play(true);
			}
			break;
			}
		}

		_recoverCountdown = RECOVER_COUNTDOWN;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CKoopa::EActionStage::PROGRESS:
	{
		/* Animation Update */
		{
			switch (_type)
			{
			case CKoopa::EType::GREEN:
			{
				_animations[ANI_KOOPA_SHELL_RECOVER_UP]->Update(elapsedMs);
				_animations[ANI_KOOPA_SHELL_RECOVER_DOWN]->Update(elapsedMs);
			}
			break;

			case CKoopa::EType::RED:
			{
				_animations[ANI_RED_KOOPA_SHELL_RECOVER_UP]->Update(elapsedMs);
				_animations[ANI_RED_KOOPA_SHELL_RECOVER_DOWN]->Update(elapsedMs);
			}
			break;
			}
		}

		if (_recoverCountdown > 0)
		{
			_recoverCountdown -= elapsedMs;
		}
		else
		{
			if (_flip) _flip = false;
			SetNextAction(EAction::MOVE);
		}
	}
	break;

	case CKoopa::EActionStage::EXIT:
	{
		/* Animation Stop */
		{
			switch (_type)
			{
			case CKoopa::EType::GREEN:
			{
				_animations[ANI_KOOPA_SHELL_RECOVER_UP]->Stop();
				_animations[ANI_KOOPA_SHELL_RECOVER_DOWN]->Stop();
			}
			break;

			case CKoopa::EType::RED:
			{
				_animations[ANI_RED_KOOPA_SHELL_RECOVER_UP]->Stop();
				_animations[ANI_RED_KOOPA_SHELL_RECOVER_DOWN]->Stop();
			}
			break;
			}
		}
	}
	NextAction();
	break;
	}
}

void CKoopa::Thrown(float elapsedMs)
{
	switch (_actionStage)
	{
	case CKoopa::EActionStage::ENTRY:
	{
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CKoopa::EActionStage::PROGRESS:
	{
	}
	break;

	case CKoopa::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

void CKoopa::DeadZoned(float elapsedMs)
{
	switch (_actionStage)
	{
	case CKoopa::EActionStage::ENTRY:
	{
		_decayTimeout = DECAY_TIMEOUT;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CKoopa::EActionStage::PROGRESS:
	{
		if (_decayTimeout > 0) _decayTimeout -= elapsedMs;
		else Destroy();
	}
	break;

	case CKoopa::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

void CKoopa::UpdateGravity(float elapsedMs)
{
	if (_action == EAction::FLY || _hold)
		return;

	_vy -= GRAVITY * elapsedMs;
}

void CKoopa::UpdateSensor(float elapsedMs)
{
	if (_sensor != nullptr)
	{
		if (_left)	_sensor->SetPosition(_x - SENSOR_OFFSETX, _y + SENSOR_OFFSETY);
		else		_sensor->SetPosition(_x + SENSOR_OFFSETX, _y + SENSOR_OFFSETY);
	}
}

void CKoopa::Stomped(bool left)
{
	if (_wing)
	{
		_wing = false;
		SetNextAction(EAction::MOVE);
		return;
	}

	if (_action == EAction::MOVE)
	{
		SetNextAction(EAction::RETRACT);
		return;
	}

	if (_action == EAction::RETRACT || _action == EAction::RECOVER)
	{
		_left = left;
		SetNextAction(EAction::SPIN);
		return;
	}

	if (_action == EAction::SPIN)
	{
		SetNextAction(EAction::RETRACT);
		return;
	}
}

void CKoopa::Kicked(bool left)
{
	_left = left;
	SetNextAction(EAction::SPIN);
}

void CKoopa::Swept(bool _left)
{
	_flip = true;
	if (_wing) _wing = false;
	SetNextAction(EAction::RETRACT);
}

void CKoopa::Shot(bool _left)
{
	SetNextAction(EAction::THROWN);
}

void CKoopa::AcquireTarget()
{
	auto target = _game->Get(_targetName);
	if (target != nullptr) target->GetPosition(_targetX, _targetY);

	if (_targetX > _x) _left = false;
	else _left = true;
}

#pragma endregion

#pragma region COLLISION

int CKoopa::IsCollidable()
{
	switch (_action)
	{
	case CKoopa::EAction::MOVE:
	case CKoopa::EAction::JUMP:
	case CKoopa::EAction::FLY:
	case CKoopa::EAction::RETRACT:
	case CKoopa::EAction::SPIN:
	case CKoopa::EAction::RECOVER:
		return 1;
		break;

	case CKoopa::EAction::THROWN:
	case CKoopa::EAction::DEADZONED:
		return 0;
		break;

	default:
		return 0;
		break;
	}
}

int CKoopa::IsBlocking()
{
	return 0;
}

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	switch (_action)
	{
	case CKoopa::EAction::MOVE:
	case CKoopa::EAction::JUMP:
	case CKoopa::EAction::FLY:
	case CKoopa::EAction::RETRACT:
	case CKoopa::EAction::SPIN:
	case CKoopa::EAction::RECOVER:
	{
		left = _x + BODY_OFFSETX - (BODY_WIDTH / 2);
		right = _x + BODY_OFFSETX + (BODY_WIDTH / 2);
		top = _y + BODY_OFFSETY + (BODY_HEIGHT / 2);
		bottom = _y + BODY_OFFSETY - (BODY_HEIGHT / 2);
	}
	break;
	}
}

void CKoopa::OnNoCollision(float elapsedMs)
{
	_x += _vx * elapsedMs;
	_y += _vy * elapsedMs;
}

void CKoopa::OnCollisionWith(pCollision collision)
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

void CKoopa::OnCollisionWithGoomba(pCollision collision)
{
	auto goomba = dynamic_cast<pGoomba>(collision->_target);
	if (_action == EAction::SPIN) goomba->Swept(_left);
}

void CKoopa::OnCollisionWithPlant(pCollision collision)
{
	auto plant = dynamic_cast<pPlant>(collision->_target);
	if (_action == EAction::SPIN) plant->Hit();

}

void CKoopa::OnCollisionWithKoopa(pCollision collision)
{
	auto koopa = dynamic_cast<pKoopa>(collision->_target);
	if (_action == EAction::SPIN) koopa->Shot(_left);
}

void CKoopa::OnCollisionWithPlatform(pCollision collision)
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
			_left = !_left;
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

void CKoopa::OnCollisionWithBlock(pCollision collision)
{
	auto block = dynamic_cast<pBlock>(collision->_target);
	if (collision->_ny != 0 && collision->_target->IsBlocking())
	{
		_vy = 0;
		if (collision->_ny > 0) _ground = true;
	}

	if (collision->_nx != 0 && collision->_target->IsBlocking())
	{
		if (_action == EAction::SPIN) block->HitSide(_left);
		_left = !_left;
	}
}

void CKoopa::OnCollisionWithBrick(pCollision collision)
{
	auto brick = dynamic_cast<pBrick>(collision->_target);
	if (collision->_ny != 0 && collision->_target->IsBlocking())
	{
		_vy = 0;
		if (collision->_ny > 0) _ground = true;
	}

	if (collision->_nx != 0 && collision->_target->IsBlocking())
	{
		_left = !_left;
		if (_action == EAction::SPIN) brick->Brake();
	}
}

void CKoopa::OnCollisionWithPipe(pCollision collision)
{
	auto pipe = dynamic_cast<pPipe>(collision->_target);

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
			_left = !_left;
			_x = pipeRight + ((right - left) / 2) + BLOCK_PUSH_FACTOR;
		}
		else
		{
			_left = !_left;
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

void CKoopa::OnCollisionWithDeadZone(pCollision collision)
{
	auto deadZone = dynamic_cast<pDeadZone>(collision->_target);
	SetNextAction(EAction::DEADZONED);
}

#pragma endregion