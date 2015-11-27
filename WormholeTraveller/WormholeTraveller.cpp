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
	vec3 viewerPos = vec3(-25, 0, 100);
	vec3 lookAt = vec3(25, 0, 25);
	vec3 up = vec3(0, 1, 0);
	
	camera.setWindowDims(mVpWidth, mVpHeight);
	camera.setCamera(viewerPos, lookAt, up);

	// Setup the shader program
	OpStatus shaderStatus = lightingShader.init();

	if (OPS_FAILURE(shaderStatus)) { return shaderStatus; }

	lightingShader.useProgram(1);

	//// Create the world scene (i.e. objects)

	vector<vec3> vertices, normals;
	vector<vec2> uvs;

	// Earth
	OpAssert(SphereObject::loadOBJ(Preferences::getModelPath("earth.obj").c_str(), 
		vertices, uvs, normals), "cannot load model: earth.DDS");

	earth.createVao(lightingShader, vertices, uvs, normals);
	earth.setInitialPosition(0, 0, 0);
	earth.setInitialRotations(478, 0, 0);
	earth.setScale(25, 25, 25);


	vertices.clear(); normals.clear(); uvs.clear();

	if (OPS_FAILURE(earthTexture.loadTexture(Preferences::getTexturePath("earth.DDS"))))
	{
		return OPS_ERROR_LOAD_TEXTURE;
	}

	earthTexture.setTextureSampler(lightingShader, "texSampler1", 0);

	// Moon
	OpAssert(SphereObject::loadOBJ(Preferences::getModelPath("moon.obj").c_str(),
		vertices, uvs, normals), "cannot load model: moon.DDS");

	moon.createVao(lightingShader, vertices, uvs, normals);
	moon.setInitialPosition(50, 0, 50);
	moon.setInitialRotations(0, 0, 0);
	moon.setScale(7, 7, 7);

	vertices.clear(); normals.clear(); uvs.clear();

	if (OPS_FAILURE(moonTexture.loadTexture(Preferences::getTexturePath("moon.DDS"))))
	{
		return OPS_ERROR_LOAD_TEXTURE;
	}

	moonTexture.setTextureSampler(lightingShader, "texSampler1", 0);

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
	

	DirectionalLight light;
	light.direction = -vec3(25, 0, 50);
	light.ambient = vec3(0.2f, 0.2f, 0.2f);
	light.diffuse = vec3(0.7f, 0.7f, 0.65f);
	light.specular = vec3(0.92f, 0.92f, 0.92f);
	
	Material mat1, mat2;
	mat1.diffuse = vec3(0.8f, 0.8f, 0.8f);
	mat1.specular = vec3(0.95f, 0.95f, 0.95f);
	mat1.shininess = 2.0f;

	mat2.diffuse = vec3(0.6f, 0.6f, 0.6f);
	mat2.specular = vec3(0.8f, 0.8f, 0.8f);
	mat2.shininess = 2.0f;

	lightingShader.setLight(light);

	glm::mat4 model, view, projection;

	camera.getViewMatrix(&view);	// world -> eye
	camera.getProjectionMatrix(&projection); // eye -> clip

	// draw earth
	lightingShader.setMaterial(mat1);

	earthTexture.bindToTextureUnit(GL_TEXTURE0);
	earth.getModelTransform(&model);

	mat4 mv = model * view;

	lightingShader.copyMatrixToShader(mv, "modelView");
	lightingShader.copyMatrixToShader(view, "view");
	lightingShader.copyMatrixToShader(glm::transpose(projection), "projection");

	earth.renderObject();

	//draw moon
	lightingShader.setMaterial(mat2);
	moonTexture.bindToTextureUnit(GL_TEXTURE0);
	moon.getModelTransform(&model);

	mv = model * view;

	lightingShader.copyMatrixToShader(mv, "modelView");
	lightingShader.copyMatrixToShader(view, "view");
	lightingShader.copyMatrixToShader(glm::transpose(projection), "projection");

	moon.renderObject();


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

	case GLUT_KEY_LEFT:
		states[GS_ROLL_LEFT] = GS_ON;
		break;

	case GLUT_KEY_RIGHT:
		states[GS_ROLL_RIGHT] = GS_ON;
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

	case GLUT_KEY_LEFT:
		states[GS_ROLL_LEFT] = GS_OFF;
		break;

	case GLUT_KEY_RIGHT:
		states[GS_ROLL_RIGHT] = GS_OFF;
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

	vec3 moonPos = moon.getPosition();
	float angle = 0.007f;
	float newX = moonPos.x * cos(angle) - moonPos.z * sin(angle);
	float newZ = moonPos.x * sin(angle) + moonPos.z * cos(angle);
	moon.setInitialPosition(vec3(newX, moonPos.y, newZ));

	earth.incrementRotations(0.0f, 0.0f, 0.02f);
	moon.incrementRotations(0.0f, 0.0f, 0.015f);

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
		//if (reverse) { cout << "[REVERSE] "; }
		//cout << "[UP] accelerating: " << travelSpeed + delta << " (by delta " << delta << ")" << endl;

		break;

	case GS_ACCEL_BACKWARD:
		// if moving forward, then decrease acceleration by -decel
		// if moving backward, then decrease accel. by -accel
		reverse = travelSpeed < 0.0;
		delta = reverse ? -accelerationUPF : -decelerationUPF;
		changeTravelAcceleration(delta);
		//if (reverse) { cout << "[REVERSE] "; }
		//cout << "[DOWN] decelerating: " << travelSpeed + delta << " (by delta " << delta << ")" << endl;
		break;

	case GS_MOVE_LEFT:
		camera.yaw(0.5f);
		break;

	case GS_MOVE_RIGHT:
		camera.yaw(-0.5f);
		break;

	case GS_PITCH_UP:
		camera.pitch(0.75f);
		break;

	case GS_PITCH_DOWN:
		camera.pitch(-0.75f);
		break;

	case GS_ROLL_LEFT:
		camera.roll(1.0f);
		break;

	case GS_ROLL_RIGHT:
		camera.roll(-1.0f);
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
			
		}
	
}
