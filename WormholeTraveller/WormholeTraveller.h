//
// Wormhole Traveller
//
// WorholeTraveller.h -- the main application class
//

#pragma once

#include <iostream>
#include <vector>
#include <string>

#include <glew.h>
#include <freeglut.h>

#define GLM_SWIZZLE

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "World.h"
#include "Camera.h"
#include "SceneObject.h"
#include "SphereObject.h"

#include "LightingShader.h"
#include "SkyboxShader.h"

#include "Texture.h"
#include "CubeMapTexture.h"

#include "prefdefs.h"

using namespace glm;
using namespace std;

enum GameState
{
	GS_STAYIDLE,
	GS_ACCEL_FORWARD,
	GS_ACCEL_BACKWARD,
	GS_MOVE_LEFT,
	GS_MOVE_RIGHT,
	GS_PITCH_UP,
	GS_PITCH_DOWN,
	GS_ROLL_LEFT,
	GS_ROLL_RIGHT,
	GS_NUM_STATES // should be the last one
};

#define GS_ON	(GameState)(GS_NUM_STATES + 1)
#define GS_OFF	(GameState)(GS_NUM_STATES + 2)

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

	LightingShader lightingShader;
	SkyboxShader skyboxShader;

	CubeMapTexture skyboxTexture;

	vector<GameState> states;
	void WormholeTraveller::executeStateHandler(GameState state);

	double travelSpeed;
	double travelAcceleration;

	double prevFrameSpeed;

	/// GLUT callback routers
	static void renderRouter();
	static void onKeyboardRouter(unsigned char key, int x, int y);
	static void onSpecialKeyboardRouter(int key, int x, int y);
	static void onKeyboardUpRouter(unsigned char key, int x, int y);
	static void onSpecialKeyboardUpRouter(int key, int x, int y);
	static void onViewportResizeRouter(int newWidth, int newHeight);
	static void onMouseRouter(int button, int state, int x, int y);
	static void onMouseWheelRouter(int button, int dir, int x, int y);
	static void timerTickRouter(int operation);

	/// GLUT callbacks
	void render();
	void onKeyboard(unsigned char key, int x, int y);
	void onSpecialKeyboard(int key, int x, int y);
	void onSpecialKeyboardUp(int key, int x, int y);
	void onKeyboardUp(unsigned char key, int x, int y);
	void onViewportResize(int newWidth, int newHeight);
	void onMouse(int button, int state, int x, int y);
	void onMouseWheel(int button, int dir, int x, int y);
	void timerTick(int operation);

	OpStatus updateWorldObjects(int frameNumber);
	OpStatus initSkybox();

	void changeTravelAcceleration(double delta);
	void setGlobalScale(float newScale);

	void performHyperjump(bool backwards);

	float globalScale;

	int mVpWidth, mVpHeight;
	int mFramesRenderred;

	int cooldownEndFrame;

	double accelerationUPF, decelerationUPF; // units per frame

	SceneObject earth;
	SceneObject moon;
	SceneObject earthWormhole;
	SceneObject moonWormhole;
	SceneObject skybox;
	vector<SceneObject*> rulerBlocks;
	float rulerBlockZScale;
	int numBlocks = 100;

	int earthMoonDistanceUnits;

	Texture earthTexture;
	Texture moonTexture;
	Texture wormholeTexture;
	Texture pathBlockTexture;

	float distanceToWH1, distanceToWH2;
};

