// Genereated binding class
// Don't change this file manually

#include "Matrix.h"
#include "LuaBind.h"

namespace GameEngine
{
class Matrix_generated final
{
private:
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void Lua_RegisterClass(LuaBind& bind)
	{
		bind.RegisterType<Matrix>();
	};
	static void Lua_RegisterMember(LuaBind& bind)
	{
		bind;
		bind.SetConstructor<Matrix>();
		bind.SetFunction<Matrix, Vector3>("Forward", &Matrix::Forward);
		bind.SetFunction<Matrix, Vector3>("Up", &Matrix::Up);
		bind.SetFunction<Matrix, Vector3>("Right", &Matrix::Right);
		bind.SetProperty<Matrix>("Identity", &Matrix::Identity);
		bind.SetProperty<Matrix>("Zero", &Matrix::Zero);
	};
};
}
