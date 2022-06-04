#pragma region INCLUDE
#include "SuperMushroom.h"

#include "SuperMarioBros3.h"

#include "Platform.h"
#include "Brick.h"
#include "Block.h"
#include "Pipe.h"
#pragma endregion

void CSuperMushroom::Load()
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

void CSuperMushroom::Start()
{
	CGameObject::Start();
}

void CSuperMushroom::Update(float elapsedMs)
{
	if (!_start) Start();

	std::vector<pGameObject> collidables = _game->GetLocal(_id);
	_collider->Process(elapsedMs, &collidables);

	HandleAction(elapsedMs);
}

void CSuperMushroom::Render()
{
	if (_renderBody) _sprites[SPR_BBOX]->Render(_x, _y);
	switch (_action)
	{
	case CSuperMushroom::EAction::SPAWN:
	case CSuperMushroom::EAction::MOVE:
		_sprites[SPR_SUPER_MUSHROOM]->Render(_x, _y);
		break;

	case CSuperMushroom::EAction::CONSUMED:
		break;
	}
}

#pragma region STATE MACHINE

void CSuperMushroom::HandleAction(float elapsedMs)
{
	switch (_action)
	{
	case CSuperMushroom::EAction::SPAWN:
		Spawn(elapsedMs);
		break;

	case CSuperMushroom::EAction::MOVE:
		Move(elapsedMs);
		break;

	case CSuperMushroom::EAction::CONSUMED:
		Consumed(elapsedMs);
		break;
	}
}

void CSuperMushroom::Spawn(float elapsedMs)
{
	switch (_actionStage)
	{
	case CSuperMushroom::EActionStage::ENTRY:
	{
		_spawnLimit = _y + SPAWN_LIMIT;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CSuperMushroom::EActionStage::PROGRESS:
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

	case CSuperMushroom::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

void CSuperMushroom::Move(float elapsedMs)
{
	switch (_actionStage)
	{
	case CSuperMushroom::EActionStage::ENTRY:
	{
		AcquireTarget();
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CSuperMushroom::EActionStage::PROGRESS:
	{
		if (_left)	_vx = -MOVE_SPEED;
		else		_vx = MOVE_SPEED;

		_vy -= GRAVITY * elapsedMs;
	}
	break;

	case CSuperMushroom::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

void CSuperMushroom::Consumed(float elapsedMs)
{
	switch (_actionStage)
	{
	case CSuperMushroom::EActionStage::ENTRY:
	{
		Destroy();
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CSuperMushroom::EActionStage::PROGRESS:
	{

	}
	break;

	case CSuperMushroom::EActionStage::EXIT:
	{

	}
	NextAction();
	break;
	}
}

void CSuperMushroom::Taken()
{
	SetNextAction(EAction::CONSUMED);
}

void CSuperMushroom::AcquireTarget()
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

int CSuperMushroom::IsCollidable()
{
	switch (_action)
	{
	case CSuperMushroom::EAction::MOVE:
		return 1;
		break;

	case CSuperMushroom::EAction::SPAWN:
	case CSuperMushroom::EAction::CONSUMED:
		return 0;
		break;

	default:
		return 0;
		break;
	}
}

int CSuperMushroom::IsBlocking()
{
	return 0;
}

void CSuperMushroom::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	switch (_action)
	{
	case CSuperMushroom::EAction::MOVE:
		left = _x + BODY_OFFSETX - (BODY_WIDTH / 2);
		right = _x + BODY_OFFSETX + (BODY_WIDTH / 2);
		top = _y + BODY_OFFSETY + (BODY_HEIGHT / 2);
		bottom = _y + BODY_OFFSETY - (BODY_HEIGHT / 2);
		break;

	case CSuperMushroom::EAction::SPAWN:
	case CSuperMushroom::EAction::CONSUMED:
		break;
	}
}

void CSuperMushroom::OnNoCollision(float elapsedMs)
{
	_x += _vx * elapsedMs;
	_y += _vy * elapsedMs;
}

void CSuperMushroom::OnCollisionWith(pCollision collision)
{
	if (dynamic_cast<pPlatform>(collision->_target))
		OnCollisionWithPlatform(collision);	
	
	else if (dynamic_cast<pBlock>(collision->_target))
		OnCollisionWithBlock(collision);

	else if (dynamic_cast<pBrick>(collision->_target))
		OnCollisionWithBrick(collision);

	else if (dynamic_cast<pPipe>(collision->_target))
		OnCollisionWithPipe(collision);
}

void CSuperMushroom::OnCollisionWithPlatform(pCollision collision)
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

void CSuperMushroom::OnCollisionWithBrick(pCollision collision)
{
	auto brick = dynamic_cast<pBrick>(collision->_target);
	if (collision->_ny != 0 && collision->_target->IsBlocking())
	{
		_vy = 0;
	}

	if (collision->_nx != 0 && collision->_target->IsBlocking())
	{
		_left = !_left;
	}
}

void CSuperMushroom::OnCollisionWithBlock(pCollision collision)
{
	auto block = dynamic_cast<pBlock>(collision->_target);
	if (collision->_ny != 0 && collision->_target->IsBlocking())
	{
		_vy = 0;
	}

	if (collision->_nx != 0 && collision->_target->IsBlocking())
	{
		_left = !_left;
	}
}

void CSuperMushroom::OnCollisionWithPipe(pCollision collision)
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
			_vx = 0;
			_x = pipeRight + ((right - left) / 2) + BLOCK_PUSH_FACTOR;
		}
		else
		{
			_vx = 0;
			_x = pipeLeft - ((right - left) / 2) - BLOCK_PUSH_FACTOR;
		}

		_left = !_left;
	}
	else if (collision->_ny != 0 && collision->_nx == 0)
	{
		if (collision->_ny > 0)
		{
			_vy = 0;
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