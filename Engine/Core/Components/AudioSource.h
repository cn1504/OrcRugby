#pragma once

#include <Core.h>
#include "Entity.h"

namespace Core
{
	namespace Components
	{
		class AudioSource : public Entity
		{
		private:
			std::string audioFile;
						
		public:
			AudioSource(std::string audioFile);
			virtual ~AudioSource();

			virtual void Update();
		};
	}
}