#include "pch.h"
#include "Game.h"
#include <iostream>
#include <fstream>
#include "SOIL/SOIL.h"
#include "Transform.h"
#include "World.h"
#include "Sector.h"
#include "Entity.h"
#include "CollisionManager.h"
#include "ActionRender.h"
#include "Event.h"
#include "../Library.Desktop/EventInput.h"
#include "EventMessageAttributed.h"

#include "Sprite.h"
#include "SpriteSheet.h"

#include <GL/GLU.h>
#include <GL/GL.h>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace GameEngine;
using namespace std;
using namespace glm;

/// <summary>
/// Init OpenGL, load shader and set projection matrix and view matrix
/// </summary>
void InitGraphics();

/// <summary>
/// Unload OpenGL
/// </summary>
void CleanupGraphics();

/// <summary>
/// Render function wrapper passed to each entity, so they can just call this function and don't worry about implementation
/// </summary>
void RenderCallback(const Transform& trans, const ActionRender::RenderParam& param);

GLuint vertexShader;
GLuint fragmentShader;
GLuint shaderProgram;
Transform viewTrans;
GLuint vao;
GLuint ebo;
GLuint vbo;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int showCommand)
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
#endif

	UNREFERENCED_PARAMETER(instance);
	UNREFERENCED_PARAMETER(previousInstance);
	UNREFERENCED_PARAMETER(commandLine);
	UNREFERENCED_PARAMETER(showCommand);
	
	// Create game instance
	Game::CreateInstance();
	Game& game = Game::GetInstance();

	if (!glfwInit())
	{
		return -1;
	}

	GLFWwindow* window = glfwCreateWindow(game.WindowWidth(), game.WindowHeight(), "Gun Vs. Sword", nullptr, nullptr);
	if (window == nullptr)
	{
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (gl3wInit() != 0)
	{
		return -1;
	}

	glViewport(0, 0, game.WindowWidth(), game.WindowHeight());


	InitGraphics();

	// Capture memory snapshot after messing up the static memory
	_CrtMemState startMemState;
	startMemState;
	_CrtMemCheckpoint(&startMemState);

	//Create Collision
	CollisionManager::CreateInstance();

	// Create world
	GameTime time;
	GameClock clock;
	clock.Reset();
	shared_ptr<World> world = make_shared<World>(time);
	{
		JsonTableParseHelper::SharedData data(world);
		JsonParseMaster master(data);
		JsonTableParseHelper helper;
		master.AddHelper(helper);
		if (!master.ParseFromFile("Resources/Config/Entry.json"))
		{
			return -1;
		}
		game.mWorld = &*world;
		world->Start();
	}
	
	//SoundTest(world);

	// Main game loop
	const vec4 clearColor = vec4(0.f, 0.f, 0.f, 1.0f);

	while (!glfwWindowShouldClose(window))
	{
		glClearBufferfv(GL_COLOR, 0, &clearColor[0]);

		// Update game logic
		clock.UpdateGameTime(time);
		world->Update();

		// Update viewport based on window size
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		glViewport(0, 0, width, height);

		// Draw the scene to window as last step
		world->Draw(RenderCallback);

		// Display new frame
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	// Free world resource, now memory should match with the beginning snapshot
	world = nullptr;

	CollisionManager::DestroyInstance();
	Event<CollisionMessage>::UnsubscribeAll();
	Event<EventInput>::UnsubscribeAll();
	Event<EventMessageAttributed>::UnsubscribeAll();

	// Check memory difference only for game loop logic
	_CrtMemState endMemState, diffMemState;
	diffMemState;
	endMemState;
	_CrtMemCheckpoint(&endMemState);
	if (_CrtMemDifference(&diffMemState, &startMemState, &endMemState))
	{
		_CrtMemDumpStatistics(&diffMemState);
		assert(("Memory Leak!", false));
	}

	CleanupGraphics();
	glfwDestroyWindow(window);
	glfwTerminate();

	Game::DestroyInstance();
	return 0;
}

/// <summary>
/// Happens once at the start of the game to ready the OpenGL elements.
/// </summary>
void InitGraphics()
{
	// Create and compile the vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	{
		ifstream stream("Resources/Shaders/SpriteVextexShader.shader");
		string str((std::istreambuf_iterator<char>(stream)), (std::istreambuf_iterator<char>()));
		const char* c = str.c_str();
		glShaderSource(vertexShader, 1, &c, NULL);
		glCompileShader(vertexShader);
	}

	// Create and compile the fragment shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	{
		ifstream stream("Resources/Shaders/SpriteFragmentShader.shader");
		string str((std::istreambuf_iterator<char>(stream)), (std::istreambuf_iterator<char>()));
		const char* c2 = str.c_str();
		glShaderSource(fragmentShader, 1, &c2, NULL);
		glCompileShader(fragmentShader);
	}

	// Link the vertex and fragment shader into a shader program
	shaderProgram = glCreateProgram();
	Game::GetInstance().mShaderProgram = shaderProgram;
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	// Create a Vertex Buffer Object and copy the vertex data to it
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Create Vertex Array Object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	// Create an element array
	glGenBuffers(1, &ebo);

	GLuint elements[] = {
		0, 1, 2,
		2, 3, 0
	};

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	// Init projection matrix
	const glm::mat4& proj = Game::GetInstance().ProjectionMatrix();
	GLint uniProj = glGetUniformLocation(shaderProgram, "proj");
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

	// Init view matrix
	const glm::mat4& view = Game::GetInstance().ViewMatrix();
	GLint uniView = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

	// Specify the layout of the vertex data
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), 0);

	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(7 * sizeof(GLfloat)));

	viewTrans = glm::lookAt(vec3(0, 0, 1),
		vec3(0, 0, 0), vec3(0, 1, 0));
	const glm::mat4& viewPort = viewTrans.GetMatrix();
	uniView = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(viewPort));

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
}

/// <summary>
/// Called at program end to clear out any OpenGL components.
/// </summary>
void CleanupGraphics()
{
	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);

	glDeleteVertexArrays(1, &vao);
}

void RenderCallback(const Transform& trans, const ActionRender::RenderParam& param)
{
	Game& game = Game::GetInstance();
	auto sprite = game.GetSprite(param.mTextureId, param.mSpriteColumn, param.mSpriteRow);
	assert(("Rendering: Missing sprite", sprite != nullptr));
	sprite->Render(trans * param.mOffset);
}
