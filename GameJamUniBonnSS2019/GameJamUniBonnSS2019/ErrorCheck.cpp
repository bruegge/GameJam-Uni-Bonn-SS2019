#include "ErrorCheck.h"


CErrorCheck::EOpenGLError CErrorCheck::GetOpenGLError(bool bEnableBreakpoint)
{
	GLenum errorcode = glGetError();
	if (errorcode)
	{
		EOpenGLError eError = static_cast<EOpenGLError>(errorcode);
		if (bEnableBreakpoint)
		{
			return eError;
		}
		else
		{
			return eError;
		}
	}
	return EOpenGLError::None;
}
