#include "pch.h"
#include "CodeGenerator.h"
#include <fstream>

using namespace GameEngine::HeaderTool;
using namespace std;
using Item = SyntaxAnalyzer::Item;
using ItemType = SyntaxAnalyzer::ItemType;

const std::vector<CodeGenerator::StateFunction> CodeGenerator::sFunctions =
{
	nullptr,
	&CodeGenerator::ClassState,
	&CodeGenerator::EndClassState,
	&CodeGenerator::VariableState,
	&CodeGenerator::FunctionState
};

void CodeGenerator::GenerateCPP(const std::vector<Item>& items, const std::string& sourceFile)
{
	std::ofstream file;
	std::string generatedFile = sourceFile.substr(0, sourceFile.size() - 2);
	file.open(generatedFile + "_generated.h", ios::out);

	// Output include files
	file << "#include \"" << sourceFile << "\"" << endl;
	file << "#include \"LuaBind.h\"" << endl;
	file << endl;

	for (size_t i = 0; i < items.size(); ++i)
	{
		const Item& item = items[i];
		auto func = sFunctions[static_cast<size_t>(item.mType)];
		if (func != nullptr)
		{
			(this->*func)(item, file);
		}
	}

	file.close();
}

void CodeGenerator::GenerateLua() {}

void CodeGenerator::WriteIndentation(std::ofstream& file)
{
	for (size_t i = 0; i < mIndentation; ++i)
	{
		file << "\t";
	}
}

void CodeGenerator::ClassState(const SyntaxAnalyzer::Item& item, std::ofstream& file)
{
	// Write class header
	file << "class " << item.mClassName << "_generated final" << endl << "{" << endl;
	++mIndentation;
	file << "private:" << endl;
	WriteIndentation(file);
	file << "using LuaWrapper = GameEngine::Lua::LuaWrapper<" << item.mClassName << ">;" << endl;
	WriteIndentation(file);
	file << "using LuaBind = GameEngine::Lua::LuaBind;" << endl;
	file << "public:" << endl;

	// Write class register
	WriteIndentation(file);
	file << "static void RegisterClass(lua_State* L)" << endl;
	WriteIndentation(file);
	file << "{" << endl;
	++mIndentation;
	WriteIndentation(file);
	file << "LuaWrapper::Register";
	if (item.mParentClassName.empty())
	{
		file << "(L);" << endl;
	}
	else
	{
		file << "<" << item.mParentClassName << ">(L);" << endl;
	}
	--mIndentation;
	WriteIndentation(file);
	file << "};" << endl;
	
	// Write member register
	WriteIndentation(file);
	file << "static void RegisterMember(LuaBind& bind)" << endl;
	WriteIndentation(file);
	file << "{" << endl;
	++mIndentation;
}

void CodeGenerator::EndClassState(const SyntaxAnalyzer::Item& item, std::ofstream& file)
{
	// Write end of member register
	--mIndentation;
	WriteIndentation(file);
	file << "};" << endl;

	// Write end of class header
	--mIndentation;
	WriteIndentation(file);
	file << "};" << endl;

	// Write macro for pointer type
	file << "DECLARE_LUA_WRAPPER(" << item.mClassName << ", \"" << item.mClassName << "\");" << endl << endl;
}

void CodeGenerator::VariableState(const SyntaxAnalyzer::Item& item, std::ofstream& file)
{
	WriteIndentation(file);
	file << "bind.SetProperty";
	if (item.mStatic)
	{
		file << "<" << item.mClassName << ">";
	}
	file << "(\"" << item.mToken.String << "\", &" << item.mClassName << "::" << item.mToken.String;
	if (!item.mWritable)
	{
		file << ", false";
	}
	file << ");" << endl;
}

void CodeGenerator::FunctionState(const SyntaxAnalyzer::Item& item, std::ofstream& file)
{
	WriteIndentation(file);
	file << "bind.SetFunction";
	if (item.mStatic)
	{
		file << "<" << item.mClassName << ">";
	}
	file << "(\"" << item.mToken.String << "\", &" << item.mClassName << "::" << item.mToken.String << ");" << endl;
}
