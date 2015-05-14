#include "Assets.h"
#include <Scene/Scene.h>

namespace Core
{
	namespace Assets
	{
		std::unordered_map<std::string, Mesh*> Meshes;
		std::unordered_map<std::string, PointCloud*> PointClouds;
		std::unordered_map<std::string, Material*> Materials;
		std::unordered_map<std::string, Texture*> Textures;
		std::unordered_map<std::string, Font*> Fonts;
		std::unordered_map<std::string, AudioFile*> AudioFiles;
		
		
		void LoadStandardAssets()
		{		
			LoadMaterials();
			CreateStandardTextures();
			CreateStandardFonts();
			CreateStandardMeshes();
			SaveMaterials();
		}

		
		Mesh* LoadMesh(std::string name)
		{
			if (Meshes.count(name) > 0)
			{
				return Meshes[name];
			}
			
			Assets::Meshes[name] = new Mesh(name + ".mesh");
			return Assets::Meshes[name];
		}


		AudioFile* LoadAudioFile(std::string name, AudioFile::Type type)
		{
			if (AudioFiles.count(name) > 0)
			{
				return AudioFiles[name];
			}

			AudioFiles[name] = new AudioFile(name + ".ogg", type);
			return AudioFiles[name];
		}
		
		
		void CreateStandardTextures()
		{
			Texture* t = new Texture;
			t->LoadFromPNG("Consolas16", 256, 256, GL_NEAREST, GL_NEAREST);
			Textures["Consolas16"] = t;

			t = new Texture;
			t->LoadFromPNG("Consolas24", 384, 384, GL_NEAREST, GL_NEAREST);
			Textures["Consolas24"] = t;

			t = new Texture;
			t->LoadFromPNG("Consolas32", 512, 512, GL_NEAREST, GL_NEAREST);
			Textures["Consolas32"] = t;

			t = new Texture;
			t->LoadFromPNG("Consolas64", 1024, 1024, GL_NEAREST, GL_NEAREST);
			Textures["Consolas64"] = t;

			t = new Texture;
			t->LoadFromPNG("Close", 17, 17);
			Assets::Textures["CloseButton"] = t;
			
			t = new Texture;
			t->LoadFromPNG("CloseMouseOver", 17, 17);
			Assets::Textures["CloseButtonMO"] = t;
			
			t = new Texture;
			t->LoadFromPNG("Maximize", 17, 17);
			Assets::Textures["MaximizeButton"] = t;

			t = new Texture;
			t->LoadFromPNG("MaximizeMouseOver", 17, 17);
			Assets::Textures["MaximizeButtonMO"] = t;

			t = new Texture;
			t->LoadFromPNG("Minimize", 17, 17);
			Assets::Textures["MinimizeButton"] = t;

			t = new Texture;
			t->LoadFromPNG("MinimizeMouseOver", 17, 17);
			Assets::Textures["MinimizeButtonMO"] = t;

			t = new Texture;
			t->LoadFromPNG("Resize", 17, 17);
			Assets::Textures["ResizeButton"] = t;

			t = new Texture;
			t->LoadFromPNG("ResizeMouseOver", 17, 17);
			Assets::Textures["ResizeButtonMO"] = t;

			t = new Texture;
			t->LoadFromPNG("Windowed", 17, 17);
			Assets::Textures["WindowedButton"] = t;

			t = new Texture;
			t->LoadFromPNG("WindowedMouseOver", 17, 17);
			Assets::Textures["WindowedButtonMO"] = t;

			t = new Texture;
			t->LoadFromPNG("CheckSmall", 17, 17);
			Assets::Textures["CheckSmall"] = t;

			t = new Texture;
			t->LoadFromPNG("CheckSmallMouseOver", 17, 17);
			Assets::Textures["CheckSmallMO"] = t;

			t = new Texture;
			t->LoadFromPNG("folder", 32, 32);
			Assets::Textures["Folder"] = t;

			t = new Texture;
			t->LoadFromPNG("Border", 1, 1);
			Assets::Textures["Border"] = t;

			t = new Texture;
			t->LoadFromPNG("DialogBackground", 1, 1);
			Assets::Textures["DialogBackground"] = t;

			t = new Texture;
			t->LoadFromPNG("DialogBackgroundMouseOver", 1, 1);
			Assets::Textures["DialogBackgroundMO"] = t;
			Assets::Textures["TextBoxMO"] = new Texture(*Assets::Textures["DialogBackgroundMO"]);

			t = new Texture;
			t->LoadFromPNG("TextBoxEditBack", 1, 1);
			Assets::Textures["TextBoxEdit"] = t;


			t = new Texture;
			t->LoadFromPNG("in", 32, 32);
			Assets::Textures["In"] = t;

			t = new Texture;
			t->LoadFromPNG("inMo", 32, 32);
			Assets::Textures["InMO"] = t;

			t = new Texture;
			t->LoadFromPNG("out", 32, 32);
			Assets::Textures["Out"] = t;

			t = new Texture;
			t->LoadFromPNG("outMo", 32, 32);
			Assets::Textures["OutMO"] = t;

			t = new Texture;
			t->LoadFromPNG("down", 16, 16);
			Assets::Textures["Down"] = t;

			t = new Texture;
			t->LoadFromPNG("downMo", 16, 16);
			Assets::Textures["DownMO"] = t;

			t = new Texture;
			t->LoadFromPNG("up", 16, 16);
			Assets::Textures["Up"] = t;

			t = new Texture;
			t->LoadFromPNG("upMo", 16, 16);
			Assets::Textures["UpMO"] = t;

			t = new Texture;
			t->LoadFromPNG("copy", 32, 32);
			Assets::Textures["Copy"] = t;

			t = new Texture;
			t->LoadFromPNG("CopyMo", 32, 32);
			Assets::Textures["CopyMO"] = t;
			
			t = new Texture;
			t->LoadFromPNG("plus", 32, 32);
			Assets::Textures["Plus"] = t;

			t = new Texture;
			t->LoadFromPNG("plusMo", 32, 32);
			Assets::Textures["PlusMO"] = t;

			t = new Texture;
			t->LoadFromPNG("trash", 32, 32);
			Assets::Textures["Trash"] = t;

			t = new Texture;
			t->LoadFromPNG("trashMo", 32, 32);
			Assets::Textures["TrashMO"] = t;
			
			t = new Texture;
			t->LoadFromPNG("play", 32, 32);
			Assets::Textures["Play"] = t;

			t = new Texture;
			t->LoadFromPNG("playMo", 32, 32);
			Assets::Textures["PlayMO"] = t;
			
			t = new Texture;
			t->LoadFromPNG("material", 32, 32);
			Assets::Textures["Material"] = t;

			t = new Texture;
			t->LoadFromPNG("materialMo", 32, 32);
			Assets::Textures["MaterialMO"] = t;
			
			t = new Texture;
			t->LoadFromPNG("check", 32, 32);
			Assets::Textures["Check"] = t;

			t = new Texture;
			t->LoadFromPNG("checkMo", 32, 32);
			Assets::Textures["CheckMO"] = t;
			
			t = new Texture;
			t->LoadFromPNG("save", 32, 32);
			Assets::Textures["Save"] = t;

			t = new Texture;
			t->LoadFromPNG("saveMo", 32, 32);
			Assets::Textures["SaveMO"] = t;

			t = new Texture;
			t->LoadSkyboxFromPNG("Skyboxes/Sky", 1024, 1024);
			Assets::Textures["Skybox"] = t;
		}


