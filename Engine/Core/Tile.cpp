#include "Tile.h"
#include "Scene.h"
#include "WorldGenerator.h"
#include "Assets.h"
#include "Feature.h"

namespace Core
{
	Tile::Tile(std::string name)
	{
		Name = name;
	}

	Tile::Tile(Scene* scene, WorldGenerator* generator, Tile* parent, Node* n, std::string name, int distance)
	{
		ScenePtr = scene;
		Generator = generator;
		Name = name;
		Distance = distance;
		Parent = parent;
		
		Load();
		
		if (OnlySpawnOnce)
		{
			Generator->SetSpawnWeights(Name, 0, 0, 0);
		}

		// Calculate World to Tile Space transform
		if (Distance != 0)
		{
			// Parent -> Node -> Node -> Tile
			std::string ttag = n->Tag;
			ttag.resize(ttag.size() - 1);
			if (n->Tag.back() == 'N')
				ttag.push_back('S');
			else if (n->Tag.back() == 'S')
				ttag.push_back('N');
			else if (n->Tag.back() == 'E')
				ttag.push_back('W');
			else if (n->Tag.back() == 'W')
				ttag.push_back('E');

			std::vector<Node*> trans;
			for (auto t : Transitions)
			{
				if (ttag == t->Tag)
					trans.push_back(t);
			}
			int i = rand() % trans.size();

			if (Settings::Misc::VerboseLogging)
				Debug::Log("To: " + std::to_string(trans[i]->Transform.Position.x) + " " + std::to_string(trans[i]->Transform.Position.y) + " " + std::to_string(trans[i]->Transform.Position.z));
						
			Transform.Position = parent->Transform.Position + n->Transform.Position - trans[i]->Transform.Position;
			
			UsedTransitions[trans[i]] = parent;

			if (Settings::Misc::VerboseLogging)
				Debug::Log("Tile Transform: " + std::to_string(Transform.Position.x) + " " + std::to_string(Transform.Position.y) + " " + std::to_string(Transform.Position.z));
		}

		// Generate objects contained in tile
		BuildContents();

		// If not highest level node, build children
		if (Distance < Generator->GetMaxTileDepth())
		{
			BuildNeighbors();
		}
	}

	Tile::~Tile() 
	{
		DestroyNeighbors();
		DestroyContents();
	}

