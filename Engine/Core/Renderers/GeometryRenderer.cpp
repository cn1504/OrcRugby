#include "GeometryRenderer.h"
#include <Components/Entity.h>
#include <Window/Window.h>
#include <Assets/Material.h>

using namespace Core::Renderers;

GeometryRenderer::GeometryRenderer()
{
	LightGeometry = std::make_unique<Shader>("mesh.vert", "light.frag");
	SkeletalMesh = std::make_unique<Shader>("skeletalmesh.vert", "material.frag");
	PointCloud = std::make_unique<Shader>("pc.vert", "pc.geom", "material.frag");
	StaticMesh = std::make_unique<Shader>("mesh.vert", "material.frag");

	Sea = std::make_unique<Shader>("sea.vert", "sea.frag");
}

GeometryRenderer::~GeometryRenderer() {}

void GeometryRenderer::SetCamera(std::shared_ptr<Core::Components::Camera> camera)
{
	Camera = camera;
}

void GeometryRenderer::DrawScene()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	Core::Scene->Draw(this);
	/*
	for (auto e : Entities)
	{
		if (DistantEntities.count(e) > 0)
			continue;

		if (e->IsRenderable()) {

			auto mat = e->GetComponent<Material>();
			if (mat != nullptr && mat->IsTranslucent)
			{
				TranslucentEntities[glm::distance(e->Transform.Position, Camera->Entity->Transform.Position)] = e;
				continue;
			}

			glm::mat4 M = e->Transform.ToMatrix();
			glm::mat4 MV = V * M;
			glm::mat4 MVP = P * MV;

			auto anim = e->GetComponent<SkeletalAnimation>();
			auto pc = e->GetComponent<PointCloud>();
			if (anim != nullptr)
			{
				SkeletalMeshShader->MakeCurrent();

				glUniformMatrix4fv(SkeletalMeshShader->GetUL("ModelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
				glUniformMatrix4fv(SkeletalMeshShader->GetUL("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(MV));
				glUniformMatrix4fv(SkeletalMeshShader->GetUL("ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(P));

				anim->Render(SkeletalMeshShader);
			}
			else if (pc != nullptr)
			{
				PointCloudShader->MakeCurrent();

				glUniformMatrix4fv(PointCloudShader->GetUL("ModelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
				glUniformMatrix4fv(PointCloudShader->GetUL("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(MV));
				glUniformMatrix4fv(PointCloudShader->GetUL("ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(P));

				mat->WriteShaderUniforms(PointCloudShader);

				pc->ViewPosition = Camera->Entity->Transform.Position;
				pc->EnableBuffers(PointCloudShader);
				pc->Render(PointCloudShader);
				pc->DisableBuffers(PointCloudShader);
			}
			else // Mesh
			{
				auto r = e->GetRenderable();
				
				MeshShader->MakeCurrent();

				glUniformMatrix4fv(MeshShader->GetUL("ModelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
				glUniformMatrix4fv(MeshShader->GetUL("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(MV));
				glUniformMatrix4fv(MeshShader->GetUL("ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(P));

				mat->WriteShaderUniforms(MeshShader);

				r->EnableBuffers(MeshShader);
				r->Render(MeshShader);
				r->DisableBuffers(MeshShader);
			}

		}
	}

	// Render Skybox
	glCullFace(GL_FRONT);
	SkyboxShader->MakeCurrent();
	glUniformMatrix4fv(SkyboxShader->GetUL("ModelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(P * V * Camera->Entity->Transform.ToMatrix() * glm::scale(glm::vec3(Settings::Video::MaxDrawDistance))));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Assets::Textures["Skybox"]->GetID());
	glUniform1i(SkyboxShader->GetUL("gCubemapTexture"), 0);

	Cube->EnableBuffers(SkyboxShader);
	Cube->Render(SkyboxShader);
	Cube->DisableBuffers(SkyboxShader);
	glCullFace(GL_BACK);
	*/
}

void GeometryRenderer::DrawMesh(const VertexArray& vao,
	const Core::Assets::Material& material, const glm::mat4& transform)
{
	StaticMesh->Activate();

	glm::mat4 MV = Camera->GetView() * transform;
	glm::mat4 MVP = Camera->GetProjection() * MV;
	StaticMesh->SetUniform("ModelViewProjectionMatrix", MVP);
	StaticMesh->SetUniform("ModelViewMatrix", MV);

	material.SetShaderUniforms(StaticMesh.get());

	vao.Bind();
	Core::Debug->GLError("ERROR: Could not bind the static mesh values.");

	// Draw the triangles !
	glDrawElements(
		GL_TRIANGLES,               // mode
		vao.GetSize(),                  // count
		GL_UNSIGNED_SHORT,          // type
		(void*)0                    // element array buffer offset
		);
	vao.Unbind();
	Core::Debug->GLError("ERROR: Could not draw the static mesh elements.");
}

void GeometryRenderer::DrawSea(const VertexArray& vao,
	const glm::mat4& transform)
{
	Sea->Activate();
	
	glm::mat4 VP = Camera->GetViewProjection();
	Sea->SetUniform("ViewProjectionMatrix", VP);
	Sea->SetUniform("ViewMatrix", Camera->GetView());
	Sea->SetUniform("ModelMatrix", transform);	
	Sea->SetUniform("iGlobalTime", Core::Time->Elapsed);

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
	Core::Debug->GLError("ERROR: Could not draw the sea mesh elements.");
}