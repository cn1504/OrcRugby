#include "GeometryToCubeMapRenderer.h"

using namespace Core::Renderers;

GeometryToCubeMapRenderer::GeometryToCubeMapRenderer()
{
	LightGeometry = std::make_unique<Shader>("", "cubemap.geom", "");
	Mesh = std::make_unique<Shader>("mesh.vert", "cubemap.geom", "material.frag");
	SkeletalMesh = std::make_unique<Shader>("skeletalmesh.vert", "cubemap.geom", "material.frag");
	Sphere = std::make_unique<Shader>("mesh.vert", "cubemap.geom", "sphere.frag");
	Cylinder = std::make_unique<Shader>("mesh.vert", "cubemap.geom", "cylinder.frag");
	PointCloud = std::make_unique<Shader>("pc.vert", "pccubemap.geom", "material.frag");
}

GeometryToCubeMapRenderer::~GeometryToCubeMapRenderer() {}

void GeometryToCubeMapRenderer::Draw()
{
}