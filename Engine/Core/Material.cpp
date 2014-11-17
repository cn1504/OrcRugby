#include "Material.h"
#include "Assets.h"

namespace Core
{

	Material::Material()
	{
		DiffuseColor = glm::vec3(1.0f);
		DiffuseIntensity = 1.0f;
		SpecularColor = glm::vec3(1.0f);
		SpecularIntensity = 1.0f;
		SpecularHardness = 5.0f;
		EmanativeColor = glm::vec3(1.0f);
		EmanativeIntensity = 0.0f;

		Noise = NoiseType::None;
		NoiseScale = 1.0f;
		NoiseOffset = glm::vec2(0.0f);
		Mat1Cieling = 0.0f;
		Mat2Floor = 1.0f;

		DiffuseColor2 = glm::vec3(1.0f);
		DiffuseIntensity2 = 1.0f;
		SpecularColor2 = glm::vec3(1.0f);
		SpecularIntensity2 = 1.0f;
		SpecularHardness2 = 5.0f;
		EmanativeColor2 = glm::vec3(1.0f);
		EmanativeIntensity2 = 0.0f;

		IsTranslucent = false;
		DiffuseAlpha1 = 1.0f;
		DiffuseAlpha2 = 1.0f;
		SpecularAlpha1 = 1.0f;
		SpecularAlpha2 = 1.0f;
		EmanativeAlpha1 = 1.0f;
		EmanativeAlpha2 = 1.0f;

		IndexOfRefraction = 1.333f;		
		Density = 1.0e3f;

		YoungsModulus = 6.0e10f;
		BulkModulus = 5.0e10f;
		ShearModulus = 2.5e10f;

		YieldStrength = 33.0e6f;
		UltimateStrength = 33.0e6f;
	}


	Material::Material(const Material& o)
	{
		Name = o.Name;

		DiffuseColor = o.DiffuseColor;
		DiffuseIntensity = o.DiffuseIntensity;
		SpecularColor = o.SpecularColor;
		SpecularIntensity = o.SpecularIntensity;
		SpecularHardness = o.SpecularHardness;
		EmanativeColor = o.EmanativeColor;
		EmanativeIntensity = o.EmanativeIntensity;

		Noise = o.Noise;
		NoiseScale = o.NoiseScale;
		NoiseOffset = o.NoiseOffset;

		DiffuseColor2 = o.DiffuseColor2;
		DiffuseIntensity2 = o.DiffuseIntensity2;
		SpecularColor2 = o.SpecularColor2;
		SpecularIntensity2 = o.SpecularIntensity2;
		SpecularHardness2 = o.SpecularHardness2;
		EmanativeColor2 = o.EmanativeColor2;
		EmanativeIntensity2 = o.EmanativeIntensity2;

		IsTranslucent = o.IsTranslucent;
		DiffuseAlpha1 = o.DiffuseAlpha1;
		DiffuseAlpha2 = o.DiffuseAlpha2;
		SpecularAlpha1 = o.SpecularAlpha1;
		SpecularAlpha2 = o.SpecularAlpha2;
		EmanativeAlpha1 = o.EmanativeAlpha1;
		EmanativeAlpha2 = o.EmanativeAlpha2;

		IndexOfRefraction = o.IndexOfRefraction;
		Density = o.Density;

		YoungsModulus = o.YoungsModulus;
		BulkModulus = o.BulkModulus;
		ShearModulus = o.ShearModulus;

		YieldStrength = o.YieldStrength;
		UltimateStrength = o.UltimateStrength;
	}


	Material::~Material()
	{
	}


	void Material::Load()
	{
	}


	void Material::Update()
	{
	}
	

