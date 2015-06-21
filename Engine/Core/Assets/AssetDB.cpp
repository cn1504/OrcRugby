#include "AssetDB.h"
#include "Database.h"
#include "Texture.h"
#include "Font.h"
#include "Light.h"
#include "AudioFile.h"
#include "Material.h"
#include <Renderers/VertexArray.h>

#include <Space/TransformIF.h>
#include <Components/StaticMesh.h>
#include <Components/LightSource.h>
#include <Components/Sea.h>

using namespace Core::Assets;

AssetDB::AssetDB(std::shared_ptr<Database> db) 
	: db(db)
{
	// Can not preload assets here since AssetDB is created before any opengl contexts
}

void AssetDB::ClearAllCaches()
{
	TextureCache.clear();
	FontCache.clear();
	VACache.clear();
	LightCache.clear();
	AudioCache.clear();
	MaterialCache.clear();
}

std::shared_ptr<Core::Assets::Texture> AssetDB::GetTexture(std::string key)
{
	auto& ptr = TextureCache[key];
	if (ptr == nullptr)
	{		
		db->Sql("SELECT width, height, filter, data FROM Textures WHERE key='" + key + "' LIMIT 1");
		if (db->FetchRow())
		{
			int width = db->GetColumnInt(0);
			int height = db->GetColumnInt(1);
			int filter = db->GetColumnInt(2);
			filter = (filter == 0) ? GL_LINEAR : GL_NEAREST;
			auto data = db->GetColumnBlob(3);
			ptr = std::make_shared<Core::Assets::Texture>(width, height, (float)filter, data->GetData());
		}
		db->FreeQuery();
	}
	return ptr;
}
std::shared_ptr<Core::Assets::Font> AssetDB::GetFont(std::string key)
{
	auto& ptr = FontCache[key];
	if (ptr == nullptr)
	{
		db->Sql("SELECT Texture, CharWidth, CharHeight, CharsPerRow, CharsPerColumn FROM Fonts WHERE key='" + key + "' LIMIT 1");
		if (db->FetchRow())
		{
			std::string Texture = db->GetColumnString(0);
			float CharWidth = (float)db->GetColumnDouble(1);
			float CharHeight = (float)db->GetColumnDouble(2);
			int CharsPerRow = db->GetColumnInt(3);
			int CharsPerColumn = db->GetColumnInt(4);

			ptr = std::make_shared<Core::Assets::Font>(Texture, CharWidth, CharHeight, CharsPerRow, CharsPerColumn);
		}
		db->FreeQuery();
	}
	return ptr;
}
std::shared_ptr<Core::Renderers::VertexArray> AssetDB::GetVertexArray(std::string key)
{
	auto& ptr = VACache[key];
	if (ptr == nullptr)
	{
		ptr = std::make_shared<Core::Renderers::VertexArray>();

		db->Sql("SELECT Data FROM VertexBuffers WHERE Key='" + key + "_Indices' LIMIT 1");
		if (db->FetchRow())
		{
			auto data = db->GetColumnBlob(0);
			ptr->SetElementBuffer(data->GetSize(), data->GetData());
		}
		db->FreeQuery();

		db->Sql("SELECT Data FROM VertexBuffers WHERE Key='" + key + "_Vertices' LIMIT 1");
		if (db->FetchRow())
		{
			auto data = db->GetColumnBlob(0);
			ptr->SetBuffer(0, 3, GL_FLOAT, GL_FALSE, data->GetSize(), data->GetData());
		}
		db->FreeQuery();

		db->Sql("SELECT Data FROM VertexBuffers WHERE Key='" + key + "_Normals' LIMIT 1");
		if (db->FetchRow())
		{
			auto data = db->GetColumnBlob(0);
			ptr->SetBuffer(1, 3, GL_FLOAT, GL_TRUE, data->GetSize(), data->GetData());
		}
		db->FreeQuery();

		db->Sql("SELECT Data FROM VertexBuffers WHERE Key='" + key + "_Uvs' LIMIT 1");
		if (db->FetchRow())
		{
			auto data = db->GetColumnBlob(0);
			ptr->SetBuffer(2, 2, GL_FLOAT, GL_FALSE, data->GetSize(), data->GetData());
		}
		db->FreeQuery();
	}
	return ptr;
}
std::shared_ptr<Core::Assets::Light> AssetDB::GetLight(std::string key)
{
	auto& ptr = LightCache[key];
	if (ptr == nullptr)
	{
		db->Sql("SELECT color_r, color_g, color_b, radius, intensity, cosInner, cosOuter, castsShadow FROM Lights WHERE key='" + key + "' LIMIT 1");
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
		db->FreeQuery();
	}
	return ptr;
}
std::shared_ptr<Core::Assets::Material> AssetDB::GetMaterial(std::string key)
{
	auto& ptr = MaterialCache[key];
	if (ptr == nullptr)
	{
		ptr = std::make_shared<Core::Assets::Material>();

		// Get Material Inputs
		std::string query = 
			"SELECT Channel, Base, MSR, Null AS Base_R, Null AS Base_G, Null AS Base_B, Null AS Metallic, Null AS Specular, Null AS Roughness FROM MaterialTextures WHERE Key='" + key + "' " +
			"UNION " +
			"SELECT Channel, Null AS Base, Null AS MSR, Base_R, Base_G, Base_B, Metallic, Specular, Roughness FROM MaterialInputs WHERE Key='" + key + "' " +
			"ORDER BY Channel";
		
		MaterialTexture tex;
		MaterialValue val;
		db->Sql(query);
		while (db->FetchRow())
		{
			auto channel = db->GetColumnInt(0);
			tex.Base = db->GetColumnString(1);
			if (tex.Base == "")
			{
				val.Base = glm::vec4(db->GetColumnDouble(3), db->GetColumnDouble(4), db->GetColumnDouble(5), 1.0);
				val.MSR = glm::vec4(db->GetColumnDouble(6), db->GetColumnDouble(7), db->GetColumnDouble(8), 1.0);
				ptr->AddInput(val);
			}
			else
			{
				tex.MSR = db->GetColumnString(2);
				ptr->AddInput(tex);
			}
		}
		db->FreeQuery();

		// Get Material Commands
		query = std::string("SELECT Command, Operand1, Operand2, Store, OpCode, ") +
			"Parameter1, Parameter2, Parameter3, Parameter4, " +
			"Parameter5, Parameter6, Parameter7, Parameter8, " +
			"Parameter9, Parameter10, Parameter11, Parameter12 " +
			"FROM MaterialCommands WHERE Key='" + key + "' ORDER BY Command";

		MaterialCommand com;
		db->Sql(query);
		while (db->FetchRow())
		{
			com.Op = glm::ivec4(db->GetColumnInt(1), db->GetColumnInt(2), db->GetColumnInt(3), db->GetColumnInt(4));
			com.Params[0] = glm::vec4(db->GetColumnDouble(5), db->GetColumnDouble(6), db->GetColumnDouble(7), db->GetColumnDouble(8));
			com.Params[1] = glm::vec4(db->GetColumnDouble(9), db->GetColumnDouble(10), db->GetColumnDouble(11), db->GetColumnDouble(12));
			com.Params[2] = glm::vec4(db->GetColumnDouble(13), db->GetColumnDouble(14), db->GetColumnDouble(15), db->GetColumnDouble(16));
			ptr->AddCommand(com);
		}
		db->FreeQuery();
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
			db->Sql("SELECT Mesh, Material FROM StaticMeshes WHERE tag='" + r.reference + "' LIMIT 1");
			if (db->FetchRow())
			{
				auto mesh = db->GetColumnString(0);
				auto mat = db->GetColumnString(1);

				auto e = std::make_shared<Core::Components::StaticMesh>(mesh, mat);
				e->Translate(r.position);
				e->Rotate(r.rotation);
				e->Scale(r.scale);
				tile->AddChild(e);			
			}
			else
			{
				Core::Debug->Log("Invalid StaticMesh: " + r.reference);
			}
			db->FreeQuery();
		}
		else if (r.type == "Light")
		{
			auto e = std::make_shared<Core::Components::LightSource>(r.reference);
			e->Translate(r.position);
			e->Rotate(r.rotation);
			e->Scale(r.scale);
			tile->AddChild(e);
		}
		else if (r.type == "Sea")
		{
			auto e = std::make_shared<Core::Components::Sea>(r.reference);
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
int AssetDB::GetTileEdgeType(std::string tag, int edge)
{
	int value = 0;
	std::string en = (edge == 0) ? "Edge_N" : (edge == 1) ? "Edge_NE" : (edge == 2) ? "Edge_SE" : (edge == 3) ? "Edge_S" : (edge == 4) ? "Edge_SW" : "Edge_NW";
	db->Sql("SELECT " + en + " FROM Tiles WHERE tag='" + tag + "' LIMIT 1");
	if (db->FetchRow())
	{
		value = db->GetColumnInt(0);
	}
	db->FreeQuery();
	return value;
}


std::shared_ptr<Core::Assets::AudioFile> AssetDB::GetAudioFile(std::string key)
{
	auto& ptr = AudioCache[key];
	if (ptr == nullptr)
	{
		db->Sql("SELECT Type, Format, Size, SampleRate, Duration, Loop, Data FROM Audio WHERE Key='" + key + "' LIMIT 1");
		if (db->FetchRow())
		{
			auto typestr = db->GetColumnString(0);
			auto type = (typestr == "SFX") ? Core::Assets::AudioFile::Type::SOUND_EFFECT :
				(typestr == "Music") ? Core::Assets::AudioFile::Type::MUSIC :
				(typestr == "Ambient") ? Core::Assets::AudioFile::Type::AMBIENT :
				// (typestr == "Voice") ? Core::Assets::AudioFile::Type::VOICE :
				Core::Assets::AudioFile::Type::VOICE; 
			auto format = db->GetColumnInt(1);
			auto size = db->GetColumnInt(2);
			auto samplerate = db->GetColumnInt(3);
			auto duration = static_cast<float>(db->GetColumnDouble(4));
			auto loop = db->GetColumnInt(5);
			auto data = db->GetColumnBlob(6);
			ptr = std::make_shared<Core::Assets::AudioFile>(data->GetData(), data->GetSize(), (ALuint)format, (DWORD)size, (DWORD)samplerate, duration, type, loop != 0);
		}
		else
		{
			Debug->Error("Database miss: GetAudioFile(" + key + ")");
		}
		db->FreeQuery();
	}
	return ptr;
}
