#include "Material.h"
#include "Assets.h"

namespace Core
{

	Material::Material()
	{
		BaseColor = glm::vec3(1.0f, 0.883f, 0.0f);
		Metallic = 1.0f;
		Specular = 0.5f;
		Roughness = 0.0f;
		Emissive = 0.0f;

		IsTranslucent = false;
		Opacity = 1.0f;

		Density = 19333.0f;
	}


	Material::Material(const Material& o)
	{
		Name = o.Name;

		BaseColor = o.BaseColor;
		Metallic = o.Metallic;
		Specular = o.Specular;
		Roughness = o.Roughness;
		Emissive = o.Emissive;

		IsTranslucent = o.IsTranslucent;
		Opacity = o.Opacity;

		Density = o.Density;
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

		file.read((char*)&BaseColor, sizeof(BaseColor));
		file.read((char*)&Metallic, sizeof(Metallic));
		file.read((char*)&Specular, sizeof(Specular));
		file.read((char*)&Roughness, sizeof(Roughness));
		file.read((char*)&Emissive, sizeof(Emissive));

		file.read((char*)&IsTranslucent, sizeof(IsTranslucent));
		file.read((char*)&Opacity, sizeof(Opacity));

		file.read((char*)&Density, sizeof(Density));

		Assets::Materials[Name] = this;
	}


	void Material::SaveFile(std::ostream& file)
	{
		auto size = Name.size();
		file.write((char*)&size, sizeof(size));
		file.write((char*)&Name[0], sizeof(Name[0]) * size);

		file.write((char*)&BaseColor, sizeof(BaseColor));
		file.write((char*)&Metallic, sizeof(Metallic));
		file.write((char*)&Specular, sizeof(Specular));
		file.write((char*)&Roughness, sizeof(Roughness));
		file.write((char*)&Emissive, sizeof(Emissive));

		file.write((char*)&IsTranslucent, sizeof(IsTranslucent));
		file.write((char*)&Opacity, sizeof(Opacity));

		file.write((char*)&Density, sizeof(Density));
	}
	

	void Material::WriteShaderUniforms(Shader* shader)
	{		
		glUniform4fv(shader->GetUL("BaseColor"), 1, glm::value_ptr(glm::vec4(BaseColor, 1.0f)));
		glUniform1f(shader->GetUL("Metallic"), Metallic);
		glUniform1f(shader->GetUL("Specular"), Specular);
		glUniform1f(shader->GetUL("Roughness"), Roughness);
		glUniform1f(shader->GetUL("Emissive"), Emissive);
	}

}