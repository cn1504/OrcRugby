#include "GeometryToCubeMapRenderer.h"

using namespace Core::Renderers;

GeometryToCubeMapRenderer::GeometryToCubeMapRenderer()
{
	//LightGeometry = std::make_unique<Shader>("", "cubemap.geom", "");
	SkeletalMesh = std::make_unique<Shader>("skeletalmesh.vert", "cubemap.geom", "material.frag");
	PointCloud = std::make_unique<Shader>("pc.vert", "pccubemap.geom", "material.frag");
}

GeometryToCubeMapRenderer::~GeometryToCubeMapRenderer() {}

void GeometryToCubeMapRenderer::Draw()
{
}