#include "LightSource.h"
#include "Scene.h"
#include "Texture.h"

namespace Core
{

	LightSource::LightSource(Scene* scene, glm::vec3 color, float radius, bool castsShadow)
	{
		this->scene = scene;
		Color = color;
		shadow = castsShadow;
		ShadowRB = nullptr;
		Radius = radius;
		shape = nullptr;
	}


	LightSource::~LightSource()
	{
		if (shape != nullptr)
			delete shape;

		if (ShadowRB != nullptr)
			delete ShadowRB;
	}


	void LightSource::Load()
	{
		if (shadow)
		{
			shape = new btSphereShape(Radius);

			ShadowRB = new RenderBuffer(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1, true, true);
		}
	}
		

	void LightSource::Update()
	{
		if (shadow)
		{
			litObjects.clear();
			
			btTransform trans;
			trans.setIdentity();
			trans.setOrigin(btVector3(Entity->Transform.Position.x, Entity->Transform.Position.y, Entity->Transform.Position.z));
			shape->setUnscaledRadius(Radius);

			scene->PhysicsWorld->GetAllEntitiesWithinBroadphase(*shape, trans, litObjects);
		}
	}


	bool LightSource::CastsShadow()
	{
		return shadow;
	}


	std::vector<Core::Entity*>* LightSource::GetLitObjects()
	{
		return &litObjects;
	}


	GLuint LightSource::GetShadowTexture()
	{
		return ShadowRB->GetOutputTexture(0);
	}

}