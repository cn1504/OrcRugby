#include "CharacterEditor.h"
#include "MaterialEditor.h"
#include "TriggerEventEditor.h"
#include "Importer.h"
#include "Assets.h"
#include "Window.h"
#include "Character.h"

namespace Core
{
	namespace Gui
	{		

		ImportButton::ImportButton(Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			WindowPtr = window;
		}

		ImportButton::~ImportButton() {}

		void ImportButton::OnClick(int button)
		{
			WindowPtr->Scene->Gui->AddItem(new AssetBrowser("Import File", "./", "dae", WindowPtr, WindowPtr->Scene->Gui, Item::Alignment::Center, glm::vec2(), glm::vec2(220, 150), nullptr, Assets::Textures["DialogBackground"]));
		}





		AssetBrowser::AssetBrowser(std::string title, std::string path, std::string targetExt, Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: FileBrowser(title, path, targetExt, window, parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
		}

		AssetBrowser::~AssetBrowser() {}

		void AssetBrowser::Resolve(std::string name)
		{
			std::string file = name + "." + TargetExt;
			if (FilePaths.count(file) > 0)
			{
				WindowPtr->Scene->AppendConsole("Importing File: " + file);
				AssetImporter::LoadFile(FilePaths[file].c_str());
				Parent->RemoveItem(this);
				delete this;
			}
			else
			{
				Path = Path + name + "/";
				for (auto c : Children)
				{
					delete c;
				}
				Children.clear();
				FilePaths.clear();

				BuildList();
			}
		}





		CameraToggleButton::CameraToggleButton(Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* cam1Image, Texture* cam1MouseOverImage, Texture* cam1MouseDownImage, Texture* cam2Image, Texture* cam2MouseOverImage, Texture* cam2MouseDownImage)
			: Item(parent, alignment, position, size, label, cam1Image, cam1MouseOverImage, cam1MouseDownImage)
		{
			WindowPtr = window;
			CurrentMode = Input::CameraMode::Orbital;

			Cam1Image = cam1Image;
			Cam1MouseOverImage = cam1MouseOverImage;
			Cam1MouseDownImage = cam1MouseDownImage;
			Cam2Image = cam2Image;
			Cam2MouseOverImage = cam2MouseOverImage;
			Cam2MouseDownImage = cam2MouseDownImage;
		}

		CameraToggleButton::~CameraToggleButton() {}

		void CameraToggleButton::OnClick(int button)
		{
			if (CurrentMode == Input::CameraMode::Game)
			{
				WindowPtr->Input->StopCharacter();
			}

			CurrentMode = (CurrentMode == Input::CameraMode::Orbital) ? Input::CameraMode::Game : Input::CameraMode::Orbital;
			WindowPtr->Input->SetCameraMode(CurrentMode);

			Image = (CurrentMode == Input::CameraMode::Orbital) ? Cam1Image : Cam2Image;
			MouseOverImage = (CurrentMode == Input::CameraMode::Orbital) ? Cam1MouseOverImage : Cam2MouseOverImage;
			MouseDownImage = (CurrentMode == Input::CameraMode::Orbital) ? Cam1MouseDownImage : Cam2MouseDownImage;
		}




		CharacterBrowser::CharacterBrowser(std::string title, std::string path, std::string targetExt, Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: FileBrowser(title, path, targetExt, window, parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			AddItem(new CharacterBrowserNewButton(WindowPtr, this, Alignment::TopRight, glm::vec2(-27, 9), glm::vec2(10, 10), Assets::Textures["Plus"], Assets::Textures["PlusMO"]));
		}

		CharacterBrowser::~CharacterBrowser() {}

		void CharacterBrowser::Resolve(std::string name)
		{
			std::string file = name + "." + TargetExt;
			if (FilePaths.count(file) > 0)
			{
				WindowPtr->Scene->LoadCharacter(std::string(&Path[21]) + file);
				Parent->RemoveItem(this);
				delete this;
			}
			else
			{
				Path = Path + name + "/";
				for (auto c : Children)
				{
					delete c;
				}
				Children.clear();
				FilePaths.clear();

				BuildList(); 
				AddItem(new CharacterBrowserNewButton(WindowPtr, this, Alignment::TopRight, glm::vec2(-27, 9), glm::vec2(10, 10), Assets::Textures["Plus"], Assets::Textures["PlusMO"]));
			}
		}




		CharacterButton::CharacterButton(Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			WindowPtr = window;
		}
		
		CharacterButton::~CharacterButton()	{}

		void CharacterButton::OnClick(int button)
		{
			auto fb = new CharacterBrowser("Load Character", "Resources/Characters/", "character", WindowPtr, WindowPtr->Scene->Gui, Item::Alignment::Center, glm::vec2(), glm::vec2(220, 150), nullptr, Assets::Textures["DialogBackground"]);
			WindowPtr->Scene->Gui->AddItem(fb);
		}




		CharacterBrowserNewButton::CharacterBrowserNewButton(Window* window, CharacterBrowser* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, nullptr, image, mouseOverImage, mouseDownImage)
		{
			WindowPtr = window;
			CB = parent;
		}

		CharacterBrowserNewButton::~CharacterBrowserNewButton() {}

		void CharacterBrowserNewButton::OnClick(int button)
		{
			auto p = CB->GetCurrentPath();
			WindowPtr->Scene->Gui->AddItem(new CharacterCreationDialog(std::string(&p[21]), WindowPtr, WindowPtr->Scene->Gui, Alignment::Center, glm::vec2(0,0), glm::vec2(220, 75), Assets::Textures["DialogBackground"]));
			Parent->Destroy();
		}




		CharacterCreationNameTextBox::CharacterCreationNameTextBox(Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label)
			: TextBox(window, parent, alignment, position, size, label, nullptr, Assets::Textures["DialogBackgroundMO"], nullptr, Assets::Textures["TextBoxEdit"])
		{
		}

		CharacterCreationNameTextBox::~CharacterCreationNameTextBox() {}
		




		CharacterCreationAcceptButton::CharacterCreationAcceptButton(CharacterCreationDialog* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			Dialog = parent;
		}

		CharacterCreationAcceptButton::~CharacterCreationAcceptButton() {}

		void CharacterCreationAcceptButton::OnClick(int button)
		{
			Dialog->Accept();
		}

		

		
		
		CharacterCreationDialog::CharacterCreationDialog(std::string path, Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, nullptr, image, mouseOverImage, mouseDownImage)
		{
			Debug::Log(path);

			WindowPtr = window;
			Path = path;
			NameTextBox = new CharacterCreationNameTextBox(window, this, Alignment::BottomCenter, glm::vec2(-10, -13), glm::vec2(160, 20), new Text("CharacterName", 4, 3, Assets::Fonts["Consolas16"]));
			AddItem(NameTextBox);
			AddItem(new CharacterCreationAcceptButton(this, Alignment::BottomCenter, glm::vec2(85, -10), glm::vec2(26, 26), nullptr, Assets::Textures["Check"], Assets::Textures["CheckMO"]));
			
			AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(5, 0), glm::vec2(150, 28), new Text("New Character", 4, 4, Assets::Fonts["Consolas24"], glm::vec4(0.9, 0.9, 0.9, 1.0))));
		
			AddItem(new Border(Border::Orientation::Horizontal, this, Item::Alignment::TopLeft, glm::vec2(1, 1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
			AddItem(new Border(Border::Orientation::Vertical, this, Item::Alignment::TopLeft, glm::vec2(1, 1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
			AddItem(new Border(Border::Orientation::Horizontal, this, Item::Alignment::BottomRight, glm::vec2(-1, -1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
			AddItem(new Border(Border::Orientation::Vertical, this, Item::Alignment::BottomRight, glm::vec2(-1, -1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));

			AddItem(new Border(Border::Orientation::Horizontal, this, Item::Alignment::TopLeft, glm::vec2(1, 28), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
			AddItem(new DialogCloseButton(this, this, Item::Alignment::TopRight, glm::vec2(-6, 6), glm::vec2(17, 17), nullptr, Assets::Textures["CloseButton"], Assets::Textures["CloseButtonMO"]));

			Invalidate();
		}

		CharacterCreationDialog::~CharacterCreationDialog() {}

		void CharacterCreationDialog::Accept()
		{
			auto name = NameTextBox->GetLabel() + ".character";

			Debug::Log("Creating new character: " + name);
			WindowPtr->Scene->AppendConsole("Creating new character: " + name);

			// Write new character file
			std::ofstream file("Resources/Characters/" + Path + name);
			file << "1.78" << std::endl;		// Height
			file << "1.0" << std::endl;			// Model Scale
			file << "Default" << std::endl;		// Skeleton File
			file << "Default" << std::endl;		// Animation File
			file << "0" << std::endl;			// Mesh/Material count
			file << "1" << std::endl;			// Animation Count
			file << "Idle" << std::endl;		//		Name
			file << "0" << std::endl;			//		First Frame
			file << "0" << std::endl;			//		Last Frame
			file << "1.0" << std::endl;			//		Speed Modifier
			file << "0" << std::endl;			// FrameEvent Count
			file << "5" << std::endl;			// Default Stat Block
			file << "5" << std::endl;			//  |
			file << "5" << std::endl;			//  |
			file << "5" << std::endl;			//  |
			file << "5" << std::endl;			// ---
			file.close();

			WindowPtr->Scene->LoadCharacter(Path + name);
			Destroy();
		}

		



		CharacterEditor::CharacterEditor(Entity* character, Window* window, Item* parent)
			: Item(parent, Item::Alignment::TopLeft, glm::vec2(-450, 100), glm::vec2(450, parent->GetDimensions().w - 160), nullptr, Assets::Textures["DialogBackground"])
		{
			WindowPtr = window;
			ShouldClose = false;
			Character = character; 
			OpenCategory = Category::Categories;

			Rebuild();

			Move(glm::vec2(Size.x, 0), 0.3f, MoveStyle::Sinosoidal);
		}

		CharacterEditor::~CharacterEditor()	{}

		void CharacterEditor::Destroy()
		{
			Move(glm::vec2(-Size.x, 0), 0.3f, MoveStyle::Sinosoidal);
			ShouldClose = true;
		}

		void CharacterEditor::Update(glm::vec2 mousePosition)
		{
			if (ShouldClose && !IsMoving())
			{
				Item::Destroy();
			}

			Item::Update(mousePosition);
		}

		void CharacterEditor::Invalidate(bool includeChildren)
		{
			Size.y = Parent->GetDimensions().w - 160;
			Item::Invalidate(includeChildren);
		}
		
		void CharacterEditor::Rebuild()
		{
			for (auto c : Children)
			{
				c->Destroy();
			}
			Children.clear();
			
			auto c = Character->GetComponent<Core::Character>();
			auto sa = Character->GetComponent<Core::SkeletalAnimation>();

			auto ts = c->Name;
			ts.resize(ts.length() - 10);
			auto Title = new Text(ts, 4, 4, Assets::Fonts["Consolas24"], glm::vec4(0.9, 0.9, 0.9, 1.0));
			AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(5, 0), glm::vec2(150, 28), Title));
			AddItem(new CharacterEditorSaveButton(Character, WindowPtr, this));
			AddItem(new DialogCloseButton(this, this, Alignment::TopRight, glm::vec2(-7, 5), glm::vec2(20, 20), nullptr, Assets::Textures["CloseButton"], Assets::Textures["CloseButtonMO"]));
			
			AddItem(new Border(Border::Orientation::Horizontal, this, Item::Alignment::TopLeft, glm::vec2(1, 1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
			AddItem(new Border(Border::Orientation::Vertical, this, Item::Alignment::TopLeft, glm::vec2(1, 1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
			AddItem(new Border(Border::Orientation::Horizontal, this, Item::Alignment::BottomRight, glm::vec2(-1, -1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
			AddItem(new Border(Border::Orientation::Vertical, this, Item::Alignment::BottomRight, glm::vec2(-1, -1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));

			AddItem(new Border(Border::Orientation::Horizontal, this, Item::Alignment::TopLeft, glm::vec2(1, 28), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));

			float y = 40.0f;
			float lh = 20.0f;
			float c1 = 10.0f;
			float cw1 = 90.0f;
			float c2 = c1 + cw1;
			float cw3 = 30.0f;
			float cwf = Size.x - c1 - cw1 - 18.0f;
			float c3 = c2 + cw3 + 10.0f;
			float c4 = c2 + (cw3 + 10.0f) * 2.0f;

			float cwh = Size.x * 0.5f - c1 * 2.0f;
			float ch2 = c1 + cwh;

			AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(cw1, lh), new Text("Height", 4, 3, Assets::Fonts["Consolas16"])));
			AddItem(new TextBoxFloatZeroPlus(&c->Height, WindowPtr, this, Item::Alignment::TopLeft, glm::vec2(c2, y), glm::vec2(cw1, lh), new Text("", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			y += lh;

			AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(cw1, lh), new Text("Scale", 4, 3, Assets::Fonts["Consolas16"])));
			AddItem(new CharacterEditorScaleTextBox(Character, WindowPtr, this, Item::Alignment::TopLeft, glm::vec2(c2, y), glm::vec2(cw1, lh), new Text("", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			y += lh;

			AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(cw1, lh), new Text("Skeleton", 4, 3, Assets::Fonts["Consolas16"])));
			AddItem(new CharacterEditorSkeletonButton(Character, WindowPtr, this, Item::Alignment::TopLeft, glm::vec2(c2, y), glm::vec2(cwf, lh), new Text(sa->GetSkeletonFilename(), 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["DialogBackgroundMO"]));
			y += lh;

			AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(cw1, lh), new Text("Animation", 4, 3, Assets::Fonts["Consolas16"])));
			AddItem(new CharacterEditorAnimationButton(Character, WindowPtr, this, Item::Alignment::TopLeft, glm::vec2(c2, y), glm::vec2(cwf, lh), new Text(sa->GetAnimationFilename(), 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["DialogBackgroundMO"]));
			y += lh;

			AddItem(new Border(Border::Orientation::Horizontal, this, Item::Alignment::TopLeft, glm::vec2(1, y + 10.0f), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
			y += lh;
			

			if (OpenCategory == Category::Categories)
			{
				AddItem(new CharacterEditorCategoryButton(Category::Meshes, this, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(Size.x - c1 * 2.0f, lh), new Text("Meshes", 4, 3, Assets::Fonts["Consolas16"]), nullptr, Assets::Textures["DialogBackgroundMO"]));
				y += lh; 
				AddItem(new CharacterEditorCategoryButton(Category::Animations, this, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(Size.x - c1 * 2.0f, lh), new Text("Animations", 4, 3, Assets::Fonts["Consolas16"]), nullptr, Assets::Textures["DialogBackgroundMO"]));
				y += lh; 
				AddItem(new CharacterEditorCategoryButton(Category::FrameEvents, this, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(Size.x - c1 * 2.0f, lh), new Text("Frame Events", 4, 3, Assets::Fonts["Consolas16"]), nullptr, Assets::Textures["DialogBackgroundMO"]));
				y += lh;
			}
			
			else if (OpenCategory == Category::Meshes)
			{
				AddItem(new CharacterEditorCategoryButton(Category::Categories, this, Alignment::TopRight, glm::vec2(-7, y), glm::vec2(20, 20), nullptr, Assets::Textures["CloseButton"], Assets::Textures["CloseButtonMO"]));

				AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(cw1, lh), new Text("Meshes", 4, 3, Assets::Fonts["Consolas16"])));
				AddItem(new CharacterEditorAddMeshButton(sa, this, Item::Alignment::TopRight, glm::vec2(-30.0f, y + 4.0f), glm::vec2(12, 12), nullptr, Assets::Textures["Plus"], Assets::Textures["PlusMO"]));
				y += lh;

				for (int i = 0; i < sa->Meshes.size(); i++)
				{
					AddItem(new CharacterEditorMeshButton(&sa->Meshes[i], WindowPtr, this, glm::vec2(c1, y), glm::vec2(Size.x - 180.0f, lh), new Text(sa->Meshes[i].first, 12, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))));
					AddItem(new CharacterEditorMaterialButton(&sa->Meshes[i], WindowPtr, this, glm::vec2(c1 + Size.x - 180.0f, y), glm::vec2(140, lh), new Text(sa->Meshes[i].second, 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))));
					AddItem(new CharacterEditorRemoveMeshButton(sa, i, this, Item::Alignment::TopRight, glm::vec2(-7, y), glm::vec2(20, 20), nullptr, nullptr, Assets::Textures["TrashMO"]));
					y += lh;
				}
			}

			else if (OpenCategory == Category::Animations)
			{
				AddItem(new CharacterEditorCategoryButton(Category::Categories, this, Alignment::TopRight, glm::vec2(-7, y), glm::vec2(20, 20), nullptr, Assets::Textures["CloseButton"], Assets::Textures["CloseButtonMO"]));
				
				AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(cw1, lh), new Text("Animations - " + std::to_string(sa->GetFrameCount()) + " frames", 4, 3, Assets::Fonts["Consolas16"])));
				AddItem(new CharacterEditorAddAnimationButton(sa, this, Item::Alignment::TopRight, glm::vec2(-30.0f, y + 4.0f), glm::vec2(12, 12), nullptr, Assets::Textures["Plus"], Assets::Textures["PlusMO"]));
				y += lh;

				for (auto anim : sa->Animations)
				{
					AddItem(new CharacterEditorAnimationLine(sa, anim.first, WindowPtr, this, Alignment::TopLeft, glm::vec2(2, y), glm::vec2(Size.x - 4, 20)));
					y += lh;
				}
			}

			else if (OpenCategory == Category::FrameEvents)
			{
				AddItem(new CharacterEditorCategoryButton(Category::Categories, this, Alignment::TopRight, glm::vec2(-7, y), glm::vec2(20, 20), nullptr, Assets::Textures["CloseButton"], Assets::Textures["CloseButtonMO"]));

				AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(cw1, lh), new Text("Frame Events", 4, 3, Assets::Fonts["Consolas16"])));
				AddItem(new CharacterEditorAddFrameEventButton(sa, this, Item::Alignment::TopRight, glm::vec2(-30.0f, y + 4.0f), glm::vec2(12, 12), nullptr, Assets::Textures["Plus"], Assets::Textures["PlusMO"]));
				y += lh;

				for (auto frame : sa->FrameEvents)
				{
					for (auto te : frame.second)
					{
						AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(10, y), glm::vec2(70, 20), new Text("Frame:", 4, 3, Assets::Fonts["Consolas16"])));
						AddItem(new CharacterEditorFrameEventFrameNumberTextBox(frame.first, te, sa, WindowPtr, this, Alignment::TopLeft, glm::vec2(100, y), glm::vec2(158, 20), new Text("", 4, 3, Assets::Fonts["Consolas16"]), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
						AddItem(new CharacterEditorRemoveFrameEventButton(frame.first, te, sa, this, Item::Alignment::TopRight, glm::vec2(-7, y), glm::vec2(20, 20), nullptr, nullptr, Assets::Textures["TrashMO"]));
						y += lh;

						BuildTriggerEventGui(&sa->FrameEvents[frame.first], te, WindowPtr, this, y);
						y += 10.0f;
					}
				}
			}

			Invalidate();
		}





		CharacterEditorCategoryButton::CharacterEditorCategoryButton(CharacterEditor::Category target, CharacterEditor* parent, Item::Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			Editor = parent;
			Target = target;
		}

		CharacterEditorCategoryButton::~CharacterEditorCategoryButton()	{}

		void CharacterEditorCategoryButton::OnClick(int button)
		{
			Editor->OpenCategory = Target;
			Editor->Rebuild();
		}




		CharacterEditorScaleTextBox::CharacterEditorScaleTextBox(Entity* character, Window* window, Item* parent, Item::Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage, Texture* editImage, Texture* editMouseOverImage, Texture* editMouseDownImage)
			: TextBox(window, parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage, editImage, editMouseOverImage, editMouseDownImage)
		{
			Character = character;
			float s = Character->Transform.Scale.x;
			Label->UpdateText(std::to_string(s));
		}

		CharacterEditorScaleTextBox::~CharacterEditorScaleTextBox() {}

		void CharacterEditorScaleTextBox::SaveValue()
		{
			auto result = Label->GetText();
			float s = Character->Transform.Scale.x;
			try	{ 
				s = std::stof(result);
				Character->Transform.Scale = glm::vec3(s, s, s);
			}
			catch (std::invalid_argument &e) { (void*)&e; }
			catch (std::out_of_range &e) { (void*)&e; }
			Label->UpdateText(std::to_string(s));

			TextBox::SaveValue();
		}





		CharacterEditorSaveButton::CharacterEditorSaveButton(Entity* character, Window* window, Item* parent)
			: Item(parent, Item::Alignment::TopRight, glm::vec2(-34, 5), glm::vec2(20, 20), nullptr, Assets::Textures["Save"], Assets::Textures["SaveMO"])
		{
			WindowPtr = window;
			Character = character;
		}

		CharacterEditorSaveButton::~CharacterEditorSaveButton()	{}

		void CharacterEditorSaveButton::OnClick(int button)
		{
			WindowPtr->Scene->AppendConsole("Saving character: " + Character->GetComponent<Core::Character>()->Name);
			Character->GetComponent<Core::Character>()->Save();
		}





		SkeletonBrowser::SkeletonBrowser(Core::SkeletalAnimation* sa, Item* opener, Window* window)
			: FileBrowser("Skeleton", "Resources/Meshes/", "skeleton", window, window->Scene->Gui, Item::Alignment::Center, glm::vec2(), glm::vec2(220, 150), nullptr, Assets::Textures["DialogBackground"])
		{
			SA = sa;
			Opener = opener;
		}

		SkeletonBrowser::~SkeletonBrowser()	{}

		void SkeletonBrowser::Resolve(std::string name)
		{
			std::string file = name + "." + TargetExt;
			if (FilePaths.count(file) > 0)
			{
				Opener->SetLabel(name);
				SA->LoadSkeleton(name);
				Parent->RemoveItem(this);
				delete this;
			}
			else
			{
				Path = Path + name + "/";
				for (auto c : Children)
				{
					delete c;
				}
				Children.clear();
				FilePaths.clear();

				BuildList();
			}
		}


		

		CharacterEditorSkeletonButton::CharacterEditorSkeletonButton(Entity* character, Window* window, Item* parent, Item::Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			WindowPtr = window;
			Character = character;
		}

		CharacterEditorSkeletonButton::~CharacterEditorSkeletonButton()	{}

		void CharacterEditorSkeletonButton::OnClick(int button)
		{
			WindowPtr->Scene->Gui->AddItem(new SkeletonBrowser(Character->GetComponent<SkeletalAnimation>(), this, WindowPtr));
		}



		AnimationBrowser::AnimationBrowser(Core::SkeletalAnimation* sa, Item* opener, Window* window)
			: FileBrowser("Animation", "Resources/Meshes/", "animation", window, window->Scene->Gui, Item::Alignment::Center, glm::vec2(), glm::vec2(300, 150), nullptr, Assets::Textures["DialogBackground"])
		{
			SA = sa;
			Opener = opener;
		}

		AnimationBrowser::~AnimationBrowser() {}

		void AnimationBrowser::Resolve(std::string name)
		{
			std::string file = name + "." + TargetExt;
			if (FilePaths.count(file) > 0)
			{
				Opener->SetLabel(name);
				SA->LoadAnimation(name);
				Parent->RemoveItem(this);
				delete this;
			}
			else
			{
				Path = Path + name + "/";
				for (auto c : Children)
				{
					delete c;
				}
				Children.clear();
				FilePaths.clear();

				BuildList();
			}
		}




		CharacterEditorAnimationButton::CharacterEditorAnimationButton(Entity* character, Window* window, Item* parent, Item::Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			WindowPtr = window;
			Character = character;
		}

		CharacterEditorAnimationButton::~CharacterEditorAnimationButton() {}

		void CharacterEditorAnimationButton::OnClick(int button)
		{
			WindowPtr->Scene->Gui->AddItem(new AnimationBrowser(Character->GetComponent<SkeletalAnimation>(), this, WindowPtr));
		}



		MeshBrowser::MeshBrowser(std::pair<std::string, std::string>* mesh, Window* window, Item* opener)
			: FileBrowser("Meshes", "Resources/Meshes/", "mesh", window, window->Scene->Gui, Item::Alignment::Center, glm::vec2(), glm::vec2(300, 150), nullptr, Assets::Textures["DialogBackground"])
		{
			Mesh = mesh;
			Opener = opener;
		}

		MeshBrowser::~MeshBrowser()	{}

		void MeshBrowser::Resolve(std::string name)
		{
			std::string relPath = std::string(&Path[17]);
			std::string file = name + "." + TargetExt;
			if (FilePaths.count(file) > 0)
			{
				Opener->SetLabel(relPath + name);
				Assets::LoadMesh(relPath + name);
				Mesh->first = relPath + name;

				Parent->RemoveItem(this);
				delete this;
			}
			else
			{
				Path = Path + name + "/";
				for (auto c : Children)
				{
					delete c;
				}
				Children.clear();
				FilePaths.clear();

				BuildList();
			}
		}



		CharacterEditorMeshButton::CharacterEditorMeshButton(std::pair<std::string, std::string>* mesh, Window* window, Item* parent, glm::vec2 position, glm::vec2 size, Text* label)
			: Item(parent, Item::Alignment::TopLeft, position, size, label, nullptr, Assets::Textures["DialogBackgroundMO"])
		{
			WindowPtr = window;
			Mesh = mesh;
		}

		CharacterEditorMeshButton::~CharacterEditorMeshButton() {}

		void CharacterEditorMeshButton::OnClick(int button)
		{
			WindowPtr->Scene->Gui->AddItem(new MeshBrowser(Mesh, WindowPtr, this));
		}




		CharacterEditorMaterialButton::CharacterEditorMaterialButton(std::pair<std::string, std::string>* material, Window* window, Item* parent, glm::vec2 position, glm::vec2 size, Text* label)
			: Item(parent, Item::Alignment::TopLeft, position, size, label, nullptr, Assets::Textures["DialogBackgroundMO"])
		{
			WindowPtr = window;
			Material = material;
		}

		CharacterEditorMaterialButton::~CharacterEditorMaterialButton() {}

		void CharacterEditorMaterialButton::OnClick(int button)
		{
			WindowPtr->Scene->Gui->AddItem(new MaterialBrowser(Material, WindowPtr, this));
		}




		CharacterEditorAddMeshButton::CharacterEditorAddMeshButton(SkeletalAnimation* sa, CharacterEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			SA = sa;
			Editor = parent;
		}
		
		CharacterEditorAddMeshButton::~CharacterEditorAddMeshButton() {}

		void CharacterEditorAddMeshButton::OnClick(int button)
		{
			SA->AddMesh("Cube", "Gold");
			Editor->Rebuild();
		}




		CharacterEditorRemoveMeshButton::CharacterEditorRemoveMeshButton(SkeletalAnimation* sa, int i, CharacterEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			SA = sa;
			Index = i;
			Editor = parent;
		}

		CharacterEditorRemoveMeshButton::~CharacterEditorRemoveMeshButton() {}

		void CharacterEditorRemoveMeshButton::OnClick(int button)
		{
			SA->Meshes[Index] = SA->Meshes.back();
			SA->Meshes.resize(SA->Meshes.size() - 1);
			Editor->Rebuild();
		}




		CharacterEditorAnimationNameTextBox::CharacterEditorAnimationNameTextBox(SkeletalAnimation* sa, std::string name, Window* window, CharacterEditorAnimationLine* parent, Item::Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label)
			: TextBox(window, parent, alignment, position, size, label, nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"])
		{
			Line = parent;
			SA = sa;
			Name = name;
		}
		
		CharacterEditorAnimationNameTextBox::~CharacterEditorAnimationNameTextBox() {}

		void CharacterEditorAnimationNameTextBox::SaveValue()
		{
			if (Name != "Idle")
			{
				auto result = Label->GetText();
				
				if (result != "Idle")
				{
					SA->Animations[result] = SA->Animations[Name];
					SA->Animations.erase(Name);
					Name = result;
					SA->PlayAnimation(Name, true);
				}
			}

			Line->RefreshData(Name);
			TextBox::SaveValue();
		}

		void CharacterEditorAnimationNameTextBox::RefreshData(std::string name)
		{
			Name = name;
			Label->UpdateText(Name);
		}



		
		CharacterEditorAnimationFirstTextBox::CharacterEditorAnimationFirstTextBox(SkeletalAnimation* sa, std::string name, Window* window, CharacterEditorAnimationLine* parent, Item::Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label)
			: TextBox(window, parent, alignment, position, size, label, nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"])
		{
			Line = parent;
			SA = sa;
			Name = name;
		}

		CharacterEditorAnimationFirstTextBox::~CharacterEditorAnimationFirstTextBox() {}

		void CharacterEditorAnimationFirstTextBox::SaveValue()
		{
			auto result = Label->GetText();
			
			int s = SA->Animations[Name]->FirstFrame;
			try	{
				s = std::stoi(result);
				SA->Animations[Name]->FirstFrame = (s < 0) ? 0 : (s > (int)SA->GetFrameCount()) ? SA->GetFrameCount() - 1 : s;
				if (SA->Animations[Name]->FirstFrame > SA->Animations[Name]->LastFrame)
					SA->Animations[Name]->LastFrame = SA->Animations[Name]->FirstFrame;
			}
			catch (std::invalid_argument &e) { (void*)&e; }
			catch (std::out_of_range &e) { (void*)&e; }
			
			Line->RefreshData(Name);
			SA->PlayAnimation(Name, true);

			TextBox::SaveValue();
		}

		void CharacterEditorAnimationFirstTextBox::RefreshData(std::string name)
		{
			Name = name;
			Label->UpdateText(std::to_string(SA->Animations[Name]->FirstFrame));
		}




		CharacterEditorAnimationLastTextBox::CharacterEditorAnimationLastTextBox(SkeletalAnimation* sa, std::string name, Window* window, CharacterEditorAnimationLine* parent, Item::Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label)
			: TextBox(window, parent, alignment, position, size, label, nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"])
		{
			Line = parent;
			SA = sa;
			Name = name;
		}

		CharacterEditorAnimationLastTextBox::~CharacterEditorAnimationLastTextBox() {}

		void CharacterEditorAnimationLastTextBox::SaveValue()
		{
			auto result = Label->GetText();

			int s = SA->Animations[Name]->LastFrame;
			try	{
				s = std::stoi(result);
				SA->Animations[Name]->LastFrame = (s >= (int)SA->GetFrameCount()) ? SA->GetFrameCount() - 1 : (s < 0) ? 0 : s;
				if (SA->Animations[Name]->LastFrame < SA->Animations[Name]->FirstFrame)
					SA->Animations[Name]->FirstFrame = SA->Animations[Name]->LastFrame;
			}
			catch (std::invalid_argument &e) { (void*)&e; }
			catch (std::out_of_range &e) { (void*)&e; }

			Line->RefreshData(Name);
			SA->PlayAnimation(Name, true);

			TextBox::SaveValue();
		}

		void CharacterEditorAnimationLastTextBox::RefreshData(std::string name)
		{
			Name = name;
			Label->UpdateText(std::to_string(SA->Animations[Name]->LastFrame));
		}





		CharacterEditorAnimationTimeScaleTextBox::CharacterEditorAnimationTimeScaleTextBox(SkeletalAnimation* sa, std::string name, Window* window, CharacterEditorAnimationLine* parent, Item::Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label)
			: TextBox(window, parent, alignment, position, size, label, nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"])
		{
			Line = parent;
			SA = sa;
			Name = name;
		}

		CharacterEditorAnimationTimeScaleTextBox::~CharacterEditorAnimationTimeScaleTextBox() {}

		void CharacterEditorAnimationTimeScaleTextBox::SaveValue()
		{
			auto result = Label->GetText();

			float s = SA->Animations[Name]->TimeScale;
			try	{
				s = std::stof(result);
				SA->Animations[Name]->TimeScale = (s < 0.001f) ? 0.001f : s;
			}
			catch (std::invalid_argument &e) { (void*)&e; }
			catch (std::out_of_range &e) { (void*)&e; }

			Line->RefreshData(Name);
			SA->PlayAnimation(Name, true);

			TextBox::SaveValue();
		}

		void CharacterEditorAnimationTimeScaleTextBox::RefreshData(std::string name)
		{
			Name = name;
			auto ts = std::to_string(SA->Animations[Name]->TimeScale);
			if (ts.length() > 5)
				ts.resize(5);
			Label->UpdateText(ts);
		}


		


		CharacterEditorPlayAnimationButton::CharacterEditorPlayAnimationButton(SkeletalAnimation* sa, std::string name, CharacterEditorAnimationLine* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			SA = sa;
			Name = name;
		}

		CharacterEditorPlayAnimationButton::~CharacterEditorPlayAnimationButton() {}

		void CharacterEditorPlayAnimationButton::OnClick(int button)
		{
			SA->PlayAnimation(Name, true);
		}

		void CharacterEditorPlayAnimationButton::RefreshData(std::string name)
		{
			Name = name;
		}




		
		CharacterEditorRemoveAnimationButton::CharacterEditorRemoveAnimationButton(SkeletalAnimation* sa, std::string name, CharacterEditor* editor, CharacterEditorAnimationLine* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			SA = sa;
			Editor = editor;
			Name = name;
		}
		
		CharacterEditorRemoveAnimationButton::~CharacterEditorRemoveAnimationButton() {}

		void CharacterEditorRemoveAnimationButton::OnClick(int button)
		{
			if (Name != "Idle")
			{
				SA->PlayAnimation("Idle", true);
				SA->Animations.erase(Name);
				Editor->Rebuild();
			}
		}

		void CharacterEditorRemoveAnimationButton::RefreshData(std::string name)
		{
			Name = name;
		}

		
		


		CharacterEditorAnimationLine::CharacterEditorAnimationLine(SkeletalAnimation* sa, std::string name, Window* window, CharacterEditor* parent, Item::Alignment alignment, glm::vec2 position, glm::vec2 size)
			: Item(parent, alignment, position, size)
		{	
			auto ts = std::to_string(sa->Animations[name]->TimeScale);
			if (ts.length() > 5)
				ts.resize(5);

			NameTextBox = new CharacterEditorAnimationNameTextBox(sa, name, window, this, Alignment::TopLeft, glm::vec2(0, 0), glm::vec2(113, 20), new Text(name, 12, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
			FirstTextBox = new CharacterEditorAnimationFirstTextBox(sa, name, window, this, Alignment::TopLeft, glm::vec2(113, 0), glm::vec2(30, 20), new Text(std::to_string(sa->Animations[name]->FirstFrame), 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
			LastTextBox = new CharacterEditorAnimationLastTextBox(sa, name, window, this, Alignment::TopLeft, glm::vec2(143, 0), glm::vec2(30, 20), new Text(std::to_string(sa->Animations[name]->LastFrame), 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
			TimeScaleTextBox = new CharacterEditorAnimationTimeScaleTextBox(sa, name, window, this, Alignment::TopLeft, glm::vec2(173, 0), glm::vec2(44, 20), new Text(ts, 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
			PlayButton = new CharacterEditorPlayAnimationButton(sa, name, this, Alignment::TopRight, glm::vec2(-32, 0), glm::vec2(20, 20), nullptr, nullptr, Assets::Textures["PlayMO"]);
			RemoveButton = new CharacterEditorRemoveAnimationButton(sa, name, parent, this, Alignment::TopRight, glm::vec2(-7, 0), glm::vec2(20, 20), nullptr, nullptr, Assets::Textures["TrashMO"]);

			AddItem(NameTextBox);
			AddItem(FirstTextBox);
			AddItem(LastTextBox);
			AddItem(TimeScaleTextBox);
			AddItem(PlayButton);
			AddItem(RemoveButton);
		}

		CharacterEditorAnimationLine::~CharacterEditorAnimationLine() {}

		void CharacterEditorAnimationLine::RefreshData(std::string name)
		{
			NameTextBox->RefreshData(name);
			FirstTextBox->RefreshData(name);
			LastTextBox->RefreshData(name);
			TimeScaleTextBox->RefreshData(name);
			PlayButton->RefreshData(name);
			RemoveButton->RefreshData(name);
		}

		



		CharacterEditorAddAnimationButton::CharacterEditorAddAnimationButton(SkeletalAnimation* sa, CharacterEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			SA = sa;
			Editor = parent;
		}

		CharacterEditorAddAnimationButton::~CharacterEditorAddAnimationButton() {}

		void CharacterEditorAddAnimationButton::OnClick(int button)
		{
			SA->AddAnimation("NewAnimation", new Animation(0, SA->GetFrameCount()));
			SA->PlayAnimation("NewAnimation");
			Editor->Rebuild();
		}





		CharacterEditorAddFrameEventButton::CharacterEditorAddFrameEventButton(SkeletalAnimation* sa, CharacterEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			SA = sa;
			Editor = parent;
		}

		CharacterEditorAddFrameEventButton::~CharacterEditorAddFrameEventButton() {}

		void CharacterEditorAddFrameEventButton::OnClick(int button)
		{
			auto te = new TriggerEventSFX();
			te->AudioTrack = "SFX/Gunshot_Colt45_Fire";
			SA->AddFrameEvent(0, te);
			Editor->Rebuild();
		}
		
		



		CharacterEditorFrameEventFrameNumberTextBox::CharacterEditorFrameEventFrameNumberTextBox(unsigned int frame, TriggerEvent* te, SkeletalAnimation* sa, Window* window, CharacterEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage, Texture* editImage, Texture* editMouseOverImage, Texture* editMouseDownImage)
			: TextBox(window, parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage, editImage, editMouseOverImage, editMouseDownImage)
		{
			Editor = parent;
			Frame = frame;
			TE = te;
			SA = sa;
			label->UpdateText(std::to_string(Frame));
		}

		CharacterEditorFrameEventFrameNumberTextBox::~CharacterEditorFrameEventFrameNumberTextBox() {}

		void CharacterEditorFrameEventFrameNumberTextBox::SaveValue()
		{
			int f = Frame;
			try	{
				f = std::stoi(Label->GetText());
			}
			catch (std::invalid_argument &e) { (void*)&e; }
			catch (std::out_of_range &e) { (void*)&e; }

			f = (f < 0) ? 0 : (f >(int)SA->GetFrameCount()) ? SA->GetFrameCount() : f;

			if (f != Frame)
			{
				// Move te from old frame's collection to the new one
				if (SA->FrameEvents.count(Frame) > 0)
				{
					int i = 0;
					while (i < SA->FrameEvents[Frame].size())
					{
						if (SA->FrameEvents[Frame][i] == TE)
							break;
						i++;
					}
					if (i != SA->FrameEvents[Frame].size())
					{
						SA->FrameEvents[Frame][i] = SA->FrameEvents[Frame].back();
						SA->FrameEvents[Frame].resize(SA->FrameEvents[Frame].size() - 1);
					}
					else
					{
						Debug::Error("TriggerEvent not found in FrameEvents");
					}

					Frame = f;
					SA->FrameEvents[Frame].push_back(TE);
					Editor->Rebuild();
				}
			}
			else
			{
				Label->UpdateText(std::to_string(Frame));
			}

			TextBox::SaveValue();
		}




		CharacterEditorRemoveFrameEventButton::CharacterEditorRemoveFrameEventButton(unsigned int frame, TriggerEvent* te, SkeletalAnimation* sa, CharacterEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			Editor = parent;
			SA = sa;
			Frame = frame;
			TE = te;
		}

		CharacterEditorRemoveFrameEventButton::~CharacterEditorRemoveFrameEventButton() {}

		void CharacterEditorRemoveFrameEventButton::OnClick(int button)
		{
			int i = 0;
			while (i < SA->FrameEvents[Frame].size())
			{
				if (TE == SA->FrameEvents[Frame][i])
					break;
				i++;
			}

			if (i == SA->FrameEvents[Frame].size())
				return;

			SA->FrameEvents[Frame][i] = SA->FrameEvents[Frame].back();
			SA->FrameEvents[Frame].resize(SA->FrameEvents[Frame].size() - 1);

			if (SA->FrameEvents[Frame].size() == 0)
			{
				SA->FrameEvents.erase(Frame);
			}

			Editor->Rebuild();
		}

	}
}