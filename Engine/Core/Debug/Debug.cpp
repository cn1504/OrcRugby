#include <Core.h>
#include "Debug.h"
#include "Log.h"

namespace Core
{	
	void error_callback(int error, const char* description) {
		Core::Debug->Error(description);
	}
}
