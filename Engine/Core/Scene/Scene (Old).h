#pragma once
#include <Core.h>
#include "Entity.h"
#include <Components/LightSource.h>

namespace Core
{
	namespace Scene
	{
		void Load();
		void Update();

		class Scene
		{
		private:
			std::vector<Entity*> Entities;
			std::vector<Core::Components::LightSource*> Lights;

			std::deque<std::string> consoleLines;
			std::string consoleInput;

			void AddLight(Core::Components::LightSource*);
			void RemoveLight(Core::Components::LightSource*);

		public:
			DynamicsWorld* PhysicsWorld;
			
			Scene();
			~Scene();

			void Load();
			void Update();

			void AddEntity(Entity*);
			void RemoveEntity(Entity*);
			void AppendConsole(std::string msg);

			void LoadScene(std::string sceneFile);
			void LoadEntity();
			void LoadCharacter(std::string characterFile);
		};
	}
}