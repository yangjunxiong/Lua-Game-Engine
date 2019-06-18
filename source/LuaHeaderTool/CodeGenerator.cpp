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
	&CodeGenerator::FunctionState,
	&CodeGenerator::ConstructorState
};

void CodeGenerator::GenerateCPP(const std::vector<Item>& items, const std::string& source, const std::string& outputPath)
{
	std::ofstream file;
	std::string output = outputPath + source + "_generated.h";
	file.open(output, ios::out);

	// Output include files
	file << "#include \"" << source << ".h" << "\"" << endl;
	file << "#include \"LuaBind.h\"" << endl;
	file << endl;

	// Output namespace
	file << "namespace GameEngine" << endl;
	file << "{" << endl;

	for (size_t i = 0; i < items.size(); ++i)
	{
		const Item& item = items[i];
		auto func = sFunctions[static_cast<size_t>(item.mType)];
		if (func != nullptr)
		{
			(this->*func)(item, file);
		}
	}

	// End namespace
	file << "}" << endl;

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
	file << "static void Lua_RegisterClass(lua_State* L)" << endl;
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
	file << "static void Lua_RegisterMember(LuaBind& bind)" << endl;
	WriteIndentation(file);
	file << "{" << endl;
	++mIndentation;
	WriteIndentation(file);
	file << "bind;" << endl;
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

void CodeGenerator::ConstructorState(const SyntaxAnalyzer::Item& item, std::ofstream& file)
{
	WriteIndentation(file);
	file << "bind.SetConstructor<";
	file << item.mClassName;
	for (const auto& arg : item.mArgumentList)
	{
		file << ", " << arg;
	}
	file << ">();" << endl;
}
