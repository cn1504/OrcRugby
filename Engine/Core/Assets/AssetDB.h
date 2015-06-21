#pragma once

#include <Core.h>
#include <Space/TransformIF.h>

namespace Core
{
	namespace Assets
	{
		class AssetDB
		{
		private:
			std::shared_ptr<Database> db;
			std::unordered_map<std::string, std::shared_ptr<Core::Assets::Texture>> TextureCache;
			std::unordered_map<std::string, std::shared_ptr<Core::Assets::Font>> FontCache;
			std::unordered_map<std::string, std::shared_ptr<Core::Renderers::VertexArray>> VACache;
			std::unordered_map<std::string, std::shared_ptr<Core::Assets::Light>> LightCache;
			std::unordered_map<std::string, std::shared_ptr<Core::Assets::AudioFile>> AudioCache;
			std::unordered_map<std::string, std::shared_ptr<Core::Assets::Material>> MaterialCache;
			
		public:
			AssetDB(std::shared_ptr<Database> db);
			virtual ~AssetDB() {}

			virtual std::shared_ptr<Core::Assets::AudioFile> GetAudioFile(std::string key);
			virtual std::shared_ptr<Core::Assets::Texture> GetTexture(std::string key);
			virtual std::shared_ptr<Core::Assets::Font> GetFont(std::string key);
			virtual std::shared_ptr<Core::Renderers::VertexArray> GetVertexArray(std::string key);
			virtual std::shared_ptr<Core::Assets::Light> GetLight(std::string key);
			virtual std::shared_ptr<Core::Assets::Material> GetMaterial(std::string key);

			virtual void SetFont(std::string key, const Font& font);
			virtual void SetLight(std::string key, const Light& light);

			virtual void AddTileContents(std::shared_ptr<Core::Space::TransformIF> tile, std::string tag);
			virtual int GetTileEdgeType(std::string tag, int edge);

			virtual void ClearAllCaches();
		};
	}
}