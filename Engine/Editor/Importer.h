#pragma once

#include "Core.h"

// Asset Loading Libraries
#pragma comment(lib, "assimp.lib")
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace AssetImporter
{
	void LoadFile(const char* filename);
	void LoadNode(const aiScene* scene, aiNode* node);
	void LoadMeshes(const aiScene* scene, aiNode* node);
	void LoadMesh(std::string name, aiMesh* mMesh);
	void LoadAnimations(const aiScene* scene);
}
