#pragma once

#include <string>

#define SHADERS_PATH			".\\renderprogs"
#define TEXTURES_PATH			".\\textures"
#define MODELS_PATH				".\\models"

/// GLUT Window Preferences
#define PREF_WINDOW_W			1280
#define PREF_WINDOW_H			800
#define PREF_WINDOW_RATE		60	// Hz
#define PREF_INIT_WPOS_X		100
#define PREF_INIT_WPOS_Y		75
#define PREF_GAME_MODE			false
#define PREF_GAME_MODE_W		1920
#define PREF_GAME_MODE_H		1200
#define PREF_GAME_MODE_RATE		60
#define PREF_WINDOW_TITLE		"Wormhole Traveller"

/// MSAA Preferences
#define PREF_MSAA_ENABLE		true
#define PREF_MSAA_SAMPLES		16

/// Performance preferences
#define PREF_FRAME_TIME			15

/// Operation defs
#define OP_UPDATE_OBJECTS		0x1000

// Movement prefs (in units per second)
#define PREF_ACCEL_UPS	0.005
#define PREF_DECEL_UPS	(PREF_ACCEL_UPS * 5)
#define PREF_TOP_SPEED	0.3
#define PREF_TOP_SPEED_BACKW 0.15
#define PREF_TOP_ACCEL	0.002
#define PREF_ACCEL_CD_TRIGGER_SPEED 0.008
#define PREF_ACCEL_CD_DELAY 60

using namespace std;

namespace Convenience {
	// Convinience for loading resources
	inline string concatPath(const char * firstComponent, const char * secondComponent) {
		char buf[100];
		sprintf_s(buf, "%s\\%s", firstComponent, secondComponent);
		return string(buf);
	}
}

namespace Preferences 
{
	inline string getShaderPath(const char * resname) {
		return Convenience::concatPath(SHADERS_PATH, resname);
	}

	inline string getTexturePath(const char * texname) {
		return Convenience::concatPath(TEXTURES_PATH, texname);
	}

	inline string getModelPath(const char * model) {
		return Convenience::concatPath(MODELS_PATH, model);
	}
}
