#pragma once
#include "Core.h"
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"
#include "Font.h"
#include "AudioFile.h"

namespace Core
{
	namespace Assets
	{
		extern std::unordered_map<std::string, Mesh*> Meshes;
		extern std::unordered_map<std::string, Material*> Materials;
		extern std::unordered_map<std::string, Texture*> Textures;
		extern std::unordered_map<std::string, Font*> Fonts;
		extern std::unordered_map<std::string, AudioFile*> AudioFiles;

		void LoadStandardAssets();
		Mesh* LoadMesh(std::string name);
		AudioFile* LoadAudioFile(std::string name, Core::AudioFile::Type type = Core::AudioFile::Type::SOUND_EFFECT);

		void CreateStandardMaterials();
		void CreateStandardTextures();
		void CreateStandardMeshes();
		void CreateStandardFonts();
		void Clear();

		void LoadMaterials();
		void SaveMaterials();

		void FromJSON(std::string& input, std::unordered_map<std::string, std::string>& output);
		std::string ToJSON(std::unordered_map<std::string, std::string>& input);
		void IndentEachLine(std::string& input);
	}
}