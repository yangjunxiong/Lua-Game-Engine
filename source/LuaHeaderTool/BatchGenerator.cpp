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

void BatchGenerator::WriteRegister(const std::string& filePath, const std::string& generatedFilePath)
{
	ofstream file;
	file.open(filePath, ios::out);
	if (file.bad())
	{
		throw runtime_error("Register file path is invalid");
	}

	// Write include
	file << "#include \"pch.h\"" << endl;
	file << "#include \"LuaBind.h\"" << endl;
	for (const auto& parsedFile : mParsedFileList)
	{
		if (parsedFile.mItems.size() > 0)
		{
			file << "#include \"" << generatedFilePath << parsedFile.mFileName << "_generated.h" << "\"" << endl;
		}
	}
	file << "using namespace GameEngine::Lua;" << endl;

	// Write function header
	file << "void " << REGISTER_FUNCTION << "(LuaBind& bind)" << endl;
	file << "{" << endl;
	file << "bind;" << endl;  // Just to unreference this variable in case nothing to bind and it triggers warning

	// Write wrapper register
	for (const auto& parsedFile : mParsedFileList)
	{
		for (const auto& item : parsedFile.mItems)
		{
			if (item.mType == ItemType::Class)
			{
				WriteWrapperRegister(item, file);
			}
		}
	}

	// Write class member binding
	for (const auto& parsedFile : mParsedFileList)
	{
		for (const auto& item : parsedFile.mItems)
		{
			if (item.mType == ItemType::Class)
			{
				file << item.mClassName << "_generated::Lua_RegisterMember(bind);" << endl;
			}
		}
	}

	// Write end of function
	file << "}" << endl;

	file.close();
}

void BatchGenerator::ClearGenerated(const std::string& dir)
{
	if (filesystem::exists(dir))
	{
		auto folder = filesystem::directory_entry(dir);
		for (const auto& file : filesystem::directory_iterator(folder))
		{
			filesystem::remove(file);
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
		if (item.mType == ItemType::Class)
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
		file << stackItem->mClassName << "_generated::Lua_RegisterClass(bind.LuaState());" << endl;
	}
}
