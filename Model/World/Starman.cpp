#pragma region INCLUDE
#include "Starman.h"

#include "../SuperMarioBros3.h"

#include "Platform.h"
#pragma endregion

void CStarman::Load()
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

	/* Move */
	pugi::xml_node move = prefab.child("Prefab").child("Move");
	GRAVITY = move.attribute("GRAVITY").as_float();
	MOVE_SPEED = move.attribute("MOVE_SPEED").as_float();
	JUMP_FORCE = move.attribute("JUMP_FORCE").as_float();
	SPAWN_LIMIT = move.attribute("SPAWN_LIMIT").as_float();
	SPAWN_SPEED = move.attribute("SPAWN_SPEED").as_float();
	_targetName = move.attribute("targetName").as_string();
}

void CStarman::Start()
{
	CGameObject::Start();
}

void CStarman::Update(float elapsedMs)
{
	if (!_start) Start();

	std::vector<pGameObject> collidables = _game->GetLocal(_id);
	_collider->Process(elapsedMs, &collidables);

	HandleAction(elapsedMs);
}

void CStarman::Render()
{
	if (_renderBody) _sprites[SPR_BBOX]->Render(_x, _y);
	switch (_action)
	{
	case CStarman::EAction::SPAWN:
	case CStarman::EAction::MOVE:
		_animations[ANI_STARMAN]->Render(_x, _y);
		break;

	case CStarman::EAction::CONSUMED:
		break;
	}
}

#pragma region STATE MACHINE

void CStarman::HandleAction(float elapsedMs)
{
	switch (_action)
	{
	case CStarman::EAction::SPAWN:
		Spawn(elapsedMs);
		break;

	case CStarman::EAction::MOVE:
		Move(elapsedMs);
		break;

	case CStarman::EAction::CONSUMED:
		Consumed(elapsedMs);
		break;
	}
}

void CStarman::Spawn(float elapsedMs)
{
	switch (_actionStage)
	{
	case CStarman::EActionStage::ENTRY:
	{
		/* Animation Start */
		_animations[ANI_STARMAN]->Play(true);

		_spawnLimit = _y + SPAWN_LIMIT;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CStarman::EActionStage::PROGRESS:
	{
		/* Animation Update */
		_animations[ANI_STARMAN]->Update(elapsedMs);

		if (_y < _spawnLimit)
		{
			_y += SPAWN_SPEED * elapsedMs;
		}
		else
		{
			SetNextAction(EAction::MOVE);
			break;
		}
	}
	break;

	case CStarman::EActionStage::EXIT:
	{
		/* Animation End */
		_animations[ANI_STARMAN]->Stop();
	}
	NextAction();
	break;
	}
}

void CStarman::Move(float elapsedMs)
{
	switch (_actionStage)
	{
	case CStarman::EActionStage::ENTRY:
	{
		/* Animation Start */
		_animations[ANI_STARMAN]->Play(true);

		AcquireTarget();
		_ground = true;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CStarman::EActionStage::PROGRESS:
	{
		/* Animation Update */
		_animations[ANI_STARMAN]->Update(elapsedMs);

		if (_left)	_vx = -MOVE_SPEED;
		else		_vx = MOVE_SPEED;

		_vy -= GRAVITY * elapsedMs;

		if (_ground)
		{
			_vy = JUMP_FORCE;
			_ground = false;
		}
	}
	break;

	case CStarman::EActionStage::EXIT:
	{
		/* Animation End */
		_animations[ANI_STARMAN]->Stop();
	}
	NextAction();
	break;
	}
}

void CStarman::Consumed(float elapsedMs)
{
	switch (_actionStage)
	{
	case CStarman::EActionStage::ENTRY:
	{
		Destroy();
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CStarman::EActionStage::PROGRESS:
	{
	}
	break;

	case CStarman::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

void CStarman::AcquireTarget()
{
	if (_game->Get(_targetName) != nullptr)
	{
		float targetX = 0;
		float targetY = 0;
		_game->Get(_targetName)->GetPosition(targetX, targetY);
		if (_x < targetX) _left = true;
		else _left = false;
	}
}

#pragma endregion

#pragma region COLLISION

int CStarman::IsCollidable()
{
	switch (_action)
	{
	case CStarman::EAction::MOVE:
		return 1;
		break;

	case CStarman::EAction::SPAWN:
	case CStarman::EAction::CONSUMED:
		return 0;
		break;
	
	default:
		return 0;
		break;
	}
}

int CStarman::IsBlocking()
{
	return 0;
}

void CStarman::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	switch (_action)
	{
	case CStarman::EAction::MOVE:
		left = _x + BODY_OFFSETX - (BODY_WIDTH / 2);
		right = _x + BODY_OFFSETX + (BODY_WIDTH / 2);
		top = _y + BODY_OFFSETY + (BODY_HEIGHT / 2);
		bottom = _y + BODY_OFFSETY - (BODY_HEIGHT / 2);
		break;

	case CStarman::EAction::SPAWN:
	case CStarman::EAction::CONSUMED:
		break;
	}
}

void CStarman::OnNoCollision(float elapsedMs)
{
	_x += _vx * elapsedMs;
	_y += _vy * elapsedMs;
}

void CStarman::OnCollisionWith(pCollision collision)
{
	if (dynamic_cast<pPlatform>(collision->_target))
		OnCollisionWithPlatform(collision);
}

void CStarman::OnCollisionWithPlatform(pCollision collision)
{
	auto platform = dynamic_cast<pPlatform>(collision->_target);
	if (platform->_solid)
	{
		if (collision->_ny != 0 && collision->_target->IsBlocking())
		{
			_vy = 0;
			_ground = true;
		}

		if (collision->_nx != 0 && collision->_target->IsBlocking())
		{
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

#pragma endregion