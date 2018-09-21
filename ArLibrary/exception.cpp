#include "exception.h"
#include <string>

Exception::Exception(int error)
	: m_errorType(error)
{
}

const char* Exception::getError()
{
	switch (m_errorType)
	{
	case Exception::Errors::GRAPHICS:
		return "Invalid source";
		break;
	case Exception::Errors::SIZE:
		return "Invalid size";
		break;
	case Exception::Errors::DIMENSION:
		return "Invalid dimension";
		break;
	case Exception::Errors::GLWINDOW:
		return "GLFW window is null";
		break;
	case Exception::Errors::GLGLAD:
		return "Failed to initialize Glad";
		break;
	case Exception::Errors::NULLCAMERA:
		return "Failed to initialize Glad";
		break;
	}
}