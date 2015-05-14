#pragma once
#include <Core.h>
#include "Shader.h"

namespace Core
{
	namespace Renderers
	{
		class ShadowMapRenderer
		{
		private:
			std::unique_ptr<Shader> Mesh;
			std::unique_ptr<Shader> SkeletalMesh;
			std::unique_ptr<Shader> Sphere;
			std::unique_ptr<Shader> Cylinder;
			std::unique_ptr<Shader> PointCloud;

		public:
			ShadowMapRenderer();
			~ShadowMapRenderer();

			void Draw();
		};
	}
}