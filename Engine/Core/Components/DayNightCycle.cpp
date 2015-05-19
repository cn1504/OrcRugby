#include "DayNightCycle.h"
#include <Assets/AssetDB.h>
#include <Renderers/LightRenderer.h>

using namespace Core::Components;

const static float SUNRISE = 5.47f / 24.0f;		// % of Day
const static float SUNSET = 19.35f / 24.0f;		// % of Day
const static float MAX_SUN_ALTITUDE = 65.0f;	// Degrees from S horizon
const static float SUNRISE_HEADING = 70.0f;	    // Degrees from E from N
const static float SUNSET_HEADING = 290.0f;	    // Degrees from E from N

const static float MOONRISE = 17.25f / 24.0f;	// % of Day
const static float MOONSET = 4.45f / 24.0f;		// % of Day
const static float MAX_MOON_ALTITUDE = 46.0f;	// Degrees from S horizon
const static float MOONRISE_HEADING = 91.0f;	// Degrees from E from N
const static float MOONSET_HEADING = 269.0f;	// Degrees from E from N

DayNightCycle::DayNightCycle(float timeRatio)
{
	ShadowBuffer = std::make_unique<Core::Renderers::ShadowBuffer>(glm::ivec2(1024, 1024));
	Renderer = std::make_unique<Core::Renderers::ShadowMapRenderer>();

	TimeRatio = timeRatio;
	CurrentTime = 8.0f / 24.0f;
	
	SunLightColor = glm::vec4(1, 1, 1, 1);
	MoonLightColor = glm::vec4(1, 1, 1, 1);

	SunLightIntensity = 3.0f;
	MoonLightIntensity = 0.25f;
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
		float sunT = (CurrentTime - SUNRISE) / (SUNSET - SUNRISE);
		float phi = glm::pi<float>() / 2.0f - sunT * glm::pi<float>();
		float theta = 0.0f; 

		SunDirection = glm::normalize(glm::vec3(glm::sin(phi)*glm::cos(theta), cos(phi), glm::sin(phi)*glm::sin(theta)));


		float SunColorTemp = 2000.0f + 15120.0f * sunT - 15120.0f * sunT * sunT;
		SunLightColor = calculateColorFromTemp(SunColorTemp);

		auto t = (CurrentTime - SUNRISE) / 0.21f;
		auto t2 = (SUNSET - CurrentTime) / 0.21f;
		SunLightColor.a *= (t > 0.0f && t < 1.0f) ? t * t : (t2 > 0.0f && t2 < 1.0f) ? t2 * t2 : 1.0f;
	}

	if (CurrentTime < MOONSET || CurrentTime > MOONRISE)
	{
		float total = 1.0f - MOONRISE + MOONSET;	
		float moonT = (CurrentTime < MOONRISE ? CurrentTime + 1.0f - MOONRISE : CurrentTime - MOONRISE) / total;
		float phi = glm::pi<float>() / 2.0f - moonT * glm::pi<float>();
		float theta = 0.0f;

		MoonDirection = glm::normalize(glm::vec3(glm::sin(phi)*glm::cos(theta), cos(phi), glm::sin(phi)*glm::sin(theta)));

		float MoonColorTemp = 7000.0f;
		MoonLightColor = calculateColorFromTemp(MoonColorTemp);
		MoonLightColor.a = 1.0f;

		auto t = (CurrentTime - MOONRISE) / 0.21f;
		auto t2 = (MOONSET - CurrentTime) / 0.42f;
		SunLightColor.a *= (t > 0.0f && t < 1.0f) ? t * t : (t2 > 0.0f && t2 < 1.0f) ? t2 * t2 : 1.0f;
	}
	//glm::vec3 MoonDirection;
	//glm::vec4 MoonLightColor;
	//glm::vec4 SkyColor;

	//ShadowBuffer->SetAsTarget();
	//ShadowBuffer->Clear();
	//Renderer->DrawScene();

	Entity::Update();
}

void DayNightCycle::DrawLights(Core::Renderers::LightRenderer* renderer)
{
	if (CurrentTime > SUNRISE && CurrentTime < SUNSET)
	{
		renderer->DrawLight(*ShadowBuffer->Depth, SunDirection, SunLightColor, SunLightIntensity);
	}	
	if (CurrentTime < MOONSET || CurrentTime > MOONRISE)
	{
		renderer->DrawLight(*ShadowBuffer->Depth, MoonDirection, MoonLightColor, MoonLightIntensity);
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