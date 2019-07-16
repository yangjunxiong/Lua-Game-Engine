#pragma once
#include "SyntaxAnalyzer.h"
#include <vector>

namespace GameEngine::HeaderTool
{
	class CodeGenerator final
	{
	public:
		void GenerateCPP(const std::vector<SyntaxAnalyzer::Item>& items, const std::string& source, const std::string& output);
		void GenerateLua(const std::vector<SyntaxAnalyzer::Item>& items, const std::string& output);

	private:
		using StateFunction = void (CodeGenerator::*)(const SyntaxAnalyzer::Item&, std::ofstream&);
		const static std::vector<StateFunction> sStateFunctions;
		const static std::vector<StateFunction> sLuaStateFunctions;
		inline void WriteIndentation(std::ofstream& file);
		inline std::string CPPToLuaType(const std::string& type) const;

		void ClassState(const SyntaxAnalyzer::Item& item, std::ofstream& file);
		void EndClassState(const SyntaxAnalyzer::Item& item, std::ofstream& file);
		void VariableState(const SyntaxAnalyzer::Item& item, std::ofstream& file);
		void FunctionState(const SyntaxAnalyzer::Item& item, std::ofstream& file);
		void ConstructorState(const SyntaxAnalyzer::Item& item, std::ofstream& file);
		void EnumState(const SyntaxAnalyzer::Item& item, std::ofstream& file);
		void WriteEvent(const SyntaxAnalyzer::Item& item, std::ofstream& file);

		void LuaClassState(const SyntaxAnalyzer::Item& item, std::ofstream& file);
		void LuaEndClassState(const SyntaxAnalyzer::Item& item, std::ofstream& file);
		void LuaVariableState(const SyntaxAnalyzer::Item& item, std::ofstream& file);
		void LuaFunctionState(const SyntaxAnalyzer::Item& item, std::ofstream& file);
		void LuaConstructorState(const SyntaxAnalyzer::Item& item, std::ofstream& file);
		void LuaEnumState(const SyntaxAnalyzer::Item& item, std::ofstream& file);

		static const inline std::string sLuaHeaderLine = "-- AUTO GENERATED INTELLISENSE INFO. DO NOT MODIFY";
		static const inline std::string sLuaEndHeaderLine = "-- END AUTO GENERATED INFO";

		size_t mIndentation = 0;
		const std::vector<SyntaxAnalyzer::Item>* mItems = nullptr;
	};
}