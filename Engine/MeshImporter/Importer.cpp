#include "Importer.h"
#include <Assets/Database.h>

typedef unsigned int uint;

namespace MeshImporter
{
	std::unordered_map<std::string, uint> BoneNames;

	void LoadFile(const char* filename)
	{
		out->Log("Importing file: " + std::string(filename));

		// Create an instance of the Importer class
		Assimp::Importer importer;

		// And have it read the given file with some example postprocessing
		// Usually - if speed is not the most important aspect for you - you'll
		// propably to request more postprocessing than we do in this example.
		const aiScene* scene = importer.ReadFile(filename,
			aiProcess_Triangulate |
			aiProcess_CalcTangentSpace |
			aiProcess_JoinIdenticalVertices |
			aiProcess_GenSmoothNormals | // aiProcess_GenNormals |
			aiProcess_LimitBoneWeights |
			aiProcess_OptimizeMeshes |
			aiProcess_SortByPType |
			aiProcess_FlipUVs);

		// If the import failed, report it
		if (!scene)
		{
			out->Log(importer.GetErrorString());
			return;
		}

		out->Log("Meshes in file: " + std::to_string(scene->mNumMeshes));

		LoadMeshes(scene, scene->mRootNode);
		/*
		// Now we can access the file's contents.
		if (scene->HasAnimations())
		{
			std::fstream meshFile;
			meshFile.open("Resources/Meshes/" + std::string(scene->mRootNode->mName.C_Str()) + ".skeleton", std::ios::out | std::ios::binary);
			LoadNode(scene, scene->mRootNode);
		}

		LoadMeshes(scene, scene->mRootNode);

		if (scene->HasAnimations())
			LoadAnimations(scene);
		*/
		// We're done. Everything will be cleaned up by the importer destructor
	}

	void LoadNode(const aiScene* scene, aiNode* node)
	{
		std::string name = std::string(node->mName.C_Str());
		
		if (name.length() > 4 && name[0] == 'C' && name[1] == 'T' && name[2] == 'R' && name[3] == 'L')
		{
			// Skeletal Animation Bone Found
			std::string boneName = &name[5];	// Ignore "CTRL_"
			BoneNames[boneName] = (uint)BoneNames.size();

			out->Log("Bone node " + std::to_string(BoneNames[boneName]) + ": " + boneName);
			
			// Extract transformation matrix
			aiVector3D position;
			aiVector3D scale;
			aiQuaternion rotation;
			node->mTransformation.Decompose(scale, rotation, position);
			float transform[10] = { position.x, position.y, position.z, 
									rotation.w, rotation.x, rotation.y, rotation.z,
									scale.x, scale.y, scale.z };

			// Export skeleton file
			std::fstream meshFile;
			meshFile.open("Resources/Meshes/" + std::string(scene->mRootNode->mName.C_Str()) + ".skeleton", std::ios::out | std::ios::binary | std::ios::app);
			
			meshFile.write((char*)&BoneNames[boneName], sizeof(uint));
			meshFile.write((char*)&transform[0], 10 * sizeof(float));
			meshFile.write((char*)&node->mNumChildren, sizeof(node->mNumChildren));

			meshFile.close();
		}

		// Add children
		for (uint i = 0; i < node->mNumChildren; i++)
		{
			LoadNode(scene, node->mChildren[i]);
		}
	}

	void LoadMeshes(const aiScene* scene, aiNode* node)
	{
		std::string name = node->mName.C_Str();

		for (uint i = 0; i < node->mNumMeshes; i++)
			LoadMesh(name, scene->mMeshes[node->mMeshes[i]]);
		
		for (uint i = 0; i < node->mNumChildren; i++)
			LoadMeshes(scene, node->mChildren[i]);
	}

