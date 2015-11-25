//
// Wormhole Traveller
//
// WorholeTraveller.h -- the main application class
//

#pragma once

#include <iostream>
#include <vector>

#include <glew.h>
#include <freeglut.h>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "World.h"
#include "Camera.h"
#include "SceneObject.h"
#include "SphereObject.h"

#include "LightingShader.h"

/// GLUT Window Preferences
#define PREF_WINDOW_W			1280
#define PREF_WINDOW_H			800
#define PREF_WINDOW_RATE		60	// Hz
#define PREF_GAME_MODE			false
#define PREF_GAME_MODE_W		2880
#define PREF_GAME_MODE_H		1800
#define PREF_GAME_MODE_RATE		60
#define PREF_WINDOW_TITLE		"Wormhole Traveller"

/// MSAA Preferences
#define PREF_MSAA_ENABLE		true
#define PREF_MSAA_SAMPLES		16

/// z-buffering preferences
#define PREF_DEPTHTEST_ENABLE	true

/// Face culling preferences
#define PREF_FACECULL_ENABLE	true
#define PREF_FACECULL_FACE		GL_BACK

/// Performance preferences
#define PREF_FRAME_TIME			15

/// Operation defs
#define OP_UPDATE_OBJECTS		0x1000

using namespace glm;

class WormholeTraveller
{
public:
	WormholeTraveller();
	~WormholeTraveller();

	OpStatus initOpenGL();
	OpStatus initApplication();

	void setApplicationInstance(WormholeTraveller* instance);

	World world;
	Camera camera;

private:
	static WormholeTraveller* appinstance;

	DirectionalLight light;
	LightingShader lightingShader;

	/// GLUT callback routers
	static void renderRouter();
	static void onKeyboardRouter(unsigned char key, int x, int y);
	static void onSpecialKeyboardRouter(int key, int x, int y);
	static void onViewportResizeRouter(int newWidth, int newHeight);
	static void onMouseRouter(int button, int state, int x, int y);
	static void onMouseWheelRouter(int button, int dir, int x, int y);
	static void timerTickRouter(int operation);

	/// GLUT callbacks
	void render();
	void onKeyboard(unsigned char key, int x, int y);
	void onSpecialKeyboard(int key, int x, int y);
	void onViewportResize(int newWidth, int newHeight);
	void onMouse(int button, int state, int x, int y);
	void onMouseWheel(int button, int dir, int x, int y);
	void timerTick(int operation);

	OpStatus updateWorldObjects(int frameNumber);

	int mVpWidth, mVpHeight;
	int mFramesRenderred;

	GLbitfield usedBuffersBits;

	std::vector<SceneObject*> worldObjects;
};

