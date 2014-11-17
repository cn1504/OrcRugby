#pragma once
#include "Core.h"

namespace Core
{
	namespace Exceptions
	{
		class InvalidException : public std::runtime_error {
		public:
			InvalidException() : runtime_error("This exception is Invalid!") { }
		};
	}
}