#include "GameObject.h"
#include "Game.h"

CGameObject::CGameObject(pGame game, pScene scene, unsigned int id, std::string name, std::string source, float x, float y, int gx, int gy, unsigned int layer)
{
	_game = game;
	_scene = scene;
	_id = id;
	_name = name;
	_source = source;
	_x = x;
	_y = y;
	_gx = gx;
	_gy = gy;
	_layer = layer;
	_collider = new CCollider(this);
}

CGameObject::~CGameObject()
{
	if (_collider != nullptr)
	{
		delete _collider;
		_collider = nullptr;
	}

	for (auto soundClip : _soundClips)
	{
		delete soundClip.second;
		soundClip.second = nullptr;
	}
	_soundClips.clear();

	for (auto animation : _animations)
	{
		delete animation.second;
		animation.second = nullptr;
	}
	_animations.clear();

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
		AddSprite(
			spriteNode.attribute("id").as_uint(),
			new CSprite(
				this,
				spriteNode.attribute("left").as_int(),
				spriteNode.attribute("top").as_int(),
				spriteNode.attribute("width").as_int(),
				spriteNode.attribute("height").as_int(),
				spriteNode.attribute("offsetX").as_int(),
				spriteNode.attribute("offsetY").as_int(),
				_game->GetGraphics()->GetTexture(
					spriteNode.attribute("textureId").as_uint()
				)
			)
		);

	/* Animation */
	for (pugi::xml_node animationNode = prefabDoc.child("Prefab").child("Animation");
		animationNode;
		animationNode = animationNode.next_sibling("Animation"))
	{
		auto animation = new CAnimation(this);
		AddAnimation(
			animationNode.attribute("id").as_uint(),
			animation
		);

		for (pugi::xml_node frameNode = animationNode.child("Frame");
			frameNode;
			frameNode = frameNode.next_sibling("Frame"))
		{
			animation->AddFrame(
				frameNode.attribute("id").as_uint(),
				frameNode.attribute("time").as_float()
			);
		}
	}

	/* Sound Clip */
	for (pugi::xml_node soundClipNode = prefabDoc.child("Prefab").child("SoundClip");
		soundClipNode;
		soundClipNode = soundClipNode.next_sibling("SoundClip"))
		AddSoundClip(
			soundClipNode.attribute("id").as_uint(),
			_game->GetAudio()->LoadSoundClip(
				soundClipNode.attribute("source").as_string()
			)
		);
}

void CGameObject::Destroy()
{
	_destroy = true;
}
