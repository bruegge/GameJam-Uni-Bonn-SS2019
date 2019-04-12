#pragma once
#include "glad/glad.h"

class CErrorCheck
{
public:
	
	enum class EOpenGLError : GLuint
	{
		None = 0x0,
		INVALID_ENUM = 0x0500,
		INVALID_VALUE = 0x0501,
		INVALID_OPERATION = 0x0502,
		INVALID_FRAMEBUFFER_OPERATION = 0x0506,
		OUT_OF_MEMORY = 0x0505,
		STACK_UNDERFLOW = 0x0504,
		STACK_OVERFLOW = 0x0503,
	};

	static EOpenGLError GetOpenGLError(bool bEnableBreakpoint);
};

