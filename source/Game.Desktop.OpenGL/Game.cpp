#include "pch.h"
#include "Game.h"
#include "World.h"
#include "Sector.h"
#include "Entity.h"
#include "ActionHeader.h"
#include "Reaction.h"
#include "ReactionAttributed.h"
#include "EventMessageAttributed.h"
#include <algorithm>
#include "JsonSpriteParseHelper.h"

using namespace GameEngine;
using namespace std;
using namespace glm;

#define REGISTER_TYPE(_type, _parentType) AttributedTypeManager::RegisterType(_type::TypeIdClass(), _type::Signatures(), _parentType::TypeIdClass());

Game::Game()
{
	sInstance = this;
	RegisterType();
	CreateFactory();
}

Game::~Game()
{
	sInstance = nullptr;
	UnregisterType();
	DestroyFactory();
}

void Game::CreateInstance()
{
	if (sInstance == nullptr)
	{
		new Game();
	}
}

void Game::DestroyInstance()
{
	if (sInstance != nullptr)
	{
		delete sInstance;
	}
}

Game& Game::GetInstance()
{
	assert(sInstance != nullptr);
	return *sInstance;
}

void Game::RegisterType()
{
	// Entities
	REGISTER_TYPE(World, Attributed);
	REGISTER_TYPE(Sector, Attributed);
	REGISTER_TYPE(Entity, Attributed);

	// Actions
	REGISTER_TYPE(Action, Attributed);
	REGISTER_TYPE(ActionList, Action);
	REGISTER_TYPE(ActionListIf, ActionList);
	REGISTER_TYPE(ActionIncrement, Action);
	REGISTER_TYPE(ActionCreateAction, Action);
	REGISTER_TYPE(ActionDestroyAction, Action);
	REGISTER_TYPE(ActionEvent, Action);
	REGISTER_TYPE(ActionAdd, Action);
	REGISTER_TYPE(ActionCollision, Action);
	REGISTER_TYPE(ActionCircleCollision, ActionCollision);
	REGISTER_TYPE(ActionBoxCollision, ActionCollision);
	REGISTER_TYPE(ActionTriangleCollision, ActionCollision);
	REGISTER_TYPE(ActionCollisionEvent, ActionEvent);
	REGISTER_TYPE(ActionRender, Action);
	REGISTER_TYPE(ActionLoadJson, Action);
	REGISTER_TYPE(ActionExpression, Action);
	REGISTER_TYPE(ActionDestroy, Action);
	REGISTER_TYPE(ActionAnimation, Action);

	// Reactions
	REGISTER_TYPE(EventMessageAttributed, Attributed);
	REGISTER_TYPE(Reaction, ActionList);
	REGISTER_TYPE(ReactionAttributed, Reaction);

	// Other
	REGISTER_TYPE(AnimState, Attributed);
	REGISTER_TYPE(AnimClip, Attributed);

	AttributedTypeManager::FinalizeSignature();
}

void Game::CreateFactory()
{
	mFactories = {
		// Entities
		new SectorFactory(),
		new EntityFactory(),

		// Reactions
		new ReactionAttributedFactory(),

		// Actions
		new ActionAddFactory(),
		new ActionCreateActionFactory(),
		new ActionDestroyActionFactory(),
		new ActionEventFactory(),
		new ActionIncrementFactory(),
		new ActionListFactory(),
		new ActionListIfFactory(),
		new ActionCircleCollisionFactory(),
		new ActionBoxCollisionFactory(),
		new ActionTriangleCollisionFactory(),
		new ActionCollisionEventFactory(),
		new ActionRenderFactory(),
		new ActionLoadJsonFactory(),
		new ActionExpressionFactory(),
		new ActionDestroyFactory(),
		new ActionAnimationFactory(),

		// Other
		new AnimStateFactory(),
		new AnimClipFactory()
	};
}

void Game::UnregisterType()
{
	AttributedTypeManager::UnregisterAllTypes();
}

void Game::DestroyFactory()
{
	for (auto& factory : mFactories)
	{
		delete factory;
	}
	mFactories.Clear();
}

const int& Game::WindowWidth() const
{
	return mWindowSize.x;
}

const int& Game::WindowHeight() const
{
	return mWindowSize.y;
}

const ivec2& Game::WindowSize() const
{
	return mWindowSize;
}

const glm::mat4 Game::ProjectionMatrix() const
{
	return mProjectionMatrix;
}

const glm::mat4 Game::ViewMatrix() const
{
	return mViewMatrix;
}

const shared_ptr<SpriteSheet> Game::GetSpriteSheet(int index) const
{
	if (mSpriteSheetList.Size() > static_cast<std::size_t>(index))
	{
		return mSpriteSheetList[index];
	}
	
	return nullptr;
}

const shared_ptr<Sprite> Game:: GetSprite(int textureId, int x, int y) const
{
	auto it = mSpriteMap.Find(make_tuple(textureId, x, y));
	if (it != mSpriteMap.end())
	{
		return it->second;
	}

	return nullptr;
}

void Game::LoadSpriteSheet()
{
	JsonSpriteParseHelper helper;
	auto sprites = helper.ParseSpriteJson("Resources/Config/Sprite.json");
	string rootPath = "Resources/Sprites/";
	for (size_t i = 0; i < sprites.Size(); ++i)
	{
		const auto& info = sprites[i];
		string path = rootPath + info.mName;
		size_t width = info.mSize;
		auto it = mSpriteSheetList.PushBack(make_shared<SpriteSheet>(path.c_str(), width, width));
		for (auto& sprite : info.mSprites)
		{
			mSpriteMap.Insert(make_pair(make_tuple(int(i), sprite.first, sprite.second), (*it)->GetSprite(size_t(sprite.first), size_t(sprite.second))));
		}
	}
}

void Game::SetProjectionMatrix(const glm::mat4& proj)
{
	mProjectionMatrix = proj;
	GLint uniProj = glGetUniformLocation(mShaderProgram, "proj");
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));
}

void Game::SetViewMatrix(const glm::mat4& view)
{
	mViewMatrix = view;
	GLint uniView = glGetUniformLocation(mShaderProgram, "view");
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
}