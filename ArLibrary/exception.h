#pragma once

#include <string>

class Exception
{
	int m_errorType;

public:

	Exception(int error);
	const char* getError();

	enum Errors
	{
		GRAPHICS,
		SIZE,
		DIMENSION,
		GLWINDOW,
		GLGLAD,
		NULLCAMERA,
	};
};