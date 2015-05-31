#pragma once

#include "MaterialViewer.h"
#include <Components/Entity.h>

namespace MaterialViewer
{
	class LightMotionControl : public Core::Components::Entity
	{
	public:
		LightMotionControl();
		~LightMotionControl();
		
		virtual void Update();
	};
}