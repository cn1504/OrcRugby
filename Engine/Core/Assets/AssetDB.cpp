#include "AssetDB.h"
#include "Database.h"
#include "Texture.h"
#include "Font.h"
#include "Light.h"
#include <Renderers/VertexBuffer.h>

#include <Space/TransformIF.h>
#include <Components/StaticMesh.h>
#include <Components/LightSource.h>

using namespace Core::Assets;

AssetDB::AssetDB(std::shared_ptr<Database> db) 
	: db(db)
{
	// Can not preload assets here since AssetDB is created before any opengl contexts
}

std::shared_ptr<Core::Assets::Texture> AssetDB::GetTexture(std::string key)
{
	auto& ptr = TextureCache[key];
	if (ptr == nullptr)
	{		
		db->Sql("SELECT width, height, data FROM Textures WHERE key='" + key + "'");
		if (db->FetchRow())
		{
			int width = db->GetColumnInt(0);
			int height = db->GetColumnInt(1);
			auto data = db->GetColumnBlob(2);
			ptr = std::make_shared<Core::Assets::Texture>(width, height, data->GetData());
		}
	}
	return ptr;
}
std::shared_ptr<Core::Assets::Font> AssetDB::GetFont(std::string key)
{
	auto& ptr = FontCache[key];
	if (ptr == nullptr)
	{
		db->Sql("SELECT Texture, CharWidth, CharHeight, CharsPerRow, CharsPerColumn FROM Fonts WHERE key='" + key + "'");
		if (db->FetchRow())
		{
			std::string Texture = db->GetColumnString(0);
			float CharWidth = (float)db->GetColumnDouble(1);
			float CharHeight = (float)db->GetColumnDouble(2);
			int CharsPerRow = db->GetColumnInt(3);
			int CharsPerColumn = db->GetColumnInt(4);

			ptr = std::make_shared<Core::Assets::Font>(Texture, CharWidth, CharHeight, CharsPerRow, CharsPerColumn);
		}
	}
	return ptr;
}
std::shared_ptr<Core::Renderers::VertexBuffer> AssetDB::GetVertexBuffer(std::string key)
{
	auto& ptr = VBCache[key];
	if (ptr == nullptr)
	{
		db->Sql("SELECT Data FROM VertexBuffers WHERE Key='" + key + "'");
		if (db->FetchRow())
		{
			auto data = db->GetColumnBlob(0);
			ptr = std::make_shared<Core::Renderers::VertexBuffer>(data->GetSize(), data->GetData());
		}
		else
		{
			Debug->Error("Database miss: GetVertexBuffer(" + key + ")");
		}
	}
	return ptr;
}
std::shared_ptr<Core::Assets::Light> AssetDB::GetLight(std::string key)
{
	auto& ptr = LightCache[key];
	if (ptr == nullptr)
	{
		db->Sql("SELECT color_r, color_g, color_b, radius, intensity, cosInner, cosOuter, castsShadow FROM Lights WHERE key='" + key + "'");
		if (db->FetchRow())
		{
			auto color = glm::vec3(db->GetColumnDouble(0), db->GetColumnDouble(1), db->GetColumnDouble(2));
			auto radius = (float)db->GetColumnDouble(3);
			auto intensity = (float)db->GetColumnDouble(4);
			auto cosInner = (float)db->GetColumnDouble(5);
			auto cosOuter = (float)db->GetColumnDouble(6);
			auto castsShadow = db->GetColumnBool(7);

			ptr = std::make_shared<Core::Assets::Light>(color, radius, intensity, cosInner, cosOuter, castsShadow);
		}
	}
	return ptr;
}

