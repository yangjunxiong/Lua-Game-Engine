#pragma once
#include "Entity.h"
#include "Factory.h"

namespace GameEngine
{
	class Obstacle :
		public Entity

	{
		RTTI_DECLARATIONS(Obstacle, Entity);
	public:
		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="type"></param>
		Obstacle(RTTI::IdType type = Obstacle::TypeIdClass());

		/// <summary>
		/// Destructor
		/// </summary>
		~Obstacle() override;


	};

	DECLARE_FACTORY(Obstacle, Scope);
}

