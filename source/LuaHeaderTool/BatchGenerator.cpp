#include "pch.h"
#include "BatchGenerator.h"
#include <iostream>
#include <filesystem>
#include <queue>

using namespace GameEngine::HeaderTool;
using namespace std;
using Item = SyntaxAnalyzer::Item;
using ItemType = SyntaxAnalyzer::ItemType;

void BatchGenerator::Reset()
{
	mParsedFileList.clear();
	mGeneratedClassSet.clear();
	mClassMap.clear();
}

void BatchGenerator::BatchParse(const std::string& dir)
{
	// BFS to scan the entire directory tree
	queue<filesystem::directory_entry> entryQueue;
	entryQueue.emplace(dir);
	while (!entryQueue.empty())
	{
		const auto& entry = entryQueue.front();
		if (entry.is_directory())
		{
			for (const auto& childEntry : filesystem::directory_iterator(entry))
			{
				if (childEntry.is_directory() || childEntry.path().extension() == ".h")
				{
					entryQueue.emplace(childEntry);
				}
			}
		}
		else
		{
			ParseFile(entry.path());
		}
		entryQueue.pop();
	}
}

void BatchGenerator::BatchWriteCPP(const std::string& dir)
{
	// First make sure the existance of directory
	auto folder = filesystem::directory_entry(dir);
	filesystem::create_directory(folder.path());

	// Iterate through all parsed files and output generated file
	CodeGenerator generator;
	for (const auto& source : mParsedFileList)
	{
		if (source.mItems.size() > 0)
		{
			generator.GenerateCPP(source.mItems, source.mFileName, dir);
		}
	}
}

void BatchGenerator::BatchWriteLua(const std::string& dir)
{
	// First make sure the existance of directory
	auto folder = filesystem::directory_entry(dir);
	filesystem::create_directory(folder.path());

	// Iterate through all parsed files and output generated file
	CodeGenerator generator;
	for (const auto& source : mParsedFileList)
	{
		if (source.mItems.size() > 0)
		{
			generator.GenerateLua(source.mItems, dir + source.mFileName + ".lua");
		}
	}

	// Write register lua file
	ofstream file;
	file.open(dir + "__Include__.lua");
	if (file.bad())
	{
		throw std::runtime_error("Lua register file is invalid");
	}
	for (const auto& source : mParsedFileList)
	{
		if (source.mItems.size() > 0)
		{
			file << "require(\"Content/Lua/Class/" << source.mFileName << "\")" << endl;
		}
	}
	file.close();
}

void BatchGenerator::WriteRegister(const std::string& filePath, const std::string& generatedFilePath)
{
	ofstream file;
	file.open(filePath, ios::out);
	if (file.bad())
	{
		throw runtime_error("Register file path is invalid");
	}

	// Write comment
	file << "// Genereated register code" << endl;
	file << "// Don't change this file manually" << endl << endl;

	// Write basic include
	file << "#include \"pch.h\"" << endl;
	file << "#include \"LuaRegister.h\"" << endl;
	file << "#include \"LuaBind.h\"" << endl;
	file << "#include \"Event.h\"" << endl;
	file << "#include \"IEventSubscriber.h\"" << endl;
	for (const auto& parsedFile : mParsedFileList)
	{
		if (parsedFile.mItems.size() > 0)
		{
			file << "#include \"" << parsedFile.mFileName << ".h\"" << endl;
		}
	}

	// Write alias
	file << "namespace GameEngine {};" << endl;
	file << "namespace GameEngine::Lua {};" << endl;
	file << "namespace GameEngine {};" << endl;
	file << "namespace Rendering {};" << endl;
	file << "using namespace GameEngine;" << endl;
	file << "using namespace GameEngine::Lua;" << endl;
	file << "using namespace GameEngine;" << endl;
	file << "using namespace Rendering;" << endl << endl;

	// Create a lua monitor
	file << "class LuaMonitor final {" << endl;
	file << "public:" << endl;
	file << "std::vector<IEventSubscriber*> subscribers;" << endl;
	file << "};" << endl;
	file << "std::map<LuaBind*, LuaMonitor*> gMonitors;" << endl;

	// Write class template specialization definition, must do this before including those generated classes, because there might be cicular dependency
	for (const auto& parsedFile : mParsedFileList)
	{
		string currentClass;
		for (const auto& item : parsedFile.mItems)
		{
			if (item.mType == ItemType::Class || item.mType == ItemType::Enum)
			{
				currentClass = item.mClassName;
				file << "DECLARE_LUA_WRAPPER(" << item.mClassName << ", \"" << item.mClassName << "\"";
				if (SyntaxAnalyzer::Item::HasFlag(item.mFlag, SyntaxAnalyzer::ItemFlag::NoLuaAuthority))
				{
					file << ", false);" << endl;
				}
				else
				{
					file << ", true);" << endl;
				}
				if (item.mType == ItemType::Class)
				{
					file << "LUA_DEFINE_CUSTOM_OBJECT_TYPE(" << item.mClassName << ");" << endl;
				}
				else
				{
					file << "LUA_DEFINE_ENUM(" << item.mClassName << ");" << endl;
					file << "DECLARE_LUA_VECTOR_WRAPPER_ALL(" << item.mClassName << ", \"" << item.mClassName << "\");" << endl;
				}
			}
			else if (item.mType == ItemType::Constructor)
			{
				if (item.mClassName == currentClass)
				{
					file << "LUA_DEFINE_CUSTOM_COPY_TYPE(" << item.mClassName << ");" << endl;
					file << "DECLARE_LUA_VECTOR_WRAPPER_ALL(" << item.mClassName << ", \"" << item.mClassName << "\");" << endl;
				}
			}
			else if (item.mType == ItemType::EndClass)
			{
				currentClass.clear();
			}
		}
	}

	// Include generated class files
	for (const auto& parsedFile : mParsedFileList)
	{
		if (parsedFile.mItems.size() > 0)
		{
			file << "#include \"" << generatedFilePath << parsedFile.mFileName << "_generated.h" << "\"" << endl;
		}
	}

	// Write function header
	file << "void " << REGISTER_FUNCTION << "(LuaBind& bind)" << endl;
	file << "{" << endl;
	file << "bind;" << endl;  // Just to unreference this variable in case nothing to bind and it triggers warning

	// Write wrapper register
	for (const auto& parsedFile : mParsedFileList)
	{
		for (const auto& item : parsedFile.mItems)
		{
			if (item.mType == ItemType::Class || item.mType == ItemType::Enum)
			{
				WriteWrapperRegister(item, file);
			}
		}
	}

	// Create event monitor
	file << endl << "LuaMonitor* monitor = new LuaMonitor();" << endl;
	file << "gMonitors[&bind] = monitor;" << endl;

	// Write class member binding
	for (const auto& parsedFile : mParsedFileList)
	{
		for (const auto& item : parsedFile.mItems)
		{
			if (item.mType == ItemType::Class || item.mType == ItemType::Enum)
			{
				file << item.mClassName << "_generated::Lua_RegisterMember(bind);" << endl;
				if (SyntaxAnalyzer::Item::HasFlag(item.mFlag, SyntaxAnalyzer::ItemFlag::EventMessage))
				{
					file << "monitor->subscribers.emplace_back(new " << item.mClassName << "_event_generated(bind));" << endl;
				}
			}
		}
	}

	// Write end of function
	file << "}" << endl;

	// Write unregister function
	file << "void " << UNREGISTER_FUNCTION << "(LuaBind& bind, bool all)" << endl;
	file << "{" << endl;
	file << "LuaMonitor* monitor = gMonitors[&bind];" << endl;
	file << "for (const auto sub : monitor->subscribers) {" << endl;
	file << "delete sub;" << endl;
	file << "}" << endl;
	file << "delete monitor;" << endl;
	file << "gMonitors.erase(&bind);" << endl;

	file << "if (all) {" << endl;
	for (const auto& parsedFile : mParsedFileList)
	{
		for (const auto& item : parsedFile.mItems)
		{
			if (item.mType == ItemType::Class && SyntaxAnalyzer::Item::HasFlag(item.mFlag, SyntaxAnalyzer::ItemFlag::EventMessage))
			{
				file << "Event<" << item.mClassName << ">::UnsubscribeAll();" << endl;
			}
		}
	}
	file << "}" << endl;

	file << "}" << endl;

	file.close();
}

