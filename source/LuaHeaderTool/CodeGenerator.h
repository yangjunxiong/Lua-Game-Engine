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
		using LuaStateFunction = void (CodeGenerator::*)(const SyntaxAnalyzer::Item&, std::vector<std::string>&);
		const static std::vector<StateFunction> sStateFunctions;
		const static std::vector<LuaStateFunction> sLuaStateFunctions;
		inline void WriteIndentation(std::ofstream& file);
		inline bool FindLine(const std::vector<std::string>& content, const std::string& search);

		void ClassState(const SyntaxAnalyzer::Item& item, std::ofstream& file);
		void EndClassState(const SyntaxAnalyzer::Item& item, std::ofstream& file);
		void VariableState(const SyntaxAnalyzer::Item& item, std::ofstream& file);
		void FunctionState(const SyntaxAnalyzer::Item& item, std::ofstream& file);
		void ConstructorState(const SyntaxAnalyzer::Item& item, std::ofstream& file);

		void LuaClassState(const SyntaxAnalyzer::Item& item, std::vector<std::string>& content);
		void LuaEndClassState(const SyntaxAnalyzer::Item& item, std::vector<std::string>& content);
		void LuaVariableState(const SyntaxAnalyzer::Item& item, std::vector<std::string>& content);
		void LuaFunctionState(const SyntaxAnalyzer::Item& item, std::vector<std::string>& content);
		void LuaConstructorState(const SyntaxAnalyzer::Item& item, std::vector<std::string>& content);

		size_t mIndentation = 0;
		const std::vector<SyntaxAnalyzer::Item>* mItems = nullptr;
	};
}