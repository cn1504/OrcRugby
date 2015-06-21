#include "ShadowMapRenderer.h"
#include <Components/Entity.h>

using namespace Core::Renderers;

ShadowMapRenderer::ShadowMapRenderer()
{
	/*
	Mesh = std::make_unique<Shader>("mesh.vert", "shadow.frag");
	SkeletalMesh = std::make_unique<Shader>("skeletalmesh.vert", "shadow.frag");
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

void ShadowMapRenderer::DrawMesh(const VertexArray& vao,
	const Core::Assets::Material& material, const glm::mat4& transform)
{
	StaticMesh->Activate();

	glm::mat4 MV = View * transform;
	glm::mat4 MVP = Projection * MV;
	StaticMesh->SetUniform("ModelViewProjectionMatrix", MVP);
	StaticMesh->SetUniform("ModelViewMatrix", MV);

	StaticMesh->SetUniform("MaxDepth", MaxDepth);

	vao.Bind();
	Core::Debug->GLError("ERROR: Could not bind the static mesh vao.");

	// Draw the triangles !
	glDrawElements(
		GL_TRIANGLES,               // mode
		vao.GetSize(),              // count
		GL_UNSIGNED_SHORT,          // type
		(void*)0                    // element array buffer offset
		);
	Core::Debug->GLError("ERROR: Could not draw the static mesh elements to shadow map.");

	vao.Unbind();

	Debug->GLError("ERROR: Failed to render static mesh to shadow map.");
}

void ShadowMapRenderer::DrawSea(const VertexArray& vao,
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

	vao.Bind();
	Core::Debug->GLError("ERROR: Could not bind the sea mesh buffers.");

	// Draw the triangles !
	glDrawElements(
		GL_TRIANGLES,               // mode
		vao.GetSize(),              // count
		GL_UNSIGNED_SHORT,          // type
		(void*)0                    // element array buffer offset
		);
	vao.Unbind();
	Core::Debug->GLError("ERROR: Could not draw the sea mesh elements to shadow map.");
}

void ShadowMapRenderer::SetMatrices(const glm::mat4& projection, const glm::mat4& view, float maxDepth)
{
	Projection = projection;
	View = view;
	MaxDepth = maxDepth;
}