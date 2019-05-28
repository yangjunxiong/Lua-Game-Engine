#pragma once
#include "Action.h"

namespace GameEngine
{
	class ActionLoadJson final : public Action
	{
		RTTI_DECLARATIONS(ActionLoadJson, Action);

	public:
		ActionLoadJson();
		virtual void Update(WorldState& state) override;
		virtual gsl::owner<ActionLoadJson*> Clone() const override;
		const std::string& GetLoadClass() const;
		const std::string& GetLoadParentKey() const;
		const std::string& GetJsonFileName() const;
		void SetLoadClass(const std::string& name);
		void SetLoadParentKey(const std::string& name);
		void SetJsonFileName(const std::string& name);
		static const Vector<Attributed::Signature> Signatures();

	private:
		std::string mClass;
		std::string mParentKey;
		std::string mJsonFile;
	};

	DECLARE_FACTORY(ActionLoadJson, Scope);
}