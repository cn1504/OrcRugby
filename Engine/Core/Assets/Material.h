#pragma once

#include <Core.h>
#include "Texture.h"

namespace Core
{
	namespace Assets
	{
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
		struct MaterialCommand
		{
			static const int MAX_PARAMS = 3;

			glm::ivec4 Op;
			glm::vec4 Params[MAX_PARAMS];
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

		public:
			Material();
			~Material();
			
			void AddInput(const MaterialValue& in);
			void AddInput(const MaterialTexture& in);
			void AddCommand(const MaterialCommand& com);

			void SetShaderUniforms(Core::Renderers::Shader* shader) const;
		};
	}
}