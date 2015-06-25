#include "LightRenderer.h"
#include <Assets/AssetDB.h>
#include <Assets/Light.h>
#include "VertexArray.h"

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

	VP = Camera->GetViewProjection();
	PInverse = Camera->GetInverseProjection();
	VInverse = Camera->GetInverseView();

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
	DirectionalLight->SetUniform("ViewInverse", VInverse);

	Core::Scene->DrawLights(this);

	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void LightRenderer::DrawLight(const Core::Assets::Texture& shadowMap, const glm::mat4& transform, const Core::Assets::Light& light)
{
	auto MVP = VP * transform;
	auto MV = Camera->GetView() * transform;

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
	
	auto vao = Core::AssetDB->GetVertexArray("Cube"); 
	vao->Bind();
	Core::Debug->GLError("ERROR: Could not setup light render.");

	// Draw the triangles !
	glDrawElements(
		GL_TRIANGLES,               // mode
		vao->GetSize(),             // count
		GL_UNSIGNED_SHORT,          // type
		(void*)0                    // element array buffer offset
		);
	vao->Unbind();
	Core::Debug->GLError("ERROR: Could not draw the light elements.");
}


void LightRenderer::DrawLight(const glm::mat4& transform, const Core::Assets::Light& light)
{
	auto MVP = VP * transform;
	auto MV = Camera->GetView() * transform;

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

	auto vao = Core::AssetDB->GetVertexArray("Cube");
	vao->Bind();
	Core::Debug->GLError("ERROR: Could not setup light render.");

	// Draw the triangles !
	glDrawElements(
		GL_TRIANGLES,               // mode
		vao->GetSize(),             // count
		GL_UNSIGNED_SHORT,          // type
		(void*)0                    // element array buffer offset
		);
	vao->Unbind();
	Core::Debug->GLError("ERROR: Could not draw the light elements.");
}



void LightRenderer::DrawLight(const Core::Assets::Texture& shadowMap, const glm::vec3& direction, const glm::vec4& color, float intensity, const glm::mat4& lightProj, const glm::mat4& lightView, float maxDepth)
{
	DirectionalLight->Activate();

	DirectionalLight->SetUniform("LightDirection", glm::vec3(Camera->GetView() * glm::vec4(direction, 0.0f)));
	DirectionalLight->SetUniform("LightColor", color);
	DirectionalLight->SetUniform("LightIntensity", intensity);
	
	DirectionalLight->SetUniform("LightView", lightView);
	DirectionalLight->SetUniform("LightProjection", lightProj);
	DirectionalLight->SetTexture("ShadowTexture", shadowMap, 8);
	DirectionalLight->SetUniform("MaxDepth", 1.0f / (maxDepth * maxDepth));

	glDisable(GL_CULL_FACE);
	SQuad->Render(CurrentBufferSize);
	glEnable(GL_CULL_FACE);

	Debug->GLError("ERROR: Failed to render directional light.");
}