void AssetDB::SetFont(std::string key, const Font& font)
{
	Core::Database->Sql("INSERT INTO Fonts(key, Texture, CharWidth, CharHeight, CharsPerRow, CharsPerColumn) VALUES(?, ?, ?, ?, ?, ?)");
	Core::Database->Bind(1, key);
	Core::Database->Bind(2, font.GetTexture());
	Core::Database->Bind(3, font.GetCharWidth());
	Core::Database->Bind(4, font.GetCharHeight());
	Core::Database->Bind(5, font.GetCharsPerRow());
	Core::Database->Bind(6, font.GetCharsPerColumn());
	Core::Database->ExecuteAndFree();
}

void AssetDB::SetLight(std::string key, const Light& light)
{
	Core::Database->Sql("INSERT INTO Lights(key, color_r, color_g, color_b, intensity, radius, cosInner, cosOuter, castsShadow) VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?)");
	Core::Database->Bind(1, key);
	Core::Database->Bind(2, light.GetColor().r);
	Core::Database->Bind(3, light.GetColor().g);
	Core::Database->Bind(4, light.GetColor().b);
	Core::Database->Bind(5, light.GetIntensity());
	Core::Database->Bind(6, light.GetRadius());
	Core::Database->Bind(7, light.GetCosInner());
	Core::Database->Bind(8, light.GetCosOuter());
	Core::Database->Bind(9, light.CastsShadow());
	Core::Database->ExecuteAndFree();
}


void AssetDB::AddTileContents(std::shared_ptr<Core::Space::TransformIF> tile, std::string tag)
{
	struct row
	{
		std::string type;
		std::string reference;
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
	};
	std::vector<row> results;

	db->Sql("SELECT Type, Reference, Pos_X, Pos_Y, Pos_Z, Rot_X, Rot_Y, Rot_Z, Scale_X, Scale_Y, Scale_Z FROM TileContents WHERE Tile='" + tag + "'");
	while (db->FetchRow())
	{
		results.resize(results.size()+1);
		auto r = &results.back();

		r->type = db->GetColumnString(0);
		r->reference = db->GetColumnString(1);
		r->position = glm::vec3(db->GetColumnDouble(2), db->GetColumnDouble(3), db->GetColumnDouble(4));
		r->rotation = glm::vec3(db->GetColumnDouble(5), db->GetColumnDouble(6), db->GetColumnDouble(7));
		r->scale = glm::vec3(db->GetColumnDouble(8), db->GetColumnDouble(9), db->GetColumnDouble(10));
	}
	db->FreeQuery();

	for (auto& r : results)
	{
		if (r.type == "StaticMesh")
		{
			db->Sql("SELECT Mesh, Material FROM StaticMeshes WHERE tag='" + r.reference + "'");
			if (db->FetchRow())
			{
				auto mesh = db->GetColumnString(0);
				auto mat = db->GetColumnString(1);
				db->FreeQuery();

				db->Sql("SELECT Base_R, Base_G, Base_B, Metallic, Specular, Roughness FROM Materials WHERE Tag='" + mat + "'");
				if (db->FetchRow())
				{
					auto base = glm::vec4(db->GetColumnDouble(0), db->GetColumnDouble(1), db->GetColumnDouble(2), 1.0);
					auto msr = glm::vec4(db->GetColumnDouble(3), db->GetColumnDouble(4), db->GetColumnDouble(5), 1.0);
					auto e = std::make_shared<Core::Components::StaticMesh>(mesh, base, msr);
					e->Translate(r.position);
					e->Rotate(r.rotation);
					e->Scale(r.scale);
					tile->AddChild(e);
				}
				else
				{
					Core::Debug->Log("Invalid Material: " + mat);
				}
			}
			else
			{
				Core::Debug->Log("Invalid StaticMesh: " + r.reference);
			}
		}
		else if (r.type == "Light")
		{
			auto e = std::make_shared<Core::Components::LightSource>(r.reference);
			e->Translate(r.position);
			e->Rotate(r.rotation);
			e->Scale(r.scale);
			tile->AddChild(e);
		}
		else
		{
			Core::Debug->Log("Invalid TileContent Type: " + r.type + " for reference: " + r.reference);
		}
	}
}