#include "stdafx.h"
#include "WormholeTraveller.h"

WormholeTraveller * WormholeTraveller::appinstance = NULL;

WormholeTraveller::WormholeTraveller() :
	mFramesRenderred(0)
{
	mVpWidth = PREF_GAME_MODE ? PREF_GAME_MODE_W : PREF_WINDOW_W;
	mVpHeight = PREF_GAME_MODE ? PREF_GAME_MODE_H : PREF_WINDOW_H;
}


WormholeTraveller::~WormholeTraveller()
{
}

OpStatus WormholeTraveller::initOpenGL()
{
	unsigned int displayModePrefs = GLUT_DOUBLE | GLUT_RGBA;

	this->usedBuffersBits = GL_COLOR_BUFFER_BIT;

	if (PREF_MSAA_ENABLE) {
		glutSetOption(GLUT_MULTISAMPLE, PREF_MSAA_SAMPLES);
		displayModePrefs = displayModePrefs | GLUT_MULTISAMPLE;
		glEnable(GL_MULTISAMPLE);
	}

	if (PREF_DEPTHTEST_ENABLE) {
		displayModePrefs = displayModePrefs | GLUT_DEPTH;
		glEnable(GL_DEPTH_TEST);
		this->usedBuffersBits |= GL_DEPTH_BUFFER_BIT;
	}

	glutInitDisplayMode(displayModePrefs);

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(mVpWidth, mVpHeight);
	glutCreateWindow(PREF_WINDOW_TITLE);

	if (PREF_GAME_MODE)
	{
		char modeString[20];
		sprintf_s(modeString, "%dx%d@%d", PREF_GAME_MODE_W, PREF_GAME_MODE_H, PREF_GAME_MODE_RATE);
		glutGameModeString(modeString);
		if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE))
		{
			glutEnterGameMode();
		}
		else {
			return OPS_MODE_UNAVAIL;
		}
	}

	if (PREF_FACECULL_ENABLE) {
		glEnable(GL_CULL_FACE);
		glCullFace(PREF_FACECULL_FACE);
	}

	glutDisplayFunc(WormholeTraveller::renderRouter);
	glutReshapeFunc(WormholeTraveller::onViewportResizeRouter);
	glutKeyboardFunc(WormholeTraveller::onKeyboardRouter);
	glutSpecialFunc(WormholeTraveller::onSpecialKeyboardRouter);

	glutTimerFunc(PREF_FRAME_TIME, WormholeTraveller::timerTickRouter, OP_UPDATE_OBJECTS);

	glewInit();

	return OPS_OK;
}

OpStatus WormholeTraveller::initApplication()
{
	// Setup the world
	world.setRotations(0.0f, 0.0f, 0.0f);
	world.setScale(vec3(1.0f));
	world.setPosition(vec3(0.0f));

	// Setup the camera
	vec3 viewerPos = vec3(200, 200, 200);
	vec3 lookAt = vec3(120, 0, 100);
	vec3 up = vec3(0, 1, 0);
	
	camera.setWindowDims(mVpWidth, mVpHeight);
	camera.setCamera(viewerPos, lookAt, up);

	// Setup the lighting
	light.color = glm::vec3(1.0f, 1.0f, 1.0f);
	light.direction = glm::vec3(240, 210, 200);
	light.ambientIntensity = 0.2f;
	light.diffuseIntensity = 0.75f;
	light.specularIntensity = 0.85f;
	light.specularPower = 5;

	// Setup the shader program
	OpStatus shdaerStatus = lightingShader.init();

	if (OPS_FAILURE(shdaerStatus)) { return shdaerStatus; }

	lightingShader.useProgram(1);

	//// Create the world scene (i.e. objects)

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	// Earth
	SphereObject earth(20, 10);
	earth.computeGeometry(vertices, indices);
	earth.createVao(lightingShader, vertices, indices);
	earth.setInitialPosition(100, 0, 100);
	earth.setInitialRotations(0, 0, 0);
	earth.setScale(10, 10, 10);

	this->worldObjects.push_back(earth);

	return OPS_OK;
}

void WormholeTraveller::setApplicationInstance(WormholeTraveller * instance)
{
	WormholeTraveller::appinstance = instance;
}

/** GLUT callbacks rounting */

void WormholeTraveller::renderRouter()
{
	appinstance->render();
}

void WormholeTraveller::onKeyboardRouter(unsigned char key, int x, int y)
{
	appinstance->onKeyboard(key, x, y);
}

void WormholeTraveller::onSpecialKeyboardRouter(int key, int x, int y)
{
	appinstance->onSpecialKeyboard(key, x, y);
}

void WormholeTraveller::onViewportResizeRouter(int newWidth, int newHeight)
{
	appinstance->onViewportResize(newWidth, newHeight);
}

void WormholeTraveller::timerTickRouter(int operation)
{
	appinstance->timerTick(operation);
	glutTimerFunc(PREF_FRAME_TIME, WormholeTraveller::timerTickRouter, OP_UPDATE_OBJECTS);
}

/* GLUT callbacks rounting **/

/** GLUT callbacks */

void WormholeTraveller::render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(this->usedBuffersBits);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// draw each object in this->worldObjects vector...

	glutSwapBuffers();
}

void WormholeTraveller::onKeyboard(unsigned char key, int x, int y)
{
	switch (key) {

	case 27: // Escape
		exit(0);
		break;

	default:
		break;

	}
}

void WormholeTraveller::onSpecialKeyboard(int key, int x, int y)
{
}

void WormholeTraveller::onViewportResize(int newWidth, int newHeight)
{
}

void WormholeTraveller::timerTick(int operation)
{
	mFramesRenderred++;

	switch (operation)
	{
	case OP_UPDATE_OBJECTS:
		updateWorldObjects(mFramesRenderred);

	default:
		break;
	}
}

/* GLUT Callbacks **/

OpStatus WormholeTraveller::updateWorldObjects(int frameNumber)
{
	glutPostRedisplay();
	return OPS_OK;
}
