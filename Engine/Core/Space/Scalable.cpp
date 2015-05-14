#include "Scalable.h"

using namespace Core::Space;



Scalable3D::~Scalable3D() {}
Scalable2D::~Scalable2D() {}	
Scalable2DInt::~Scalable2DInt() {}



Scale3DAction::~Scale3DAction() {}

Scale3D::Scale3D(Scalable3D* target)
{
	Target = target;
}
Scale3D::~Scale3D() {}
void Scale3D::Perform(const glm::vec3& delta)
{
	Target->Scale(delta);
}

DontScale3D::DontScale3D(Scalable3D* target)
{
	Target = target;
}
DontScale3D::~DontScale3D() {}
void DontScale3D::Perform(const glm::vec3& delta) {}



Scale2DAction::~Scale2DAction() {}

Scale2D::Scale2D(Scalable2D* target)
{
	Target = target;
}
Scale2D::~Scale2D() {}
void Scale2D::Perform(const glm::vec2& delta)
{
	Target->Scale(delta);
}

DontScale2D::DontScale2D(Scalable2D* target)
{
	Target = target;
}
DontScale2D::~DontScale2D() {}
void DontScale2D::Perform(const glm::vec2& delta) {}


		
Scale2DIntAction::~Scale2DIntAction() {}

Scale2DInt::Scale2DInt(Scalable2DInt* target) 
{
	Target = target;
}
Scale2DInt::~Scale2DInt() {}
void Scale2DInt::Perform(const glm::ivec2& delta)
{
	Target->Scale(delta);
}

DontScale2DInt::DontScale2DInt(Scalable2DInt* target)
{
	Target = target;
}
DontScale2DInt::~DontScale2DInt() {}
void DontScale2DInt::Perform(const glm::ivec2& delta) {}