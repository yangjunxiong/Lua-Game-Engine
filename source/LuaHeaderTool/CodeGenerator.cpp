#include "pch.h"
#include "CodeGenerator.h"
#include <fstream>
#include <sstream>

using namespace GameEngine::HeaderTool;
using namespace std;
using Item = SyntaxAnalyzer::Item;
using ItemType = SyntaxAnalyzer::ItemType;

const std::vector<CodeGenerator::StateFunction> CodeGenerator::sStateFunctions =
{
	nullptr,
	&CodeGenerator::ClassState,
	&CodeGenerator::EndClassState,
	&CodeGenerator::VariableState,
	&CodeGenerator::FunctionState,
	&CodeGenerator::ConstructorState
};

const std::vector<CodeGenerator::LuaStateFunction> CodeGenerator::sLuaStateFunctions =
{
	nullptr,
	&CodeGenerator::LuaClassState,
	&CodeGenerator::LuaEndClassState,
	&CodeGenerator::LuaVariableState,
	&CodeGenerator::LuaFunctionState,
	&CodeGenerator::LuaConstructorState
};

void CodeGenerator::GenerateCPP(const std::vector<Item>& items, const std::string& source, const std::string& outputPath)
{
	std::ofstream file;
	std::string output = outputPath + source + "_generated.h";
	file.open(output, ios::out);

	// Output generate comment
	file << "// Genereated binding class" << endl;
	file << "// Don't change this file manually" << endl << endl;

	// Output include files
	file << "#include \"" << source << ".h" << "\"" << endl;
	file << "#include \"LuaBind.h\"" << endl;
	file << endl;

	// Output namespace
	file << "namespace GameEngine" << endl;
	file << "{" << endl;

	// Process all items and output code
	mItems = &items;
	for (size_t i = 0; i < items.size(); ++i)
	{
		const Item& item = items[i];
		auto func = sStateFunctions[static_cast<size_t>(item.mType)];
		if (func != nullptr)
		{
			(this->*func)(item, file);
		}
	}
	mItems = nullptr;

	// End namespace
	file << "}" << endl;

	file.close();
}

void CodeGenerator::GenerateLua(const std::vector<SyntaxAnalyzer::Item>& items, const std::string& output)
{
	// Read existing lua file content, to check to duplicated code
	ifstream ifile;
	ifile.open(output, ios::in);
	vector<string> content;
	if (!ifile.bad())
	{
		while (getline(ifile, content.emplace_back()))
		{
		}
	}
	ifile.close();

	// Process all items and output code
	mItems = &items;
	for (size_t i = 0; i < items.size(); ++i)
	{
		const Item& item = items[i];
		auto func = sLuaStateFunctions[static_cast<size_t>(item.mType)];
		if (func != nullptr)
		{
			(this->*func)(item, content);
		}
	}
	mItems = nullptr;

	// Write lua file
	ofstream ofile;
	ofile.open(output, ios::out);
	for (const auto& line : content)
	{
		ofile << line << endl;
	}

	ofile.close();
}

void CodeGenerator::WriteIndentation(std::ofstream& file)
{
	for (size_t i = 0; i < mIndentation; ++i)
	{
		file << "\t";
	}
}

bool CodeGenerator::FindLine(const std::vector<std::string>& content, const std::string& search)
{
	return std::find_if(content.begin(), content.end(), [&search](const std::string& line)
		{
			return line == search;
		}) != content.cend();
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

void CodeGenerator::EndClassState(const SyntaxAnalyzer::Item& /*item*/, std::ofstream& file)
{
	// Write end of member register
	--mIndentation;
	WriteIndentation(file);
	file << "};" << endl;

	// Write end of class header
	--mIndentation;
	WriteIndentation(file);
	file << "};" << endl;
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

void CodeGenerator::LuaClassState(const SyntaxAnalyzer::Item& item, std::vector<std::string>& content)
{
	char buffer[1024];
	sprintf_s(buffer, "-- C++ class %s, parent class %s", item.mClassName.c_str(), item.mParentClassName.empty() ? "(None)" : item.mParentClassName.c_str());
	if (!FindLine(content, buffer))
	{
		content.emplace_back(buffer);
	}
}

void CodeGenerator::LuaEndClassState(const SyntaxAnalyzer::Item& item, std::vector<std::string>& content)
{
	item;
	content;
}

void CodeGenerator::LuaVariableState(const SyntaxAnalyzer::Item& item, std::vector<std::string>& content)
{
	char buffer[1024];
	sprintf_s(buffer, "-- Class %s variable: %s%s%s %s",
		item.mClassName.c_str(),
		item.mWritable ? "" : "const ",
		(item.mStatic ? "static " : ""),
		item.mMemberType.c_str(),
		item.mToken.String.c_str());
	if (!FindLine(content, buffer))
	{
		content.emplace_back(buffer);
	}
}

void CodeGenerator::LuaFunctionState(const SyntaxAnalyzer::Item& item, std::vector<std::string>& content)
{
	ostringstream buffer;
	buffer << "-- Class " << item.mClassName << " function: ";
	if (!item.mWritable)
	{
		buffer << "const ";
	}
	if (item.mStatic)
	{
		buffer << "static ";
	}
	buffer << item.mMemberType << " " << item.mToken.String << "(";
	for (size_t i = 0; i < item.mArgumentList.size(); ++i)
	{
		buffer << item.mArgumentList[i] << (i < item.mArgumentList.size() - 1 ? ", " : "");
	}
	buffer << ")";

	string str = buffer.str();
	if (!FindLine(content, str))
	{
		content.emplace_back(str);
	}
}

void CodeGenerator::LuaConstructorState(const SyntaxAnalyzer::Item& item, std::vector<std::string>& content)
{
	ostringstream buffer;
	buffer << "-- Class " << item.mClassName << " constructor: ";
	buffer << item.mToken.String << "(";
	for (size_t i = 0; i < item.mArgumentList.size(); ++i)
	{
		buffer << item.mArgumentList[i] << (i < item.mArgumentList.size() - 1 ? ", " : "");
	}
	buffer << ")";

	string str = buffer.str();
	if (!FindLine(content, str))
	{
		content.emplace_back(str);
	}
}
