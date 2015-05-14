#include "Rotatable.h"

using namespace Core::Space;



Rotatable3D::~Rotatable3D() {}
Rotatable2D::~Rotatable2D() {}	



Rotate3DAction::~Rotate3DAction() {}

Rotate3D::Rotate3D(Rotatable3D* target)
{
	Target = target;
}
Rotate3D::~Rotate3D() {}
void Rotate3D::Perform(const glm::quat& delta)
{
	Target->Rotate(delta);
}

DontRotate3D::DontRotate3D(Rotatable3D* target)
{
	Target = target;
}
DontRotate3D::~DontRotate3D() {}
void DontRotate3D::Perform(const glm::quat& delta) {}



Rotate3DEulerAction::~Rotate3DEulerAction() {}

Rotate3DEuler::Rotate3DEuler(Rotatable3D* target)
{
	Target = target;
}
Rotate3DEuler::~Rotate3DEuler() {}
void Rotate3DEuler::Perform(const glm::vec3& delta)
{
	Target->Rotate(delta);
}

DontRotate3DEuler::DontRotate3DEuler(Rotatable3D* target)
{
	Target = target;
}
DontRotate3DEuler::~DontRotate3DEuler() {}
void DontRotate3DEuler::Perform(const glm::vec3& delta) {}



Rotate2DAction::~Rotate2DAction() {}

Rotate2D::Rotate2D(Rotatable2D* target)
{
	Target = target;
}
Rotate2D::~Rotate2D() {}
void Rotate2D::Perform(float delta)
{
	Target->Rotate(delta);
}

DontRotate2D::DontRotate2D(Rotatable2D* target)
{
	Target = target;
}
DontRotate2D::~DontRotate2D() {}
void DontRotate2D::Perform(float delta) {}