		void CreateStandardFonts()
		{
			Font* f = new Font(Textures["Consolas16"], 16, 16, 16, 9, 0);
			Fonts["Consolas16"] = f;

			f = new Font(Textures["Consolas24"], 24, 24, 16, 14, 0);
			Fonts["Consolas24"] = f;

			f = new Font(Textures["Consolas32"], 32, 32, 16, 18, 0);
			Fonts["Consolas32"] = f;

			f = new Font(Textures["Consolas64"], 64, 64, 16, 36, 0);
			Fonts["Consolas64"] = f;
		}


		void CreateStandardMeshes()
		{
			Mesh* m = new Mesh("Cube.mesh");
			Meshes["Cube"] = m;
			Meshes["Sphere"] = new Mesh(Meshes["Cube"]);
			Meshes["Cylinder"] = new Mesh(Meshes["Cube"]);

			//PointCloud* pc = new PointCloud();
			
			//PointClouds["CubeFromMesh"] = m->ToPointCloud();
			//PointClouds["CubeFromMesh"]->SaveToFile("CubeFromMesh.pc");
			//PointClouds["Cube"] = new PointCloud();
			//PointClouds["Cube"]->BuildTestObject();
			
			//PointClouds["Cube"] = new PointCloud("Cube.pc"); 
			//PointClouds["Ground"] = new PointCloud("Ground.pc");
		}

		
		void Clear()
		{
			for (auto c : Meshes)
			{
				delete c.second;
			}

			for (auto c : PointClouds)
			{
				delete c.second;
			}

			for (auto c : Materials)
			{
				delete c.second;
			}

			for (auto c : Fonts)
			{
				delete c.second;
			}

			for (auto c : Textures)
			{
				delete c.second;
			}

			for (auto c : AudioFiles)
			{
				delete c.second;
			}

			Meshes.clear();
			PointClouds.clear();
			Materials.clear();
			Fonts.clear();
			Textures.clear();
			AudioFiles.clear();
		}


		void LoadMaterials()
		{			
			std::ifstream file;
			file.open("Resources/Materials.json");
			std::string in((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
			file.close();

			json::Object root = json::Deserialize(in).ToObject();
			auto mats = root["Content"].ToArray();
			for (auto m : mats)
			{
				Material* mat = new Material();
				mat->Deserialize(m.ToObject());
				Materials[mat->Name] = mat;
			}			
		}


		void SaveMaterials()
		{
			json::Object root;
			root["DataType"] = "Materials";
			root["Count"] = (int)Materials.size();

			json::Array content;
			for (auto mat : Materials)
			{
				content.push_back(mat.second->Serialize());
			}
			root["Content"] = content;

			std::ofstream file;
			file.open("Resources/Materials.json");
			file << json::Serialize(json::Value(root));
			file.close();
		}


		void IndentEachLine(std::string& input)
		{
			input.insert(0, "\t");

			std::size_t found = input.find("\n");
			while (found != std::string::npos)
			{
				input.insert(found + 1, "\t");
				found = input.find("\n", found + 2);
			}
		}

	}
}