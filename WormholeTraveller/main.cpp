//
// Wormhole Traveller
//
// main.cpp -- entry point
//

#pragma once

#include "stdafx.h"
#include <iostream>

#include "WormholeTraveller.h"

int main(int argc, char** argv)
{
	WormholeTraveller application;
	OpStatus status;

	glutInit(&argc, argv);

	status = application.initOpenGL();
	OpAssert(status, "Cannot initialize OpenGL");

	status = application.initApplication();
	OpAssert(status, "Cannot initialize application");

	application.setApplicationInstance(&application);

	glutMainLoop();

	return 0;
}
