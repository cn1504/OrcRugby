#pragma once
#include "Core.h"
#include "Component.h"
#include "Shader.h"

namespace Core
{
	
	class Material
		: public Component
	{
	public:
		enum class NoiseType { None, Value2D, Perlin2D, SimplexPerlin2D, Cellular2D, TurbulentSine };

		Material();
		Material(const Material& o);
		virtual ~Material();
		virtual void Load();
		virtual void Update();
		
		void WriteShaderUniforms(Shader* shader);

		json::Value Serialize();
		void Deserialize(json::Object& obj);
		
		std::string Name;

		glm::vec3 BaseColor;
		float Metallic; 
		float Specular;
		float Roughness;
		float Emissive;

		bool IsTranslucent;
		float Opacity;

		float Density;

		/*
		glm::vec3 DiffuseColor;
		float DiffuseIntensity;
		glm::vec3 SpecularColor;
		float SpecularIntensity;
		float SpecularHardness;
		glm::vec3 EmanativeColor;
		float EmanativeIntensity;

		NoiseType Noise;
		float NoiseScale;
		glm::vec2 NoiseOffset;
		float Mat1Cieling;
		float Mat2Floor;

		glm::vec3 DiffuseColor2;
		float DiffuseIntensity2;
		glm::vec3 SpecularColor2;
		float SpecularIntensity2;
		float SpecularHardness2;
		glm::vec3 EmanativeColor2;
		float EmanativeIntensity2;
		
		bool IsTranslucent;
		float DiffuseAlpha1;
		float DiffuseAlpha2;
		float SpecularAlpha1;
		float SpecularAlpha2;
		float EmanativeAlpha1;
		float EmanativeAlpha2;

		float IndexOfRefraction;
		
		float YoungsModulus;
		float BulkModulus;
		float ShearModulus;

		float YieldStrength;
		float UltimateStrength;
		*/
	};

}