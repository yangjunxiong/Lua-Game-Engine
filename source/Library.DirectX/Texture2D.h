#pragma once

#include "Texture.h"
#include "Rectangle.h"

namespace GameEngine
{
	class Texture2D final : public Texture
	{
		RTTI_DECLARATIONS(Texture2D, Texture)

	public:
		Texture2D(const winrt::com_ptr<ID3D11ShaderResourceView>& shaderResourceView, const std::int32_t width, const std::int32_t height);
		Texture2D(const Texture2D&) = default;
		Texture2D& operator=(const Texture2D&) = default;
		Texture2D(Texture2D&&) = default;
		Texture2D& operator=(Texture2D&&) = default;
		~Texture2D() = default;

		std::int32_t Width() const;
		std::int32_t Height() const;
		Rectangle Bounds() const;

	private:
		std::int32_t mWidth;
		std::int32_t mHeight;
	};
}