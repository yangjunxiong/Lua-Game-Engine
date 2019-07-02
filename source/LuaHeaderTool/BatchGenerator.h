#pragma once
#include "CodeGenerator.h"
#include <vector>
#include <set>

#define REGISTER_FUNCTION "RegisterLua"
#define UNREGISTER_FUNCTION "UnregisterLua"

namespace GameEngine::HeaderTool
{
	class BatchGenerator final
	{
	public:
		void Reset();
		void BatchParse(const std::string& dir);
		void BatchWriteCPP(const std::string& dir);
		void BatchWriteLua(const std::string& dir);
		void WriteRegister(const std::string& filePath, const std::string& generatedFilePath);
		void ClearGenerated(const std::vector<std::string>& dirs);

	private:
		struct ParsedFile
		{
			std::string mFileName;
			std::vector<HeaderTokenizer::Token> mTokens;
			std::vector<SyntaxAnalyzer::Item> mItems;
		};

		std::vector<ParsedFile> mParsedFileList;
		std::map<std::string, const SyntaxAnalyzer::Item*> mClassMap;
		std::set<const SyntaxAnalyzer::Item*> mGeneratedClassSet;

		void ParseFile(const std::filesystem::path& filePath);
		void WriteWrapperRegister(const SyntaxAnalyzer::Item& item, std::ofstream& file);
	};
}