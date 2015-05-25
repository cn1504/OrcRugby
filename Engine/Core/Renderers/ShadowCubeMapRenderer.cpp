#include "ShadowCubeMapRenderer.h"

using namespace Core::Renderers;

ShadowCubeMapRenderer::ShadowCubeMapRenderer()
{
	Mesh = std::make_unique<Shader>("mesh.vert", "cubemap.geom", "shadow.frag");
	SkeletalMesh = std::make_unique<Shader>("skeletalmesh.vert", "cubemap.geom", "shadow.frag");
	Sphere = std::make_unique<Shader>("mesh.vert", "cubemap.geom", "shadowsphere.frag");
	Cylinder = std::make_unique<Shader>("mesh.vert", "cubemap.geom", "shadowcylinder.frag");
	PointCloud = std::make_unique<Shader>("pc.vert", "pccubemap.geom", "shadow.frag");
}

ShadowCubeMapRenderer::~ShadowCubeMapRenderer() {}

void ShadowCubeMapRenderer::DrawScene()
{
	/*
	for (auto l : Lights)
	{
		if (l->CastsShadow())
		{
			l->ShadowRB->MakeCurrent();
			l->ShadowRB->Clear();
			Debug::GLError("ERROR: Could not initialize the shadow framebuffer.");


			l->Projection = glm::perspective(90.0f, 1.0f, 0.01f, l->Radius);	

			glm::mat4 LV = glm::translate(-l->Entity->Transform.Position);
				
					
			// Get objects within light range
			auto objs = l->GetLitObjects();

			// Render them
			for (auto e : *objs)
			{
			// Render everything
			//for (auto e : Entities)
			//{
				if (e->IsRenderable() && e != l->Entity) {

					glm::mat4 M = e->Transform.ToMatrix();
					glm::mat4 MV = LV * M;
					glm::mat4 MVP = MV;

					auto anim = e->GetComponent<SkeletalAnimation>();
					auto pc = e->GetComponent<PointCloud>();
					if (anim != nullptr)
					{
						ShadowSkeletalMeshShader->MakeCurrent();

						glUniformMatrix4fv(ShadowSkeletalMeshShader->GetUL("CubeMapProjections[0]"), 1, GL_FALSE, glm::value_ptr(l->Projection * CubeMapViewMatrices[0]));
						glUniformMatrix4fv(ShadowSkeletalMeshShader->GetUL("CubeMapProjections[1]"), 1, GL_FALSE, glm::value_ptr(l->Projection * CubeMapViewMatrices[1]));
						glUniformMatrix4fv(ShadowSkeletalMeshShader->GetUL("CubeMapProjections[2]"), 1, GL_FALSE, glm::value_ptr(l->Projection * CubeMapViewMatrices[2]));
						glUniformMatrix4fv(ShadowSkeletalMeshShader->GetUL("CubeMapProjections[3]"), 1, GL_FALSE, glm::value_ptr(l->Projection * CubeMapViewMatrices[3]));
						glUniformMatrix4fv(ShadowSkeletalMeshShader->GetUL("CubeMapProjections[4]"), 1, GL_FALSE, glm::value_ptr(l->Projection * CubeMapViewMatrices[4]));
						glUniformMatrix4fv(ShadowSkeletalMeshShader->GetUL("CubeMapProjections[5]"), 1, GL_FALSE, glm::value_ptr(l->Projection * CubeMapViewMatrices[5]));

						glUniform1f(ShadowSkeletalMeshShader->GetUL("MaxDepth"), l->Radius);

						glUniformMatrix4fv(ShadowSkeletalMeshShader->GetUL("ModelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
						glUniformMatrix4fv(ShadowSkeletalMeshShader->GetUL("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(MV));

						anim->Render(ShadowSkeletalMeshShader);
					}
					else if (pc != nullptr)
					{							
						ShadowPointCloudShader->MakeCurrent();

						glUniformMatrix4fv(ShadowPointCloudShader->GetUL("CubeMapProjections[0]"), 1, GL_FALSE, glm::value_ptr(l->Projection * CubeMapViewMatrices[0]));
						glUniformMatrix4fv(ShadowPointCloudShader->GetUL("CubeMapProjections[1]"), 1, GL_FALSE, glm::value_ptr(l->Projection * CubeMapViewMatrices[1]));
						glUniformMatrix4fv(ShadowPointCloudShader->GetUL("CubeMapProjections[2]"), 1, GL_FALSE, glm::value_ptr(l->Projection * CubeMapViewMatrices[2]));
						glUniformMatrix4fv(ShadowPointCloudShader->GetUL("CubeMapProjections[3]"), 1, GL_FALSE, glm::value_ptr(l->Projection * CubeMapViewMatrices[3]));
						glUniformMatrix4fv(ShadowPointCloudShader->GetUL("CubeMapProjections[4]"), 1, GL_FALSE, glm::value_ptr(l->Projection * CubeMapViewMatrices[4]));
						glUniformMatrix4fv(ShadowPointCloudShader->GetUL("CubeMapProjections[5]"), 1, GL_FALSE, glm::value_ptr(l->Projection * CubeMapViewMatrices[5]));

						glUniform1f(ShadowPointCloudShader->GetUL("MaxDepth"), l->Radius);

						glUniformMatrix4fv(ShadowPointCloudShader->GetUL("ModelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
						glUniformMatrix4fv(ShadowPointCloudShader->GetUL("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(MV));

						pc->EnableBuffers(ShadowPointCloudShader);
						pc->Render(ShadowPointCloudShader);
						pc->DisableBuffers(ShadowPointCloudShader);							
}

					else // Mesh
					{
						auto r = e->GetRenderable();
						if (r == Sphere)
						{
							ShadowSphereShader->MakeCurrent();

							glUniformMatrix4fv(ShadowSphereShader->GetUL("ModelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
							glUniformMatrix4fv(ShadowSphereShader->GetUL("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(MV));
							glUniformMatrix4fv(ShadowSphereShader->GetUL("ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(l->Projection));
							glUniform3fv(ShadowSphereShader->GetUL("SpherePosition"), 1, glm::value_ptr(glm::vec3(V * glm::vec4(e->Transform.Position, 1.0f))));
							glUniform1f(ShadowSphereShader->GetUL("Radius"), r->Entity->Transform.Scale.x / 2);

							r->EnableBuffers(ShadowSphereShader);
							r->Render(ShadowSphereShader);
							r->DisableBuffers(ShadowSphereShader);
						}
						else if (r == Cylinder)
						{
							ShadowCylinderShader->MakeCurrent();

							glUniformMatrix4fv(ShadowCylinderShader->GetUL("ModelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
							glUniformMatrix4fv(ShadowCylinderShader->GetUL("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(MV));
							glUniformMatrix4fv(ShadowCylinderShader->GetUL("ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(l->Projection));
							glUniform3fv(ShadowCylinderShader->GetUL("Direction"), 1, glm::value_ptr(glm::normalize(glm::vec3(MV * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)))));
							glUniform3fv(ShadowCylinderShader->GetUL("Center"), 1, glm::value_ptr(glm::vec3(V * glm::vec4(e->Transform.Position, 1.0f))));
							glUniform1f(ShadowCylinderShader->GetUL("Length"), r->Entity->Transform.Scale.y);
							glUniform1f(ShadowCylinderShader->GetUL("Radius"), r->Entity->Transform.Scale.x / 2);

							r->EnableBuffers(ShadowCylinderShader);
							r->Render(ShadowCylinderShader);
							r->DisableBuffers(ShadowCylinderShader);
						}
						else
						{
							ShadowMeshShader->MakeCurrent();

							glUniformMatrix4fv(ShadowMeshShader->GetUL("CubeMapProjections[0]"), 1, GL_FALSE, glm::value_ptr(l->Projection * CubeMapViewMatrices[0]));
							glUniformMatrix4fv(ShadowMeshShader->GetUL("CubeMapProjections[1]"), 1, GL_FALSE, glm::value_ptr(l->Projection * CubeMapViewMatrices[1]));
							glUniformMatrix4fv(ShadowMeshShader->GetUL("CubeMapProjections[2]"), 1, GL_FALSE, glm::value_ptr(l->Projection * CubeMapViewMatrices[2]));
							glUniformMatrix4fv(ShadowMeshShader->GetUL("CubeMapProjections[3]"), 1, GL_FALSE, glm::value_ptr(l->Projection * CubeMapViewMatrices[3]));
							glUniformMatrix4fv(ShadowMeshShader->GetUL("CubeMapProjections[4]"), 1, GL_FALSE, glm::value_ptr(l->Projection * CubeMapViewMatrices[4]));
							glUniformMatrix4fv(ShadowMeshShader->GetUL("CubeMapProjections[5]"), 1, GL_FALSE, glm::value_ptr(l->Projection * CubeMapViewMatrices[5]));

							glUniform1f(ShadowMeshShader->GetUL("MaxDepth"), l->Radius);

							glUniformMatrix4fv(ShadowMeshShader->GetUL("ModelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
							glUniformMatrix4fv(ShadowMeshShader->GetUL("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(MV));

							r->EnableBuffers(ShadowMeshShader);
							r->Render(ShadowMeshShader);
							r->DisableBuffers(ShadowMeshShader);
						}
					}

				}
			}
		}
	}
	*/
}


void ShadowCubeMapRenderer::DrawMesh(const VertexBuffer& indices, const VertexBuffer& vertices,
	const VertexBuffer& uvs, const VertexBuffer& normals,
	const glm::vec4& color, const glm::vec4& msr, const glm::mat4& transform)
{}


void ShadowCubeMapRenderer::DrawSea(const VertexBuffer& indices, const VertexBuffer& vertices,
	const VertexBuffer& uvs, const VertexBuffer& normals,
	const glm::mat4& transform)
{
}