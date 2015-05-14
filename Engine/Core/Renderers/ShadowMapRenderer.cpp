#include "ShadowMapRenderer.h"

using namespace Core::Renderers;

ShadowMapRenderer::ShadowMapRenderer()
{
	Mesh = std::make_unique<Shader>("mesh.vert", "shadow.frag");
	SkeletalMesh = std::make_unique<Shader>("skeletalmesh.vert", "shadow.frag");
	Sphere = std::make_unique<Shader>("mesh.vert", "shadowsphere.frag");
	Cylinder = std::make_unique<Shader>("mesh.vert", "shadowcylinder.frag");
	PointCloud = std::make_unique<Shader>("pc.vert", "shadow.frag");
}

ShadowMapRenderer::~ShadowMapRenderer() {}

void ShadowMapRenderer::Draw()
{
}