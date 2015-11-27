#include "stdafx.h"
#include "WormholeTraveller.h"

WormholeTraveller * WormholeTraveller::appinstance = NULL;

WormholeTraveller::WormholeTraveller() :
	mFramesRenderred(0),
	travelSpeed(0.0),
	prevFrameSpeed(0.0),
	travelAcceleration(0.0),
	cooldownEndFrame(-1)
{
	mVpWidth = PREF_GAME_MODE ? PREF_GAME_MODE_W : PREF_WINDOW_W;
	mVpHeight = PREF_GAME_MODE ? PREF_GAME_MODE_H : PREF_WINDOW_H;

	int targetFPS = 1000 / PREF_FRAME_TIME;
	accelerationUPF = (double)PREF_ACCEL_UPS / (double)targetFPS;
	decelerationUPF = (double)PREF_DECEL_UPS / (double)targetFPS;
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
	glutInitWindowPosition(PREF_INIT_WPOS_X, PREF_INIT_WPOS_Y);
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

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);

	glutDisplayFunc(WormholeTraveller::renderRouter);
	glutReshapeFunc(WormholeTraveller::onViewportResizeRouter);
	glutKeyboardFunc(WormholeTraveller::onKeyboardRouter);
	glutKeyboardUpFunc(WormholeTraveller::onKeyboardUpRouter);
	glutSpecialFunc(WormholeTraveller::onSpecialKeyboardRouter);
	glutSpecialUpFunc(WormholeTraveller::onSpecialKeyboardUpRouter);
	glutMouseFunc(WormholeTraveller::onMouseRouter);
	glutMouseWheelFunc(WormholeTraveller::onMouseWheelRouter);

	glutTimerFunc(PREF_FRAME_TIME, WormholeTraveller::timerTickRouter, OP_UPDATE_OBJECTS);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glewInit();

	return OPS_OK;
}

