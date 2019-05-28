#pragma once
#include "Attributed.h"
#include "Foo.h"
#include "Factory.h"

namespace GameEngine
{
	class AttributedFoo : public Attributed, public Foo
	{
		RTTI_DECLARATIONS(AttributedFoo, Attributed);

	public:
		explicit AttributedFoo(int data = 0);
		AttributedFoo(const AttributedFoo& other) = default;
		AttributedFoo(AttributedFoo&& other) = default;
		AttributedFoo& operator=(const AttributedFoo& other) = default;
		AttributedFoo& operator=(AttributedFoo&& other) = default;
		virtual ~AttributedFoo() = default;
		bool operator==(const AttributedFoo& other) const;
		bool operator!=(const AttributedFoo& other) const;
		virtual std::string ToString() const override;
		virtual gsl::owner<AttributedFoo*> Clone() const override;
		virtual bool Equals(const RTTI* other) const override;

		static const Vector<Attributed::Signature> Signatures();
		static const size_t ArraySize = 3;

		int32_t mInt = 1;
		float mFloat = 1.f;
		glm::vec4 mVector = glm::vec4(1.f);
		glm::mat4 mMatrix = glm::mat4(1.f);
		std::string mString = "1";
		RTTI* mPointer = nullptr;

		int32_t mIntArray[ArraySize] = { 1, 2, 3 };
		float mFloatArray[ArraySize] = { 1.f, 2.f, 3.f };
		glm::vec4 mVectorArray[ArraySize] = { glm::vec4(1.f), glm::vec4(2.f), glm::vec4(3.f) };
		glm::mat4 mMatrixArray[ArraySize] = { glm::mat4(1.f), glm::mat4(2.f), glm::mat4(3.f) };
		std::string mStringArray[ArraySize] = { std::string("1"), std::string("2"), std::string("3") };
		RTTI* mPointerArray[ArraySize] = { nullptr, nullptr, nullptr };
	};

	DECLARE_FACTORY(AttributedFoo, Foo);
}
