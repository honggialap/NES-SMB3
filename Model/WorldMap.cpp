#pragma region INCLUDE
#include "WorldMap.h"

#include "SuperMarioBros3.h"
#pragma endregion

void CWorldMap::Load()
{
	CGameObject::Load();

	/* Read file */
	pugi::xml_document prefab;
	prefab.load_file(_source.c_str());

	// Command
	pugi::xml_node command = prefab.child("Prefab").child("Command");
	LEFT = command.attribute("LEFT").as_uint();
	UP = command.attribute("UP").as_uint();
	RIGHT = command.attribute("RIGHT").as_uint();
	DOWN = command.attribute("DOWN").as_uint();
	SELECT = command.attribute("SELECT").as_uint();

	pugi::xml_node camera = prefab.child("Prefab").child("Camera");
	CAMERA_ORIGIN_X = camera.attribute("x").as_float();
	CAMERA_ORIGIN_Y = camera.attribute("y").as_float();

	pugi::xml_node background = prefab.child("Prefab").child("Background");
	_backgroundX = background.attribute("x").as_float();
	_backgroundY = background.attribute("y").as_float();

	pugi::xml_node moveSpeed = prefab.child("Prefab").child("MoveSpeed");
	MOVE_SPEED = moveSpeed.attribute("MOVE_SPEED").as_float();

	pugi::xml_node kactus = prefab.child("Prefab").child("Kactus");
	for (auto element = kactus.child("Element");
		element;
		element = element.next_sibling("Element"))
	{
		_kactus.push_back(
			std::make_pair(
				element.attribute("x").as_float(),
				element.attribute("y").as_float()
			)
		);
	}

	pugi::xml_node map = prefab.child("Prefab").child("Map");
	for (auto node = map.child("Node");
		node;
		node = node.next_sibling("Node"))
	{
		_nodes[
			node.attribute("id").as_uint()
		] = std::make_tuple(
			node.attribute("x").as_float(),
			node.attribute("y").as_float(),
			node.attribute("left").as_uint(),
			node.attribute("top").as_uint(),
			node.attribute("right").as_uint(),
			node.attribute("bottom").as_uint(),
			node.attribute("flag").as_bool(),
			node.attribute("nextScene").as_uint()
		);
	}
	_startNode = map.attribute("startNode").as_uint();
}

void CWorldMap::Start()
{
	CGameObject::Start();
	_game->MoveCameraTo(CAMERA_ORIGIN_X, CAMERA_ORIGIN_Y);
	_animations[ANI_KACTUS]->Play(true);
	_animations[ANI_MARIO_SMALL]->Play(true);
	_animations[ANI_KACTUS]->Play(true);

	SetState(EState::IDLE);
	_currentNode = _startNode;
	_currentX = std::get<0>(_nodes.find(_currentNode)->second);
	_currentY = std::get<1>(_nodes.find(_currentNode)->second);
}

void CWorldMap::Update(float elapsedMs)
{
	if (!_start) Start();

	_animations[ANI_KACTUS]->Update(elapsedMs);
	_animations[ANI_MARIO_SMALL]->Update(elapsedMs);

	switch (_state)
	{
	case CWorldMap::EState::IDLE:
	{
		if (_game->IsKeyPressed(SELECT)) {
			if (std::get<6>(_nodes.find(_currentNode)->second)) {
				_game->PlayScene(std::get<7>(_nodes.find(_currentNode)->second));
			}
		}
		else if (_game->IsKeyPressed(LEFT)) {
			if (std::get<2>(_nodes.find(_currentNode)->second) != 0) {
				_targetNode = std::get<2>(_nodes.find(_currentNode)->second);
				_targetX = std::get<0>(_nodes.find(_targetNode)->second);
				_targetY = std::get<1>(_nodes.find(_targetNode)->second);
				SetState(EState::MOVE);
				break;
			}
		}
		else if (_game->IsKeyPressed(UP)) {
			if (std::get<3>(_nodes.find(_currentNode)->second) != 0) {
				_targetNode = std::get<3>(_nodes.find(_currentNode)->second);
				_targetX = std::get<0>(_nodes.find(_targetNode)->second);
				_targetY = std::get<1>(_nodes.find(_targetNode)->second);
				SetState(EState::MOVE);
				break;
			}
		}
		else if (_game->IsKeyPressed(RIGHT)) {
			if (std::get<4>(_nodes.find(_currentNode)->second) != 0) {
				_targetNode = std::get<4>(_nodes.find(_currentNode)->second);
				_targetX = std::get<0>(_nodes.find(_targetNode)->second);
				_targetY = std::get<1>(_nodes.find(_targetNode)->second);
				SetState(EState::MOVE);
				break;
			}
		}
		else if (_game->IsKeyPressed(DOWN)) {
			if (std::get<5>(_nodes.find(_currentNode)->second) != 0) {
				_targetNode = std::get<5>(_nodes.find(_currentNode)->second);
				_targetX = std::get<0>(_nodes.find(_targetNode)->second);
				_targetY = std::get<1>(_nodes.find(_targetNode)->second);
				SetState(EState::MOVE);
				break;
			}
		}
	}
	break;

	case CWorldMap::EState::MOVE:
	{
		if (_currentX < _targetX) {
			_currentX += MOVE_SPEED * elapsedMs;
			if (_currentX >= _targetX) {
				_currentNode = _targetNode;
				_currentX = _targetX;
				SetState(EState::IDLE);
				break;
			}
		}
		else if (_currentX > _targetX) {
			_currentX -= MOVE_SPEED * elapsedMs;
			if (_currentX <= _targetX) {
				_currentNode = _targetNode;
				_currentX = _targetX;
				SetState(EState::IDLE);
				break;
			}
		}

		if (_currentY < _targetY) {
			_currentY += MOVE_SPEED * elapsedMs;
			if (_currentY >= _targetY) {
				_currentNode = _targetNode;
				_currentY = _targetY;
				SetState(EState::IDLE);
				break;
			}
		}
		else if (_currentY > _targetY) {
			_currentY -= MOVE_SPEED * elapsedMs;
			if (_currentY <= _targetY) {
				_currentNode = _targetNode;
				_currentY = _targetY;
				SetState(EState::IDLE);
				break;
			}
		}
	}
	break;
	}
}

void CWorldMap::Render()
{
	_sprites[SPR_WORLDMAP1_BACKGROUND]->Render(_backgroundX, _backgroundY);

	for (auto element : _kactus)
	{
		_animations[ANI_KACTUS]->Render(element.first, element.second);
	}

	_animations[ANI_MARIO_SMALL]->Render(_currentX, _currentY);
}

void CWorldMap::SetState(EState state)
{
	_state = state;
	switch (_state)
	{
	case CWorldMap::EState::IDLE:
	{

	}
	break;

	case CWorldMap::EState::MOVE:
	{

	}
	break;
	}
}


int CWorldMap::IsCollidable()
{
	return 0;
}

int CWorldMap::IsBlocking()
{
	return 0;
}

void CWorldMap::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}
