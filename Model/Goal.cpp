#pragma region INCLUDE
#include "Goal.h"

#include "SuperMarioBros3.h"
#include "World.h"
#pragma endregion

void CGoal::Load()
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

	pugi::xml_node stats = prefab.child("Prefab").child("Stats");
	_controllerName = stats.attribute("_controllerName").as_string();
	ACQUIRE_COUNTDOWN = stats.attribute("ACQUIRE_COUNTDOWN").as_float();
	POP_UP_SPEED = stats.attribute("POP_UP_SPEED").as_float();
	ANNOUNCE_COUNTDOWN = stats.attribute("ANNOUNCE_COUNTDOWN").as_float();
	NEXT_REWARD_COUNTDOWN = stats.attribute("NEXT_REWARD_COUNTDOWN").as_float();
	ANNOUNCE_OFFSETX = stats.attribute("ANNOUNCE_OFFSETX").as_float();
	ANNOUNCE_OFFSETY = stats.attribute("ANNOUNCE_OFFSETY").as_float();
}

void CGoal::Start()
{
	CGameObject::Start();
	SetState(EState::IDLE);
}

void CGoal::Update(float elapsedMs)
{
	if (!_start) Start();

	switch (_state)
	{
	case CGoal::EState::IDLE:
		_animations[ANI_SPIN_IDLE]->Update(elapsedMs);
		if (_nextRewardCountdown > 0) 
		{
			_nextRewardCountdown -= elapsedMs;
		}
		else
		{
			ChangeReward();
			_nextRewardCountdown = NEXT_REWARD_COUNTDOWN;
		}
		break;

	case CGoal::EState::ACQUIRED:
		_animations[ANI_SPIN_END]->Update(elapsedMs);
		if (_acquireCoundown > 0)
		{
			_acquireCoundown -= elapsedMs;
			_renderY += POP_UP_SPEED * elapsedMs;
		}
		else
		{
			SetState(EState::ANNOUNCE);
		}
		break;

	case CGoal::EState::ANNOUNCE:
		if (_announceCountdown > 0)
		{
			_announceCountdown -= elapsedMs;
		}
		else
		{
			pWorld world = dynamic_cast<pWorld>(_game->Get(_controllerName));
			world->BackToWorldMap();
		}
		break;
	}
}

void CGoal::Render()
{
	if (_renderBody) _sprites[SPR_BBOX]->Render(_x, _y);
	switch (_state)
	{
	case CGoal::EState::IDLE:
		_sprites[SPR_GOAL_FRAME]->Render(_x, _y);
		_animations[ANI_SPIN_IDLE]->Render(_x, _y);
		break;

	case CGoal::EState::ACQUIRED:
		_animations[ANI_SPIN_IDLE]->Render(_renderX, _renderY);
		break;

	case CGoal::EState::ANNOUNCE:
		_sprites[SPR_GOAL_ANNOUNCE]->Render(_x, _y);
		switch (_reward)
		{
		case CGoal::EReward::MUSHROOM:
			_sprites[SPR_MUSHROOM_ANNOUNCE]->Render(_x + ANNOUNCE_OFFSETX, _y + ANNOUNCE_OFFSETY);
			break;
		case CGoal::EReward::FIRE:
			_sprites[SPR_FIRE_ANNOUNCE]->Render(_x + ANNOUNCE_OFFSETX, _y + ANNOUNCE_OFFSETY);
			break;
		case CGoal::EReward::STAR:
			_sprites[SPR_STAR_ANNOUNCE]->Render(_x + ANNOUNCE_OFFSETX, _y + ANNOUNCE_OFFSETY);
			break;
		}
		break;
	}
}


void CGoal::ChangeReward()
{
	if (_reward == EReward::MUSHROOM) {
		_reward = EReward::FIRE;
	}
	else if (_reward == EReward::FIRE) {
		_reward = EReward::STAR;
	}
	else if (_reward == EReward::STAR) {
		_reward = EReward::MUSHROOM;
	}
}

void CGoal::SetState(EState state)
{
	_state = state;
	switch (state)
	{
	case CGoal::EState::IDLE:
		_animations[ANI_SPIN_IDLE]->Play(true);
		_nextRewardCountdown = NEXT_REWARD_COUNTDOWN;
		break;

	case CGoal::EState::ACQUIRED:
		_animations[ANI_SPIN_END]->Play(true);
		_renderX = _x;
		_renderY = _y;
		_acquireCoundown = ACQUIRE_COUNTDOWN;
		break;

	case CGoal::EState::ANNOUNCE:
		_announceCountdown = ANNOUNCE_COUNTDOWN;
		break;
	}
}


int CGoal::IsCollidable()
{
	switch (_state)
	{
	case CGoal::EState::IDLE:
		return 1;
		break;

	case CGoal::EState::ACQUIRED:
	case CGoal::EState::ANNOUNCE:
		return 0;
		break;
	}
	return 0;
}

int CGoal::IsBlocking()
{
	return 0;
}

void CGoal::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = _x + BODY_OFFSETX - (BODY_WIDTH / 2);
	right = _x + BODY_OFFSETX + (BODY_WIDTH / 2);
	top = _y + BODY_OFFSETY + (BODY_HEIGHT / 2);
	bottom = _y + BODY_OFFSETY - (BODY_HEIGHT / 2);
}
