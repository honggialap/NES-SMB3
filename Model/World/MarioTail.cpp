#pragma region INCLUDE
#include "MarioTail.h"
#include "../SuperMarioBros3.h"

#include "Goomba.h"
#include "Koopa.h"
#include "Plant.h"
#include "Brick.h"
#include "Block.h"
#pragma endregion

void CMarioTail::Load()
{
	CGameObject::Load();

	/* Read file */
	pugi::xml_document prefab;
	prefab.load_file(_source.c_str());

	/* Body */
	auto leftBodyNode = prefab.child("Prefab").child("LeftBody");
	_renderLeftBody = leftBodyNode.attribute("render").as_bool();
	LEFT_BODY_WIDTH = leftBodyNode.attribute("width").as_float();
	LEFT_BODY_HEIGHT = leftBodyNode.attribute("height").as_float();
	LEFT_BODY_OFFSETX = leftBodyNode.attribute("offsetX").as_float();
	LEFT_BODY_OFFSETY = leftBodyNode.attribute("offsetY").as_float();

	auto rightBodyNode = prefab.child("Prefab").child("RightBody");
	_renderRightBody = rightBodyNode.attribute("render").as_bool();
	RIGHT_BODY_WIDTH = rightBodyNode.attribute("width").as_float();
	RIGHT_BODY_HEIGHT = rightBodyNode.attribute("height").as_float();
	RIGHT_BODY_OFFSETX = rightBodyNode.attribute("offsetX").as_float();
	RIGHT_BODY_OFFSETY = rightBodyNode.attribute("offsetY").as_float();
}

void CMarioTail::Start()
{
	CGameObject::Start();
	_left = true;
	_activate = false;
}

void CMarioTail::Update(float elapsedMs)
{
	if (!_start) Start();

	std::vector<pGameObject> colidables = _game->GetLocal(_id);
	_collider->Process(elapsedMs, &colidables);
}

void CMarioTail::Render()
{
	if (_renderLeftBody && _activate)
	{
		_sprites[SPR_MARIO_TAIL_BBOX_LEFT]->Render(_x, _y);
	}
	
	if (_renderRightBody && _activate)
	{
		_sprites[SPR_MARIO_TAIL_BBOX_RIGHT]->Render(_x, _y);
	}
}

int CMarioTail::IsCollidable()
{
	if (_activate) return 1;
	else return 0;
}

int CMarioTail::IsBlocking()
{
	return 0;
}

void CMarioTail::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (_left)
	{
		left = _x + LEFT_BODY_OFFSETX - (LEFT_BODY_WIDTH / 2);
		right = _x + LEFT_BODY_OFFSETX + (LEFT_BODY_WIDTH / 2);
		top = _y + LEFT_BODY_OFFSETY + (LEFT_BODY_HEIGHT / 2);
		bottom = _y + LEFT_BODY_OFFSETY - (LEFT_BODY_HEIGHT / 2);
	}
	else
	{
		left = _x + RIGHT_BODY_OFFSETX - (RIGHT_BODY_WIDTH / 2);
		right = _x + RIGHT_BODY_OFFSETX + (RIGHT_BODY_WIDTH / 2);
		top = _y + RIGHT_BODY_OFFSETY + (RIGHT_BODY_HEIGHT / 2);
		bottom = _y + RIGHT_BODY_OFFSETY - (RIGHT_BODY_HEIGHT / 2);
	}
}

void CMarioTail::OnNoCollision(float elapsedMs)
{
}

void CMarioTail::OnCollisionWith(pCollision e)
{
	/* Characters */
	if (dynamic_cast<pGoomba>(e->_target))
		OnCollisionWithGoomba(e);

	else if (dynamic_cast<pKoopa>(e->_target))
		OnCollisionWithKoopa(e);

	else if (dynamic_cast<pPlant>(e->_target))
		OnCollisionWithPlant(e);

	/* Props */
	else if (dynamic_cast<pBrick>(e->_target))
		OnCollisionWithBrick(e);

	else if (dynamic_cast<pBlock>(e->_target))
		OnCollisionWithBlock(e);
}

void CMarioTail::OnCollisionWithGoomba(pCollision e)
{
	pGoomba goomba = dynamic_cast<pGoomba>(e->_target);
	if (goomba->_action != CGoomba::EAction::DIE
		&& goomba->_action != CGoomba::EAction::THROWN)
	{
		goomba->Swept(_left);
	}
}

void CMarioTail::OnCollisionWithKoopa(pCollision e)
{
	pKoopa koopa = dynamic_cast<pKoopa>(e->_target);
	if (koopa->_action != CKoopa::EAction::THROWN)
	{
		koopa->Swept(_left);
	}
}

void CMarioTail::OnCollisionWithPlant(pCollision e)
{
	pPlant plant = dynamic_cast<pPlant>(e->_target);
	//plant->SetNextAction(CPlant::EAction::DIE);
}

void CMarioTail::OnCollisionWithBrick(pCollision e)
{
	pBrick brick = dynamic_cast<pBrick>(e->_target);
	brick->SetNextAction(CBrick::EAction::BROKE);
}

void CMarioTail::OnCollisionWithBlock(pCollision e)
{
	pBlock block = dynamic_cast<pBlock>(e->_target);
	if (block->_action != CBlock::EAction::EMPTY)
	{
		block->_topBounce = false;
		if (e->_nx > 0) block->_leftBounce = false;
		else block->_leftBounce = true;
		block->SetNextAction(CBlock::EAction::BOUNCE);
	}
}