#include "ShadowMapRenderer.h"
#include <Components/Entity.h>

using namespace Core::Renderers;

ShadowMapRenderer::ShadowMapRenderer()
{
	/*
	Mesh = std::make_unique<Shader>("mesh.vert", "shadow.frag");
	SkeletalMesh = std::make_unique<Shader>("skeletalmesh.vert", "shadow.frag");
	Sphere = std::make_unique<Shader>("mesh.vert", "shadowsphere.frag");
	Cylinder = std::make_unique<Shader>("mesh.vert", "shadowcylinder.frag");
	PointCloud = std::make_unique<Shader>("pc.vert", "shadow.frag");
	*/
	
	StaticMesh = std::make_unique<Shader>("mesh.vert", "shadow.frag");	
	Sea = std::make_unique<Shader>("sea.vert", "seashadow.frag");
}

ShadowMapRenderer::~ShadowMapRenderer() {}

void ShadowMapRenderer::DrawScene()
{
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	Core::Scene->Draw(this);
	glCullFace(GL_BACK);
}

void ShadowMapRenderer::DrawMesh(const VertexBuffer& indices, const VertexBuffer& vertices,
	const VertexBuffer& uvs, const VertexBuffer& normals,
	const Core::Assets::Material& material, const glm::mat4& transform)
{
	StaticMesh->Activate();

	glm::mat4 MV = View * transform;
	glm::mat4 MVP = Projection * MV;
	StaticMesh->SetUniform("ModelViewProjectionMatrix", MVP);
	StaticMesh->SetUniform("ModelViewMatrix", MV);

	StaticMesh->SetUniform("MaxDepth", MaxDepth);

	StaticMesh->SetVec3Attribute("Vertex", vertices.GetID());
	StaticMesh->SetVec3Attribute("Normal", normals.GetID());
	StaticMesh->SetVec2Attribute("Uv", uvs.GetID());

	StaticMesh->SetElementBuffer(indices.GetID());
	Core::Debug->GLError("ERROR: Could not bind the static mesh buffers.");

	// Draw the triangles !
	glDrawElements(
		GL_TRIANGLES,               // mode
		(GLsizei)(indices.GetSize() / sizeof(unsigned short)),                  // count
		GL_UNSIGNED_SHORT,          // type
		(void*)0                    // element array buffer offset
		);
	Core::Debug->GLError("ERROR: Could not draw the static mesh elements to shadow map.");

	StaticMesh->DisableAttribute("Vertex");
	StaticMesh->DisableAttribute("Normal");
	StaticMesh->DisableAttribute("Uv");

	Debug->GLError("ERROR: Failed to render static mesh to shadow map.");
}

void ShadowMapRenderer::DrawSea(const VertexBuffer& indices, const VertexBuffer& vertices,
	const VertexBuffer& uvs, const VertexBuffer& normals,
	const glm::mat4& transform)
{
	Sea->Activate();

	glm::mat4 VP = Projection * View;
	Sea->SetUniform("ViewProjectionMatrix", VP);
	Sea->SetUniform("ProjectionMatrix", Projection);
	Sea->SetUniform("ViewMatrix", View);
	Sea->SetUniform("ModelMatrix", transform);
	Sea->SetUniform("iGlobalTime", Core::Time->Elapsed);

	Sea->SetUniform("MaxDepth", MaxDepth);

	Sea->SetVec3Attribute("Vertex", vertices.GetID());

	Sea->SetElementBuffer(indices.GetID());
	Core::Debug->GLError("ERROR: Could not bind the sea mesh buffers.");

	// Draw the triangles !
	glDrawElements(
		GL_TRIANGLES,               // mode
		(GLsizei)(indices.GetSize() / sizeof(unsigned short)),                  // count
		GL_UNSIGNED_SHORT,          // type
		(void*)0                    // element array buffer offset
		);
	Core::Debug->GLError("ERROR: Could not draw the sea mesh elements to shadow map.");

	Sea->DisableAttribute("Vertex");

	Debug->GLError("ERROR: Failed to render sea mesh to shadow map.");
}

void ShadowMapRenderer::SetMatrices(const glm::mat4& projection, const glm::mat4& view, float maxDepth)
{
	Projection = projection;
	View = view;
	MaxDepth = maxDepth;
}