#include "Scene.h"
#include "Window.h"
#include "Assets.h"
#include "LightSource.h"
#include "Text.h"
#include "AudioListener.h"
#include "SkeletalAnimation.h"
#include "Character.h"

#include "DefaultButtons.h"

#include "OrbitingPosition.h"

namespace Core
{

	Scene::Scene(Core::Window* window)
	{
		Window = window;

		CubeMapViewMatrices[GL_TEXTURE_CUBE_MAP_POSITIVE_X - GL_TEXTURE_CUBE_MAP_POSITIVE_X] = glm::lookAt(glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)); // +X
		CubeMapViewMatrices[GL_TEXTURE_CUBE_MAP_NEGATIVE_X - GL_TEXTURE_CUBE_MAP_POSITIVE_X] = glm::lookAt(glm::vec3(0.0, 0.0, 0.0), glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)); // -X
		CubeMapViewMatrices[GL_TEXTURE_CUBE_MAP_POSITIVE_Y - GL_TEXTURE_CUBE_MAP_POSITIVE_X] = glm::lookAt(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)); // +Y
		CubeMapViewMatrices[GL_TEXTURE_CUBE_MAP_NEGATIVE_Y - GL_TEXTURE_CUBE_MAP_POSITIVE_X] = glm::lookAt(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)); // -Y
		CubeMapViewMatrices[GL_TEXTURE_CUBE_MAP_POSITIVE_Z - GL_TEXTURE_CUBE_MAP_POSITIVE_X] = glm::lookAt(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)); // +Z
		CubeMapViewMatrices[GL_TEXTURE_CUBE_MAP_NEGATIVE_Z - GL_TEXTURE_CUBE_MAP_POSITIVE_X] = glm::lookAt(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)); // -Z

		// Load RenderBuffers
		GeometryRB = new RenderBuffer(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), 6, RenderBufferType::GBuffer);
		LightRB = new RenderBuffer(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), 2, RenderBufferType::CBuffer);
		GlowMapHorizontalRB = new RenderBuffer(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), 1, RenderBufferType::CBuffer);
		GlowMapVerticalRB = new RenderBuffer(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), 1, RenderBufferType::CBuffer);
		BufferCombineRB = new RenderBuffer(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), 1, RenderBufferType::CBuffer);
		Debug::GLError("ERROR: Could not complete renderbuffers.");

		DistantLightProbe = new RenderBuffer(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), 2, RenderBufferType::LightProbe);
		Debug::GLError("ERROR: Could not create light probes.");

		// Load Shader Programs
		NOAAShader = new Shader("fspassthrough.vert", "noaa.frag");
		FXAAShader = new Shader("fspassthrough.vert", "fxaa.frag");
		MeshShader = new Shader("mesh.vert", "material.frag");
		SkeletalMeshShader = new Shader("skeletalmesh.vert", "material.frag");
		SphereShader = new Shader("mesh.vert", "sphere.frag");
		CylinderShader = new Shader("mesh.vert", "cylinder.frag");
		SkyboxShader = new Shader("skybox.vert", "skybox.frag");
		PointCloudShader = new Shader("pc.vert", "pc.geom", "material.frag");
		LightShader = new Shader("mesh.vert", "light.frag");
		LightWithShadowShader = new Shader("mesh.vert", "lightwshadow.frag");
		LightProbeShader = new Shader("mesh.vert", "lightprobe.frag");
		BufferCombineShader = new Shader("fspassthrough.vert", "combinebuffers.frag");
		FontShader = new Shader("font.vert", "font.frag");
		GuiTextureShader = new Shader("guitexture.vert", "guitexture.frag");
		ShadowMeshShader = new Shader("mesh.vert", "cubemap.geom", "shadow.frag");
		ShadowSkeletalMeshShader = new Shader("skeletalmesh.vert", "cubemap.geom", "shadow.frag");
		ShadowSphereShader = new Shader("mesh.vert", "cubemap.geom", "shadowsphere.frag");
		ShadowCylinderShader = new Shader("mesh.vert", "cubemap.geom", "shadowcylinder.frag");
		ShadowPointCloudShader = new Shader("pc.vert", "pccubemap.geom", "shadow.frag");
		BlurShader = new Shader("fspassthrough.vert", "blur.frag"); 
		TranslucentMeshShader = new Shader("mesh.vert", "translucentmaterial.frag");
		TranslucentSphereShader = new Shader("mesh.vert", "translucentsphere.frag");
		TranslucentCylinderShader = new Shader("mesh.vert", "translucentcylinder.frag");
		Debug::GLError("ERROR: Could not complete shader compilation.");
		
		ShadowMeshShader->MakeCurrent();
		glUniformMatrix4fv(ShadowMeshShader->GetUL("CubeMapViews[0]"), 1, GL_FALSE, glm::value_ptr(CubeMapViewMatrices[0]));
		glUniformMatrix4fv(ShadowMeshShader->GetUL("CubeMapViews[1]"), 1, GL_FALSE, glm::value_ptr(CubeMapViewMatrices[1]));
		glUniformMatrix4fv(ShadowMeshShader->GetUL("CubeMapViews[2]"), 1, GL_FALSE, glm::value_ptr(CubeMapViewMatrices[2]));
		glUniformMatrix4fv(ShadowMeshShader->GetUL("CubeMapViews[3]"), 1, GL_FALSE, glm::value_ptr(CubeMapViewMatrices[3]));
		glUniformMatrix4fv(ShadowMeshShader->GetUL("CubeMapViews[4]"), 1, GL_FALSE, glm::value_ptr(CubeMapViewMatrices[4]));
		glUniformMatrix4fv(ShadowMeshShader->GetUL("CubeMapViews[5]"), 1, GL_FALSE, glm::value_ptr(CubeMapViewMatrices[5]));
		ShadowSkeletalMeshShader->MakeCurrent();
		glUniformMatrix4fv(ShadowSkeletalMeshShader->GetUL("CubeMapViews[0]"), 1, GL_FALSE, glm::value_ptr(CubeMapViewMatrices[0]));
		glUniformMatrix4fv(ShadowSkeletalMeshShader->GetUL("CubeMapViews[1]"), 1, GL_FALSE, glm::value_ptr(CubeMapViewMatrices[1]));
		glUniformMatrix4fv(ShadowSkeletalMeshShader->GetUL("CubeMapViews[2]"), 1, GL_FALSE, glm::value_ptr(CubeMapViewMatrices[2]));
		glUniformMatrix4fv(ShadowSkeletalMeshShader->GetUL("CubeMapViews[3]"), 1, GL_FALSE, glm::value_ptr(CubeMapViewMatrices[3]));
		glUniformMatrix4fv(ShadowSkeletalMeshShader->GetUL("CubeMapViews[4]"), 1, GL_FALSE, glm::value_ptr(CubeMapViewMatrices[4]));
		glUniformMatrix4fv(ShadowSkeletalMeshShader->GetUL("CubeMapViews[5]"), 1, GL_FALSE, glm::value_ptr(CubeMapViewMatrices[5]));
		ShadowSphereShader->MakeCurrent();
		glUniformMatrix4fv(ShadowSphereShader->GetUL("CubeMapViews[0]"), 1, GL_FALSE, glm::value_ptr(CubeMapViewMatrices[0]));
		glUniformMatrix4fv(ShadowSphereShader->GetUL("CubeMapViews[1]"), 1, GL_FALSE, glm::value_ptr(CubeMapViewMatrices[1]));
		glUniformMatrix4fv(ShadowSphereShader->GetUL("CubeMapViews[2]"), 1, GL_FALSE, glm::value_ptr(CubeMapViewMatrices[2]));
		glUniformMatrix4fv(ShadowSphereShader->GetUL("CubeMapViews[3]"), 1, GL_FALSE, glm::value_ptr(CubeMapViewMatrices[3]));
		glUniformMatrix4fv(ShadowSphereShader->GetUL("CubeMapViews[4]"), 1, GL_FALSE, glm::value_ptr(CubeMapViewMatrices[4]));
		glUniformMatrix4fv(ShadowSphereShader->GetUL("CubeMapViews[5]"), 1, GL_FALSE, glm::value_ptr(CubeMapViewMatrices[5]));
		ShadowCylinderShader->MakeCurrent();
		glUniformMatrix4fv(ShadowCylinderShader->GetUL("CubeMapViews[0]"), 1, GL_FALSE, glm::value_ptr(CubeMapViewMatrices[0]));
		glUniformMatrix4fv(ShadowCylinderShader->GetUL("CubeMapViews[1]"), 1, GL_FALSE, glm::value_ptr(CubeMapViewMatrices[1]));
		glUniformMatrix4fv(ShadowCylinderShader->GetUL("CubeMapViews[2]"), 1, GL_FALSE, glm::value_ptr(CubeMapViewMatrices[2]));
		glUniformMatrix4fv(ShadowCylinderShader->GetUL("CubeMapViews[3]"), 1, GL_FALSE, glm::value_ptr(CubeMapViewMatrices[3]));
		glUniformMatrix4fv(ShadowCylinderShader->GetUL("CubeMapViews[4]"), 1, GL_FALSE, glm::value_ptr(CubeMapViewMatrices[4]));
		glUniformMatrix4fv(ShadowCylinderShader->GetUL("CubeMapViews[5]"), 1, GL_FALSE, glm::value_ptr(CubeMapViewMatrices[5]));
		ShadowPointCloudShader->MakeCurrent();
		glUniformMatrix4fv(ShadowPointCloudShader->GetUL("CubeMapViews[0]"), 1, GL_FALSE, glm::value_ptr(CubeMapViewMatrices[0]));
		glUniformMatrix4fv(ShadowPointCloudShader->GetUL("CubeMapViews[1]"), 1, GL_FALSE, glm::value_ptr(CubeMapViewMatrices[1]));
		glUniformMatrix4fv(ShadowPointCloudShader->GetUL("CubeMapViews[2]"), 1, GL_FALSE, glm::value_ptr(CubeMapViewMatrices[2]));
		glUniformMatrix4fv(ShadowPointCloudShader->GetUL("CubeMapViews[3]"), 1, GL_FALSE, glm::value_ptr(CubeMapViewMatrices[3]));
		glUniformMatrix4fv(ShadowPointCloudShader->GetUL("CubeMapViews[4]"), 1, GL_FALSE, glm::value_ptr(CubeMapViewMatrices[4]));
		glUniformMatrix4fv(ShadowPointCloudShader->GetUL("CubeMapViews[5]"), 1, GL_FALSE, glm::value_ptr(CubeMapViewMatrices[5]));

		// Rendering settings
		glEnable(GL_PROGRAM_POINT_SIZE);

		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);
		Debug::GLError("ERROR: Could not set OpenGL depth testing options");

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
		Debug::GLError("ERROR: Could not set OpenGL culling options");		

		// Initialize Properties
		PhysicsWorld = new DynamicsWorld();
		Gui = nullptr;
	}


	Scene::~Scene()
	{
		delete GeometryRB;
		delete LightRB;
		delete GlowMapHorizontalRB;
		delete GlowMapVerticalRB;
		delete BufferCombineRB; 

		delete DistantLightProbe;

		delete NOAAShader;
		delete FXAAShader;
		delete MeshShader;
		delete SkeletalMeshShader;
		delete SphereShader;
		delete CylinderShader;
		delete SkyboxShader;
		delete PointCloudShader;
		delete LightShader;
		delete LightWithShadowShader;
		delete LightProbeShader;
		delete BufferCombineShader;
		delete FontShader;
		delete GuiTextureShader;
		delete ShadowMeshShader;
		delete ShadowSkeletalMeshShader;
		delete ShadowSphereShader;
		delete ShadowCylinderShader;
		delete ShadowPointCloudShader;
		delete BlurShader;
		delete TranslucentMeshShader;
		delete TranslucentSphereShader;
		delete TranslucentCylinderShader;

		for (auto e : Entities)
		{
			delete e;
		}

		delete Gui;

		delete fpsText;
		delete consoleText;

		Assets::Clear();
	}

	void Scene::Load()
	{
		// Gui Objects
		Gui = new Gui::Item(nullptr, Gui::Item::Alignment::TopLeft, glm::vec2(0.0f, 0.0f), glm::vec2(Settings::Window::Width, Settings::Window::Height));
		Gui->AddItem(new Gui::Border(Gui::Border::Orientation::Horizontal, Gui, Gui::Item::Alignment::TopLeft, glm::vec2(1, 1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
		Gui->AddItem(new Gui::Border(Gui::Border::Orientation::Vertical, Gui, Gui::Item::Alignment::TopLeft, glm::vec2(1, 1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
		Gui->AddItem(new Gui::Border(Gui::Border::Orientation::Horizontal, Gui, Gui::Item::Alignment::BottomRight, glm::vec2(-1, -1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
		Gui->AddItem(new Gui::Border(Gui::Border::Orientation::Vertical, Gui, Gui::Item::Alignment::BottomRight, glm::vec2(-1, -1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));

		Gui->AddItem(new Gui::ExitButton(Window, Gui, Gui::Item::Alignment::TopRight, glm::vec2(-10, 10), glm::vec2(17, 17), nullptr, Assets::Textures["CloseButton"], Assets::Textures["CloseButtonMO"]));
		Gui->AddItem(new Gui::MaximizeButton(Window, Gui, Gui::Item::Alignment::TopRight, glm::vec2(-37, 10), glm::vec2(17, 17), Assets::Textures["WindowedButton"], Assets::Textures["WindowedButtonMO"], nullptr, Assets::Textures["MaximizeButton"], Assets::Textures["MaximizeButtonMO"]));
		Gui->AddItem(new Gui::MinimizeButton(Window, Gui, Gui::Item::Alignment::TopRight, glm::vec2(-64, 10), glm::vec2(17, 17), nullptr, Assets::Textures["MinimizeButton"], Assets::Textures["MinimizeButtonMO"]));
		Gui->AddItem(new Gui::ResizeButton(Window, Gui, Gui::Item::Alignment::BottomRight, glm::vec2(-5, -5), glm::vec2(17, 17), nullptr, Assets::Textures["ResizeButton"], Assets::Textures["ResizeButtonMO"]));
		Gui->AddItem(new Gui::MoveButton(Window, Gui, Gui::Item::Alignment::TopCenter, glm::vec2(0, 0), glm::vec2(100, 37)));
		Gui->Invalidate();

		fpsText = new Text("FPS: " + std::to_string((int)Time::FPS), 0, 0, Assets::Fonts["Consolas16"]);
		auto fpsi = new Gui::Item(Gui, Gui::Item::Alignment::BottomLeft, glm::vec2(10, (Settings::Video::ShowFPS) ? -10 : 42), glm::vec2(Settings::Window::Width - 20, 42), fpsText);
		Window->Input->SetFPSDisplay(fpsi);
		Gui->AddItem(fpsi);

		consoleText = new Text("", 0, 0, Assets::Fonts["Consolas16"]);
		auto ci = new Gui::Item(Gui, Gui::Item::Alignment::TopLeft, glm::vec2(10, (Settings::Misc::ShowConsole) ? 10 : -80), glm::vec2(Settings::Window::Width - 20, 80), consoleText);
		Window->Input->SetConsole(ci);
		Gui->AddItem(ci);

		/*
		// Editor Specific Gui Objects
		Gui->AddItem(new Gui::ImportButton(Window, Gui, Gui::Item::Alignment::TopRight, glm::vec2(-20, 47), glm::vec2(32, 32), nullptr, Assets::Textures["In"], Assets::Textures["InMO"]));

		Assets::Textures["Camera"] = new Texture;
		Assets::Textures["Camera"]->LoadFromPNG("Camera", 32, 32);
		Assets::Textures["CameraMO"] = new Texture;
		Assets::Textures["CameraMO"]->LoadFromPNG("CameraMO", 32, 32);
		Assets::Textures["GameCamera"] = new Texture;
		Assets::Textures["GameCamera"]->LoadFromPNG("GameCamera", 32, 32);
		Assets::Textures["GameCameraMO"] = new Texture;
		Assets::Textures["GameCameraMO"]->LoadFromPNG("GameCameraMO", 32, 32);
		Gui->AddItem(new Gui::CameraToggleButton(Window, Gui, Gui::Item::Alignment::BottomRight, glm::vec2(-27, -6), glm::vec2(16, 16), nullptr, Assets::Textures["Camera"], Assets::Textures["CameraMO"], nullptr, Assets::Textures["GameCamera"], Assets::Textures["GameCameraMO"]));

		Gui->AddItem(new Gui::MaterialButton(Window, Gui, Gui::Item::Alignment::TopRight, glm::vec2(-72, 47), glm::vec2(32, 32), nullptr, Assets::Textures["Material"], Assets::Textures["MaterialMO"]));

		Assets::Textures["CharacterButton"] = new Texture;
		Assets::Textures["CharacterButton"]->LoadFromPNG("Character", 32, 32);
		Assets::Textures["CharacterButtonMO"] = new Texture;
		Assets::Textures["CharacterButtonMO"]->LoadFromPNG("CharacterMO", 32, 32);
		Gui->AddItem(new Gui::CharacterButton(Window, Gui, Gui::Item::Alignment::TopRight, glm::vec2(-124, 47), glm::vec2(32, 32), nullptr, Assets::Textures["CharacterButton"], Assets::Textures["CharacterButtonMO"]));
		*/

		Cube = Assets::Meshes["Cube"];
		Cylinder = Assets::Meshes["Cylinder"];
		Sphere = Assets::Meshes["Sphere"];
		
		LoadScene("");

		// Must be after camera is created
		ResizeRenderBuffers();

		for (auto e : Entities)
			e->Load();
		
		if (Settings::Misc::VerboseLogging)
		{
			Debug::Log("Done loading scene:");
			Debug::Log("\t" + std::to_string(Entities.size()) + " entities.");
			Debug::Log("\t" + std::to_string(Lights.size()) + " lights.");
		}
	}

	void Scene::Update()
	{
		// Order of Operations:
		// 1. Tick Physics
		// 2. Update all entities
		// 3. Render Scene
		PhysicsWorld->Update();

		for (auto e : Entities)
			e->Update();
		
		AudioListener.Update();
		P = Camera->GetProjectionMatrix();
		V = Camera->GetViewMatrix();
		PInverse = glm::inverse(P);
		VInverse = glm::inverse(V);
		
		RenderShadows();
		RenderGeometry();
		RenderLight();
		RenderBloom();
		CombineGeometryAndLight();
		RenderTranslucent();
		RenderPost();
		RenderUI();
	}
	

	void Scene::RenderGeometry()
	{
		GeometryRB->MakeCurrent();
		GeometryRB->Clear();

		TranslucentEntities.clear();

		for (auto e : Entities)
		{
			if (DistantEntities.count(e) > 0)
				continue;

			if (e->IsRenderable()) {
				
				auto mat = e->GetComponent<Material>();
				if (mat != nullptr && mat->IsTranslucent)
				{
					TranslucentEntities[glm::distance(e->Transform.Position, Camera->Entity->Transform.Position)] = e;
					continue;
				}

				glm::mat4 M = e->Transform.ToMatrix();
				glm::mat4 MV = V * M;
				glm::mat4 MVP = P * MV;

				auto anim = e->GetComponent<SkeletalAnimation>();
				auto pc = e->GetComponent<PointCloud>();
				if (anim != nullptr)
				{
					SkeletalMeshShader->MakeCurrent();

					glUniformMatrix4fv(SkeletalMeshShader->GetUL("ModelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
					glUniformMatrix4fv(SkeletalMeshShader->GetUL("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(MV));
					glUniformMatrix4fv(SkeletalMeshShader->GetUL("ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(P));
					
					anim->Render(SkeletalMeshShader);
				}
				else if (pc != nullptr)
				{
					PointCloudShader->MakeCurrent();

					glUniformMatrix4fv(PointCloudShader->GetUL("ModelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
					glUniformMatrix4fv(PointCloudShader->GetUL("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(MV));
					glUniformMatrix4fv(PointCloudShader->GetUL("ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(P));

					mat->WriteShaderUniforms(PointCloudShader);

					pc->ViewPosition = Camera->Entity->Transform.Position;
					pc->EnableBuffers(PointCloudShader);
					pc->Render(PointCloudShader);
					pc->DisableBuffers(PointCloudShader);
				}
				else // Mesh
				{
					auto r = e->GetRenderable();
					if (r == Sphere)
					{
						SphereShader->MakeCurrent();

						mat->WriteShaderUniforms(SphereShader);

						glUniformMatrix4fv(SphereShader->GetUL("ModelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
						glUniformMatrix4fv(SphereShader->GetUL("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(MV));
						glUniformMatrix4fv(SphereShader->GetUL("ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(P));
												
						glUniform3fv(SphereShader->GetUL("SpherePosition"), 1, glm::value_ptr(glm::vec3(V * glm::vec4(e->Transform.Position, 1.0f))));
						glUniform1f(SphereShader->GetUL("Radius"), r->Entity->Transform.Scale.x / 2);

						r->EnableBuffers(SphereShader);
						r->Render(SphereShader);
						r->DisableBuffers(SphereShader);
					}
					else if (r == Cylinder)
					{
						CylinderShader->MakeCurrent();

						glUniformMatrix4fv(CylinderShader->GetUL("ModelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
						glUniformMatrix4fv(CylinderShader->GetUL("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(MV));
						glUniformMatrix4fv(CylinderShader->GetUL("ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(P));

						mat->WriteShaderUniforms(CylinderShader);

						glUniform3fv(CylinderShader->GetUL("Direction"), 1, glm::value_ptr(glm::normalize(glm::vec3(MV * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)))));
						glUniform3fv(CylinderShader->GetUL("Center"), 1, glm::value_ptr(glm::vec3(V * glm::vec4(e->Transform.Position, 1.0f))));
						glUniform1f(CylinderShader->GetUL("Length"), r->Entity->Transform.Scale.y);
						glUniform1f(CylinderShader->GetUL("Radius"), r->Entity->Transform.Scale.x / 2);
					
						r->EnableBuffers(CylinderShader);
						r->Render(CylinderShader);
						r->DisableBuffers(CylinderShader);
					}
					else
					{
						MeshShader->MakeCurrent();

						glUniformMatrix4fv(MeshShader->GetUL("ModelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
						glUniformMatrix4fv(MeshShader->GetUL("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(MV));
						glUniformMatrix4fv(MeshShader->GetUL("ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(P));

						mat->WriteShaderUniforms(MeshShader);

						r->EnableBuffers(MeshShader);
						r->Render(MeshShader);
						r->DisableBuffers(MeshShader);
					}
				}

			}
		}
		
		// Render Skybox
		glCullFace(GL_FRONT);
		SkyboxShader->MakeCurrent();
		glUniformMatrix4fv(SkyboxShader->GetUL("ModelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(P * V * Camera->Entity->Transform.ToMatrix() * glm::scale(glm::vec3(Settings::Video::MaxDrawDistance))));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, Assets::Textures["Skybox"]->GetID());
		glUniform1i(SkyboxShader->GetUL("gCubemapTexture"), 0);
		
		Cube->EnableBuffers(SkyboxShader);
		Cube->Render(SkyboxShader);
		Cube->DisableBuffers(SkyboxShader);
		glCullFace(GL_BACK);
	}


	void Scene::RenderShadows()
	{		
		//glCullFace(GL_FRONT);
		

		for (auto l : Lights)
		{
			if (l->CastsShadow())
			{
				l->ShadowRB->MakeCurrent();
				l->ShadowRB->Clear();
				Debug::GLError("ERROR: Could not initialize the shadow framebuffer.");


				l->Projection = glm::perspective(90.0f, 1.0f, 0.01f, l->Radius);	

				glm::mat4 LV = glm::translate(-l->Entity->Transform.Position);
				
					
				// Get objects within light range
				auto objs = l->GetLitObjects();

				// Render them
				for (auto e : *objs)
				{
				// Render everything
				//for (auto e : Entities)
				//{
					if (e->IsRenderable() && e != l->Entity) {

						glm::mat4 M = e->Transform.ToMatrix();
						glm::mat4 MV = LV * M;
						glm::mat4 MVP = MV;

						auto anim = e->GetComponent<SkeletalAnimation>();
						auto pc = e->GetComponent<PointCloud>();
						if (anim != nullptr)
						{
							ShadowSkeletalMeshShader->MakeCurrent();

							glUniformMatrix4fv(ShadowSkeletalMeshShader->GetUL("CubeMapProjections[0]"), 1, GL_FALSE, glm::value_ptr(l->Projection * CubeMapViewMatrices[0]));
							glUniformMatrix4fv(ShadowSkeletalMeshShader->GetUL("CubeMapProjections[1]"), 1, GL_FALSE, glm::value_ptr(l->Projection * CubeMapViewMatrices[1]));
							glUniformMatrix4fv(ShadowSkeletalMeshShader->GetUL("CubeMapProjections[2]"), 1, GL_FALSE, glm::value_ptr(l->Projection * CubeMapViewMatrices[2]));
							glUniformMatrix4fv(ShadowSkeletalMeshShader->GetUL("CubeMapProjections[3]"), 1, GL_FALSE, glm::value_ptr(l->Projection * CubeMapViewMatrices[3]));
							glUniformMatrix4fv(ShadowSkeletalMeshShader->GetUL("CubeMapProjections[4]"), 1, GL_FALSE, glm::value_ptr(l->Projection * CubeMapViewMatrices[4]));
							glUniformMatrix4fv(ShadowSkeletalMeshShader->GetUL("CubeMapProjections[5]"), 1, GL_FALSE, glm::value_ptr(l->Projection * CubeMapViewMatrices[5]));

							glUniform1f(ShadowSkeletalMeshShader->GetUL("MaxDepth"), l->Radius);

							glUniformMatrix4fv(ShadowSkeletalMeshShader->GetUL("ModelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
							glUniformMatrix4fv(ShadowSkeletalMeshShader->GetUL("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(MV));

							anim->Render(ShadowSkeletalMeshShader);
						}
						else if (pc != nullptr)
						{
							/*
							ShadowPointCloudShader->MakeCurrent();

							glUniformMatrix4fv(ShadowPointCloudShader->GetUL("CubeMapProjections[0]"), 1, GL_FALSE, glm::value_ptr(l->Projection * CubeMapViewMatrices[0]));
							glUniformMatrix4fv(ShadowPointCloudShader->GetUL("CubeMapProjections[1]"), 1, GL_FALSE, glm::value_ptr(l->Projection * CubeMapViewMatrices[1]));
							glUniformMatrix4fv(ShadowPointCloudShader->GetUL("CubeMapProjections[2]"), 1, GL_FALSE, glm::value_ptr(l->Projection * CubeMapViewMatrices[2]));
							glUniformMatrix4fv(ShadowPointCloudShader->GetUL("CubeMapProjections[3]"), 1, GL_FALSE, glm::value_ptr(l->Projection * CubeMapViewMatrices[3]));
							glUniformMatrix4fv(ShadowPointCloudShader->GetUL("CubeMapProjections[4]"), 1, GL_FALSE, glm::value_ptr(l->Projection * CubeMapViewMatrices[4]));
							glUniformMatrix4fv(ShadowPointCloudShader->GetUL("CubeMapProjections[5]"), 1, GL_FALSE, glm::value_ptr(l->Projection * CubeMapViewMatrices[5]));

							glUniform1f(ShadowPointCloudShader->GetUL("MaxDepth"), l->Radius);

							glUniformMatrix4fv(ShadowPointCloudShader->GetUL("ModelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
							glUniformMatrix4fv(ShadowPointCloudShader->GetUL("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(MV));

							pc->EnableBuffers(ShadowPointCloudShader);
							pc->Render(ShadowPointCloudShader);
							pc->DisableBuffers(ShadowPointCloudShader);
							*/
						}
						else // Mesh
						{								
							auto r = e->GetRenderable();
							if (r == Sphere)
							{	
								ShadowSphereShader->MakeCurrent();

								glUniformMatrix4fv(ShadowSphereShader->GetUL("ModelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
								glUniformMatrix4fv(ShadowSphereShader->GetUL("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(MV));
								glUniformMatrix4fv(ShadowSphereShader->GetUL("ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(l->Projection));
								glUniform3fv(ShadowSphereShader->GetUL("SpherePosition"), 1, glm::value_ptr(glm::vec3(V * glm::vec4(e->Transform.Position, 1.0f))));
								glUniform1f(ShadowSphereShader->GetUL("Radius"), r->Entity->Transform.Scale.x / 2);

								r->EnableBuffers(ShadowSphereShader);
								r->Render(ShadowSphereShader);
								r->DisableBuffers(ShadowSphereShader);								
							}
							else if (r == Cylinder)
							{
								ShadowCylinderShader->MakeCurrent();

								glUniformMatrix4fv(ShadowCylinderShader->GetUL("ModelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
								glUniformMatrix4fv(ShadowCylinderShader->GetUL("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(MV));
								glUniformMatrix4fv(ShadowCylinderShader->GetUL("ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(l->Projection));
								glUniform3fv(ShadowCylinderShader->GetUL("Direction"), 1, glm::value_ptr(glm::normalize(glm::vec3(MV * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)))));
								glUniform3fv(ShadowCylinderShader->GetUL("Center"), 1, glm::value_ptr(glm::vec3(V * glm::vec4(e->Transform.Position, 1.0f))));
								glUniform1f(ShadowCylinderShader->GetUL("Length"), r->Entity->Transform.Scale.y);
								glUniform1f(ShadowCylinderShader->GetUL("Radius"), r->Entity->Transform.Scale.x / 2);

								r->EnableBuffers(ShadowCylinderShader);
								r->Render(ShadowCylinderShader);
								r->DisableBuffers(ShadowCylinderShader);
							}
							else
							{
								ShadowMeshShader->MakeCurrent();

								glUniformMatrix4fv(ShadowMeshShader->GetUL("CubeMapProjections[0]"), 1, GL_FALSE, glm::value_ptr(l->Projection * CubeMapViewMatrices[0]));
								glUniformMatrix4fv(ShadowMeshShader->GetUL("CubeMapProjections[1]"), 1, GL_FALSE, glm::value_ptr(l->Projection * CubeMapViewMatrices[1]));
								glUniformMatrix4fv(ShadowMeshShader->GetUL("CubeMapProjections[2]"), 1, GL_FALSE, glm::value_ptr(l->Projection * CubeMapViewMatrices[2]));
								glUniformMatrix4fv(ShadowMeshShader->GetUL("CubeMapProjections[3]"), 1, GL_FALSE, glm::value_ptr(l->Projection * CubeMapViewMatrices[3]));
								glUniformMatrix4fv(ShadowMeshShader->GetUL("CubeMapProjections[4]"), 1, GL_FALSE, glm::value_ptr(l->Projection * CubeMapViewMatrices[4]));
								glUniformMatrix4fv(ShadowMeshShader->GetUL("CubeMapProjections[5]"), 1, GL_FALSE, glm::value_ptr(l->Projection * CubeMapViewMatrices[5]));

								glUniform1f(ShadowMeshShader->GetUL("MaxDepth"), l->Radius);

								glUniformMatrix4fv(ShadowMeshShader->GetUL("ModelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
								glUniformMatrix4fv(ShadowMeshShader->GetUL("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(MV));

								r->EnableBuffers(ShadowMeshShader);
								r->Render(ShadowMeshShader);
								r->DisableBuffers(ShadowMeshShader);
							}
						}

					}
				}
			}
		}

		//glCullFace(GL_BACK);
	}


	void Scene::RenderLight()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);	// Additive Blending


		LightRB->MakeCurrent();
		LightRB->Clear();


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, GeometryRB->GetDepthTexture());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, GeometryRB->GetOutputTexture(1));
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, GeometryRB->GetOutputTexture(2));
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, GeometryRB->GetOutputTexture(0));
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_CUBE_MAP, Assets::Textures["Skybox"]->GetID());


		LightShader->MakeCurrent();
		glUniform1i(LightShader->GetUL("DepthTexture"), 0);
		glUniform1i(LightShader->GetUL("NormalTexture"), 1);
		glUniform1i(LightShader->GetUL("BaseTexture"), 3);
		glUniform1i(LightShader->GetUL("MSRTexture"), 2);
		glUniform2f(LightShader->GetUL("PixelSize"), 1.0f / (float)(Settings::Window::Width), 1.0f / (float)(Settings::Window::Height));
		glUniformMatrix4fv(LightShader->GetUL("ProjectionInverse"), 1, GL_FALSE, glm::value_ptr(PInverse));



		LightWithShadowShader->MakeCurrent();
		glUniform1i(LightWithShadowShader->GetUL("DepthTexture"), 0);
		glUniform1i(LightWithShadowShader->GetUL("NormalTexture"), 1);
		glUniform1i(LightWithShadowShader->GetUL("BaseTexture"), 3);
		glUniform1i(LightWithShadowShader->GetUL("MSRTexture"), 2);
		glUniform2f(LightWithShadowShader->GetUL("PixelSize"), 1.0f / (float)(Settings::Window::Width), 1.0f / (float)(Settings::Window::Height));
		glUniformMatrix4fv(LightWithShadowShader->GetUL("ProjectionInverse"), 1, GL_FALSE, glm::value_ptr(PInverse));
		glUniformMatrix4fv(LightWithShadowShader->GetUL("ViewInverse"), 1, GL_FALSE, glm::value_ptr(VInverse));



		LightProbeShader->MakeCurrent();
		glUniform1i(LightProbeShader->GetUL("DepthTexture"), 0);
		glUniform1i(LightProbeShader->GetUL("NormalTexture"), 1);
		glUniform1i(LightProbeShader->GetUL("BaseTexture"), 3);
		glUniform1i(LightProbeShader->GetUL("MSRTexture"), 2);
		glUniform1i(LightProbeShader->GetUL("DistantProbe"), 5);
		glUniform2f(LightProbeShader->GetUL("PixelSize"), 1.0f / (float)(Settings::Window::Width), 1.0f / (float)(Settings::Window::Height));
		glUniformMatrix4fv(LightProbeShader->GetUL("ProjectionInverse"), 1, GL_FALSE, glm::value_ptr(PInverse));
		glUniformMatrix4fv(LightProbeShader->GetUL("ViewInverse"), 1, GL_FALSE, glm::value_ptr(VInverse));


		glCullFace(GL_FRONT);

		for (auto r : Lights)
		{
			auto e = r->Entity;
			glm::mat4 M = glm::translate(e->Transform.Position) * glm::scale(glm::vec3(r->Radius) * 2.0f);
			glm::mat4 MV = V * M;
			glm::mat4 MVP = P * MV;

			if (r->CastsShadow() && r->GetLitObjects()->size() > 0)
			{
				LightWithShadowShader->MakeCurrent();
				
				glActiveTexture(GL_TEXTURE4);
				glBindTexture(GL_TEXTURE_CUBE_MAP, r->GetShadowTexture());
				glUniform1i(LightWithShadowShader->GetUL("ShadowTexture"), 4);

				glUniformMatrix4fv(LightWithShadowShader->GetUL("ModelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
				glUniformMatrix4fv(LightWithShadowShader->GetUL("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(MV));
				glUniform3fv(LightWithShadowShader->GetUL("LightPosition"), 1, glm::value_ptr(glm::vec3(V * glm::vec4(e->Transform.Position, 1.0))));
				glUniform3fv(LightWithShadowShader->GetUL("LightDirection"), 1, glm::value_ptr(e->Transform.WSForward()));

				glUniformMatrix4fv(LightWithShadowShader->GetUL("LightViewMatrix"), 1, GL_FALSE, glm::value_ptr(glm::translate(-e->Transform.Position)));
				glUniformMatrix4fv(LightWithShadowShader->GetUL("LightProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(r->Projection));

				r->WriteShaderUniforms(LightWithShadowShader);

				Cube->EnableBuffers(LightWithShadowShader);
				Cube->Render(LightWithShadowShader);
				Cube->DisableBuffers(LightWithShadowShader);
			}
			else
			{
				LightShader->MakeCurrent();

				glUniformMatrix4fv(LightShader->GetUL("ModelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
				glUniformMatrix4fv(LightShader->GetUL("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(MV));
				glUniform3fv(LightShader->GetUL("LightPosition"), 1, glm::value_ptr(glm::vec3(V * glm::vec4(e->Transform.Position, 1.0))));
				glUniform3fv(LightShader->GetUL("LightDirection"), 1, glm::value_ptr(e->Transform.WSForward()));

				r->WriteShaderUniforms(LightShader);

				Cube->EnableBuffers(LightShader);
				Cube->Render(LightShader);
				Cube->DisableBuffers(LightShader);
			}

		}

		// Light Probe
		LightProbeShader->MakeCurrent();

		glm::mat4 M = Camera->Entity->Transform.ToMatrix() * glm::scale(glm::vec3(Settings::Video::MaxDrawDistance));
		glm::mat4 MV = V * M;
		glm::mat4 MVP = P * MV;
		glUniformMatrix4fv(LightProbeShader->GetUL("ModelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
		glUniformMatrix4fv(LightProbeShader->GetUL("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(MV));

		Cube->EnableBuffers(LightProbeShader);
		Cube->Render(LightProbeShader);
		Cube->DisableBuffers(LightProbeShader);
		

		glCullFace(GL_BACK);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// Reset Blending

	}

	
	void Scene::RenderBloom()
	{
		GlowMapHorizontalRB->MakeCurrent();
		GlowMapHorizontalRB->Clear();

		BlurShader->MakeCurrent();
		glUniform2f(BlurShader->GetUL("texelSize"), 1.0f / ((float)Settings::Window::Width), 1.0f / ((float)Settings::Window::Height));
				
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, GeometryRB->GetOutputTexture(4));
		glUniform1i(BlurShader->GetUL("textureSample"), 0);

		glUniform1i(BlurShader->GetUL("blurAmount"), 32);
		glUniform1f(BlurShader->GetUL("blurScale"), 1.0f);
		glUniform1f(BlurShader->GetUL("blurStrength"), 0.2f);

		glUniform1i(BlurShader->GetUL("orientation"), 0);

		SQuad.Render();

		GlowMapVerticalRB->MakeCurrent();
		GlowMapVerticalRB->Clear();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, GlowMapHorizontalRB->GetOutputTexture(0));
		glUniform1i(BlurShader->GetUL("textureSample"), 0);

		glUniform1i(BlurShader->GetUL("blurAmount"), 32);
		glUniform1f(BlurShader->GetUL("blurScale"), 1.0f);
		glUniform1f(BlurShader->GetUL("blurStrength"), 0.2f);

		glUniform1i(BlurShader->GetUL("orientation"), 1);

		SQuad.Render();
				

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// Reset Blending
	}
	

	void Scene::CombineGeometryAndLight()
	{
		// Combine render buffer output into final texture
		BufferCombineRB->MakeCurrent();
		BufferCombineRB->Clear();
		
		BufferCombineShader->MakeCurrent();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, GeometryRB->GetOutputTexture(0));
		glUniform1i(BufferCombineShader->GetUL("BaseTexture"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, GeometryRB->GetOutputTexture(2));
		glUniform1i(BufferCombineShader->GetUL("MSRTexture"), 1);

		//glActiveTexture(GL_TEXTURE2);
		//glBindTexture(GL_TEXTURE_2D, GeometryRB->GetOutputTexture(3));
		//glUniform1i(BufferCombineShader->GetUL("SpecularHardnessTexture"), 2);

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, LightRB->GetOutputTexture(0));
		glUniform1i(BufferCombineShader->GetUL("DiffuseLightTexture"), 3);

		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, LightRB->GetOutputTexture(1));
		glUniform1i(BufferCombineShader->GetUL("SpecularLightTexture"), 4);

		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, GlowMapVerticalRB->GetOutputTexture(0));
		glUniform1i(BufferCombineShader->GetUL("GlowTexture"), 5);

		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, GeometryRB->GetOutputTexture(4));
		glUniform1i(BufferCombineShader->GetUL("EmissiveTexture"), 6);
				
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, GeometryRB->GetOutputTexture(5));
		glUniform1i(BufferCombineShader->GetUL("SkyboxTexture"), 7);

		SQuad.Render();
	}


	void Scene::RenderTranslucent()
	{
		BufferCombineRB->MakeCurrent();
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, GeometryRB->GetDepthTexture());

		TranslucentMeshShader->MakeCurrent();
		glUniform1i(TranslucentMeshShader->GetUL("DepthTexture"), 0);
		glUniform2f(TranslucentMeshShader->GetUL("PixelSize"), 1.0f / (float)(Settings::Window::Width), 1.0f / (float)(Settings::Window::Height));
		glUniformMatrix4fv(TranslucentMeshShader->GetUL("ProjectionInverse"), 1, GL_FALSE, glm::value_ptr(PInverse));
		TranslucentSphereShader->MakeCurrent();
		glUniform1i(TranslucentSphereShader->GetUL("DepthTexture"), 0);
		glUniform2f(TranslucentSphereShader->GetUL("PixelSize"), 1.0f / (float)(Settings::Window::Width), 1.0f / (float)(Settings::Window::Height));
		glUniformMatrix4fv(TranslucentSphereShader->GetUL("ProjectionInverse"), 1, GL_FALSE, glm::value_ptr(PInverse));
		TranslucentCylinderShader->MakeCurrent();
		glUniform1i(TranslucentCylinderShader->GetUL("DepthTexture"), 0);
		glUniform2f(TranslucentCylinderShader->GetUL("PixelSize"), 1.0f / (float)(Settings::Window::Width), 1.0f / (float)(Settings::Window::Height));
		glUniformMatrix4fv(TranslucentCylinderShader->GetUL("ProjectionInverse"), 1, GL_FALSE, glm::value_ptr(PInverse));
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Sort Translucent entities, farthest to closest from camera position

		for (auto pair : TranslucentEntities)
		{
			auto e = pair.second;
			auto mat = e->GetComponent<Material>();
			
			glm::mat4 M = e->Transform.ToMatrix();
			glm::mat4 MV = V * M;
			glm::mat4 MVP = P * MV;

			// Mesh
			auto r = e->GetRenderable();
			if (r == Sphere)
			{
				TranslucentSphereShader->MakeCurrent();

				mat->WriteShaderUniforms(TranslucentSphereShader);

				glUniformMatrix4fv(TranslucentSphereShader->GetUL("ModelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
				glUniformMatrix4fv(TranslucentSphereShader->GetUL("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(MV));
				glUniformMatrix4fv(TranslucentSphereShader->GetUL("ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(P));

				glUniform3fv(TranslucentSphereShader->GetUL("SpherePosition"), 1, glm::value_ptr(glm::vec3(V * glm::vec4(e->Transform.Position, 1.0f))));
				glUniform1f(TranslucentSphereShader->GetUL("Radius"), r->Entity->Transform.Scale.x / 2);

				r->EnableBuffers(TranslucentSphereShader);
				r->Render(TranslucentSphereShader);
				r->DisableBuffers(TranslucentSphereShader);
			}
			else if (r == Cylinder)
			{
				TranslucentCylinderShader->MakeCurrent();

				glUniformMatrix4fv(TranslucentCylinderShader->GetUL("ModelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
				glUniformMatrix4fv(TranslucentCylinderShader->GetUL("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(MV));
				glUniformMatrix4fv(TranslucentCylinderShader->GetUL("ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(P));

				mat->WriteShaderUniforms(TranslucentCylinderShader);

				glUniform3fv(TranslucentCylinderShader->GetUL("Direction"), 1, glm::value_ptr(glm::normalize(glm::vec3(MV * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)))));
				glUniform3fv(TranslucentCylinderShader->GetUL("Center"), 1, glm::value_ptr(glm::vec3(V * glm::vec4(e->Transform.Position, 1.0f))));
				glUniform1f(TranslucentCylinderShader->GetUL("Length"), r->Entity->Transform.Scale.y);
				glUniform1f(TranslucentCylinderShader->GetUL("Radius"), r->Entity->Transform.Scale.x / 2);

				r->EnableBuffers(TranslucentCylinderShader);
				r->Render(TranslucentCylinderShader);
				r->DisableBuffers(TranslucentCylinderShader);
			}
			else
			{
				TranslucentMeshShader->MakeCurrent();

				glUniformMatrix4fv(TranslucentMeshShader->GetUL("ModelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
				glUniformMatrix4fv(TranslucentMeshShader->GetUL("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(MV));
				glUniformMatrix4fv(TranslucentMeshShader->GetUL("ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(P));

				mat->WriteShaderUniforms(TranslucentMeshShader);

				r->EnableBuffers(TranslucentMeshShader);
				r->Render(TranslucentMeshShader);
				r->DisableBuffers(TranslucentMeshShader);
			}
		}
	}


	void Scene::RenderPost()
	{
		// Perform antialiasing pass(es)
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		if (Settings::Video::FXAA > 0)
		{
			FXAAShader->MakeCurrent();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, BufferCombineRB->GetOutputTexture(0));
			glUniform1i(FXAAShader->GetUL("sourceTexture"), 0);
			glUniform2f(FXAAShader->GetUL("frameSize"), (GLfloat)Settings::Window::Width, (GLfloat)Settings::Window::Height);
			SQuad.Render();
		}
		else {
			NOAAShader->MakeCurrent();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, BufferCombineRB->GetOutputTexture(0));
			glUniform1i(NOAAShader->GetUL("sourceTexture"), 0);
			SQuad.Render();
		}
		
		glUseProgram(0);
	}

	
	void Scene::RenderUI()
	{
		glViewport(0, 0, Settings::Window::Width, Settings::Window::Height);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_DEPTH_TEST);

		//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		//glEnable(GL_BLEND);
		//glDisable(GL_ALPHA_TEST);

		if (Settings::Video::ShowFPS)
		{
			fpsText->UpdateText("TS: " + std::to_string(Time::Scale) + "\nDPF: " + std::to_string(Time::DrawCallsPerFrame) + "\nFPS : " + std::to_string((int)Time::FPS));
		}

		Gui->Render(GuiTextureShader, FontShader);
		

		//glDisable(GL_BLEND);
		//glEnable(GL_ALPHA_TEST);
		glEnable(GL_DEPTH_TEST);
		Debug::GLError("ERROR: failed to render ui.");
	}


	void Scene::ResizeRenderBuffers()
	{
		GeometryRB->Rebuild();
		LightRB->Rebuild();
		BufferCombineRB->Rebuild(); 
		GlowMapHorizontalRB->Rebuild();
		GlowMapVerticalRB->Rebuild();

		for (auto e : Entities)
		{
			auto c = e->GetComponent<Core::Camera>();
			if (c != nullptr)
			{
				c->UpdateProjection();
			}
		}
	}
	

	void Scene::SetActiveCamera(Core::Camera* camera)
	{
		Camera = camera;
	}


	void Scene::AddEntity(Entity* e)
	{
		Entities.push_back(e);

		auto ls = e->GetComponent<LightSource>();
		if (ls != nullptr)
			AddLight(ls);
	}


	void Scene::RemoveEntity(Entity* e)
	{
		auto ls = e->GetComponent<LightSource>();
		if (ls != nullptr)
			RemoveLight(ls);

		int i = 0;
		while (i < Entities.size())
		{
			if (Entities[i] == e)
				break;
			i++;
		}
		if (i == Entities.size())
			return;

		Entities[i] = Entities.back();
		Entities.resize(Entities.size() - 1);
	}


	void Scene::AddLight(LightSource* l)
	{
		Lights.push_back(l);
	}

	void Scene::RemoveLight(LightSource* e)
	{
		int i = 0;
		while (i < Lights.size())
		{
			if (Lights[i] == e)
				break;
			i++;
		}
		if (i == Lights.size())
			return;

		Lights[i] = Lights.back();
		Lights.resize(Lights.size() - 1);
	}

	
	void Scene::AppendConsole(std::string msg)
	{
		consoleLines.push_back(msg);
		if (consoleLines.size() > 3)
		{
			consoleLines.pop_front();
		}

		std::string text = "";
		for (auto s : consoleLines)
		{
			text += s + "\n";
		}

		consoleText->UpdateText(text);
	}

	
	void Scene::LoadScene(std::string sceneFile)
	{
		// Load Scene Objects

		// Camera
		auto c = new Entity();
		c->Transform.Position = glm::vec3(0.0f, 2.0f, -10.0f);
		Camera = new Core::Camera();
		c->AddComponent(Camera);
		Window->Input->SetCameraEntity(c);
		AudioListener.SetCurrentListener(c);
		AddEntity(c);

		Entity* e;
		RigidBody* fb;

		// Camera Target Center Point
		e = new Entity();
		e->Transform.Position = glm::vec3(0.0f, 1.0f, 0.0f);
		CameraTarget = new Core::CameraTarget(nullptr);
		e->AddComponent(CameraTarget);
		Window->Input->SetTargetEntity(e);
		AddEntity(e);

		Entity* l = new Entity();
		l->Transform.Position = glm::vec3(-1.0f, 1.5f, 0.0f);
		l->Transform.Scale = glm::vec3(0.05f, 0.05f, 0.05f);
		l->AddComponent(new LightSource(this, glm::vec3(1.0f), 20.0f, 5.0f, -1.0f, -1.0f, true));
		l->AddComponent(new OrbitingPosition(e, glm::vec3(0.0f, 0.0f, 1.0f), 1.0f));
		l->AddComponent(Assets::Meshes["Sphere"]);
		l->AddComponent(Assets::Materials["Gold"]);
		AddEntity(l);
				

		e = new Entity();
		e->Transform.Position = glm::vec3(0.0f, 5.0f, -2.0f);
		e->Transform.Scale = glm::vec3(1.0f, 1.0f, 1.0f);
		e->Transform.Rotation = glm::quat(glm::vec3(45.0f, 0.0f, 45.0f));
		e->AddComponent(Assets::Meshes["Cube"]);
		e->AddComponent(Assets::Materials["Gold"]);
		fb = new RigidBody(PhysicsWorld, Assets::Materials["Gold"], new btBoxShape(btVector3(1.0f, 1.0f, 1.0f) * 0.5f), glm::vec3(), Assets::Materials["Gold"]->Density, RigidBody::Type::DYNAMIC);
		e->AddComponent(fb);
		AddEntity(e);

		// Ground Collider
		e = new Entity();
		e->Transform.Position = glm::vec3(0.0f, -5.0f, 0.0f);
		e->Transform.Scale = glm::vec3(100.0f, 10.0f, 100.0f);
		btCollisionShape* shape = new btBoxShape(btVector3(e->Transform.Scale.x, e->Transform.Scale.y, e->Transform.Scale.z) * 0.5f);
		e->AddComponent(new RigidBody(PhysicsWorld, Assets::Materials["Gold"], shape));
		e->AddComponent(Assets::Meshes["Cube"]);
		e->AddComponent(Assets::Materials["Gold"]);
		AddEntity(e);

		// Point Cloud Test
		e = new Entity();
		e->Transform.Position = glm::vec3(2.0f, 2.0f, 2.0f);
		e->Transform.Scale = glm::vec3(1.0f, 1.0f, 1.0f);
		e->Transform.Rotation = glm::quat(glm::vec3(45.0f, 0.0f, 45.0f));
		e->AddComponent(Assets::PointClouds["Cube"]);
		e->AddComponent(Assets::Materials["Gold"]);
		fb = new RigidBody(PhysicsWorld, Assets::Materials["Gold"], new btBoxShape(btVector3(1.0f, 1.0f, 1.0f) * 0.5f), glm::vec3(), Assets::Materials["Gold"]->Density, RigidBody::Type::DYNAMIC);
		e->AddComponent(fb);
		AddEntity(e);

		// Point Cloud Test
		e = new Entity();
		e->Transform.Position = glm::vec3(-2.0f, 2.0f, 2.0f);
		e->Transform.Scale = glm::vec3(1.0f, 1.0f, 1.0f);
		e->Transform.Rotation = glm::quat(glm::vec3(45.0f, 0.0f, 45.0f));
		e->AddComponent(Assets::PointClouds["CubeFromMesh"]);
		e->AddComponent(Assets::Materials["Gold"]);
		fb = new RigidBody(PhysicsWorld, Assets::Materials["Gold"], new btBoxShape(btVector3(1.0f, 1.0f, 1.0f) * 0.5f), glm::vec3(), Assets::Materials["Gold"]->Density, RigidBody::Type::DYNAMIC);
		e->AddComponent(fb);
		AddEntity(e);
	}


	void Scene::LoadCharacter(std::string characterFile)
	{	
		Window->Scene->AppendConsole("Loading character: " + characterFile);
		
		auto character = new Entity();
		auto c = new Character(character, characterFile);
		character->AddComponent(c);
		character->Transform.Position = glm::vec3(0.0f, c->Height * 0.5f, 0.0f);
		auto fb = new RigidBody(PhysicsWorld, Assets::Materials["Gold"], new btCapsuleShape(0.25f, 1.3f), glm::vec3(0, -(c->Height * 0.5f), 0), 100.0f, RigidBody::Type::DYNAMIC);
		character->AddComponent(fb);
		character->Load();
		
		//fb->GetBody()->setActivationState(DISABLE_DEACTIVATION);
		btTransform t;
		t.setIdentity();
		auto constraint = new btGeneric6DofConstraint(*fb->GetBody(), t, false);

		constraint->setLimit(0, 1, 0);
		constraint->setLimit(1, 1, 0);
		constraint->setLimit(2, 1, 0);
		constraint->setLimit(3, 0, 0);
		constraint->setLimit(4, 1, 0);
		constraint->setLimit(5, 0, 0);
		
		// constraint->getTranslationalLimitMotor()->m_enableMotor[0] = true;
		// pGen6DOF->getTranslationalLimitMotor()->m_targetVelocity[0] = 5.0f;
		// pGen6DOF->getTranslationalLimitMotor()->m_maxMotorForce[0] = 0.1f;

		PhysicsWorld->AddConstraint(constraint);

		CameraTarget->SetTarget(character);
		Window->Input->SetCharacterEntity(character);
		Window->Scene->AddEntity(character);
	}
	
}