#pragma once
#include "Attributed.h"
#include "Factory.h"
#include "GameClock.h"
#include "GameTime.h"
#include "WorldState.h"
#include "World.h"
#include "JsonParseMaster.h"
#include "JsonTableParseHelper.h"
#include "SpriteSheet.h"
#include <glm/glm.hpp>

namespace GameEngine
{
	class Game final
	{
	public:
		static void CreateInstance();
		static void DestroyInstance();
		static Game& GetInstance();
		
		const int& WindowWidth() const;
		const int& WindowHeight() const;
		const glm::ivec2& WindowSize() const;
		const glm::mat4 ProjectionMatrix() const;
		const glm::mat4 ViewMatrix() const;
		const shared_ptr<SpriteSheet> GetSpriteSheet(int index) const;
		const shared_ptr<Sprite> GetSprite(int textureId, int x, int y) const;

		void SetProjectionMatrix(const glm::mat4& proj);
		void SetViewMatrix(const glm::mat4& view);

		GLuint mShaderProgram = 0;
		World* mWorld = nullptr;

	private:
		Game();
		Game(const Game& other) = delete;
		Game(Game&& other) = delete;
		Game& operator=(const Game& other) = delete;
		Game& operator=(Game&& other) = delete;
		~Game();
		static inline Game* sInstance = nullptr;

		void RegisterType();
		void CreateFactory();
		void UnregisterType();
		void DestroyFactory();
		Vector<Factory<Scope>*> mFactories;

		glm::ivec2 mWindowSize = glm::ivec2(1080, 1080);
		glm::mat4 mProjectionMatrix = glm::mat4(0.075, 0, 0, 0, 0, 0.075, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1);
		glm::mat4 mViewMatrix = glm::lookAt(glm::vec3(0, 0, -500), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		Vector<shared_ptr<SpriteSheet>> mSpriteSheetList;
		HashMap<std::tuple<int, int, int>, shared_ptr<Sprite>> mSpriteMap;
	};
}