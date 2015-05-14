#include "Material.h"
#include "Assets.h"
#include <Renderers/Shader.h>

using namespace Core::Assets;
typedef Core::Renderers::Shader Shader;

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


json::Value Material::Serialize()
{
	json::Object obj;
	obj["DataType"] = "Material";
	obj["Name"] = Name;

	obj["BaseColor"] = std::to_string(BaseColor);
	obj["Metallic"] = Metallic;
	obj["Specular"] = Specular;
	obj["Roughness"] = Roughness;
	obj["Emissive"] = Emissive;

	obj["IsTranslucent"] = IsTranslucent;
	obj["Opacity"] = Opacity;

	obj["Density"] = Density;
	return obj;
}


void Material::Deserialize(json::Object& obj)
{
	Name = obj["Name"];

	BaseColor = std::stovec3(obj["BaseColor"]);
	Metallic = obj["Metallic"];
	Specular = obj["Specular"];
	Roughness = obj["Roughness"];
	Emissive = obj["Emissive"];

	IsTranslucent = obj["IsTranslucent"];
	Opacity = obj["Opacity"];

	Density = obj["Density"];
}	
	

void Material::WriteShaderUniforms(Shader* shader)
{	
	shader->SetUniform("BaseColor", glm::vec4(BaseColor, 1.0f));
	shader->SetUniform("Metallic", Metallic);
	shader->SetUniform("Specular", Specular);
	shader->SetUniform("Roughness", Roughness);
	shader->SetUniform("Emissive", Emissive);
}
