#include "pch.h"
#include "BatchGenerator.h"
#include "LuaBind.h"

using namespace GameEngine::HeaderTool;
using namespace GameEngine::Lua;

int main(int argc, const char* argv[])
{
	if (argc != 6)
	{
		cout << "Wrong number of arguments. Expect source folder, generated file folder, register cpp file, register include path, and lua output directory.";
		cout << "Got " << argc << " arguments" << endl;
		return -1;
	}

	const char* sourceDir = argv[1];
	const char* generatedDir = argv[2];
	const char* registerFile = argv[3];
	const char* includePath = argv[4];
	const char* luaPath = argv[5];
	cout << "Running Lua header tool ......" << endl;
	cout << "Source scanning directory: " << sourceDir << endl;
	cout << "Generated files directory: " << generatedDir << endl;
	cout << "LuaRegister.cpp file path: " << registerFile << endl;
	cout << "LuaRegister.cpp file include path: " << includePath << endl;
	cout << "Lua files directory: " << luaPath << endl;

	BatchGenerator batch;
	try
	{
		cout << "Clearing old generated files ......" << endl;
		batch.ClearGenerated({ generatedDir, registerFile});
		cout << "Parsing C++ header files ......" << endl;
		batch.BatchParse(sourceDir);
		cout << "Generating C++ binding code ......" << endl;
		batch.BatchWriteCPP(generatedDir);
		batch.WriteRegister(registerFile, includePath);
		cout << "Generating Lua code ......" << endl;
		batch.BatchWriteLua(luaPath);
	}
	catch (const runtime_error& e)
	{
		cout << "ERROR: " << e.what() << endl;
	}

	cout << "Lua header tool runs successfully!" << endl;
}
