#pragma once

namespace GameEngine
{
	class HealthSystem
	{
	public:
		HealthSystem();
		~HealthSystem();
		
	private:
		int m_Health = 3;
	};
}