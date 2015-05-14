#pragma once
#include <Core.h>
#include "FreeBody.h"

namespace Core
{
	namespace Components
	{

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
}