	void Material::LoadFile(std::istream& file)
	{
		auto size = Name.size();
		file.read((char*)&size, sizeof(size));
		for (unsigned long long i = 0; i < size; i++)
		{
			char ch;
			file.read((char*)&ch, sizeof(ch));
			Name.push_back(ch);
		}

		file.read((char*)&DiffuseColor, sizeof(DiffuseColor));
		file.read((char*)&DiffuseIntensity, sizeof(DiffuseIntensity));
		file.read((char*)&SpecularColor, sizeof(SpecularColor));
		file.read((char*)&SpecularIntensity, sizeof(SpecularIntensity));
		file.read((char*)&SpecularHardness, sizeof(SpecularHardness));
		file.read((char*)&EmanativeColor, sizeof(EmanativeColor));
		file.read((char*)&EmanativeIntensity, sizeof(EmanativeIntensity));

		file.read((char*)&Noise, sizeof(Noise));
		file.read((char*)&NoiseScale, sizeof(NoiseScale));
		file.read((char*)&NoiseOffset, sizeof(NoiseOffset));
		file.read((char*)&Mat1Cieling, sizeof(Mat1Cieling));
		file.read((char*)&Mat2Floor, sizeof(Mat2Floor));

		file.read((char*)&DiffuseColor2, sizeof(DiffuseColor));
		file.read((char*)&DiffuseIntensity2, sizeof(DiffuseIntensity));
		file.read((char*)&SpecularColor2, sizeof(SpecularColor));
		file.read((char*)&SpecularIntensity2, sizeof(SpecularIntensity));
		file.read((char*)&SpecularHardness2, sizeof(SpecularHardness));
		file.read((char*)&EmanativeColor2, sizeof(EmanativeColor));
		file.read((char*)&EmanativeIntensity2, sizeof(EmanativeIntensity));

		file.read((char*)&IsTranslucent, sizeof(IsTranslucent));
		file.read((char*)&DiffuseAlpha1, sizeof(DiffuseAlpha1));
		file.read((char*)&DiffuseAlpha2, sizeof(DiffuseAlpha2));
		file.read((char*)&SpecularAlpha1, sizeof(SpecularAlpha1));
		file.read((char*)&SpecularAlpha2, sizeof(SpecularAlpha2));
		file.read((char*)&EmanativeAlpha1, sizeof(EmanativeAlpha1));
		file.read((char*)&EmanativeAlpha2, sizeof(EmanativeAlpha2));

		file.read((char*)&IndexOfRefraction, sizeof(IndexOfRefraction));

		file.read((char*)&Density, sizeof(Density));

		file.read((char*)&YoungsModulus, sizeof(YoungsModulus));
		file.read((char*)&BulkModulus, sizeof(BulkModulus));
		file.read((char*)&ShearModulus, sizeof(ShearModulus));

		file.read((char*)&YieldStrength, sizeof(YieldStrength));
		file.read((char*)&UltimateStrength, sizeof(UltimateStrength));

		Assets::Materials[Name] = this;
	}


	void Material::SaveFile(std::ostream& file)
	{
		auto size = Name.size();
		file.write((char*)&size, sizeof(size));
		file.write((char*)&Name[0], sizeof(Name[0]) * size);

		file.write((char*)&DiffuseColor, sizeof(DiffuseColor));
		file.write((char*)&DiffuseIntensity, sizeof(DiffuseIntensity));
		file.write((char*)&SpecularColor, sizeof(SpecularColor));
		file.write((char*)&SpecularIntensity, sizeof(SpecularIntensity));
		file.write((char*)&SpecularHardness, sizeof(SpecularHardness));
		file.write((char*)&EmanativeColor, sizeof(EmanativeColor));
		file.write((char*)&EmanativeIntensity, sizeof(EmanativeIntensity));

		file.write((char*)&Noise, sizeof(Noise));
		file.write((char*)&NoiseScale, sizeof(NoiseScale));
		file.write((char*)&NoiseOffset, sizeof(NoiseOffset));
		file.write((char*)&Mat1Cieling, sizeof(Mat1Cieling));
		file.write((char*)&Mat2Floor, sizeof(Mat2Floor));

		file.write((char*)&DiffuseColor2, sizeof(DiffuseColor));
		file.write((char*)&DiffuseIntensity2, sizeof(DiffuseIntensity));
		file.write((char*)&SpecularColor2, sizeof(SpecularColor));
		file.write((char*)&SpecularIntensity2, sizeof(SpecularIntensity));
		file.write((char*)&SpecularHardness2, sizeof(SpecularHardness));
		file.write((char*)&EmanativeColor2, sizeof(EmanativeColor));
		file.write((char*)&EmanativeIntensity2, sizeof(EmanativeIntensity));

		file.write((char*)&IsTranslucent, sizeof(IsTranslucent));
		file.write((char*)&DiffuseAlpha1, sizeof(DiffuseAlpha1));
		file.write((char*)&DiffuseAlpha2, sizeof(DiffuseAlpha2));
		file.write((char*)&SpecularAlpha1, sizeof(SpecularAlpha1));
		file.write((char*)&SpecularAlpha2, sizeof(SpecularAlpha2));
		file.write((char*)&EmanativeAlpha1, sizeof(EmanativeAlpha1));
		file.write((char*)&EmanativeAlpha2, sizeof(EmanativeAlpha2));

		file.write((char*)&IndexOfRefraction, sizeof(IndexOfRefraction));

		file.write((char*)&Density, sizeof(Density));

		file.write((char*)&YoungsModulus, sizeof(YoungsModulus));
		file.write((char*)&BulkModulus, sizeof(BulkModulus));
		file.write((char*)&ShearModulus, sizeof(ShearModulus));

		file.write((char*)&YieldStrength, sizeof(YieldStrength));
		file.write((char*)&UltimateStrength, sizeof(UltimateStrength));
	}
	

