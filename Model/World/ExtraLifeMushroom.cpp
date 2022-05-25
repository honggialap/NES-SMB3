#pragma region INCLUDE
#include "ExtraLifeMushroom.h"

#include "../SuperMarioBros3.h"

#include "Platform.h"
#pragma endregion

void CExtraLifeMushroom::Load()
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
	SPAWN_LIMIT = move.attribute("SPAWN_LIMIT").as_float();
	SPAWN_SPEED = move.attribute("SPAWN_SPEED").as_float();
	_targetName = move.attribute("targetName").as_string();
}

void CExtraLifeMushroom::Start()
{
	CGameObject::Start();
}

void CExtraLifeMushroom::Update(float elapsedMs)
{
	if (!_start) Start();

	std::vector<pGameObject> collidables = _game->GetLocal(_id);
	_collider->Process(elapsedMs, &collidables);

	HandleAction(elapsedMs);
}

void CExtraLifeMushroom::Render()
{
	if (_renderBody) _sprites[SPR_BBOX]->Render(_x, _y);
	switch (_action)
	{
	case CExtraLifeMushroom::EAction::SPAWN:
	case CExtraLifeMushroom::EAction::MOVE:
		_sprites[SPR_EXTRALIFE_MUSHROOM]->Render(_x, _y);
		break;

	case CExtraLifeMushroom::EAction::CONSUMED:
		break;
	}
}


#pragma region STATE MACHINE

void CExtraLifeMushroom::HandleAction(float elapsedMs)
{
	switch (_action)
	{
	case CExtraLifeMushroom::EAction::SPAWN:
		Spawn(elapsedMs);
		break;

	case CExtraLifeMushroom::EAction::MOVE:
		Move(elapsedMs);
		break;

	case CExtraLifeMushroom::EAction::CONSUMED:
		Consumed(elapsedMs);
		break;
	}
}

void CExtraLifeMushroom::Spawn(float elapsedMs)
{
	switch (_actionStage)
	{
	case CExtraLifeMushroom::EActionStage::ENTRY:
	{
		_spawnLimit = _y + SPAWN_LIMIT;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CExtraLifeMushroom::EActionStage::PROGRESS:
	{
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

	case CExtraLifeMushroom::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

void CExtraLifeMushroom::Move(float elapsedMs)
{
	switch (_actionStage)
	{
	case CExtraLifeMushroom::EActionStage::ENTRY:
	{
		AcquireTarget();
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CExtraLifeMushroom::EActionStage::PROGRESS:
	{
		if (_left)	_vx = -MOVE_SPEED;
		else		_vx = MOVE_SPEED;

		_vy -= GRAVITY * elapsedMs;
	}
	break;

	case CExtraLifeMushroom::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

void CExtraLifeMushroom::Consumed(float elapsedMs)
{
	switch (_actionStage)
	{
	case CExtraLifeMushroom::EActionStage::ENTRY:
	{
		Destroy();
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CExtraLifeMushroom::EActionStage::PROGRESS:
	{

	}
	break;

	case CExtraLifeMushroom::EActionStage::EXIT:
	{

	}
	NextAction();
	break;
	}
}

void CExtraLifeMushroom::AcquireTarget()
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

int CExtraLifeMushroom::IsCollidable()
{
	switch (_action)
	{
	case CExtraLifeMushroom::EAction::MOVE:
		return 1;
		break;

	case CExtraLifeMushroom::EAction::SPAWN:
	case CExtraLifeMushroom::EAction::CONSUMED:
		return 0;
		break;

	default:
		return 0;
		break;
	}
}

int CExtraLifeMushroom::IsBlocking()
{
	return 0;
}

void CExtraLifeMushroom::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	switch (_action)
	{
	case CExtraLifeMushroom::EAction::MOVE:
		left = _x + BODY_OFFSETX - (BODY_WIDTH / 2);
		right = _x + BODY_OFFSETX + (BODY_WIDTH / 2);
		top = _y + BODY_OFFSETY + (BODY_HEIGHT / 2);
		bottom = _y + BODY_OFFSETY - (BODY_HEIGHT / 2);
		break;

	case CExtraLifeMushroom::EAction::SPAWN:
	case CExtraLifeMushroom::EAction::CONSUMED:
		break;
	}
}

void CExtraLifeMushroom::OnNoCollision(float elapsedMs)
{
	_x += _vx * elapsedMs;
	_y += _vy * elapsedMs;
}

void CExtraLifeMushroom::OnCollisionWith(pCollision collision)
{
	if (dynamic_cast<pPlatform>(collision->_target))
		OnCollisionWithPlatform(collision);
}

void CExtraLifeMushroom::OnCollisionWithPlatform(pCollision collision)
{
	auto platform = dynamic_cast<pPlatform>(collision->_target);
	if (platform->_solid)
	{
		if (collision->_ny != 0 && collision->_target->IsBlocking())
		{
			_vy = 0;
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
		}
	}
}

#pragma endregion
