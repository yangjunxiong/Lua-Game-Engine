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
	&CodeGenerator::ConstructorState,
	&CodeGenerator::EnumState
};

const std::vector<CodeGenerator::StateFunction> CodeGenerator::sLuaStateFunctions =
{
	nullptr,
	&CodeGenerator::LuaClassState,
	&CodeGenerator::LuaEndClassState,
	&CodeGenerator::LuaVariableState,
	&CodeGenerator::LuaFunctionState,
	&CodeGenerator::LuaConstructorState,
	&CodeGenerator::LuaEnumState
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

	// Remove blank lines at the end
	while (content.size() > 0 && (content.back().empty() || content.back() == "\n"))
	{
		content.erase(content.end() - 1);
	}

	// Remove existing infos if they exist
	auto infoStart = std::find(content.begin(), content.end(), sLuaHeaderLine);
	if (infoStart != content.end())
	{
		auto infoEnd = std::find(content.begin(), content.end(), sLuaEndHeaderLine);
		if (infoEnd != content.end())
		{
			content.erase(infoStart, infoEnd + 1);
		}
	}

	// Process all items and output code
	ofstream ofile;
	ofile.open(output, ios::out);
	ofile << sLuaHeaderLine << endl;

	mItems = &items;
	bool inClass = false;
	std::vector<const Item*> functionItems;
	for (size_t i = 0; i < items.size(); ++i)
	{
		// Get state function
		const Item& item = items[i];
		auto func = sLuaStateFunctions[static_cast<size_t>(item.mType)];
		if (func == nullptr)
		{
			continue;
		}

		// Select different item types
		// Class declaration comes first, then property, then function, then class end
		if (inClass)
		{
			// Finish up the class and print all functions
			if (item.mType == ItemType::EndClass)
			{
				// Output end class
				(this->*func)(item, ofile);

				// Output functions and constructor
				for (const auto funcItem : functionItems)
				{
					if (funcItem->mType == ItemType::Function)
					{
						func = sLuaStateFunctions[static_cast<size_t>(ItemType::Function)];
					}
					else
					{
						func = sLuaStateFunctions[static_cast<size_t>(ItemType::Constructor)];
					}
					(this->*func)(*funcItem, ofile);
				}
				inClass = false;
			}
			else
			{
				if (item.mType == ItemType::Constructor || item.mType == ItemType::Function)
				{
					functionItems.push_back(&item);
				}
				else
				{
					(this->*func)(item, ofile);
				}
			}
		}
		else
		{
			if (item.mType == ItemType::Class)
			{
				functionItems.clear();
				inClass = true;
				(this->*func)(item, ofile);
			}
			else if (item.mType == ItemType::Enum)
			{
				(this->*func)(item, ofile);
			}
		}
	}
	mItems = nullptr;
	ofile << sLuaEndHeaderLine << endl;

	// Append existing content
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

void CodeGenerator::ClassState(const SyntaxAnalyzer::Item& item, std::ofstream& file)
{
	WriteEvent(item, file);

	// Write class header
	file << "class " << item.mClassName << "_generated final" << endl << "{" << endl;
	++mIndentation;
	file << "private:" << endl;
	WriteIndentation(file);
	file << "using LuaBind = GameEngine::Lua::LuaBind;" << endl;
	file << "public:" << endl;

	// Write class register
	WriteIndentation(file);
	file << "static void Lua_RegisterClass(LuaBind& bind)" << endl;
	WriteIndentation(file);
	file << "{" << endl;
	++mIndentation;
	WriteIndentation(file);
	file << "bind.RegisterType<" << item.mClassName;
	if (!item.mParentClassName.empty())
	{
		file << ", " << item.mParentClassName;
	}
	file << ">();" << endl;
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

	// Fill in template arguments
	file << "<";
	file << item.mClassName << ", ";
	if (item.mStatic)
	{
		file << item.mMemberType;
		file << "(*)";
		file << "(";
		for (size_t i = 0; i < item.mArgumentList.size(); ++i)
		{
			file << item.mArgumentList[i];
			if (i < item.mArgumentList.size() - 1)
			{
				file << ", ";
			}
		}
		file << ")";
	}
	else
	{
		file << item.mMemberType;
		for (const auto& arg : item.mArgumentList)
		{
			file << ", " << arg;
		}
	}
	file << ">";

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

void CodeGenerator::EnumState(const SyntaxAnalyzer::Item& item, std::ofstream& file)
{
	// Need to treat enum as class too
	ClassState(item, file);
	for (const auto& name : item.mEnumList)
	{
		WriteIndentation(file);
		file << "bind.SetEnumValue<" << item.mClassName << ">(";
		file << "\"" << name << "\"" << ", static_cast<int>(" << item.mClassName << "::" << name << "));\n";
	}
	EndClassState(item, file);
}

void CodeGenerator::WriteEvent(const SyntaxAnalyzer::Item& item, std::ofstream& file)
{
	if (!SyntaxAnalyzer::Item::HasFlag(item.mFlag, SyntaxAnalyzer::ItemFlag::EventMessage))
	{
		return;
	}

	std::string className = item.mClassName + "_event_generated";
	file << "class " << className << " final : public IEventSubscriber" << endl;
	file << "{" << endl;
	file << "public:" << endl;
	file << "LuaBind* bind;" << endl;

	// Constructor
	file << className << "(LuaBind& b)" << endl;
	file << "{" << endl;
	file << "bind = &b;" << endl;
	file << "Event<" << item.mClassName << ">::Subscribe(*this);" << endl;
	file << "}" << endl;

	// Destructor
	file << "~" << className << "()" << endl;
	file << "{" << endl;
	file << "Event<" << item.mClassName << ">::Unsubscribe(*this);" << endl;
	file << "}" << endl;

	// Event notify method
	file << "virtual void Notify(const BaseEvent& event) override" << endl;
	file << "{" << endl;
	file << "const " << item.mClassName << "* msg = &static_cast<const Event<" << item.mClassName << ">&>(event).Message();" << endl;
	file << "bind->CallFunctionNoReturn(\"EventNotify\", msg, \"" << item.mClassName <<"\");" << endl;
	file << "}" << endl;

	// End of class
	file << "};" << endl << endl;
}

void CodeGenerator::LuaClassState(const SyntaxAnalyzer::Item& item, std::ofstream& file)
{
	file << "---@class " << item.mClassName;
	if (!item.mParentClassName.empty())
	{
		file << " : " << item.mParentClassName;
	}
	file << endl;
}

void CodeGenerator::LuaEndClassState(const SyntaxAnalyzer::Item& item, std::ofstream& file)
{
	file << item.mClassName << " = " << item.mClassName << endl;
}

void CodeGenerator::LuaVariableState(const SyntaxAnalyzer::Item& item, std::ofstream& file)
{
	file << "---@field " << item.mToken.String << " " << CPPToLuaType(item.mMemberType) << endl;
}

void CodeGenerator::LuaFunctionState(const SyntaxAnalyzer::Item& item, std::ofstream& file)
{
	file << "local " << item.mToken.String << "_Impl = " << item.mClassName << "." << item.mToken.String << endl;
	if (!item.mStatic)
	{
		file << "---@param obj " << item.mClassName << endl;
	}
	for (size_t i = 0; i < item.mArgumentList.size(); ++i)
	{
		file << "---@param param" << (i + 1) << " " << CPPToLuaType(item.mArgumentList[i]) << endl;
	}
	file << "---@return " << CPPToLuaType(item.mMemberType) << endl;

	file << "function " << item.mClassName << "." << item.mToken.String << "(";
	if (!item.mStatic)
	{
		file << "obj";
		if (item.mArgumentList.size() > 0)
		{
			file << ", ";
		}
	}
	for (size_t i = 0; i < item.mArgumentList.size(); ++i)
	{
		file << "param" << (i + 1);
		if (i < item.mArgumentList.size() - 1)
		{
			file << ", ";
		}
	}
	file << ") end" << endl;
	file << item.mClassName << "." << item.mToken.String << " = " << item.mToken.String << "_Impl" << endl;
}

void CodeGenerator::LuaConstructorState(const SyntaxAnalyzer::Item& item, std::ofstream& file)
{
	file << "local " << item.mClassName << "_New = " << item.mClassName << ".New" << endl;
	for (size_t i = 0; i < item.mArgumentList.size(); ++i)
	{
		file << "---@param param" << (i + 1) << " " << CPPToLuaType(item.mArgumentList[i]) << endl;
	}
	file << "---@return " << CPPToLuaType(item.mClassName) << endl;

	file << "function " << item.mClassName << ".New(";
	for (size_t i = 0; i < item.mArgumentList.size(); ++i)
	{
		file << "param" << (i + 1);
		if (i < item.mArgumentList.size() - 1)
		{
			file << ", ";
		}
	}
	file << ") end" << endl;
	file << item.mClassName << ".New = " << item.mClassName << "_New" << endl;
}

void CodeGenerator::LuaEnumState(const SyntaxAnalyzer::Item& item, std::ofstream& file)
{
	file << "---@class " << item.mClassName << endl;
	for (const auto& name : item.mEnumList)
	{
		file << "---@field " << name << " number" << endl;
	}
	file << item.mClassName << " = " << item.mClassName << endl;
}

std::string CodeGenerator::CPPToLuaType(const std::string& type) const
{
	// Remove useless symbols
	string copy = type;
	size_t pos = copy.find("**");
	if (pos != string::npos)
	{
		copy.erase(pos, 2);
	}
	pos = copy.find("*");
	if (pos != string::npos)
	{
		copy.erase(pos, 1);
	}
	pos = copy.find("&&");
	if (pos != string::npos)
	{
		copy.erase(pos, 2);
	}
	pos = copy.find("&");
	if (pos != string::npos)
	{
		copy.erase(pos, 1);
	}
	pos = copy.find("const");
	if (pos != string::npos)
	{
		copy.erase(pos, 5);
	}
	pos = copy.find(' ');
	while (pos != string::npos)
	{
		copy.erase(pos, 1);
		pos = copy.find(' ');
	}
	
	// Output Lua type
	if (copy == "int" || copy == "unsignedint" || copy == "long" || copy == "unsignedlong" || copy == "longlong" || copy == "unsignedlonglong"
		|| copy == "float" || copy == "double" || copy == "unsignedchar")
	{
		return "number";
	}
	else if (copy == "bool")
	{
		return "boolean";
	}
	else if (copy == "char" || copy == "std::string")
	{
		return "string";
	}
	else if (copy == "void")
	{
		return "nil";
	}
	else
	{
		return copy;
	}
}