	void Tile::LoadDefaults()
	{
		// Create Tile w/ Default Values
		TileWeight = 100;
		TileAdjustment = 100;
		OnlySpawnOnce = false;

		LowerBound = glm::vec3(-11, -9, -9);
		UpperBound = glm::vec3(11, 9, 9);

		/*
		auto node = new Node;
		node->Tag = "Building_Width1_Base";
		node->Transform.Position = glm::vec3(10.88f, 0.12f, 6.0f);
		node->Transform.Rotation = glm::quat(glm::vec3(0.0f, glm::radians(180.0f), 0.0f));
		node->Transform.Scale = glm::vec3(1.0f);
		Features.push_back(node);

		node = new Node;
		node->Tag = "Building_Width1_Base";
		node->Transform.Position = glm::vec3(10.88f, 0.12f, 0.0f);
		node->Transform.Rotation = glm::quat(glm::vec3(0.0f, glm::radians(180.0f), 0.0f));
		node->Transform.Scale = glm::vec3(1.0f);
		Features.push_back(node);

		node = new Node;
		node->Tag = "Building_Width1_Base";
		node->Transform.Position = glm::vec3(10.88f, 0.12f, -6.0f);
		node->Transform.Rotation = glm::quat(glm::vec3(0.0f, glm::radians(180.0f), 0.0f));
		node->Transform.Scale = glm::vec3(1.0f);
		Features.push_back(node);

		node = new Node;
		node->Tag = "Building_Width1_Base";
		node->Transform.Position = glm::vec3(-10.88f, 0.12f, 6.0f);
		node->Transform.Rotation = glm::quat(glm::vec3(0.0f, glm::radians(0.0f), 0.0f));
		node->Transform.Scale = glm::vec3(1.0f);
		Features.push_back(node);

		node = new Node;
		node->Tag = "Building_Width1_Base";
		node->Transform.Position = glm::vec3(-10.88f, 0.12f, 0.0f);
		node->Transform.Rotation = glm::quat(glm::vec3(0.0f, glm::radians(0.0f), 0.0f));
		node->Transform.Scale = glm::vec3(1.0f);
		Features.push_back(node);

		node = new Node;
		node->Tag = "Building_Width1_Base";
		node->Transform.Position = glm::vec3(-10.88f, 0.12f, -6.0f);
		node->Transform.Rotation = glm::quat(glm::vec3(0.0f, glm::radians(0.0f), 0.0f));
		node->Transform.Scale = glm::vec3(1.0f);
		Features.push_back(node);

		node = new Node;
		node->Tag = "Street_W";
		node->Transform.Position = glm::vec3(0.0f, 0.0f, -9.0f);
		Transitions.push_back(node);

		node = new Node;
		node->Tag = "Street_E";
		node->Transform.Position = glm::vec3(0.0f, 0.0f, 9.0f);
		Transitions.push_back(node);

		Meshes.push_back(std::pair<std::string, std::string>("Road", "Asphalt"));
		Meshes.push_back(std::pair<std::string, std::string>("RoadPaint_White", "Paint_White"));
		Meshes.push_back(std::pair<std::string, std::string>("RoadPaint_Yellow", "Paint_Yellow"));
		Meshes.push_back(std::pair<std::string, std::string>("Sidewalk", "Concrete"));
		Meshes.push_back(std::pair<std::string, std::string>("Sidewalk_Drain", "Steel"));

		auto c = new Collider;
		c->ShapeType = Collider::Type::Box;
		c->Transform.Position = glm::vec3(7.0f, -4.5f + 0.12f, 0.0f);
		c->Transform.Rotation = glm::quat(glm::vec3(0.0f, glm::radians(0.0f), 0.0f));
		c->Transform.Scale = glm::vec3(2.0f, 4.5f, 9.0f) * 2.0f;
		Colliders.push_back(c);

		c = new Collider;
		c->ShapeType = Collider::Type::Box;
		c->Transform.Position = glm::vec3(-7.0f, -4.5f + 0.12f, 0.0f);
		c->Transform.Rotation = glm::quat(glm::vec3(0.0f, glm::radians(0.0f), 0.0f));
		c->Transform.Scale = glm::vec3(2.0f, 4.5f, 9.0f) * 2.0f;
		Colliders.push_back(c);

		c = new Collider;
		c->ShapeType = Collider::Type::Box;
		c->Transform.Position = glm::vec3(0.0f, -4.5f, 0.0f);
		c->Transform.Rotation = glm::quat(glm::vec3(0.0f, glm::radians(0.0f), 0.0f));
		c->Transform.Scale = glm::vec3(11.0f, 4.5f, 9.0f) * 2.0f;
		Colliders.push_back(c);
		*/
	}

