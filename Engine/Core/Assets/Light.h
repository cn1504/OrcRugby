#pragma once

#include <Core.h>

namespace Core
{
	namespace Assets
	{
		class Light
		{
		private:
			glm::vec3 Color;
			float Radius;
			float Intensity;
			float CosInner;
			float CosOuter;
			bool castsShadow;

		public:
			Light(const glm::vec3& color, float radius, float intensity, float cosInner, float cosOuter, bool castsShadow)
				: Color(color), Radius(radius), Intensity(intensity), CosInner(cosInner), CosOuter(cosOuter), castsShadow(castsShadow)
			{}

			~Light() {}

			const glm::vec3& GetColor() const { return Color; }
			float GetRadius() const { return Radius; }
			float GetIntensity() const { return Intensity; }
			float GetCosInner() const { return CosInner; }
			float GetCosOuter() const { return CosOuter; }
			bool CastsShadow() const { return castsShadow; }
		};
	}
}