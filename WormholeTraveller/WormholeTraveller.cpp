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
	for (int i = 0; i < this->worldObjects.size(); i++)
	{
		delete this->worldObjects[i];
	}
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
	glutMouseFunc(WormholeTraveller::onMouseRouter);
	glutMouseWheelFunc(WormholeTraveller::onMouseWheelRouter);

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
	vec3 viewerPos = vec3(0, 0, 0);
	vec3 lookAt = vec3(40, 0, 50);
	vec3 up = vec3(0, 1, 0);
	
	camera.setWindowDims(mVpWidth, mVpHeight);
	camera.setCamera(viewerPos, lookAt, up);

	// Setup the shader program
	OpStatus shaderStatus = lightingShader.init();

	if (OPS_FAILURE(shaderStatus)) { return shaderStatus; }

	lightingShader.useProgram(1);

	//// Create the world scene (i.e. objects)

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	// Earth
	SphereObject* earth = new SphereObject(60, 30);
	earth->computeGeometry(vertices, indices);
	earth->createVao(lightingShader, vertices, indices);
	earth->setInitialPosition(100, 0, 100);
	earth->setInitialRotations(0, 0, 0);
	earth->setScale(10, 10, 10);

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

void WormholeTraveller::onMouseRouter(int button, int state, int x, int y)
{
	appinstance->onMouse(button, state, x, y);
}

void WormholeTraveller::onMouseWheelRouter(int button, int dir, int x, int y)
{
	appinstance->onMouseWheel(button, dir, x, y);
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

	lightingShader.useProgram(1);
	/*
	//// <-->
	GLuint loc = glGetUniformLocation(lightingShader.getProgId(), "vDiffuseMaterial");
	glUniform3f(loc, 0.8, 0.8, 0.8);

	loc = glGetUniformLocation(lightingShader.getProgId(), "vDiffuseLight");
	glUniform3f(loc, 0.9, 0.9, 0.9);

	loc = glGetUniformLocation(lightingShader.getProgId(), "vLightDir");
	glUniform3f(loc, 50, 0, 50);

	loc = glGetUniformLocation(lightingShader.getProgId(), "vSpecularMaterial");
	glUniform3f(loc, 0.9, 0.9, 0.9);

	loc = glGetUniformLocation(lightingShader.getProgId(), "vSpecularLight");
	glUniform3f(loc, 0.95, 0.95, 0.95);

	loc = glGetUniformLocation(lightingShader.getProgId(), "fShininess");
	glUniform1f(loc, 128.0f);
	*/
	//lightingShader.setAmbient(vec3(0.2f, 0.2f, 0.2f));

	for (int i = 0; i < this->worldObjects.size(); i++)
	{
		SceneObject* object = this->worldObjects[i];
		glm::mat4 model, view, projection;
		
		object->getModelTransform(&model); // model -> world
		camera.getViewMatrix(&view);	// world -> eye
		camera.getProjectionMatrix(&projection); // eye -> clip

		glm::mat4 mv = model * view;

		lightingShader.copyMatrixToShader(mv, "modelView");
		lightingShader.copyMatrixToShader(view, "view");
		lightingShader.copyMatrixToShader(glm::transpose(projection), "projection");

		object->renderObject(lightingShader);
	}

	glutSwapBuffers();
}

void WormholeTraveller::onKeyboard(unsigned char key, int x, int y)
{
	switch (key) {

	case 27: // Escape
		exit(0);
		break;

	case 'w':
		camera.moveForwardRelative(4.0f);
		break;

	case 's':
		camera.moveForwardRelative(-4.0f);
		break;

	case 'a':
		camera.yaw(2.0f);
		break;

	case 'd':
		camera.yaw(-2.0f);
		break;

	case 'q':
		camera.moveRightRelative(-4.0f);
		break;

	case 'e':
		camera.moveRightRelative(4.0f);
		break;

	default:
		break;

	}
}

void WormholeTraveller::onSpecialKeyboard(int key, int x, int y)
{
	switch (key)
	{
	
	case GLUT_KEY_UP:
		camera.pitch(1.0f);
		break;

	case GLUT_KEY_DOWN:
		camera.pitch(-1.0f);
		break;

	default:
		break;

	}
}

void WormholeTraveller::onViewportResize(int newWidth, int newHeight)
{
	glViewport(0, 0, newWidth, newHeight);
	camera.setWindowDims(newWidth, newHeight);
}

void WormholeTraveller::onMouse(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) { camera.resetZoom(); }
		break;

	default:
		break;
	}
}

void WormholeTraveller::onMouseWheel(int button, int dir, int x, int y)
{
	camera.zoomIn(2.0f * -dir);
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
	//this->worldObjects[0]->incrementPosition(sin((frameNumber + 100) / 75) * 3 , 0, 0 );
	glutPostRedisplay();
	return OPS_OK;
}
