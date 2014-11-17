#pragma once
#include "Core.h"
#include "RenderBuffer.h"
#include "Shader.h"
#include "ScreenQuad.h"
#include "Mesh.h"
#include "Camera.h"
#include "CameraTarget.h"
#include "DynamicsWorld.h"
#include "Text.h"
#include "LightSource.h"
#include "GuiItem.h"
#include "WorldGenerator.h"

namespace Core
{
	class Window;

	class Scene
	{
	private:
		Window* Window;

		RenderBuffer* GeometryRB;
		RenderBuffer* LightRB;
		RenderBuffer* GlowMapHorizontalRB;
		RenderBuffer* GlowMapVerticalRB;
		RenderBuffer* BufferCombineRB;
		
		// Shaders
		Shader* NOAAShader;
		Shader* FXAAShader;

		Shader* MeshShader;
		Shader* SkeletalMeshShader;
		Shader* SphereShader;
		Shader* CylinderShader;

		Shader* LightShader;
		Shader* LightWithShadowShader;
		Shader* BufferCombineShader;

		Shader* FontShader;
		Shader* GuiTextureShader;

		Shader* ShadowMeshShader;
		Shader* ShadowSkeletalMeshShader;
		Shader* ShadowSphereShader;
		Shader* ShadowCylinderShader;

		Shader* TranslucentMeshShader;
		Shader* TranslucentSphereShader;
		Shader* TranslucentCylinderShader;

		Shader* BlurShader;

		ScreenQuad SQuad;

		std::vector<Entity*> Entities;
		std::vector<LightSource*> Lights;
		Mesh* Cube;
		Mesh* Sphere;
		Mesh* Cylinder;
		std::map<float, Entity*> TranslucentEntities;

		Text* fpsText;
		Text* consoleText;
		std::deque<std::string> consoleLines;
		std::string consoleInput;
		
		// Camera
		Camera* Camera;
		CameraTarget* CameraTarget;
		glm::mat4 V;	
		glm::mat4 P;
		glm::mat4 VInverse;
		glm::mat4 PInverse;

		glm::mat4 CubeMapViewMatrices[6];

		void RenderGeometry();
		void RenderLight();
		void RenderBloom();
		void RenderShadows();
		void RenderTranslucent();
		void CombineGeometryAndLight();
		void RenderPost();
		void RenderUI();

		void AddLight(LightSource*);
		void RemoveLight(LightSource*);

		// Editor Specific:
		Entity* character;

	public:
		DynamicsWorld* PhysicsWorld;
		Gui::Item* Gui;
		Gui::Item* CharacterEditor;
		Gui::Item* TileEditor;
		Gui::Item* FeatureEditor;
		Theme* Theme;
		WorldGenerator* WG;
		std::unordered_set<Entity*> DistantEntities;
		
		Scene(Core::Window* window);
		~Scene();

		void Load();
		void Update();
		void ResizeRenderBuffers();
		void SetActiveCamera(Core::Camera* camera);

		void AddEntity(Entity*);
		void RemoveEntity(Entity*);
		void AppendConsole(std::string msg);

		void LoadScene(std::string sceneFile);
		void LoadCharacter(std::string characterFile);
		void LoadTile(std::string tileFile);
		void LoadFeature(std::string featureFile);
	};

}