	void Tile::Load()
	{
		if (Settings::Misc::VerboseLogging)
			Debug::Log("\nLoading tile: " + Name);

		std::ifstream file("Resources/Tiles/" + Name);
		if (!file)
		{
			Debug::Error("Invalid tile: " + Name);
		}

		file.read((char*)&TileWeight, sizeof(TileWeight));
		file.read((char*)&TileAdjustment, sizeof(TileAdjustment));
		file.read((char*)&OnlySpawnOnce, sizeof(OnlySpawnOnce));

		file.read((char*)&LowerBound, sizeof(LowerBound));
		file.read((char*)&UpperBound, sizeof(UpperBound));

		file.read((char*)&Transform.Position, sizeof(Transform.Position));
		file.read((char*)&Transform.Rotation, sizeof(Transform.Rotation));
		file.read((char*)&Transform.Scale, sizeof(Transform.Scale));

		// Build Nodes
		auto size = Features.size();
		auto ss = Name.size();

		// Transitions
		file.read((char*)&size, sizeof(size));
		for (unsigned int i = 0; i < size; i++)
		{
			auto n = new Node;
			file.read((char*)&ss, sizeof(ss));
			n->Tag.resize(ss);
			file.read(&n->Tag[0], ss);

			file.read((char*)&n->Transform.Position, sizeof(n->Transform.Position));
			file.read((char*)&n->Transform.Rotation, sizeof(n->Transform.Rotation));
			file.read((char*)&n->Transform.Scale, sizeof(n->Transform.Scale));
			Transitions.push_back(n);
		}

		// Features
		file.read((char*)&size, sizeof(size));
		for (unsigned int i = 0; i < size; i++)
		{
			auto n = new Node;
			file.read((char*)&ss, sizeof(ss));
			n->Tag.resize(ss);
			file.read(&n->Tag[0], ss);

			file.read((char*)&n->Transform.Position, sizeof(n->Transform.Position));
			file.read((char*)&n->Transform.Rotation, sizeof(n->Transform.Rotation));
			file.read((char*)&n->Transform.Scale, sizeof(n->Transform.Scale));
			Features.push_back(n);
		}

		// Placeables
		file.read((char*)&size, sizeof(size));
		for (unsigned int i = 0; i < size; i++)
		{
			auto n = new Node;
			file.read((char*)&ss, sizeof(ss));
			n->Tag.resize(ss);
			file.read(&n->Tag[0], ss);

			file.read((char*)&n->Transform.Position, sizeof(n->Transform.Position));
			file.read((char*)&n->Transform.Rotation, sizeof(n->Transform.Rotation));
			file.read((char*)&n->Transform.Scale, sizeof(n->Transform.Scale));
			Placeables.push_back(n);
		}

		// Creatures
		file.read((char*)&size, sizeof(size));
		for (unsigned int i = 0; i < size; i++)
		{
			auto n = new Node;
			file.read((char*)&ss, sizeof(ss));
			n->Tag.resize(ss);
			file.read(&n->Tag[0], ss);

			file.read((char*)&n->Transform.Position, sizeof(n->Transform.Position));
			file.read((char*)&n->Transform.Rotation, sizeof(n->Transform.Rotation));
			file.read((char*)&n->Transform.Scale, sizeof(n->Transform.Scale));
			Creatures.push_back(n);
		}

		// Lights	
		file.read((char*)&size, sizeof(size));
		for (unsigned int i = 0; i < size; i++)
		{
			auto n = new LightNode;
			file.read((char*)&n->Color, sizeof(n->Color));
			file.read((char*)&n->Radius, sizeof(n->Radius));
			file.read((char*)&n->CastsShadow, sizeof(n->CastsShadow));
			file.read((char*)&n->Transform.Position, sizeof(n->Transform.Position));
			file.read((char*)&n->Transform.Rotation, sizeof(n->Transform.Rotation));
			file.read((char*)&n->Transform.Scale, sizeof(n->Transform.Scale));
			Lights.push_back(n);
		} 

		// AudioSources;

		// Meshes
		file.read((char*)&size, sizeof(size));
		for (unsigned int i = 0; i < size; i++)
		{
			std::string mesh, mat;

			file.read((char*)&ss, sizeof(ss));
			mesh.resize(ss);
			file.read(&mesh[0], ss);

			file.read((char*)&ss, sizeof(ss));
			mat.resize(ss);
			file.read(&mat[0], ss);

			mat = "Gold";
			Meshes.push_back(std::pair<std::string, std::string>(mesh, mat));
		}

		// Colliders
		file.read((char*)&size, sizeof(size));
		for (unsigned int i = 0; i < size; i++)
		{
			auto n = new Collider;
			file.read((char*)&n->ShapeType, sizeof(n->ShapeType));
			file.read((char*)&n->Transform.Position, sizeof(n->Transform.Position));
			file.read((char*)&n->Transform.Rotation, sizeof(n->Transform.Rotation));
			file.read((char*)&n->Transform.Scale, sizeof(n->Transform.Scale));
			Colliders.push_back(n);
		}

		file.close();
	}
	