OpStatus WormholeTraveller::initApplication()
{
	// Setup the states
	states.resize(GS_NUM_STATES);

	for (int i = 0; i < GS_NUM_STATES; ++i)
	{
		states[i] = GS_OFF;
	}

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
	earth->setScale(25, 25, 25);
	this->worldObjects.push_back(earth);

	vertices.clear();
	indices.clear();

	// Wormhole #1
	SphereObject* wh1 = new SphereObject(60, 30);
	wh1->computeGeometry(vertices, indices);
	wh1->createVao(lightingShader, vertices, indices);
	wh1->setInitialPosition(100, 250, 100);
	wh1->setInitialRotations(0, 0, 0);
	wh1->setScale(15, 15, 15);
	this->worldObjects.push_back(wh1);

	vertices.clear();
	indices.clear();

	//// Initialize skydome shader

	//shaderStatus = skydomeShader.init();
	//if (OPS_FAILURE(shaderStatus)) { return shaderStatus; }


	//// Initialize skydome texture
	//skydomeTexture = new SkydomeTexture(
	//	Preferences::getTexturePath("skydome/right.png"),
	//	Preferences::getTexturePath("skydome/left.png"),
	//	Preferences::getTexturePath("skydome/top.png"),
	//	Preferences::getTexturePath("skydome/bottom.png"),
	//	Preferences::getTexturePath("skydome/front.png"),
	//	Preferences::getTexturePath("skydome/back.png")
	//);

	//if (OPS_FAILURE(skydomeTexture->load())) { return OPS_ERROR_LOAD_TEXTURE; }

	//// Skydome sphere

	//skydomeSphere = new SphereObject(100, 50);
	//skydomeSphere->computeGeometry(vertices, indices);
	//skydomeSphere->createVao(skydomeShader, vertices, indices);
	//skydomeSphere->setInitialPosition(viewerPos);
	//skydomeSphere->setInitialRotations(0, 0, 0);
	//skydomeSphere->setScale(150, 150, 150);

	//vertices.clear();
	//indices.clear();

	// Earth texture

	earthTexture = new Texture();
	
	if (OPS_FAILURE(earthTexture->loadTextures(Preferences::getTexturePath("earth.png"), GL_TEXTURE0)))
	{
		return OPS_ERROR_LOAD_TEXTURE;
	}

	earthTexture->setTextureSampler(lightingShader, "texSampler1", 0);

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

void WormholeTraveller::onKeyboardUpRouter(unsigned char key, int x, int y)
{
	appinstance->onKeyboardUp(key, x, y);
}

void WormholeTraveller::onSpecialKeyboardUpRouter(int key, int x, int y)
{
	appinstance->onSpecialKeyboardUp(key, x, y);
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	lightingShader.useProgram(1);
	
	//// <-->
	GLuint loc = glGetUniformLocation(lightingShader.getProgId(), "vDiffuseMaterial");
	glUniform3f(loc, 0.8, 0.8, 0.8);

	loc = glGetUniformLocation(lightingShader.getProgId(), "vDiffuseLight");
	glUniform3f(loc, 0.6, 0.6, 0.6);

	loc = glGetUniformLocation(lightingShader.getProgId(), "vLightDir");
	glUniform3f(loc, 50, 0, 50);

	loc = glGetUniformLocation(lightingShader.getProgId(), "vSpecularMaterial");
	glUniform3f(loc, 0.9, 0.9, 0.9);

	loc = glGetUniformLocation(lightingShader.getProgId(), "vSpecularLight");
	glUniform3f(loc, 0.95, 0.95, 0.95);

	loc = glGetUniformLocation(lightingShader.getProgId(), "fShininess");
	glUniform1f(loc, 63.0f);
	
	lightingShader.setAmbient(vec3(0.2f, 0.2f, 0.2f));

	glm::mat4 model, view, projection;

	camera.getViewMatrix(&view);	// world -> eye
	camera.getProjectionMatrix(&projection); // eye -> clip

	for (int i = 0; i < this->worldObjects.size(); i++)
	{
		earthTexture->bindToTextureUnit(0);
		
		SceneObject* object = this->worldObjects[i];
		
		object->getModelTransform(&model); // model -> world

		glm::mat4 mv = model * view;

		lightingShader.copyMatrixToShader(mv, "modelView");
		lightingShader.copyMatrixToShader(view, "view");
		lightingShader.copyMatrixToShader(glm::transpose(projection), "projection");

		object->renderObject(lightingShader);
	}

	lightingShader.useProgram(0);

	//skydomeShader.useProgram(1);

	//GLint OldCullFaceMode;
	//glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
	//GLint OldDepthFuncMode;
	//glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);

	//glCullFace(GL_FRONT);
	//glDepthFunc(GL_LEQUAL);

	//skydomeSphere->getModelTransform(&model);
	//mat4 mv = model * view; 

	//skydomeShader.copyMatrixToShader(mv, "modelView");
	//skydomeShader.copyMatrixToShader(transpose(projection), "projection");

	//skydomeSphere->renderObject(skydomeShader);

	//glCullFace(OldCullFaceMode);
	//glDepthFunc(OldDepthFuncMode);

	//skydomeShader.useProgram(0);

	glutSwapBuffers();
}

void WormholeTraveller::onKeyboard(unsigned char key, int x, int y)
{
	switch (key) {

	case 27: // Escape
		exit(0);
		break;

	case 'w':
		states[GS_ACCEL_FORWARD] = GS_ON;
		break;

	case 's':
		states[GS_ACCEL_BACKWARD] = GS_ON;
		break;

	case 'a':
		states[GS_MOVE_LEFT] = GS_ON;
		break;

	case 'd':
		states[GS_MOVE_RIGHT] = GS_ON;
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

void WormholeTraveller::onKeyboardUp(unsigned char key, int x, int y)
{
	switch (key)
	{

	case 'w':
		states[GS_ACCEL_FORWARD] = GS_OFF;
		travelAcceleration = 0.0;
		break;

	case 's':
		states[GS_ACCEL_BACKWARD] = GS_OFF;
		travelAcceleration = 0.0;
		break;

	case 'a':
		states[GS_MOVE_LEFT] = GS_OFF;
		break;

	case 'd':
		states[GS_MOVE_RIGHT] = GS_OFF;
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
		states[GS_PITCH_UP] = GS_ON;
		break;

	case GLUT_KEY_DOWN:
		states[GS_PITCH_DOWN] = GS_ON;
		break;

	default:
		break;

	}
}

void WormholeTraveller::onSpecialKeyboardUp(int key, int x, int y)
{
	switch (key)
	{

	case GLUT_KEY_UP:
		states[GS_PITCH_UP] = GS_OFF;
		break;

	case GLUT_KEY_DOWN:
		states[GS_PITCH_DOWN] = GS_OFF;
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
	if (frameNumber % 20 == 0) {
		//cout << "Speed: " << travelSpeed << " Acceleration: " << travelAcceleration << endl;
	}

	// Move forward for the value of current speed
	prevFrameSpeed = travelSpeed;
	travelSpeed += travelAcceleration;
	this->camera.moveForwardRelative(travelSpeed);
	
	// enumerate all possible states and execute its handler if enabled
	for (int i = 0; i < GS_NUM_STATES; ++i)
	{
		if (states[i] == GS_ON)
		{
			executeStateHandler((GameState)i);
		}
	}
	
	glutPostRedisplay();
	return OPS_OK;
}

void WormholeTraveller::executeStateHandler(GameState state)
{
	boolean reverse;
	double delta;

	switch (state)
	{

	case GS_ACCEL_FORWARD:
		// If moving forward (or stationary), then accelerate
		// If backwards, then brake instead
		reverse = travelSpeed < 0.0;
		delta = reverse ? decelerationUPF : accelerationUPF;
		changeTravelAcceleration(delta);
		if (reverse) { cout << "[REVERSE] "; }
		cout << "[UP] accelerating: " << travelSpeed + delta << " (by delta " << delta << ")" << endl;

		break;

	case GS_ACCEL_BACKWARD:
		// if moving forward, then decrease acceleration by -decel
		// if moving backward, then decrease accel. by -accel
		reverse = travelSpeed < 0.0;
		delta = reverse ? -accelerationUPF : -decelerationUPF;
		changeTravelAcceleration(delta);
		if (reverse) { cout << "[REVERSE] "; }
		cout << "[DOWN] decelerating: " << travelSpeed + delta << " (by delta " << delta << ")" << endl;
		break;

	case GS_MOVE_LEFT:
		camera.yaw(0.5f);
		break;

	case GS_MOVE_RIGHT:
		camera.yaw(-0.5f);
		break;

	case GS_PITCH_UP:
		camera.pitch(1.0f);
		break;

	case GS_PITCH_DOWN:
		camera.pitch(-1.0f);
		break;

	default:
		cerr << "unknown state handler for state `" << state << "`" << endl;
		break;
	}
}

void WormholeTraveller::changeTravelAcceleration(double delta)
{
	// i.e. during this frame, the user is holding 'W'

	if (cooldownEndFrame > 0 && mFramesRenderred < cooldownEndFrame)
	{
		// we're on cooldown -> noop
		cout << "COOLDOWN" << endl;
		return;
	} 
	else if (cooldownEndFrame > 0 && mFramesRenderred >= cooldownEndFrame)
	{
		// disable cooldown and proceed
		cooldownEndFrame = -1;
	}

	// proceed only when speed is decreasing, not increasing
	if (abs(prevFrameSpeed) > abs(travelSpeed))
	{
		if (abs(travelSpeed) <= PREF_ACCEL_CD_TRIGGER_SPEED && cooldownEndFrame < 0) {
			// setup cooldown delay (after full stop)
			cooldownEndFrame = mFramesRenderred + PREF_ACCEL_CD_DELAY;
			travelAcceleration = 0.0;
			travelSpeed = 0.0;
			return;
		}
	}


		bool reverse = travelSpeed < 0.0;
		bool topSpeed;

		if (!reverse && travelSpeed < PREF_TOP_SPEED)
		{
			topSpeed = false;
		}
		else if (!reverse && travelSpeed >= PREF_TOP_SPEED) {
			topSpeed = true;
		}

		if (reverse && travelSpeed > -PREF_TOP_SPEED_BACKW) {
			topSpeed = false;
		}
		else if (reverse && travelSpeed <= -PREF_TOP_SPEED_BACKW) {
			topSpeed = true;
		}

		if (((!reverse && !topSpeed) || (!reverse && topSpeed && (delta < 0.0))) ||
			((reverse && !topSpeed) || (reverse && topSpeed && (delta > 0.0))))
		{
			travelAcceleration += delta;
		}
		else {
			cout << " >>> TOP SPEED <<< " << endl;
		}
	
}
