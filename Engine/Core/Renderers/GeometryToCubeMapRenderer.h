#pragma once
#include <Core.h>
#include "Shader.h"

namespace Core
{
	namespace Renderers
	{
		class GeometryToCubeMapRenderer
		{
		private:
			std::unique_ptr<Shader> LightGeometry;
			std::unique_ptr<Shader> SkeletalMesh;
			std::unique_ptr<Shader> PointCloud;

		public:
			GeometryToCubeMapRenderer();
			~GeometryToCubeMapRenderer();

			void Draw();
		};
	}
}