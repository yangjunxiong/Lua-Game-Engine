// Genereated binding class
// Don't change this file manually

#include "StaticMeshMaterial.h"
#include "LuaBind.h"

namespace GameEngine
{
class StaticMeshMaterial_generated final
{
private:
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void Lua_RegisterClass(LuaBind& bind)
	{
		bind.RegisterType<StaticMeshMaterial, Material>();
	};
	static void Lua_RegisterMember(LuaBind& bind)
	{
		bind;
		bind.SetFunction<StaticMeshMaterial, void,  const std::string&>("SetColorMap", &StaticMeshMaterial::SetColorMap);
		bind.SetFunction<StaticMeshMaterial, Vector3>("CoverColor", &StaticMeshMaterial::CoverColor);
		bind.SetFunction<StaticMeshMaterial, void,  const Vector3&>("SetCoverColor", &StaticMeshMaterial::SetCoverColor);
		bind.SetFunction<StaticMeshMaterial, float>("Outline", &StaticMeshMaterial::Outline);
		bind.SetFunction<StaticMeshMaterial, void, float>("SetOutline", &StaticMeshMaterial::SetOutline);
	};
};
}
