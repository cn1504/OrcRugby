#include "LightRenderer.h"
#include <Assets/AssetDB.h>
#include <Assets/Light.h>
#include "VertexBuffer.h"

using namespace Core::Renderers;

LightRenderer::LightRenderer()
{
	Light = std::make_unique<Shader>("mesh.vert", "light.frag");
	LightWithShadow = std::make_unique<Shader>("mesh.vert", "lightwshadow.frag");
	DirectionalLight = std::make_unique<Shader>("fspassthrough.vert", "directionallight.frag");
}

LightRenderer::~LightRenderer() {}

void LightRenderer::SetCamera(std::shared_ptr<Core::Components::Camera> camera)
{
	Camera = camera;
}

void LightRenderer::SetGeometryTextures(const Core::Assets::Texture& depth, const Core::Assets::Texture& normal, const Core::Assets::Texture& base, const Core::Assets::Texture& msr)
{
	Light->Activate();
	Light->SetTexture("DepthTexture", depth, 0);
	Light->SetTexture("NormalTexture", normal, 1);
	Light->SetTexture("BaseTexture", base, 2);
	Light->SetTexture("MSRTexture", msr, 3);
	Core::Debug->GLError("ERROR: Setting geometry buffer textures in Light shader.");

	LightWithShadow->Activate();
	LightWithShadow->SetTexture("DepthTexture", depth, 0);
	LightWithShadow->SetTexture("NormalTexture", normal, 1);
	LightWithShadow->SetTexture("BaseTexture", base, 2);
	LightWithShadow->SetTexture("MSRTexture", msr, 3);
	Core::Debug->GLError("ERROR: Setting geometry buffer textures in LightWithShadow shader.");
		
	DirectionalLight->Activate();
	DirectionalLight->SetTexture("DepthTexture", depth, 0);
	DirectionalLight->SetTexture("NormalTexture", normal, 1);
	DirectionalLight->SetTexture("BaseTexture", base, 2);
	DirectionalLight->SetTexture("MSRTexture", msr, 3);
	Core::Debug->GLError("ERROR: Setting geometry buffer textures in DirectionalLight shader.");
}

