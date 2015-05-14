#include "Translatable.h"

using namespace Core::Space;



Translatable3D::~Translatable3D() {}
Translatable2D::~Translatable2D() {}	
Translatable2DInt::~Translatable2DInt() {}



Translate3DAction::~Translate3DAction() {}

Translate3D::Translate3D(Translatable3D* target)
{
	Target = target;
}
Translate3D::~Translate3D() {}
void Translate3D::Perform(const glm::vec3& delta)
{
	Target->Translate(delta);
}

DontTranslate3D::DontTranslate3D(Translatable3D* target)
{
	Target = target;
}
DontTranslate3D::~DontTranslate3D() {}
void DontTranslate3D::Perform(const glm::vec3& delta) {}



Translate2DAction::~Translate2DAction() {}

Translate2D::Translate2D(Translatable2D* target)
{
	Target = target;
}
Translate2D::~Translate2D() {}
void Translate2D::Perform(const glm::vec2& delta)
{
	Target->Translate(delta);
}

DontTranslate2D::DontTranslate2D(Translatable2D* target)
{
	Target = target;
}
DontTranslate2D::~DontTranslate2D() {}
void DontTranslate2D::Perform(const glm::vec2& delta) {}


		
Translate2DIntAction::~Translate2DIntAction() {}

Translate2DInt::Translate2DInt(Translatable2DInt* target) 
{
	Target = target;
}
Translate2DInt::~Translate2DInt() {}
void Translate2DInt::Perform(const glm::ivec2& delta)
{
	Target->Translate(delta);
}

DontTranslate2DInt::DontTranslate2DInt(Translatable2DInt* target)
{
	Target = target;
}
DontTranslate2DInt::~DontTranslate2DInt() {}
void DontTranslate2DInt::Perform(const glm::ivec2& delta) {}