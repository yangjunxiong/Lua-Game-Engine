#pragma once
#include "SyntaxAnalyzer.h"
#include <vector>

namespace GameEngine::HeaderTool
{
	class CodeGenerator final
	{
	public:
		void GenerateCPP(const std::vector<SyntaxAnalyzer::Item>& items, const std::string& sourceFile);
		void GenerateLua();

	private:
		using StateFunction = void (CodeGenerator::*)(const SyntaxAnalyzer::Item&, std::ofstream&);
		const static std::vector<StateFunction> sFunctions;
		inline void WriteIndentation(std::ofstream& file);
		void ClassState(const SyntaxAnalyzer::Item& item, std::ofstream& file);
		void EndClassState(const SyntaxAnalyzer::Item& item, std::ofstream& file);
		void VariableState(const SyntaxAnalyzer::Item& item, std::ofstream& file);
		void FunctionState(const SyntaxAnalyzer::Item& item, std::ofstream& file);

		int mIndentation = 0;
	};
}