void LightRenderer::DrawScene(const glm::vec2& bufferSize)
{
	CurrentBufferSize = bufferSize;

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glCullFace(GL_FRONT);

	VP = Camera->GetProjectionMatrix() * Camera->GetViewMatrix();
	PInverse = glm::inverse(Camera->GetProjectionMatrix());
	VInverse = glm::inverse(Camera->GetViewMatrix());

	Light->Activate();
	Light->SetUniform("PixelSize", 1.0f / bufferSize);
	Light->SetUniform("ProjectionInverse", PInverse);
	Light->SetUniform("ViewInverse", VInverse);

	LightWithShadow->Activate();
	LightWithShadow->SetUniform("PixelSize", 1.0f / bufferSize);
	LightWithShadow->SetUniform("ProjectionInverse", PInverse);
	LightWithShadow->SetUniform("ViewInverse", VInverse);

	DirectionalLight->Activate();
	DirectionalLight->SetUniform("ProjectionInverse", PInverse);

	Core::Scene->DrawLights(this);

	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	/*
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);	// Additive Blending


	LightRB->MakeCurrent();
	LightRB->Clear();


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, GeometryRB->GetDepthTexture());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, GeometryRB->GetOutputTexture(1));
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, GeometryRB->GetOutputTexture(2));
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, GeometryRB->GetOutputTexture(0));
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Assets::Textures["Skybox"]->GetID());


	LightShader->MakeCurrent();
	glUniform1i(LightShader->GetUL("DepthTexture"), 0);
	glUniform1i(LightShader->GetUL("NormalTexture"), 1);
	glUniform1i(LightShader->GetUL("BaseTexture"), 3);
	glUniform1i(LightShader->GetUL("MSRTexture"), 2);
	glUniform2f(LightShader->GetUL("PixelSize"), 1.0f / (float)(Settings::Window::Width), 1.0f / (float)(Settings::Window::Height));
	glUniformMatrix4fv(LightShader->GetUL("ProjectionInverse"), 1, GL_FALSE, glm::value_ptr(PInverse));



	LightWithShadowShader->MakeCurrent();
	glUniform1i(LightWithShadowShader->GetUL("DepthTexture"), 0);
	glUniform1i(LightWithShadowShader->GetUL("NormalTexture"), 1);
	glUniform1i(LightWithShadowShader->GetUL("BaseTexture"), 3);
	glUniform1i(LightWithShadowShader->GetUL("MSRTexture"), 2);
	glUniform2f(LightWithShadowShader->GetUL("PixelSize"), 1.0f / (float)(Settings::Window::Width), 1.0f / (float)(Settings::Window::Height));
	glUniformMatrix4fv(LightWithShadowShader->GetUL("ProjectionInverse"), 1, GL_FALSE, glm::value_ptr(PInverse));
	glUniformMatrix4fv(LightWithShadowShader->GetUL("ViewInverse"), 1, GL_FALSE, glm::value_ptr(VInverse));



	LightProbeShader->MakeCurrent();
	glUniform1i(LightProbeShader->GetUL("DepthTexture"), 0);
	glUniform1i(LightProbeShader->GetUL("NormalTexture"), 1);
	glUniform1i(LightProbeShader->GetUL("BaseTexture"), 3);
	glUniform1i(LightProbeShader->GetUL("MSRTexture"), 2);
	glUniform1i(LightProbeShader->GetUL("DistantProbe"), 5);
	glUniform2f(LightProbeShader->GetUL("PixelSize"), 1.0f / (float)(Settings::Window::Width), 1.0f / (float)(Settings::Window::Height));
	glUniformMatrix4fv(LightProbeShader->GetUL("ProjectionInverse"), 1, GL_FALSE, glm::value_ptr(PInverse));
	glUniformMatrix4fv(LightProbeShader->GetUL("ViewInverse"), 1, GL_FALSE, glm::value_ptr(VInverse));


	glCullFace(GL_FRONT);

	for (auto r : Lights)
	{
		auto e = r->Entity;
		glm::mat4 M = glm::translate(e->Transform.Position) * glm::scale(glm::vec3(r->Radius) * 2.0f);
		glm::mat4 MV = V * M;
		glm::mat4 MVP = P * MV;

		if (r->CastsShadow() && r->GetLitObjects()->size() > 0)
		{
			LightWithShadowShader->MakeCurrent();

			glActiveTexture(GL_TEXTURE4);
			glBindTexture(GL_TEXTURE_CUBE_MAP, r->GetShadowTexture());
			glUniform1i(LightWithShadowShader->GetUL("ShadowTexture"), 4);

			glUniformMatrix4fv(LightWithShadowShader->GetUL("ModelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
			glUniformMatrix4fv(LightWithShadowShader->GetUL("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(MV));
			glUniform3fv(LightWithShadowShader->GetUL("LightPosition"), 1, glm::value_ptr(glm::vec3(V * glm::vec4(e->Transform.Position, 1.0))));
			glUniform3fv(LightWithShadowShader->GetUL("LightDirection"), 1, glm::value_ptr(e->Transform.WSForward()));

			glUniformMatrix4fv(LightWithShadowShader->GetUL("LightViewMatrix"), 1, GL_FALSE, glm::value_ptr(glm::translate(-e->Transform.Position)));
			glUniformMatrix4fv(LightWithShadowShader->GetUL("LightProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(r->Projection));

			r->WriteShaderUniforms(LightWithShadowShader);

			Cube->EnableBuffers(LightWithShadowShader);
			Cube->Render(LightWithShadowShader);
			Cube->DisableBuffers(LightWithShadowShader);
		}
		else
		{
			LightShader->MakeCurrent();

			glUniformMatrix4fv(LightShader->GetUL("ModelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
			glUniformMatrix4fv(LightShader->GetUL("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(MV));
			glUniform3fv(LightShader->GetUL("LightPosition"), 1, glm::value_ptr(glm::vec3(V * glm::vec4(e->Transform.Position, 1.0))));
			glUniform3fv(LightShader->GetUL("LightDirection"), 1, glm::value_ptr(e->Transform.WSForward()));

			r->WriteShaderUniforms(LightShader);

			Cube->EnableBuffers(LightShader);
			Cube->Render(LightShader);
			Cube->DisableBuffers(LightShader);
		}

	}

	// Light Probe
	LightProbeShader->MakeCurrent();

	glm::mat4 M = Camera->Entity->Transform.ToMatrix() * glm::scale(glm::vec3(Settings::Video::MaxDrawDistance));
	glm::mat4 MV = V * M;
	glm::mat4 MVP = P * MV;
	glUniformMatrix4fv(LightProbeShader->GetUL("ModelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
	glUniformMatrix4fv(LightProbeShader->GetUL("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(MV));

	Cube->EnableBuffers(LightProbeShader);
	Cube->Render(LightProbeShader);
	Cube->DisableBuffers(LightProbeShader);


	glCullFace(GL_BACK);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// Reset Blending
	*/
}

