#pragma once
#include "Macro.h"
#include <glm/fwd.hpp>

namespace GameEngine
{
	CLASS();
	class Transform final
	{
		friend class Entity;

	public:
		CONSTRUCTOR();
		Transform() = default;
		Transform(const glm::mat4& matrix);
		Transform(const glm::vec4& position, const glm::vec4& rotation, const glm::vec4& scale);
		Transform(const Transform& other) = default;
		Transform(Transform&& other) = default;
		Transform& operator=(const Transform& other) = default;
		Transform& operator=(Transform&& other) = default;
		glm::vec4 operator*(const glm::vec4& point) const;
		Transform operator*(const Transform& trans) const;

		FUNCTION();
		void SetPosition(const glm::vec4& position);
		void SetPosition(float x, float y, float z);

		FUNCTION();
		void SetRotation(const glm::vec4& rotation);
		void SetRotation(float x, float y, float z);

		FUNCTION();
		void SetScale(const glm::vec4& scale);
		void SetScale(float x, float y, float z);

		FUNCTION();
		const glm::vec4& GetPosition() const;

		FUNCTION();
		const glm::vec4& GetRotation() const;

		FUNCTION();
		const glm::vec4& GetScale() const;

		const glm::mat4& GetMatrix() const;

		static bool Equal(float f1, float f2);
		static bool Equal(const glm::vec4& v1, const glm::vec4& v2);

	private:
		inline void UpdateMatrix();
		inline void UpdateTransform();

		glm::mat4 mMatrix = glm::mat4(1.f);
		glm::vec4 mPosition = glm::vec4(0.f);  // [x, y, z, 1]
		glm::vec4 mRotation = glm::vec4(0.f);  // [x in angle, y in angle, z in angle, 1]
		glm::vec4 mScale = glm::vec4(1.f);     // [x, y, z, 1]
	};
}
