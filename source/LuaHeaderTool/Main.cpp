#include "pch.h"
#include "HeaderTokenizer.h"
#include "SyntaxAnalyzer.h"
#include "CodeGenerator.h"
#include "LuaBind.h"
#include "HeaderTest_generated.h"

using namespace GameEngine::HeaderTool;

int main()
{
	std::fstream file;
	file.open("HeaderTest.h");
	std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();

	HeaderTokenizer t;
	std::vector<HeaderTokenizer::Token> tokens;
	t.Tokenize(str, tokens);

	SyntaxAnalyzer sa;
	std::vector<SyntaxAnalyzer::Item> items;
	sa.Run(tokens, items);

	CodeGenerator cg;
	cg.GenerateCPP(items, "HeaderTest.h");

	LuaBind bind;
	HeaderTest_generated::RegisterClass(bind.LuaState());
	HeaderTest2_generated::RegisterClass(bind.LuaState());
	HeaderTest_generated::RegisterMember(bind);
	HeaderTest2_generated::RegisterMember(bind);
	std::string lua = R"#(
		local test = HeaderTest.New()
		test.mFloat = test:Do()
		assert(test.mFloat == 112.125)
	)#";
	bind.LoadString(lua);
}
