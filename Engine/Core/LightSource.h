#pragma once
#include "Core.h"
#include "Component.h"
#include <Bullet/BulletCollision/CollisionDispatch/btGhostObject.h>
#include "RenderBuffer.h"

#include "Shader.h"

namespace Core
{
	class Scene;
	class Texture;
	class Entity;

	class LightSource
		: public Component
	{
	protected:
		Scene* scene;
		bool shadow;
		btSphereShape* shape;
		std::vector<Core::Entity*> litObjects;

	public:
		RenderBuffer* ShadowRB;
		glm::mat4 Projection;
		glm::vec3 Color;
		float Radius;
		float Intensity;
		float CosInner;
		float CosOuter;

		LightSource(Scene* scene, glm::vec3 color, float radius, float intensity, float cosInner, float cosOuter, bool castsShadow = false);
		virtual ~LightSource();

		virtual void Load();
		virtual void Update();

		bool CastsShadow();
		std::vector<Core::Entity*>* GetLitObjects();
		GLuint GetShadowTexture();
		void WriteShaderUniforms(Shader* shader);
	};

}