	void Material::WriteShaderUniforms(Shader* shader)
	{
		if (IsTranslucent)
		{
			glUniform4fv(shader->GetUL("DiffuseColor"), 1, glm::value_ptr(glm::vec4(DiffuseColor * DiffuseIntensity, DiffuseAlpha1)));
			glUniform4fv(shader->GetUL("SpecularColor"), 1, glm::value_ptr(glm::vec4(SpecularColor * SpecularIntensity, SpecularAlpha1)));
			glUniform1f(shader->GetUL("SpecularHardness"), SpecularHardness);
			glUniform4fv(shader->GetUL("EmanativeColor"), 1, glm::value_ptr(glm::vec4(EmanativeColor * EmanativeIntensity, EmanativeAlpha1)));

			glUniform1i(shader->GetUL("NoiseType"), (GLuint)Noise);
			glUniform1f(shader->GetUL("NoiseScale"), NoiseScale);
			glUniform2fv(shader->GetUL("NoiseOffset"), 1, glm::value_ptr(NoiseOffset));
			glUniform1f(shader->GetUL("Mat1Cieling"), Mat1Cieling);
			glUniform1f(shader->GetUL("Mat2Floor"), Mat2Floor);

			glUniform4fv(shader->GetUL("DiffuseColor2"), 1, glm::value_ptr(glm::vec4(DiffuseColor2 * DiffuseIntensity2, DiffuseAlpha2)));
			glUniform4fv(shader->GetUL("SpecularColor2"), 1, glm::value_ptr(glm::vec4(SpecularColor2 * SpecularIntensity2, SpecularAlpha2)));
			glUniform1f(shader->GetUL("SpecularHardness2"), SpecularHardness2);
			glUniform4fv(shader->GetUL("EmanativeColor2"), 1, glm::value_ptr(glm::vec4(EmanativeColor2 * EmanativeIntensity2, EmanativeAlpha2)));
		}
		else
		{
			glUniform4fv(shader->GetUL("DiffuseColor"), 1, glm::value_ptr(glm::vec4(DiffuseColor * DiffuseIntensity, 1.0f)));
			glUniform4fv(shader->GetUL("SpecularColor"), 1, glm::value_ptr(glm::vec4(SpecularColor * SpecularIntensity, 1.0f)));
			glUniform1f(shader->GetUL("SpecularHardness"), SpecularHardness);
			glUniform4fv(shader->GetUL("EmanativeColor"), 1, glm::value_ptr(glm::vec4(EmanativeColor * EmanativeIntensity, 1.0f)));

			glUniform1i(shader->GetUL("NoiseType"), (GLuint)Noise);
			glUniform1f(shader->GetUL("NoiseScale"), NoiseScale);
			glUniform2fv(shader->GetUL("NoiseOffset"), 1, glm::value_ptr(NoiseOffset));
			glUniform1f(shader->GetUL("Mat1Cieling"), Mat1Cieling);
			glUniform1f(shader->GetUL("Mat2Floor"), Mat2Floor);

			glUniform4fv(shader->GetUL("DiffuseColor2"), 1, glm::value_ptr(glm::vec4(DiffuseColor2 * DiffuseIntensity2, 1.0f)));
			glUniform4fv(shader->GetUL("SpecularColor2"), 1, glm::value_ptr(glm::vec4(SpecularColor2 * SpecularIntensity2, 1.0f)));
			glUniform1f(shader->GetUL("SpecularHardness2"), SpecularHardness2);
			glUniform4fv(shader->GetUL("EmanativeColor2"), 1, glm::value_ptr(glm::vec4(EmanativeColor2 * EmanativeIntensity2, 1.0f)));
		}
	}

}