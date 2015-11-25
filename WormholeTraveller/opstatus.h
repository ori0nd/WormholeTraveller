//
// Wormhole Traveller
//
// opstatus.h -- defines the datatype for operation status reports + codes
//

#pragma once

#ifdef WIN32
#include <windows.h>
#endif

#define OPS_OK 0
#define OPS_NOK 0x1

#define OPS_MODE_UNAVAIL			0x100
#define OPS_ERR_SHADER_READ			0x101
#define OPS_ERR_SHADER_COMPILE		0x102
#define OPS_ERR_SHADER_ATTACH		0x103
#define OPS_ERR_SHADER_LINK			0x104
#define OPS_UNIFORM_NOT_FOUND		0x105

#define OPS_SUCCESS(X) ((X) <= 0)
#define OPS_FAILURE(X) ((X) > 0)

typedef int OpStatus;

inline void __op_fail(OpStatus s, const char* message) 
{
	fprintf(stderr, "FATAL ERROR (%h): ", s);
	fprintf(stderr, message);
	Sleep(12000); // TODO: REMOVE IT
	exit(s);
}

inline void __op_fail(OpStatus s) 
{
	__op_fail(s, "");
}

inline void OpAssert(OpStatus s, const char* message) 
{
	if (OPS_FAILURE(s))
	{
		__op_fail(s, message);
	}
}

inline void OpAssert(OpStatus s) 
{
	OpAssert(s, "");
}