	void Tile::Save()
	{
		if (Settings::Misc::VerboseLogging)
			Debug::Log("\nSaving tile: " + Name);

		std::ofstream file("Resources/Tiles/" + Name);
		if (!file)
		{
			Debug::Error("Invalid tile: " + Name);
		}

		file.write((char*)&TileWeight, sizeof(TileWeight));
		file.write((char*)&TileAdjustment, sizeof(TileAdjustment));
		file.write((char*)&OnlySpawnOnce, sizeof(OnlySpawnOnce));

		file.write((char*)&LowerBound, sizeof(LowerBound));
		file.write((char*)&UpperBound, sizeof(UpperBound));

		file.write((char*)&Transform.Position, sizeof(Transform.Position));
		file.write((char*)&Transform.Rotation, sizeof(Transform.Rotation));
		file.write((char*)&Transform.Scale, sizeof(Transform.Scale));

		auto size = Transitions.size();
		file.write((char*)&size, sizeof(size));
		for (auto n : Transitions)
		{
			size = n->Tag.size();
			file.write((char*)&size, sizeof(size));
			file.write((char*)&n->Tag[0], size);
			file.write((char*)&n->Transform.Position, sizeof(n->Transform.Position));
			file.write((char*)&n->Transform.Rotation, sizeof(n->Transform.Rotation));
			file.write((char*)&n->Transform.Scale, sizeof(n->Transform.Scale));
		}
				
		size = Features.size();
		file.write((char*)&size, sizeof(size));
		for (auto n : Features)
		{
			size = n->Tag.size();
			file.write((char*)&size, sizeof(size));
			file.write((char*)&n->Tag[0], size);
			file.write((char*)&n->Transform.Position, sizeof(n->Transform.Position));
			file.write((char*)&n->Transform.Rotation, sizeof(n->Transform.Rotation));
			file.write((char*)&n->Transform.Scale, sizeof(n->Transform.Scale));
		}

		size = Placeables.size();
		file.write((char*)&size, sizeof(size));
		for (auto n : Placeables)
		{
			size = n->Tag.size();
			file.write((char*)&size, sizeof(size));
			file.write((char*)&n->Tag[0], size);
			file.write((char*)&n->Transform.Position, sizeof(n->Transform.Position));
			file.write((char*)&n->Transform.Rotation, sizeof(n->Transform.Rotation));
			file.write((char*)&n->Transform.Scale, sizeof(n->Transform.Scale));
		}
		
		size = Creatures.size();
		file.write((char*)&size, sizeof(size));
		for (auto n : Creatures)
		{
			size = n->Tag.size();
			file.write((char*)&size, sizeof(size));
			file.write((char*)&n->Tag[0], size);
			file.write((char*)&n->Transform.Position, sizeof(n->Transform.Position));
			file.write((char*)&n->Transform.Rotation, sizeof(n->Transform.Rotation));
			file.write((char*)&n->Transform.Scale, sizeof(n->Transform.Scale));
		}

		size = Lights.size();
		file.write((char*)&size, sizeof(size));
		for (auto n : Lights)
		{
			file.write((char*)&n->Color, sizeof(n->Color));
			file.write((char*)&n->Radius, sizeof(n->Radius));
			file.write((char*)&n->CastsShadow, sizeof(n->CastsShadow));
			file.write((char*)&n->Transform.Position, sizeof(n->Transform.Position));
			file.write((char*)&n->Transform.Rotation, sizeof(n->Transform.Rotation));
			file.write((char*)&n->Transform.Scale, sizeof(n->Transform.Scale));
		}

		//AudioSources;

		size = Meshes.size();
		file.write((char*)&size, sizeof(size));
		for (auto n : Meshes)
		{
			size = n.first.size();
			file.write((char*)&size, sizeof(size));
			file.write((char*)&n.first[0], size);
			size = n.second.size();
			file.write((char*)&size, sizeof(size));
			file.write((char*)&n.second[0], size);
		}

		//colliders
		size = Colliders.size();
		file.write((char*)&size, sizeof(size));
		for (auto n : Colliders)
		{
			file.write((char*)&n->ShapeType, sizeof(n->ShapeType));
			file.write((char*)&n->Transform.Position, sizeof(n->Transform.Position));
			file.write((char*)&n->Transform.Rotation, sizeof(n->Transform.Rotation));
			file.write((char*)&n->Transform.Scale, sizeof(n->Transform.Scale));
		}

		file.close();
	}

