#pragma once

#include "Material.h"
#include "AssetDB.h"
#include <Renderers/Shader.h>

using namespace Core::Assets;

/*
		struct MaterialValue
		{
			glm::vec4 Base;
			glm::vec4 MSR;
		};
		struct MaterialTexture
		{
			std::string Base;
			std::string MSR;
		};

class Material
{
private:
			static const int MAX_INPUTS = 8;
			static const int MAX_COMMANDS = 16;

			MaterialValue InputValues[MAX_INPUTS];
			MaterialTexture InputTextures[MAX_INPUTS];
			MaterialCommand Commands[MAX_COMMANDS];
			int InputSources[MAX_INPUTS];
			int InputCount;
			int CommandCount;

*/

Material::Material() { InputCount = 0; CommandCount = 0; }
Material::~Material() {}

void Material::AddInput(const MaterialValue& in)
{
	InputValues[InputCount] = in;
	InputSources[InputCount] = 0;
	InputCount++;
}
void Material::AddInput(const MaterialTexture& in)
{
	InputTextures[InputCount] = in;
	InputSources[InputCount] = 1;
	InputCount++;
}
void Material::AddCommand(const MaterialCommand& com)
{
	Commands[CommandCount] = com;
	CommandCount++;
}

void Material::SetShaderUniforms(Core::Renderers::Shader* shader) const
{
	shader->SetUniform("InputCount", InputCount);
	shader->SetUniform("CommandCount", CommandCount);

	for (int i = 0; i < InputCount; i++)
	{
		shader->SetUniform("InputSource[" + std::to_string(i) + "]", InputSources[i]);
		
		if (InputSources[i] == 0)
		{
			shader->SetUniform("InputValues[" + std::to_string(i) + "].Base", InputValues[i].Base);
			shader->SetUniform("InputValues[" + std::to_string(i) + "].MSR", InputValues[i].MSR);			
		}
		else
		{
			shader->SetTexture("InputTextures[" + std::to_string(i) + "].Base", *Core::AssetDB->GetTexture(InputTextures[i].Base), i * 2);
			shader->SetTexture("InputTextures[" + std::to_string(i) + "].MSR", *Core::AssetDB->GetTexture(InputTextures[i].MSR), i * 2 + 1);
		}
	}

	for (int i = 0; i < CommandCount; i++)
	{
		shader->SetUniform("Commands[" + std::to_string(i) + "].Op", Commands[i].Op);
		for (int j = 0; j < MaterialCommand::MAX_PARAMS; j++)
		{
			shader->SetUniform("Commands[" + std::to_string(i) + "].Params[" + std::to_string(j) + "]", Commands[i].Params[j]);
		}
	}
}