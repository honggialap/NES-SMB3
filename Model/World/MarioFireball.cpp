#pragma region INCLUDE
#include "MarioFireball.h"
#include "../SuperMarioBros3.h"

#include "Platform.h"
#include "Pipe.h"
#include "Brick.h"
#include "Block.h"

#include "Goomba.h"
#include "Koopa.h"
#include "Plant.h"
#pragma endregion

void CMarioFireball::Load()
{
	CGameObject::Load();

	/* Read file */
	pugi::xml_document prefab;
	prefab.load_file(_source.c_str());

	/* Body */
	auto bodyNode = prefab.child("Prefab").child("Body");
	_renderBody = bodyNode.attribute("render").as_bool();
	BODY_WIDTH = bodyNode.attribute("width").as_float();
	BODY_HEIGHT = bodyNode.attribute("height").as_float();
	BODY_OFFSETX = bodyNode.attribute("offsetX").as_float();
	BODY_OFFSETY = bodyNode.attribute("offsetY").as_float();

	/* Logic */
	pugi::xml_node statsNode = prefab.child("Prefab").child("Stats");
	DECAY_INTERVAL = statsNode.attribute("DECAY_INTERVAL").as_float();
	DRAG_FORCE = statsNode.attribute("DRAG_FORCE").as_float();
	GRAVITY = statsNode.attribute("GRAVITY").as_float();
	INITIAL_FORCE = statsNode.attribute("INITIAL_FORCE").as_float();
}

void CMarioFireball::Start()
{
	CGameObject::Start();
	_animations[ANI_MARIO_FIREBALL_LEFT]->Play(true);
	_animations[ANI_MARIO_FIREBALL_RIGHT]->Play(true);
	_decayInterval = DECAY_INTERVAL;
	_vy = 0;
}

void CMarioFireball::Update(float elapsedMs)
{
	if (!_start) Start();

	_vy -= GRAVITY * elapsedMs;

	_vx = _vx / DRAG_FORCE;
	_vy = _vy / DRAG_FORCE;

	_animations[ANI_MARIO_FIREBALL_LEFT]->Update(elapsedMs);
	_animations[ANI_MARIO_FIREBALL_RIGHT]->Update(elapsedMs);
	if (_decayInterval > 0) _decayInterval -= elapsedMs;
	else
	{
		_animations[ANI_MARIO_FIREBALL_LEFT]->Stop();
		_animations[ANI_MARIO_FIREBALL_RIGHT]->Stop();
		Destroy();
	}

	std::vector<pGameObject> colidables = _game->GetLocal(_id);
	_collider->Process(elapsedMs, &colidables);
}

void CMarioFireball::Render()
{
	if (_renderBody) _sprites[SPR_MARIO_FIREBALL_BBOX]->Render(_x, _y);
	if (_vx > 0)
	{
		_animations[ANI_MARIO_FIREBALL_LEFT]->Render(_x, _y);
	}
	else
	{
		_animations[ANI_MARIO_FIREBALL_RIGHT]->Render(_x, _y);
	}
}

int CMarioFireball::IsCollidable()
{
	return 1;
}

int CMarioFireball::IsBlocking()
{
	return 0;
}

void CMarioFireball::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = _x + BODY_OFFSETX - (BODY_WIDTH / 2);
	right = _x + BODY_OFFSETX + (BODY_WIDTH / 2);
	top = _y + BODY_OFFSETY + (BODY_HEIGHT / 2);
	bottom = _y + BODY_OFFSETY - (BODY_HEIGHT / 2);
}

void CMarioFireball::OnNoCollision(float elapsedMs)
{
	_x += _vx * elapsedMs;
	_y += _vy * elapsedMs;
}

void CMarioFireball::OnCollisionWith(pCollision e)
{
	/* Prop*/
	if (dynamic_cast<pPlatform>(e->_target))
		OnCollisionWithPlatform(e);

	else if (dynamic_cast<pPipe>(e->_target))
		OnCollisionWithPipe(e);

	else if (dynamic_cast<pBrick>(e->_target))
		OnCollisionWithBrick(e);

	else if (dynamic_cast<pBlock>(e->_target))
		OnCollisionWithBlock(e);

	/* Character */
	else if (dynamic_cast<pGoomba>(e->_target))
		OnCollisionWithGoomba(e);

	else if (dynamic_cast<pKoopa>(e->_target))
		OnCollisionWithKoopa(e);

	else if (dynamic_cast<pPlant>(e->_target))
		OnCollisionWithPlant(e);
}

void CMarioFireball::OnCollisionWithPlatform(pCollision e)
{
	if ((e->_ny != 0 && e->_target->IsBlocking())
		|| (e->_ny > 0 && !e->_target->IsBlocking()))
	{
		_vy = -_vy;
	}
	else if (e->_nx != 0 && e->_target->IsBlocking())
	{
		_vx = -_vx;
	}
}

void CMarioFireball::OnCollisionWithPipe(pCollision e)
{
	if (e->_ny != 0)
	{
		_vy = -_vy;
	}
	else if (e->_nx != 0)
	{
		_vx = -_vx;
	}
}

void CMarioFireball::OnCollisionWithBrick(pCollision e)
{
	if (e->_ny != 0)
	{
		_vy = -_vy;
	}
	else if (e->_nx != 0)
	{
		_vx = -_vx;
	}
}

void CMarioFireball::OnCollisionWithBlock(pCollision e)
{
	if (e->_ny != 0)
	{
		_vy = -_vy;
	}
	else if (e->_nx != 0)
	{
		_vx = -_vx;
	}
}

void CMarioFireball::OnCollisionWithGoomba(pCollision e)
{
	pGoomba goomba = dynamic_cast<pGoomba>(e->_target);
	bool left = _vx > 0 ? false : true;
	goomba->Swept(left);
	Destroy();
}

void CMarioFireball::OnCollisionWithKoopa(pCollision e)
{
	pKoopa koopa = dynamic_cast<pKoopa>(e->_target);
	bool left = _vx > 0 ? false : true;
	koopa->Shot(left);
	Destroy();
}

void CMarioFireball::OnCollisionWithPlant(pCollision e)
{
	pPlant plant = dynamic_cast<pPlant>(e->_target);
	//piranha->SetNextAction(CPiranha::EAction::DIE);
	Destroy();
}