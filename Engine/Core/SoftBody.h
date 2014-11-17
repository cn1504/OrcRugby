#pragma once
#include "Core.h"
#include "FreeBody.h"

namespace Core
{

	class DynamicsWorld;
	class Material;
	class Mesh;

	class SoftBody
		: public FreeBody
	{
	public:
		SoftBody(DynamicsWorld* world, Material* material, btSoftBody* body, Mesh* mesh);
		virtual ~SoftBody();

		virtual void Load();

		btSoftBody* GetBody();
		void RebuildMesh();

	protected:
		btSoftBody* body;

		Mesh* mesh;
	};

}