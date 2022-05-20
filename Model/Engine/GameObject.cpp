#pragma region INCLUDE
#include "GameObject.h"
#include "Game.h"
#include "Ultilities.h"
#pragma endregion

CGameObject::CGameObject(
	pGame game, 
	unsigned int ID, std::string name, std::string source, 
	float x, float y, 
	int gx, int gy,
	unsigned int layer
)
{
	_game = game;
	_ID = ID;
	_name = name;
	_source = source;
	_x = x;
	_y = y;
	_gx = gx;
	_gy = gy;
	_layer = layer;
}

CGameObject::~CGameObject()
{
	for (auto sprite : _sprites) 
	{
		delete sprite.second;
		sprite.second = nullptr;
	}
	_sprites.clear();
}

void CGameObject::Load()
{
	/* Read file */
	pugi::xml_document prefabDoc;
	prefabDoc.load_file(_source.c_str());

	/* Sprite */
	for (pugi::xml_node spriteNode = prefabDoc.child("Prefab").child("Sprite");
		spriteNode;
		spriteNode = spriteNode.next_sibling("Sprite")) 
	{
		auto sprite = new CSprite(
			this,
			spriteNode.attribute("left").as_uint(),
			spriteNode.attribute("top").as_uint(),
			spriteNode.attribute("width").as_uint(),
			spriteNode.attribute("height").as_uint(),
			spriteNode.attribute("offsetX").as_int(),
			spriteNode.attribute("offsetY").as_int(),
			_game->GetGraphics()->GetTexture(
				spriteNode.attribute("textureID").as_uint()
			)
		);

		AddSprite(
			spriteNode.attribute("ID").as_uint(),
			sprite
		);
	}

	/* Animation */
	for (pugi::xml_node animationNode = prefabDoc.child("Prefab").child("Animation");
		animationNode;
		animationNode = animationNode.next_sibling("Animation")) {
		auto animation = new CAnimation(this);

		for (pugi::xml_node frameNode = animationNode.child("Frame");
			frameNode;
			frameNode = frameNode.next_sibling("Frame")) {
			animation->AddFrame(
				frameNode.attribute("ID").as_uint(),
				frameNode.attribute("time").as_float()
			);
		}

		AddAnimation(
			animationNode.attribute("ID").as_uint(),
			animation
		);
	}
}

void CGameObject::AddSprite(unsigned int ID, pSprite sprite)
{
	if (_sprites.find(ID) != _sprites.end()) 
	{
		DebugOut(L"[Engine] Sprite ID is already existed: %d.\n", ID);

		delete sprite;
		sprite = nullptr;

		return;
	}

	_sprites[ID] = sprite;
}

void CGameObject::AddAnimation(
	unsigned int ID,
	pAnimation animation
) {
	if (_animations.find(ID) != _animations.end()) {
		DebugOut(L"[Engine] Animation ID is already existed: %d.\n", ID);

		delete animation;
		animation = nullptr;

		return;
	}

	_animations[ID] = animation;
}