void BatchGenerator::ClearGenerated(const std::vector<std::string>& dirs)
{
	for (const auto& dir : dirs)
	{
		if (filesystem::exists(dir))
		{
			auto entry = filesystem::directory_entry(dir);
			if (entry.is_directory())
			{
				for (const auto& file : filesystem::directory_iterator(entry))
				{
					filesystem::remove(file);
				}
			}
			else
			{
				filesystem::remove(entry);
			}
		}
	}
}

void BatchGenerator::ParseFile(const std::filesystem::path& filePath)
{
	// Get file name out
	std::string fileName = filePath.filename().string();
	fileName = fileName.substr(0, fileName.size() - 2);

	// Open a file stream
	ifstream stream;
	stream.open(filePath);

	// Read the file content as one string
	std::string content((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
	auto& parsedFile = mParsedFileList.emplace_back();
	parsedFile.mFileName = fileName;

	// Tokenize
	HeaderTokenizer tokenizer;
	tokenizer.Tokenize(content, parsedFile.mTokens);

	// Identify binding items
	SyntaxAnalyzer analyzer;
	analyzer.Run(parsedFile.mTokens, parsedFile.mItems);

	stream.close();

	// Build class name map
	for (auto& item : parsedFile.mItems)
	{
		if (item.mType == ItemType::Class || item.mType == ItemType::Enum)
		{
			if (mClassMap.find(item.mClassName) != mClassMap.end())
			{
				char msg[256];
				sprintf_s(msg, "Redundant declaration for class %s when parsing file %s", item.mClassName.c_str(), filePath.string().c_str());
				throw runtime_error(msg);
			}
			mClassMap.insert(make_pair(item.mClassName, &item));
		}
	}
}

void BatchGenerator::WriteWrapperRegister(const SyntaxAnalyzer::Item& item, std::ofstream& file)
{
	// Build the inheritance stack from current class
	std::stack<const Item*> itemStack;
	const Item* current = &item;
	while (true)
	{
		// Only generate wrapper register function if this class has not yet been registered
		if (mGeneratedClassSet.find(current) == mGeneratedClassSet.end())
		{
			mGeneratedClassSet.insert(current);
			itemStack.push(current);
		}

		// Move to current's parent
		if (!current->mParentClassName.empty())
		{
			auto it = mClassMap.find(current->mParentClassName);
			if (it == mClassMap.end())
			{
				char msg[256];
				sprintf_s(msg, "Class %s has parent %s, but parent class doesn't appear to be a Lua mirrored class", current->mClassName.c_str(), current->mParentClassName.c_str());
				throw runtime_error(msg);
			}
			current = it->second;
		}
		else
		{
			break;
		}
	}

	// Generate binding code from parent to derived
	while (!itemStack.empty())
	{
		const Item* stackItem = itemStack.top();
		itemStack.pop();
		file << stackItem->mClassName << "_generated::Lua_RegisterClass(bind);" << endl;
	}
}