	void Tile::UpdateDistance(int distance, Tile* parent)
	{
		if (distance == Generator->GetMaxTileDepth())
		{
			Distance = distance;
			Parent = parent;
			DestroyNeighbors();
		}
		else if (Distance == Generator->GetMaxTileDepth())
		{
			Distance = distance;
			Parent = parent;
			BuildNeighbors();
		}
		else
		{
			Distance = distance;
			Parent = parent;
		}

		for (auto n : UsedTransitions)
		{
			if (n.second != parent)
				n.second->UpdateDistance(distance + 1, this);
		}

	}

	Tile* Tile::FindClosest(glm::vec3 position)
	{
		if (ContainsPosition(position))
			return this;

		for (auto t : UsedTransitions)
		{
			if (t.second->Distance > Distance)
			{
				auto n = t.second->FindClosest(position);
				if (n != nullptr)
					return n;
			}
		}

		return nullptr;
	}

	bool Tile::ContainsPosition(glm::vec3 position)
	{
		// Convert position to Tile space
		auto p = glm::inverse(Transform.ToMatrix()) * glm::vec4(position, 1.0f);
		
		// Check if in bounding area
		bool result = ((p.x > LowerBound.x && p.y > LowerBound.y && p.z > LowerBound.z) && (p.x < UpperBound.x && p.y < UpperBound.y && p.z < UpperBound.z));

		if (Settings::Misc::VerboseLogging)
			Debug::Log("Tile Space Position: (" + std::to_string(p.x) + ", " + std::to_string(p.y) + ", " + std::to_string(p.z) + ") " + std::to_string(result));

		return result;
	}

	void Tile::Destroy()
	{	
		delete this;
	}