	void LoadMesh(std::string name, aiMesh* mMesh)
	{
		out->Log("Loading mesh: " + name);

		if (mMesh->GetNumUVChannels() <= 0)
		{
			out->Log("Mesh missing UV channels");
		}

		// Extract the mesh data into buffers
		std::vector<unsigned short> indices;
		std::vector<glm::vec3> indexed_vertices;
		std::vector<glm::vec2> indexed_uvs;
		std::vector<glm::vec3> indexed_normals;
		std::vector<glm::ivec4> indexed_boneIndices;
		std::vector<glm::vec4> indexed_boneWeights;

		auto vertexCount = mMesh->mNumVertices;
		auto faceCount = mMesh->mNumFaces;
		
		// Indices
		for (uint i = 0; i < faceCount; i++)
		{
			auto indexCount = mMesh->mFaces[i].mNumIndices;
			for (uint j = 0; j < indexCount; j++)
			{
				indices.push_back(mMesh->mFaces[i].mIndices[j]);
			}
		}

		// Vertices
		for (uint i = 0; i < vertexCount; i++)
		{
			indexed_vertices.push_back(glm::vec3(mMesh->mVertices[i].x, mMesh->mVertices[i].y, mMesh->mVertices[i].z));
		}

		// UV Coords
		for (uint i = 0; i < vertexCount; i++)
		{
			indexed_uvs.push_back(glm::vec2(mMesh->mTextureCoords[0][i].x, mMesh->mTextureCoords[0][i].y));
		}

		// Normals
		for (uint i = 0; i < vertexCount; i++)
		{
			indexed_normals.push_back(glm::vec3(mMesh->mNormals[i].x, mMesh->mNormals[i].y, mMesh->mNormals[i].z));
		}
		
		bool hasBones = mMesh->HasBones();
		if (hasBones)
		{
			indexed_boneIndices.resize(indexed_vertices.size(), glm::ivec4(-1));
			indexed_boneWeights.resize(indexed_vertices.size());
			
			for (uint i = 0; i < mMesh->mNumBones; i++)
			{				
				// Find bone index
				auto boneName = std::string(&mMesh->mBones[i]->mName.C_Str()[3]);	// Ignore "JT_"
				if (BoneNames.count(boneName) == 0)
					continue;	// Bone node not found
				unsigned int boneIndex = BoneNames[boneName];

				out->Log("\tBone " + std::to_string(i) + ": " + boneName + " " + std::to_string(boneIndex) + " - " + std::to_string(mMesh->mBones[i]->mNumWeights) + " / " + std::to_string(mMesh->mNumVertices));
				
				for (uint j = 0; j < mMesh->mBones[i]->mNumWeights; j++)
				{
					auto indices = indexed_boneIndices[mMesh->mBones[i]->mWeights[j].mVertexId];

					if (indices.w == -1) {
						indexed_boneIndices[mMesh->mBones[i]->mWeights[j].mVertexId].w = boneIndex;
						indexed_boneWeights[mMesh->mBones[i]->mWeights[j].mVertexId].w = mMesh->mBones[i]->mWeights[j].mWeight;
					}
					else if (indices.x == -1) {
						indexed_boneIndices[mMesh->mBones[i]->mWeights[j].mVertexId].x = boneIndex;
						indexed_boneWeights[mMesh->mBones[i]->mWeights[j].mVertexId].x = mMesh->mBones[i]->mWeights[j].mWeight;
					}
					else if (indices.y == -1) {
						indexed_boneIndices[mMesh->mBones[i]->mWeights[j].mVertexId].y = boneIndex;
						indexed_boneWeights[mMesh->mBones[i]->mWeights[j].mVertexId].y = mMesh->mBones[i]->mWeights[j].mWeight;
					}
					else {
						indexed_boneIndices[mMesh->mBones[i]->mWeights[j].mVertexId].z = boneIndex;
						indexed_boneWeights[mMesh->mBones[i]->mWeights[j].mVertexId].z = mMesh->mBones[i]->mWeights[j].mWeight;
					}
				}
			}
		}

		// Insert into database
		// Remove old entries
		Core::Database->SqlStatement("DELETE FROM VertexBuffers WHERE key='" + name + "_Indices" + "'");
		Core::Database->SqlStatement("DELETE FROM VertexBuffers WHERE key='" + name + "_Vertices" + "'");
		Core::Database->SqlStatement("DELETE FROM VertexBuffers WHERE key='" + name + "_Uvs" + "'");
		Core::Database->SqlStatement("DELETE FROM VertexBuffers WHERE key='" + name + "_Normals" + "'");
		Core::Database->SqlStatement("DELETE FROM VertexBuffers WHERE key='" + name + "_BoneIndices" + "'");
		Core::Database->SqlStatement("DELETE FROM VertexBuffers WHERE key='" + name + "_BoneWeights" + "'");

		// Indices
		Core::Database->Sql("INSERT INTO VertexBuffers(key, data) VALUES(?, ?)");
		Core::Database->Bind(1, name + "_Indices");
		Core::Database->Bind(2, Core::Assets::Blob((unsigned char*)&indices[0], sizeof(unsigned short) * indices.size()));
		Core::Database->ExecuteAndFree(); 
		
		// Vertices
		Core::Database->Sql("INSERT INTO VertexBuffers(key, data) VALUES(?, ?)");
		Core::Database->Bind(1, name + "_Vertices");
		Core::Database->Bind(2, Core::Assets::Blob((unsigned char*)&indexed_vertices[0], sizeof(glm::vec3) * indexed_vertices.size()));
		Core::Database->ExecuteAndFree();

		// UVS
		Core::Database->Sql("INSERT INTO VertexBuffers(key, data) VALUES(?, ?)");
		Core::Database->Bind(1, name + "_Uvs");
		Core::Database->Bind(2, Core::Assets::Blob((unsigned char*)&indexed_uvs[0], sizeof(glm::vec2) * indexed_uvs.size()));
		Core::Database->ExecuteAndFree();

		// Normals
		Core::Database->Sql("INSERT INTO VertexBuffers(key, data) VALUES(?, ?)");
		Core::Database->Bind(1, name + "_Normals");
		Core::Database->Bind(2, Core::Assets::Blob((unsigned char*)&indexed_normals[0], sizeof(glm::vec3) * indexed_normals.size()));
		Core::Database->ExecuteAndFree();

		if (hasBones)
		{
			// Bone Indices
			Core::Database->Sql("INSERT INTO VertexBuffers(key, data) VALUES(?, ?)");
			Core::Database->Bind(1, name + "_BoneIndices");
			Core::Database->Bind(2, Core::Assets::Blob((unsigned char*)&indexed_boneIndices[0], sizeof(glm::ivec4) * indexed_boneIndices.size()));
			Core::Database->ExecuteAndFree();

			// Bone Weights
			Core::Database->Sql("INSERT INTO VertexBuffers(key, data) VALUES(?, ?)");
			Core::Database->Bind(1, name + "_BoneWeights");
			Core::Database->Bind(2, Core::Assets::Blob((unsigned char*)&indexed_boneWeights[0], sizeof(glm::vec4) * indexed_boneWeights.size()));
			Core::Database->ExecuteAndFree();
		}
		/*
		// Export mesh file
		std::fstream meshFile;
		meshFile.open("Resources/Meshes/" + name + ".mesh", std::ios::out | std::ios::binary);

		auto size = indices.size();
		meshFile.write((char*)&size, sizeof(size));
		for (auto v : indices)
			meshFile.write((char*)&v, sizeof(v));

		size = indexed_vertices.size();
		meshFile.write((char*)&size, sizeof(size));
		for (auto v : indexed_vertices)
			meshFile.write((char*)&v, sizeof(v));

		size = indexed_uvs.size();
		meshFile.write((char*)&size, sizeof(size));
		for (auto v : indexed_uvs)
			meshFile.write((char*)&v, sizeof(v));

		size = indexed_normals.size();
		meshFile.write((char*)&size, sizeof(size));
		for (auto v : indexed_normals)
			meshFile.write((char*)&v, sizeof(v));
		
		meshFile.write((char*)&hasBones, sizeof(hasBones));
		if (hasBones)
		{
			size = indexed_boneIndices.size();
			meshFile.write((char*)&size, sizeof(size));
			for (auto v : indexed_boneIndices)
				meshFile.write((char*)&v, sizeof(v));

			size = indexed_boneWeights.size();
			meshFile.write((char*)&size, sizeof(size));
			for (auto v : indexed_boneWeights)
				meshFile.write((char*)&v, sizeof(v));
		}

		meshFile.close();
		*/
	}

