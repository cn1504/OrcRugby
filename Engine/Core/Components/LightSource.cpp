#include "LightSource.h"
#include <Assets/AssetDB.h>
#include <Assets/Light.h>
#include <Renderers/LightRenderer.h>

using namespace Core::Components;

int LightSource::LightCount = 0;
std::unique_ptr<Core::Renderers::ShadowCubeMapRenderer> LightSource::Renderer = nullptr;

LightSource::LightSource(std::string light)
{
	LightCount++;

	Light = light;
	ShadowBuffer = nullptr;

	auto l = Core::AssetDB->GetLight(Light);
	if (l != nullptr)
	{
		auto r = l->GetRadius();
		Scale(glm::vec3(r * 2.0f));

		if (l->CastsShadow())
		{
			ShadowBuffer = std::make_unique<Core::Renderers::ShadowCubeMapBuffer>(glm::ivec2(1024, 1024));

			if (Renderer == nullptr)
			{
				Renderer = std::make_unique<Core::Renderers::ShadowCubeMapRenderer>();
			}
		}
	}
}
LightSource::~LightSource()
{
	LightCount--;
	if (LightCount <= 0)
	{
		Renderer = nullptr;
	}
}

void LightSource::Update()
{
	if (ShadowBuffer != nullptr)
	{
		ShadowBuffer->SetAsTarget();
		ShadowBuffer->Clear();
		Renderer->DrawScene();
	}

	Entity::Update();
}

void LightSource::DrawLights(Core::Renderers::LightRenderer* renderer)
{
	auto l = Core::AssetDB->GetLight(Light);
	if (l != nullptr)
	{
		if (l->CastsShadow())
		{
			renderer->DrawLight(*ShadowBuffer->Depth, GetMatrix(), *Core::AssetDB->GetLight(Light));
		}
		else
		{
			renderer->DrawLight(GetMatrix(), *Core::AssetDB->GetLight(Light));
		}
	}

	Entity::DrawLights(renderer);
}