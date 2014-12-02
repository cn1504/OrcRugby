#include "LightSource.h"
#include "Scene.h"
#include "Texture.h"

namespace Core
{

	LightSource::LightSource(Scene* scene, glm::vec3 color, float radius, float intensity, float cosInner, float cosOuter, bool castsShadow)
	{
		this->scene = scene;
		Color = color;
		shadow = castsShadow;
		ShadowRB = nullptr;
		Radius = radius;
		Intensity = intensity;
		CosInner = cosInner;
		CosOuter = cosOuter;
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


	void LightSource::WriteShaderUniforms(Shader* shader)
	{
		glUniform4fv(shader->GetUL("LightColor"), 1, glm::value_ptr(Color));
		glUniform1f(shader->GetUL("LightInvSqrRadius"), 1.0f / (Radius * Radius));
		glUniform1f(shader->GetUL("LightIntensity"), Intensity);

		float anglescale = 1.0f / glm::max(0.001f, (CosInner - CosOuter));
		glUniform1f(shader->GetUL("LightAngleScale"), anglescale);
		glUniform1f(shader->GetUL("LightAngleOffset"), -CosOuter * anglescale);
	}

}