	void LoadAnimations(const aiScene* scene)
	{
		{
			out->Log("Animation Count: " + std::to_string(scene->mNumAnimations));

			for (uint i = 0; i < scene->mNumAnimations; i++)
			{
				out->Log("\tAnimation " + std::string(scene->mAnimations[i]->mName.C_Str()) + ": " + std::to_string(scene->mAnimations[i]->mDuration) + " " + std::to_string(scene->mAnimations[i]->mDuration * scene->mAnimations[i]->mTicksPerSecond));
			}

			out->Log("\tAnimation Channel Count: " + std::to_string(scene->mAnimations[0]->mNumChannels));
		}

		// Export animation file header
		std::fstream mFile;
		mFile.open("Resources/Meshes/" + std::string(scene->mRootNode->mName.C_Str()) + ".animation", std::ios::out | std::ios::binary);
		mFile.write((char*)&scene->mAnimations[0]->mChannels[0]->mNumPositionKeys, sizeof(scene->mAnimations[0]->mChannels[0]->mNumPositionKeys));
		mFile.write((char*)&scene->mAnimations[0]->mDuration, sizeof(scene->mAnimations[0]->mDuration));
		mFile.write((char*)&scene->mAnimations[0]->mNumChannels, sizeof(scene->mAnimations[0]->mNumChannels));
		mFile.close();

		for (uint i = 0; i < scene->mAnimations[0]->mNumChannels; i++)
		{
			auto channel = scene->mAnimations[0]->mChannels[i];
			std::string boneName = &scene->mAnimations[0]->mChannels[i]->mNodeName.C_Str()[5];

			if (BoneNames.count(boneName) == 0)
				continue;	// Bone node not found
			unsigned int boneIndex = BoneNames[boneName];
			

			out->Log("\tChannel " + std::to_string(i) + ": " + boneName + " " + std::to_string(channel->mNumPositionKeys));

			std::vector<Transform> frames;
			frames.resize(channel->mNumPositionKeys);
			
			for (uint j = 0; j < channel->mNumPositionKeys; j++)
			{
				frames[j].Position = glm::vec3(
					channel->mPositionKeys[j].mValue.x,
					channel->mPositionKeys[j].mValue.y,
					channel->mPositionKeys[j].mValue.z
					);

				frames[j].Rotation = glm::quat(
					channel->mRotationKeys[j].mValue.w,
					channel->mRotationKeys[j].mValue.x,
					channel->mRotationKeys[j].mValue.y,
					channel->mRotationKeys[j].mValue.z
					);

				frames[j].Scale = glm::vec3(
					channel->mScalingKeys[j].mValue.x,
					channel->mScalingKeys[j].mValue.y,
					channel->mScalingKeys[j].mValue.z
					);
			}

			// Export animation file
			std::fstream meshFile;
			meshFile.open("Resources/Meshes/" + std::string(scene->mRootNode->mName.C_Str()) + ".animation", std::ios::out | std::ios::binary | std::ios::app);

			meshFile.write((char*)&boneIndex, sizeof(boneIndex));
			meshFile.write((char*)&channel->mNumPositionKeys, sizeof(channel->mNumPositionKeys));

			for (auto f : frames)
			{
				meshFile.write((char*)&f.Position, sizeof(f.Position));
				meshFile.write((char*)&f.Rotation, sizeof(f.Rotation));
				meshFile.write((char*)&f.Scale, sizeof(f.Scale));
			}

			meshFile.close();
		}
	}
}