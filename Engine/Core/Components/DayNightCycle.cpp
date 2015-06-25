#include "DayNightCycle.h"
#include <Assets/AssetDB.h>
#include <Renderers/LightRenderer.h>

using namespace Core::Components;

const static double SUNRISE = 5.47f / 24.0f;		// % of Day
const static double SUNSET = 19.35f / 24.0f;		// % of Day
const static float MAX_SUN_ALTITUDE = 65.0f;	// Degrees from S horizon
const static float SUNRISE_HEADING = 70.0f;	    // Degrees from E from N
const static float SUNSET_HEADING = 290.0f;	    // Degrees from E from N

const static double MOONRISE = 17.25f / 24.0f;	// % of Day
const static double MOONSET = 4.45f / 24.0f;		// % of Day
const static float MAX_MOON_ALTITUDE = 46.0f;	// Degrees from S horizon
const static float MOONRISE_HEADING = 91.0f;	// Degrees from E from N
const static float MOONSET_HEADING = 269.0f;	// Degrees from E from N

DayNightCycle::DayNightCycle(float timeRatio)
{
	SunBuffer = std::make_unique<Core::Renderers::ShadowBuffer>(glm::ivec2(2048, 2048));
	MoonBuffer = std::make_unique<Core::Renderers::ShadowBuffer>(glm::ivec2(2048, 2048));
	Renderer = std::make_unique<Core::Renderers::ShadowMapRenderer>();

	TimeRatio = timeRatio;
	CurrentTime = 5.0f / 24.0f;
	
	SunLightColor = glm::vec4(1, 1, 1, 1);
	MoonLightColor = glm::vec4(1, 1, 1, 1);

	SunLightIntensity = 110000.0f;	
	MoonLightIntensity = 0.25f;
	
	// In Lux or Lumins per meter squared
	// Common Lux values:
	// Bright Sunlight: 110000.0f;
	// Sunlight, Overcast: 1000.0f to 2000.0f;
	// Shade illuminated by sky midday: 20000.0f;
	// Darkest of storm clouds: 200.0f;
	// Sunrise/Sunset, Clear Day: 400.0f;
	// Sunrise/Sunset, Fully overcast: 40.0f;

	// Fullmoon, clear night: 0.25f;
	// Quartermoon, clear night: 0.01f;
	// Starlight, clear night: 0.002f;
	// Starlight, overcast moonless: 0.0001f;
}

DayNightCycle::~DayNightCycle() {}

void DayNightCycle::Update()
{
	const static float SECONDS_PER_DAY = 86400.0f;
	
	// Advance Clock
	CurrentTime += (TimeRatio * Core::Time->Delta) / SECONDS_PER_DAY; // Convert elapsed seconds to days
	while (CurrentTime >= 1.0f)
	{
		CurrentTime -= 1.0f;
	}	
	
	// Calculate Sun, Moon and Sky values
	if (CurrentTime > SUNRISE && CurrentTime < SUNSET)
	{
		double sunT = ((double)CurrentTime - SUNRISE) / (SUNSET - SUNRISE);
		float phi = glm::pi<float>() / 2.0f - (float)sunT * glm::pi<float>();
		float theta = 0.0f; 

		SunDirection = glm::normalize(glm::vec3(-glm::sin(phi)*glm::cos(theta), glm::cos(phi), glm::sin(phi)*glm::sin(theta)));
				
		double ColorShiftRange = 0.1;
		double colorT = ((sunT > 1.0 - ColorShiftRange) ? (1.0 - sunT) / ColorShiftRange : (sunT < ColorShiftRange) ? sunT / ColorShiftRange : 1.0);

		float SunColorTemp = 1000.0f + (float)(colorT * colorT) * 4780.0f;
		SunLightColor = calculateColorFromTemp(SunColorTemp);

		double RiseAndSetRange = 0.02;
		SunLightColor.a *= (float)((sunT > 1.0 - RiseAndSetRange) ? (1.0 - sunT) / RiseAndSetRange : (sunT < RiseAndSetRange) ? sunT / RiseAndSetRange : 1.0);
	}

	if (CurrentTime < MOONSET || CurrentTime > MOONRISE)
	{
		double total = 1.0 - MOONRISE + MOONSET;
		double moonT = (CurrentTime < MOONRISE ? CurrentTime + 1.0 - MOONRISE : CurrentTime - MOONRISE) / total;
		float phi = glm::pi<float>() / 2.0f - (float)moonT * glm::pi<float>();
		float theta = 0.0f;

		MoonDirection = glm::normalize(glm::vec3(-glm::sin(phi)*glm::cos(theta), glm::cos(phi), glm::sin(phi)*glm::sin(theta)));

		float MoonColorTemp = 7000.0f;
		MoonLightColor = calculateColorFromTemp(MoonColorTemp);

		double RiseAndSetRange = 0.05;
		MoonLightColor.a *= (float)((moonT > 1.0 - RiseAndSetRange) ? (1.0 - moonT) / RiseAndSetRange : (moonT < RiseAndSetRange) ? moonT / RiseAndSetRange : 1.0);
	}

	RenderSceneToShadowmap();

	Entity::Update();
}

