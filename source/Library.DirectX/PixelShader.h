#pragma once

#include <vector>
#include "Shader.h"

namespace GameEngine
{
	class PixelShader final : public Shader
	{
		RTTI_DECLARATIONS(PixelShader, Shader)

	public:
		PixelShader(const winrt::com_ptr<ID3D11PixelShader>& pixelShader);
		PixelShader(const PixelShader&) = default;
		PixelShader& operator=(const PixelShader&) = default;
		PixelShader(PixelShader&&) = default;
		PixelShader& operator=(PixelShader&&) = default;
		~PixelShader() = default;

		winrt::com_ptr<ID3D11PixelShader> Shader() const;

	private:
		winrt::com_ptr<ID3D11PixelShader> mShader;
	};
}