	void Tile::BuildContents()
	{
		if (Settings::Misc::VerboseLogging)
			Debug::Log("Building contents:");

		if (Settings::Misc::VerboseLogging)
			Debug::Log("Features...");
		for (auto n : Features)
		{
			Generator->BuildFeature(this, n);
		}

		if (Settings::Misc::VerboseLogging)
			Debug::Log("Placeables ...");
		for (auto n : Placeables)
		{

		}

		if (Settings::Misc::VerboseLogging)
			Debug::Log("Creatures ...");
		for (auto n : Creatures)
		{

		}

		if (Settings::Misc::VerboseLogging)
			Debug::Log("Lights ...");
		for (auto n : Lights)
		{
			Debug::Log("\t(" + std::to_string(n->Transform.Position.x) + ", " + std::to_string(n->Transform.Position.y) + ", " + std::to_string(n->Transform.Position.z) + ") => " + std::to_string((int)(n->Color.x * 256.0f)) + ", " + std::to_string((int)(n->Color.y * 256.0f)) + ", " + std::to_string((int)(n->Color.z * 256.0f)));
			Entity* l = new Entity();
			l->Transform.Position = Transform.Position + n->Transform.Position;
			l->Transform.Rotation = n->Transform.Rotation;
			l->Transform.Scale = n->Transform.Scale;
			l->AddComponent(new LightSource(ScenePtr, n->Color, n->Radius, n->CastsShadow));
			l->AddComponent(Assets::Meshes["Sphere"]);
			l->AddComponent(Assets::Materials["Light"]);
			AddEntity(l);
		}

		if (Settings::Misc::VerboseLogging)
			Debug::Log("Audio Sources ...");
		for (auto n : AudioSources)
		{

		}

		if (Settings::Misc::VerboseLogging)
			Debug::Log("Meshes ...");
		for (auto n : Meshes)
		{
			Debug::Log("\t" + n.first + " " + n.second);
			auto e = new Entity();
			e->Transform.Position = Transform.Position;
			e->Transform.Rotation = Transform.Rotation;
			e->Transform.Scale = Transform.Scale;
			e->AddComponent(Assets::LoadMesh(n.first));
			e->AddComponent(Assets::Materials[(Assets::Materials.count(n.second) > 0) ? n.second : "Gold"]);
			AddEntity(e);
		}

		if (Settings::Misc::VerboseLogging)
			Debug::Log("Colliders ...");
		for (auto n : Colliders)
		{
			auto e = new Entity();
			e->Transform.Position = Transform.Position + n->Transform.Position;
			e->Transform.Scale = n->Transform.Scale;

			btCollisionShape* shape = nullptr;
			if (n->ShapeType == Collider::Type::Box)
				shape = new btBoxShape(btVector3(n->Transform.Scale.x, n->Transform.Scale.y, n->Transform.Scale.z) * 0.5f);
			
			e->AddComponent(new RigidBody(ScenePtr->PhysicsWorld, Assets::Materials["Gold"], shape));
		
			Entities.push_back(e);
			ScenePtr->AddEntity(e);
		}

		if (Settings::Misc::VerboseLogging)
			Debug::Log("Loading Entities ...");
		for (auto e : Entities)
			e->Load();
	}

	void Tile::DestroyContents()
	{
		for (auto e : Entities)
		{
			ScenePtr->RemoveEntity(e);
			delete e;
		}

		for (auto t : Transitions)
			delete t;
		for (auto t : Features)
			delete t;
		for (auto t : Placeables)
			delete t;
		for (auto t : Creatures)
			delete t;
		for (auto t : Lights)
			delete t;
		for (auto t : AudioSources)
			delete t;
		for (auto t : Colliders)
			delete t;
	}

	void Tile::BuildNeighbors()
	{
		for (auto n : Transitions)
		{
			if (UsedTransitions.count(n) == 0)
			{
				if (Settings::Misc::VerboseLogging)
					Debug::Log("From: " + std::to_string(n->Transform.Position.x) + " " + std::to_string(n->Transform.Position.y) + " " + std::to_string(n->Transform.Position.z));

				std::string ttag = n->Tag;
				ttag.resize(ttag.size() - 1);
				if (n->Tag.back() == 'N')
					ttag.push_back('S');
				else if (n->Tag.back() == 'S')
					ttag.push_back('N');
				else if (n->Tag.back() == 'E')
					ttag.push_back('W');
				else if (n->Tag.back() == 'W')
					ttag.push_back('E');
				auto next = Generator->PickNextTile(ttag);
				UsedTransitions[n] = new Tile(ScenePtr, Generator, this, n, next, Distance + 1);
			}
		}
	}

	void Tile::DestroyNeighbors()
	{
		std::vector<Node*> toDestroy;

		for (auto t : UsedTransitions)
		{
			if (t.second != Parent)
			{
				toDestroy.push_back(t.first);
			}
		}
		
		for (auto n : toDestroy)
		{
			delete UsedTransitions[n];
			UsedTransitions.erase(n);
		}
	}

	void Tile::AddEntity(Entity* e)
	{
		Entities.push_back(e);
		ScenePtr->AddEntity(e);
	}
	
	void Tile::GetDistantEntities(std::unordered_set<Entity*>* result, int maxTileDepth)
	{
		if (Distance > maxTileDepth)
		{
			for (auto e : Entities)
			{
				result->insert(e);
			}
		}

		for (auto t : UsedTransitions)
		{
			if (t.second->Distance > Distance)
				t.second->GetDistantEntities(result, maxTileDepth);
		}
	}

}