void DayNightCycle::DrawLights(Core::Renderers::LightRenderer* renderer)
{
	if (CurrentTime > SUNRISE && CurrentTime < SUNSET)
	{
		renderer->DrawLight(*SunBuffer->Depth, SunDirection, SunLightColor, SunLightIntensity * SunLightColor.a, SunLightProjection, SunLightView, SunMaxDepth);
	}	
	if (CurrentTime < MOONSET || CurrentTime > MOONRISE)
	{
		//renderer->DrawLight(*MoonBuffer->Depth, MoonDirection, MoonLightColor, MoonLightIntensity * MoonLightColor.a, MoonLightProjection, MoonLightView, MoonMaxDepth);
	}

	Entity::DrawLights(renderer);
}

float DayNightCycle::GetCurrentTime() { return CurrentTime; }


glm::vec4 DayNightCycle::calculateColorFromTemp(float k)
{
	glm::vec4 color(1.0f);

	k /= 100.0f;
	if (k <= 66.0f)
	{
		color.r = 1.0f;
		color.g = glm::clamp((99.4708025861f * glm::log(k) - 161.1195681661f) / 255.0f, 0.0f, 1.0f);
		if (k <= 19.0f)
			color.b = 0.0f;
		else
			color.b = glm::clamp((138.5177312231f * glm::log(k - 10.0f) - 305.0447927307f) / 255.0f, 0.0f, 1.0f);
	}
	else
	{
		color.r = glm::clamp(329.698727446f * glm::pow(k - 60.0f, -0.1332047592f) / 255.0f, 0.0f, 1.0f);
		color.g = glm::clamp(288.1221695283f * glm::pow(k - 60.0f, -0.0755148492f) / 255.0f, 0.0f, 1.0f);
		color.b = 1.0f;
	}

	return color;
}

void DayNightCycle::RenderSceneToShadowmap()
{
	if (CurrentTime > SUNRISE && CurrentTime < SUNSET)
	{
		// Compute the MVP matrix from the light's point of view
		std::vector<glm::vec3> cc;
		camera->GetWSFrustumCorners(cc);

		auto gl = camera->GetGridLocation();
		SunLightView = glm::lookAt(SunDirection * 50.0f + gl, gl, glm::vec3(0, 1, 0));
		float minX = FLT_MAX;
		float minY = FLT_MAX;
		float minZ = FLT_MAX;
		float maxX = -FLT_MAX;
		float maxY = -FLT_MAX;
		float maxZ = -FLT_MAX;
		for (int i = 0; i < 8; i++)
		{
			auto point = SunLightView * glm::vec4(cc[i], 1.0);
			cc[i] = glm::vec3(point);
			minX = glm::min(point.x, minX);
			minY = glm::min(point.y, minY);
			minZ = glm::min(point.z, minZ);
			maxX = glm::max(point.x, maxX);
			maxY = glm::max(point.y, maxY);
			maxZ = glm::max(point.z, maxZ);
		}
		
		SunMaxDepth = glm::abs(minZ);
		SunLightProjection = glm::ortho(minX, maxX, minY, maxY, 0.0f, -minZ);
		Renderer->SetMatrices(SunLightProjection, SunLightView, SunMaxDepth);
		 
		// Render Scene to ShadowMap
		SunBuffer->SetAsTarget();
		SunBuffer->Clear();
		Renderer->DrawScene();
	}

	/*
	if (CurrentTime < MOONSET || CurrentTime > MOONRISE)
	{
		// Compute the MVP matrix from the light's point of view
		std::vector<glm::vec3> cc;
		camera->GetWSFrustumCorners(cc);

		auto gl = camera->GetGridLocation();
		MoonLightView = glm::lookAt(MoonDirection * 50.0f + gl, gl, glm::vec3(0, 1, 0));
		float minX = FLT_MAX;
		float minY = FLT_MAX;
		float minZ = FLT_MAX;
		float maxX = -FLT_MAX;
		float maxY = -FLT_MAX;
		float maxZ = -FLT_MAX;
		for (int i = 0; i < 8; i++)
		{
			auto point = MoonLightView * glm::vec4(cc[i], 1.0);
			cc[i] = glm::vec3(point);
			minX = glm::min(point.x, minX);
			minY = glm::min(point.y, minY);
			minZ = glm::min(point.z, minZ);
			maxX = glm::max(point.x, maxX);
			maxY = glm::max(point.y, maxY);
			maxZ = glm::max(point.z, maxZ);
		}

		MoonMaxDepth = glm::abs(minZ);
		MoonLightProjection = glm::ortho(minX, maxX, minY, maxY, 0.0f, -minZ);
		Renderer->SetMatrices(MoonLightProjection, MoonLightView, MoonMaxDepth);

		// Render Scene to ShadowMap
		MoonBuffer->SetAsTarget();
		MoonBuffer->Clear();
		Renderer->DrawScene();
	}*/
}
