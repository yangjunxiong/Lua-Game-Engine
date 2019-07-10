// Genereated binding class
// Don't change this file manually

#include "Transform.h"
#include "LuaBind.h"

namespace GameEngine
{
class Transform_generated final
{
private:
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void Lua_RegisterClass(LuaBind& bind)
	{
		bind.RegisterType<Transform>();
	};
	static void Lua_RegisterMember(LuaBind& bind)
	{
		bind;
		bind.SetConstructor<Transform>();
		bind.SetFunction<Transform, void,  const glm::vec4&>("SetPosition", &Transform::SetPosition);
		bind.SetFunction<Transform, void,  const glm::vec4&>("SetRotation", &Transform::SetRotation);
		bind.SetFunction<Transform, void,  const glm::vec4&>("SetScale", &Transform::SetScale);
		bind.SetFunction<Transform,  const glm::vec4&>("GetPosition", &Transform::GetPosition);
		bind.SetFunction<Transform,  const glm::vec4&>("GetRotation", &Transform::GetRotation);
		bind.SetFunction<Transform,  const glm::vec4&>("GetScale", &Transform::GetScale);
	};
};
}