void LightRenderer::DrawLight(const Core::Assets::Texture& shadowMap, const glm::mat4& transform, const Core::Assets::Light& light)
{
	auto MVP = VP * transform;
	auto MV = Camera->GetViewMatrix() * transform;

	LightWithShadow->Activate();
	LightWithShadow->SetTexture("ShadowTexture", shadowMap, 4);
	Core::Debug->GLError("ERROR: Could not set shadowmap texture.");
	LightWithShadow->SetUniform("ModelViewProjectionMatrix", MVP);
	LightWithShadow->SetUniform("ModelViewMatrix", MV);
	LightWithShadow->SetUniform("LightViewMatrix", -(transform * glm::vec4(0,0,0,1)));
	LightWithShadow->SetUniform("LightProjectionMatrix", glm::perspective(90.0f, 1.0f, 0.1f, light.GetRadius()));

	auto pos = MV * glm::vec4(0, 0, 0, 1);
	LightWithShadow->SetUniform("LightPosition", glm::vec3(pos.x, pos.y, pos.z));
	auto dir = MV * glm::vec4(0, 0, -1, 0);
	LightWithShadow->SetUniform("LightDirection", glm::vec3(dir.x, dir.y, dir.z));

	LightWithShadow->SetUniform("LightInvSqrRadius", 1.0f / (light.GetRadius() * light.GetRadius()));
	LightWithShadow->SetUniform("LightColor", light.GetColor());
	LightWithShadow->SetUniform("LightIntensity", light.GetIntensity());

	float anglescale = 1.0f / glm::max(0.001f, (light.GetCosInner() - light.GetCosOuter()));
	LightWithShadow->SetUniform("LightAngleScale", anglescale);
	LightWithShadow->SetUniform("LightAngleOffset", -light.GetCosOuter() * anglescale);
	
	auto indices = Core::AssetDB->GetVertexBuffer("Cube_Indices");
	auto vertices = Core::AssetDB->GetVertexBuffer("Cube_Vertices");
	auto uvs = Core::AssetDB->GetVertexBuffer("Cube_Uvs");
	auto normals = Core::AssetDB->GetVertexBuffer("Cube_Normals");
	LightWithShadow->SetVec3Attribute("Vertex", vertices->GetID());
	LightWithShadow->SetVec3Attribute("Normal", normals->GetID());
	LightWithShadow->SetVec2Attribute("Uv", uvs->GetID());
	LightWithShadow->SetElementBuffer(indices->GetID());
	Core::Debug->GLError("ERROR: Could not setup light render.");

	// Draw the triangles !
	glDrawElements(
		GL_TRIANGLES,               // mode
		(GLsizei)(indices->GetSize() / sizeof(unsigned short)),                  // count
		GL_UNSIGNED_SHORT,          // type
		(void*)0                    // element array buffer offset
		);
	Core::Debug->GLError("ERROR: Could not draw the light elements.");

	LightWithShadow->DisableAttribute("Vertex");
	LightWithShadow->DisableAttribute("Normal");
	LightWithShadow->DisableAttribute("Uv");

	Debug->GLError("ERROR: Failed to render light.");
}


void LightRenderer::DrawLight(const glm::mat4& transform, const Core::Assets::Light& light)
{
	auto MVP = VP * transform;
	auto MV = Camera->GetViewMatrix() * transform;

	Light->Activate();
	Light->SetUniform("ModelViewProjectionMatrix", MVP);
	Light->SetUniform("ModelViewMatrix", MV);

	auto pos = MV * glm::vec4(0, 0, 0, 1);
	Light->SetUniform("LightPosition", glm::vec3(pos.x, pos.y, pos.z));
	auto dir = MV * glm::vec4(0, 0, -1, 0);
	Light->SetUniform("LightDirection", glm::normalize(glm::vec3(dir.x, dir.y, dir.z)));

	Light->SetUniform("LightInvSqrRadius", 1.0f / (light.GetRadius() * light.GetRadius()));
	Light->SetUniform("LightColor", glm::vec4(light.GetColor(), 1.0f));
	Light->SetUniform("LightIntensity", light.GetIntensity());

	float anglescale = 1.0f / glm::max(0.001f, (light.GetCosInner() - light.GetCosOuter()));
	Light->SetUniform("LightAngleScale", anglescale);
	Light->SetUniform("LightAngleOffset", -light.GetCosOuter() * anglescale);

	auto indices = Core::AssetDB->GetVertexBuffer("Cube_Indices");
	auto vertices = Core::AssetDB->GetVertexBuffer("Cube_Vertices");
	auto uvs = Core::AssetDB->GetVertexBuffer("Cube_Uvs");
	auto normals = Core::AssetDB->GetVertexBuffer("Cube_Normals");
	Light->SetVec3Attribute("Vertex", vertices->GetID());
	Light->SetVec3Attribute("Normal", normals->GetID());
	Light->SetVec2Attribute("Uv", uvs->GetID());
	Light->SetElementBuffer(indices->GetID());
	Core::Debug->GLError("ERROR: Could not setup light render.");

	// Draw the triangles !
	glDrawElements(
		GL_TRIANGLES,               // mode
		(GLsizei)(indices->GetSize() / sizeof(unsigned short)),                  // count
		GL_UNSIGNED_SHORT,          // type
		(void*)0                    // element array buffer offset
		);
	Core::Debug->GLError("ERROR: Could not draw the light elements.");

	Light->DisableAttribute("Vertex");
	Light->DisableAttribute("Normal");
	Light->DisableAttribute("Uv");

	Debug->GLError("ERROR: Failed to render light.");
}



void LightRenderer::DrawLight(const Core::Assets::Texture& shadowMap, const glm::vec3& direction, const glm::vec4& color, float intensity)
{
	DirectionalLight->Activate();

	DirectionalLight->SetUniform("LightDirection", glm::vec3(Camera->GetViewMatrix() * glm::vec4(direction, 0.0f)));
	DirectionalLight->SetUniform("LightColor", color);
	DirectionalLight->SetUniform("LightIntensity", intensity);

	glDisable(GL_CULL_FACE);
	SQuad->Render(CurrentBufferSize);
	glEnable(GL_CULL_FACE);

	Debug->GLError("ERROR: Failed to render directional light.");
}