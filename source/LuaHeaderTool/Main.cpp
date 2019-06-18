#include "pch.h"
#include "BatchGenerator.h"
#include "LuaBind.h"

using namespace GameEngine::HeaderTool;
using namespace GameEngine::Lua;

int main(int argc, const char* argv[])
{
	if (argc != 5)
	{
		cout << "Wrong number of arguments. Expect source folder, generated file folder, register cpp file, and register include path.";
		cout << "Got " << argc << " arguments" << endl;
		return -1;
	}

	const char* sourceDir = argv[1];
	const char* generatedDir = argv[2];
	const char* registerFile = argv[3];
	const char* includePath = argv[4];
	cout << "Running Lua header tool ......" << endl;
	cout << "Source scanning directory: " << sourceDir << endl;
	cout << "Generated files directory: " << generatedDir << endl;
	cout << "LuaRegister.cpp file path: " << registerFile << endl;
	cout << "LuaRegister.cpp file include path: " << includePath << endl;

	BatchGenerator batch;
	cout << "Clearing old generated files ......" << endl;
	batch.ClearGenerated(generatedDir);
	cout << "Parsing C++ header files ......" << endl;
	batch.BatchParse(sourceDir);
	cout << "Generating C++ binding code ......" << endl;
	batch.BatchWriteCPP(generatedDir);
	batch.WriteRegister(registerFile, includePath);

	cout << "Lua header tool runs successfully!" << endl;
}
