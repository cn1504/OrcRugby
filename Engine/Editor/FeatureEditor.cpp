#include "FeatureEditor.h"
#include "Assets.h"
#include "Window.h"

namespace Core
{
	namespace Gui
	{

		EditableFeature::EditableFeature(std::string name)
			: Feature(name)
		{
			Load();
		}

		EditableFeature::~EditableFeature() {}

		std::string* EditableFeature::GetTagPtr() { return &Tag; }

		int* EditableFeature::GetSpawnWeightPtr() { return &SpawnWeight; }
		int* EditableFeature::GetSpawnWeightAdjustmentPtr() { return &SpawnWeightAdjustment; }

		std::vector<Node*>* EditableFeature::GetPlaceables() { return &Placeables; }
		std::vector<Node*>* EditableFeature::GetCreatures() { return &Creatures; }
		std::vector<LightNode*>* EditableFeature::GetLights() { return &Lights; }
		std::vector<Node*>* EditableFeature::GetAudioSources() { return &AudioSources; }
		std::vector<std::pair<std::string, std::string>>* EditableFeature::GetMeshes() { return &Meshes; }
		std::vector<Collider*>* EditableFeature::GetColliders() { return &Colliders; }





		FeatureButton::FeatureButton(Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			WindowPtr = window;
		}

		FeatureButton::~FeatureButton()	{}

		void FeatureButton::OnClick(int button)
		{
			WindowPtr->Scene->Gui->AddItem(new FeatureBrowser("Features", "Resources/Features/", "", WindowPtr, WindowPtr->Scene->Gui, Item::Alignment::Center, glm::vec2(), glm::vec2(220, 150), nullptr, Assets::Textures["DialogBackground"]));
		}





		FeatureBrowser::FeatureBrowser(std::string title, std::string path, std::string targetExt, Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: FileBrowser(title, path, targetExt, window, parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			AddItem(new FeatureBrowserNewButton(WindowPtr, this, Alignment::TopRight, glm::vec2(-27, 9), glm::vec2(10, 10), Assets::Textures["Plus"], Assets::Textures["PlusMO"]));
		}

		FeatureBrowser::~FeatureBrowser() {}

		void FeatureBrowser::Resolve(std::string name)
		{
			std::string file = name;
			if (FilePaths.count(file) > 0)
			{
				Debug::Log("Loading feature: " + Path + " " + ((Path.length() > 19) ? std::string(&Path[19]) + file : file));
				WindowPtr->Scene->LoadFeature((Path.length() > 19) ? std::string(&Path[19]) + file : file);
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
				AddItem(new FeatureBrowserNewButton(WindowPtr, this, Alignment::TopRight, glm::vec2(-27, 9), glm::vec2(10, 10), Assets::Textures["Plus"], Assets::Textures["PlusMO"]));
			}
		}





		FeatureBrowserNewButton::FeatureBrowserNewButton(Window* window, FeatureBrowser* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, nullptr, image, mouseOverImage, mouseDownImage)
		{
			WindowPtr = window;
			CB = parent;
		}

		FeatureBrowserNewButton::~FeatureBrowserNewButton() {}

		void FeatureBrowserNewButton::OnClick(int button)
		{
			auto p = CB->GetCurrentPath();
			if (p.length() > 19)
				p = std::string(&p[19]);
			else
				p = "";
			WindowPtr->Scene->Gui->AddItem(new FeatureCreationDialog(p, WindowPtr, WindowPtr->Scene->Gui, Alignment::Center, glm::vec2(0, 0), glm::vec2(220, 75), Assets::Textures["DialogBackground"]));
			Parent->Destroy();
		}





		FeatureCreationAcceptButton::FeatureCreationAcceptButton(FeatureCreationDialog* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			Dialog = parent;
		}

		FeatureCreationAcceptButton::~FeatureCreationAcceptButton() {}

		void FeatureCreationAcceptButton::OnClick(int button)
		{
			Dialog->Accept();
		}





		FeatureCreationDialog::FeatureCreationDialog(std::string path, Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, nullptr, image, mouseOverImage, mouseDownImage)
		{
			Debug::Log(path);

			WindowPtr = window;
			Name = "FeatureName";
			Path = path;
			NameTextBox = new TextBoxString(&Name, window, this, Alignment::BottomCenter, glm::vec2(-10, -13), glm::vec2(160, 20), new Text("0", 4, 3, Assets::Fonts["Consolas16"]), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]);
			AddItem(NameTextBox);
			AddItem(new FeatureCreationAcceptButton(this, Alignment::BottomCenter, glm::vec2(85, -10), glm::vec2(26, 26), nullptr, Assets::Textures["Check"], Assets::Textures["CheckMO"]));

			AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(5, 0), glm::vec2(150, 28), new Text("New Feature", 4, 4, Assets::Fonts["Consolas24"], glm::vec4(0.9, 0.9, 0.9, 1.0))));

			AddItem(new Border(Border::Orientation::Horizontal, this, Item::Alignment::TopLeft, glm::vec2(1, 1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
			AddItem(new Border(Border::Orientation::Vertical, this, Item::Alignment::TopLeft, glm::vec2(1, 1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
			AddItem(new Border(Border::Orientation::Horizontal, this, Item::Alignment::BottomRight, glm::vec2(-1, -1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
			AddItem(new Border(Border::Orientation::Vertical, this, Item::Alignment::BottomRight, glm::vec2(-1, -1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));

			AddItem(new Border(Border::Orientation::Horizontal, this, Item::Alignment::TopLeft, glm::vec2(1, 28), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
			AddItem(new DialogCloseButton(this, this, Item::Alignment::TopRight, glm::vec2(-6, 6), glm::vec2(17, 17), nullptr, Assets::Textures["CloseButton"], Assets::Textures["CloseButtonMO"]));

			Invalidate();
		}

		FeatureCreationDialog::~FeatureCreationDialog() {}

		void FeatureCreationDialog::Accept()
		{
			auto name = NameTextBox->GetLabel();

			Debug::Log("Creating new feature: " + name);
			WindowPtr->Scene->AppendConsole("Creating new feature: " + name);

			Feature feature(Path + name);
			feature.LoadDefaultValues();
			feature.Save();

			WindowPtr->Scene->LoadFeature(Path + name);
			Destroy();
		}





		FeatureEditor::FeatureEditor(std::string tile, Window* window, Item* parent)
			: Item(parent, Item::Alignment::TopLeft, glm::vec2(-500, 100), glm::vec2(500, parent->GetDimensions().w - 160), nullptr, Assets::Textures["DialogBackground"])
		{
			WindowPtr = window;
			ShouldClose = false;
			Feature = new EditableFeature(tile);

			OpenCategory = Category::Categories;
			ScrollPosition = 0.0f;

			Rebuild();

			Move(glm::vec2(Size.x, 0), 0.3f, MoveStyle::Sinosoidal);
		}

		FeatureEditor::~FeatureEditor()
		{
			delete Feature;
		}

		void FeatureEditor::Destroy()
		{
			Move(glm::vec2(-Size.x, 0), 0.3f, MoveStyle::Sinosoidal);
			ShouldClose = true;
		}

		void FeatureEditor::Update(glm::vec2 mousePosition)
		{
			if (ShouldClose && !IsMoving())
			{
				if (WindowPtr->Scene->TileEditor == this)
					WindowPtr->Scene->TileEditor = nullptr;
				Item::Destroy();
			}

			Item::Update(mousePosition);
		}

		void FeatureEditor::Invalidate(bool includeChildren)
		{
			Size.y = Parent->GetDimensions().w - 160;
			Item::Invalidate(includeChildren);
		}

		void FeatureEditor::Rebuild()
		{
			for (auto c : Children)
			{
				c->Destroy();
			}
			Children.clear();


			auto Title = new Text(Feature->GetName(), 4, 4, Assets::Fonts["Consolas24"], glm::vec4(0.9, 0.9, 0.9, 1.0));
			AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(5, 0), glm::vec2(150, 28), Title));
			AddItem(new DialogCloseButton(this, this, Alignment::TopRight, glm::vec2(-7, 5), glm::vec2(20, 20), nullptr, Assets::Textures["CloseButton"], Assets::Textures["CloseButtonMO"]));

			AddItem(new FeatureEditorSaveButton(this, WindowPtr, Alignment::TopRight, glm::vec2(-34, 5), glm::vec2(20, 20), Assets::Textures["Save"], Assets::Textures["SaveMO"]));

			AddItem(new Border(Border::Orientation::Horizontal, this, Alignment::TopLeft, glm::vec2(1, 1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
			AddItem(new Border(Border::Orientation::Vertical, this, Alignment::TopLeft, glm::vec2(1, 1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
			AddItem(new Border(Border::Orientation::Horizontal, this, Alignment::BottomRight, glm::vec2(-1, -1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
			AddItem(new Border(Border::Orientation::Vertical, this, Alignment::BottomRight, glm::vec2(-1, -1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));

			AddItem(new Border(Border::Orientation::Horizontal, this, Item::Alignment::TopLeft, glm::vec2(1, 28), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));

			int i = 0;
			float y = 40.0f;

			float ls = 20.0f;					// Line Spacing
			float ps = 30.0f;					// Paragraph Spacing
			float lblw = 140.0f;				// Label Width
			float cw1 = Size.x - 30.0f - lblw;	// 1 Column Item Width
			float cw3 = cw1 / 3.0f;				// 3 Column Item Width
			float cw1M = cw1 - 27.0f;			// 1 Column Item Width - Button Margin
			float c1 = 10.0f;					// Column Positions
			float c2 = c1 + lblw;				//  |
			float c3 = c2 + cw3;				//  |
			float c4 = c3 + cw3;				// ---

			AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(lblw, ls), new Text("Tag", 4, 3, Assets::Fonts["Consolas16"])));
			AddItem(new TextBoxString(Feature->GetTagPtr(), WindowPtr, this, Alignment::TopLeft, glm::vec2(c2, y), glm::vec2(cw1, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			y += ls;

			AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(lblw, ls), new Text("Spawn Weight", 4, 3, Assets::Fonts["Consolas16"])));
			AddItem(new TextBoxIntZeroPlus(Feature->GetSpawnWeightPtr(), WindowPtr, this, Alignment::TopLeft, glm::vec2(c2, y), glm::vec2(cw1, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			y += ls;

			AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(lblw, ls), new Text("Spawn Adjustment", 4, 3, Assets::Fonts["Consolas16"])));
			AddItem(new TextBoxIntZeroPlus(Feature->GetSpawnWeightAdjustmentPtr(), WindowPtr, this, Alignment::TopLeft, glm::vec2(c2, y), glm::vec2(cw1, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			y += ls;

			AddItem(new Border(Border::Orientation::Horizontal, this, Item::Alignment::TopLeft, glm::vec2(1, y), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
			y += 10.0f;

			float maxHeight = Size.y - ls + ScrollPosition;
			float minHeight = y + ScrollPosition + ls;

			if (OpenCategory == Category::Categories)
			{
				AddItem(new FeatureEditorCategoryButton(Category::Lights, this, Item::Alignment::TopLeft, glm::vec2(3.0f, y), glm::vec2(Size.x - 6.0f, ls), nullptr, Assets::Textures["DialogBackgroundMO"]));
				AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(lblw, ls), new Text("Lights", 4, 3, Assets::Fonts["Consolas16"])));
				y += ls;

				AddItem(new FeatureEditorCategoryButton(Category::Meshes, this, Item::Alignment::TopLeft, glm::vec2(3.0f, y), glm::vec2(Size.x - 6.0f, ls), nullptr, Assets::Textures["DialogBackgroundMO"]));
				AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(lblw, ls), new Text("Meshes", 4, 3, Assets::Fonts["Consolas16"])));
				y += ls;

				AddItem(new FeatureEditorCategoryButton(Category::Colliders, this, Item::Alignment::TopLeft, glm::vec2(3.0f, y), glm::vec2(Size.x - 6.0f, ls), nullptr, Assets::Textures["DialogBackgroundMO"]));
				AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(lblw, ls), new Text("Colliders", 4, 3, Assets::Fonts["Consolas16"])));
				y += ls;
			}
			else if (OpenCategory == Category::Lights)
			{
				AddItem(new FeatureEditorCategoryButton(Category::Categories, this, Item::Alignment::TopRight, glm::vec2(-7, y), glm::vec2(20, 20), Assets::Textures["CloseButton"], Assets::Textures["CloseButtonMO"]));
				AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(lblw, ls), new Text("Lights:", 4, 3, Assets::Fonts["Consolas16"])));
				AddItem(new FeatureEditorAddLightButton(this, Item::Alignment::TopRight, glm::vec2(-34, y + 4.0f), glm::vec2(12, 12), nullptr, Assets::Textures["Plus"], Assets::Textures["PlusMO"]));
				y += ps;

				auto c = Feature->GetLights();
				for (i = 0; i < c->size(); i++)
				{
					if (y < maxHeight)
					{
						if (y > minHeight)
						{
							AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y - ScrollPosition), glm::vec2(lblw, ls), new Text("Radius:", 4, 3, Assets::Fonts["Consolas16"])));
							AddItem(new TextBoxFloatZeroPlus(&(*c)[i]->Radius, WindowPtr, this, Alignment::TopLeft, glm::vec2(c2, y - ScrollPosition), glm::vec2(cw1M, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
							AddItem(new FeatureEditorRemoveLightButton(i, this, Item::Alignment::TopRight, glm::vec2(-27, y - ScrollPosition), glm::vec2(20, 20), nullptr, nullptr, Assets::Textures["TrashMO"]));
						}
						y += ls;

						if (y > minHeight)
						{
							AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y - ScrollPosition), glm::vec2(lblw, ls), new Text("Color:", 4, 3, Assets::Fonts["Consolas16"])));
							AddItem(new TextBoxRGB(&(*c)[i]->Color.x, WindowPtr, this, Alignment::TopLeft, glm::vec2(c2, y - ScrollPosition), glm::vec2(cw3, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
							AddItem(new TextBoxRGB(&(*c)[i]->Color.y, WindowPtr, this, Alignment::TopLeft, glm::vec2(c3, y - ScrollPosition), glm::vec2(cw3, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
							AddItem(new TextBoxRGB(&(*c)[i]->Color.z, WindowPtr, this, Alignment::TopLeft, glm::vec2(c4, y - ScrollPosition), glm::vec2(cw3, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
							}
						y += ls;

						if (y > minHeight)
						{
							AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y - ScrollPosition), glm::vec2(lblw, ls), new Text("Casts Shadow:", 4, 3, Assets::Fonts["Consolas16"])));
							AddItem(new CheckBox(&(*c)[i]->CastsShadow, this, Alignment::TopLeft, glm::vec2(c2, y + 2.0f - ScrollPosition), glm::vec2(17.0f, 17.0f), Assets::Textures["CloseButton"], Assets::Textures["CloseButtonMO"], nullptr, Assets::Textures["CheckSmall"], Assets::Textures["CheckSmallMO"]));
						}
						y += ls;

						if (y < maxHeight)
						{
							if (y > minHeight)
							{
								AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y - ScrollPosition), glm::vec2(lblw, ls), new Text("Position: X", 4, 3, Assets::Fonts["Consolas16"])));
								AddItem(new TextBoxFloat(&(*c)[i]->Transform.Position.x, WindowPtr, this, Alignment::TopLeft, glm::vec2(c2, y - ScrollPosition), glm::vec2(cw1, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
							}
							y += ls;
						}
						if (y < maxHeight)
						{
							if (y > minHeight)
							{
								AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y - ScrollPosition), glm::vec2(lblw, ls), new Text("          Y", 4, 3, Assets::Fonts["Consolas16"])));
								AddItem(new TextBoxFloat(&(*c)[i]->Transform.Position.y, WindowPtr, this, Alignment::TopLeft, glm::vec2(c2, y - ScrollPosition), glm::vec2(cw1, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
							}
							y += ls;
						}
						if (y < maxHeight)
						{
							if (y > minHeight)
							{
								AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y - ScrollPosition), glm::vec2(lblw, ls), new Text("          Z", 4, 3, Assets::Fonts["Consolas16"])));
								AddItem(new TextBoxFloat(&(*c)[i]->Transform.Position.z, WindowPtr, this, Alignment::TopLeft, glm::vec2(c2, y - ScrollPosition), glm::vec2(cw1, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
							}
							y += ls;
						}

						if (y < maxHeight)
						{
							if (y > minHeight)
							{
								AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y - ScrollPosition), glm::vec2(lblw, ls), new Text("Scale:    X", 4, 3, Assets::Fonts["Consolas16"])));
								AddItem(new TextBoxFloatZeroPlus(&(*c)[i]->Transform.Scale.x, WindowPtr, this, Alignment::TopLeft, glm::vec2(c2, y - ScrollPosition), glm::vec2(cw1, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
							}
							y += ls;
						}
						if (y < maxHeight)
						{
							if (y > minHeight)
							{
								AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y - ScrollPosition), glm::vec2(lblw, ls), new Text("          Y", 4, 3, Assets::Fonts["Consolas16"])));
								AddItem(new TextBoxFloatZeroPlus(&(*c)[i]->Transform.Scale.y, WindowPtr, this, Alignment::TopLeft, glm::vec2(c2, y - ScrollPosition), glm::vec2(cw1, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
							}
							y += ls;
						}
						if (y < maxHeight)
						{
							if (y > minHeight)
							{
								AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y - ScrollPosition), glm::vec2(lblw, ls), new Text("          Z", 4, 3, Assets::Fonts["Consolas16"])));
								AddItem(new TextBoxFloatZeroPlus(&(*c)[i]->Transform.Scale.z, WindowPtr, this, Alignment::TopLeft, glm::vec2(c2, y - ScrollPosition), glm::vec2(cw1, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
							}
							y += ps;
						}
					}
				}
			}
			else if (OpenCategory == Category::Meshes)
			{
				AddItem(new FeatureEditorCategoryButton(Category::Categories, this, Item::Alignment::TopRight, glm::vec2(-7, y), glm::vec2(20, 20), Assets::Textures["CloseButton"], Assets::Textures["CloseButtonMO"]));
				AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(lblw, ls), new Text("Meshes:", 4, 3, Assets::Fonts["Consolas16"])));
				AddItem(new FeatureEditorAddMeshButton(this, Item::Alignment::TopRight, glm::vec2(-34, y + 4.0f), glm::vec2(12, 12), nullptr, Assets::Textures["Plus"], Assets::Textures["PlusMO"]));
				y += ps;

				auto c = Feature->GetMeshes();
				for (i = 0; i < c->size(); i++)
				{
					if (y < maxHeight)
					{
						if (y > minHeight)
						{
							AddItem(new CharacterEditorMeshButton(&(*c)[i], WindowPtr, this, glm::vec2(2, y - ScrollPosition), glm::vec2(350, 20), new Text((*c)[i].first, 12, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))));
							AddItem(new CharacterEditorMaterialButton(&(*c)[i], WindowPtr, this, glm::vec2(352, y - ScrollPosition), glm::vec2(100, ls), new Text((*c)[i].second, 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))));
							AddItem(new FeatureEditorRemoveMeshButton(i, this, Item::Alignment::TopRight, glm::vec2(-27, y - ScrollPosition), glm::vec2(20, 20), nullptr, nullptr, Assets::Textures["TrashMO"]));
						}
						y += ls;
					}
				}
			}
			else if (OpenCategory == Category::Colliders)
			{
				AddItem(new FeatureEditorCategoryButton(Category::Categories, this, Item::Alignment::TopRight, glm::vec2(-7, y), glm::vec2(20, 20), Assets::Textures["CloseButton"], Assets::Textures["CloseButtonMO"]));
				AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(lblw, ls), new Text("Colliders:", 4, 3, Assets::Fonts["Consolas16"])));
				AddItem(new FeatureEditorAddColliderButton(this, Item::Alignment::TopRight, glm::vec2(-34, y + 4.0f), glm::vec2(12, 12), nullptr, Assets::Textures["Plus"], Assets::Textures["PlusMO"]));
				y += ps;

				auto colliders = Feature->GetColliders();
				for (i = 0; i < colliders->size(); i++)
				{
					if (y < maxHeight)
					{
						if (y > minHeight)
						{
							AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y - ScrollPosition), glm::vec2(lblw, ls), new Text("Shape:", 4, 3, Assets::Fonts["Consolas16"])));
							//AddItem(new TextBoxInt(&f->ShapeType, WindowPtr, this, Alignment::TopLeft, glm::vec2(c3, y - ScrollPosition), glm::vec2(cw3, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
							AddItem(new FeatureEditorRemoveColliderButton(i, this, Item::Alignment::TopRight, glm::vec2(-27, y - ScrollPosition), glm::vec2(20, 20), nullptr, nullptr, Assets::Textures["TrashMO"]));
						}
						y += ls;
					}

					if (y < maxHeight)
					{
						if (y > minHeight)
						{
							AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y - ScrollPosition), glm::vec2(lblw, ls), new Text("Position: X", 4, 3, Assets::Fonts["Consolas16"])));
							AddItem(new TextBoxFloat(&(*colliders)[i]->Transform.Position.x, WindowPtr, this, Alignment::TopLeft, glm::vec2(c3, y - ScrollPosition), glm::vec2(cw1, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
						}
						y += ls;
					}
					if (y < maxHeight)
					{
						if (y > minHeight)
						{
							AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y - ScrollPosition), glm::vec2(lblw, 20), new Text("          Y", 4, 3, Assets::Fonts["Consolas16"])));
							AddItem(new TextBoxFloat(&(*colliders)[i]->Transform.Position.y, WindowPtr, this, Alignment::TopLeft, glm::vec2(c3, y - ScrollPosition), glm::vec2(cw1, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
						}
						y += ls;
					}
					if (y < maxHeight)
					{
						if (y > minHeight)
						{
							AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y - ScrollPosition), glm::vec2(lblw, 20), new Text("          Z", 4, 3, Assets::Fonts["Consolas16"])));
							AddItem(new TextBoxFloat(&(*colliders)[i]->Transform.Position.z, WindowPtr, this, Alignment::TopLeft, glm::vec2(c3, y - ScrollPosition), glm::vec2(cw1, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
						}
						y += ls;
					}

					if (y < maxHeight)
					{
						if (y > minHeight)
						{
							AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y - ScrollPosition), glm::vec2(lblw, 20), new Text("Rotation: X", 4, 3, Assets::Fonts["Consolas16"])));
							AddItem(new TextBoxFloat(&(*colliders)[i]->Transform.Rotation.x, WindowPtr, this, Alignment::TopLeft, glm::vec2(c3, y - ScrollPosition), glm::vec2(cw1, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
						}
						y += ls;
					}
					if (y < maxHeight)
					{
						if (y > minHeight)
						{
							AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y - ScrollPosition), glm::vec2(lblw, 20), new Text("          Y", 4, 3, Assets::Fonts["Consolas16"])));
							AddItem(new TextBoxFloat(&(*colliders)[i]->Transform.Rotation.y, WindowPtr, this, Alignment::TopLeft, glm::vec2(c3, y - ScrollPosition), glm::vec2(cw1, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
						}
						y += ls;
					}
					if (y < maxHeight)
					{
						if (y > minHeight)
						{
							AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y - ScrollPosition), glm::vec2(lblw, 20), new Text("          Z", 4, 3, Assets::Fonts["Consolas16"])));
							AddItem(new TextBoxFloat(&(*colliders)[i]->Transform.Rotation.z, WindowPtr, this, Alignment::TopLeft, glm::vec2(c3, y - ScrollPosition), glm::vec2(cw1, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
						}
						y += ls;
					}
					if (y < maxHeight)
					{
						if (y > minHeight)
						{
							AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y - ScrollPosition), glm::vec2(lblw, ls), new Text("          W", 4, 3, Assets::Fonts["Consolas16"])));
							AddItem(new TextBoxFloat(&(*colliders)[i]->Transform.Rotation.w, WindowPtr, this, Alignment::TopLeft, glm::vec2(c3, y - ScrollPosition), glm::vec2(cw1, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
						}
						y += ls;
					}

					if (y < maxHeight)
					{
						if (y > minHeight)
						{
							AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y - ScrollPosition), glm::vec2(lblw, ls), new Text("Scale:    X", 4, 3, Assets::Fonts["Consolas16"])));
							AddItem(new TextBoxFloat(&(*colliders)[i]->Transform.Scale.x, WindowPtr, this, Alignment::TopLeft, glm::vec2(c2, y - ScrollPosition), glm::vec2(cw1, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
						}
						y += ls;
					}
					if (y < maxHeight)
					{
						if (y > minHeight)
						{
							AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y - ScrollPosition), glm::vec2(lblw, ls), new Text("          Y", 4, 3, Assets::Fonts["Consolas16"])));
							AddItem(new TextBoxFloat(&(*colliders)[i]->Transform.Scale.y, WindowPtr, this, Alignment::TopLeft, glm::vec2(c2, y - ScrollPosition), glm::vec2(cw1, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
						}
						y += ls;
					}
					if (y < maxHeight)
					{
						if (y > minHeight)
						{
							AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y - ScrollPosition), glm::vec2(lblw, ls), new Text("          Z", 4, 3, Assets::Fonts["Consolas16"])));
							AddItem(new TextBoxFloat(&(*colliders)[i]->Transform.Scale.z, WindowPtr, this, Alignment::TopLeft, glm::vec2(c2, y - ScrollPosition), glm::vec2(cw1, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
						}
						y += ps;
					}
				}
			}

			if (y >= maxHeight || ScrollPosition > 5.0f)
			{
				// Add Scroll Buttons / Bar
				AddItem(new FeatureEditorScrollButton(-40.0f, this, Alignment::TopRight, glm::vec2(-7, minHeight - ScrollPosition + 7), glm::vec2(16, 16), Assets::Textures["Up"], Assets::Textures["UpMO"]));
				AddItem(new FeatureEditorScrollButton(40.0f, this, Alignment::BottomRight, glm::vec2(-7, -7), glm::vec2(16, 16), Assets::Textures["Down"], Assets::Textures["DownMO"]));
			}

			Invalidate();
		}





		FeatureEditorSaveButton::FeatureEditorSaveButton(FeatureEditor* editor, Window* window, Alignment alignment, glm::vec2 position, glm::vec2 size, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(editor, alignment, position, size, nullptr, image, mouseOverImage, mouseDownImage)
		{
			WindowPtr = window;
			Editor = editor;
		}

		FeatureEditorSaveButton::~FeatureEditorSaveButton()	{}

		void FeatureEditorSaveButton::OnClick(int button)
		{
			WindowPtr->Scene->AppendConsole("Saving tile: " + Editor->Feature->GetName());
			Editor->Feature->Save();
			WindowPtr->Scene->LoadFeature(Editor->Feature->GetName());
		}





		FeatureEditorCategoryButton::FeatureEditorCategoryButton(FeatureEditor::Category target, FeatureEditor* editor, Alignment alignment, glm::vec2 position, glm::vec2 size, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(editor, alignment, position, size, nullptr, image, mouseOverImage, mouseDownImage)
		{
			Editor = editor;
			Target = target;
		}

		FeatureEditorCategoryButton::~FeatureEditorCategoryButton()	{}

		void FeatureEditorCategoryButton::OnClick(int button)
		{
			Editor->OpenCategory = Target;
			Editor->ScrollPosition = 0.0f;
			Editor->Rebuild();
		}





		FeatureEditorScrollButton::FeatureEditorScrollButton(float delta, FeatureEditor* editor, Alignment alignment, glm::vec2 position, glm::vec2 size, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(editor, alignment, position, size, nullptr, image, mouseOverImage, mouseDownImage)
		{
			Editor = editor;
			Delta = delta;
		}

		FeatureEditorScrollButton::~FeatureEditorScrollButton()	{}

		void FeatureEditorScrollButton::OnClick(int button)
		{
			Editor->ScrollPosition += Delta;
			if (Editor->ScrollPosition < 0.0f)
			{
				Editor->ScrollPosition = 0.0f;
			}
			Editor->Rebuild();
		}





		FeatureEditorAddLightButton::FeatureEditorAddLightButton(FeatureEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			Editor = parent;
		}

		FeatureEditorAddLightButton::~FeatureEditorAddLightButton() {}

		void FeatureEditorAddLightButton::OnClick(int button)
		{
			auto n = new LightNode;
			n->Color = glm::vec3(1.0f);
			n->Radius = 1.0f;
			n->CastsShadow = false;
			n->Transform.Scale = glm::vec3(0.05f, 0.05f, 0.05f);
			Editor->Feature->GetLights()->push_back(n);
			Editor->Rebuild();
		}




		FeatureEditorRemoveLightButton::FeatureEditorRemoveLightButton(int i, FeatureEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			Index = i;
			Editor = parent;
		}

		FeatureEditorRemoveLightButton::~FeatureEditorRemoveLightButton() {}

		void FeatureEditorRemoveLightButton::OnClick(int button)
		{
			auto c = Editor->Feature->GetLights();
			(*c)[Index] = c->back();
			c->resize(c->size() - 1);
			Editor->Rebuild();
		}





		FeatureEditorAddMeshButton::FeatureEditorAddMeshButton(FeatureEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			Editor = parent;
		}

		FeatureEditorAddMeshButton::~FeatureEditorAddMeshButton() {}

		void FeatureEditorAddMeshButton::OnClick(int button)
		{
			Editor->Feature->GetMeshes()->push_back(std::pair<std::string, std::string>(std::string("Cube"), std::string("Gold")));
			Editor->Rebuild();
		}




		FeatureEditorRemoveMeshButton::FeatureEditorRemoveMeshButton(int i, FeatureEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			Index = i;
			Editor = parent;
		}

		FeatureEditorRemoveMeshButton::~FeatureEditorRemoveMeshButton() {}

		void FeatureEditorRemoveMeshButton::OnClick(int button)
		{
			auto c = Editor->Feature->GetMeshes();
			(*c)[Index] = c->back();
			c->resize(c->size() - 1);
			Editor->Rebuild();
		}





		FeatureEditorAddColliderButton::FeatureEditorAddColliderButton(FeatureEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			Editor = parent;
		}

		FeatureEditorAddColliderButton::~FeatureEditorAddColliderButton() {}

		void FeatureEditorAddColliderButton::OnClick(int button)
		{
			auto n = new Collider;
			n->ShapeType = Collider::Type::Box;
			Editor->Feature->GetColliders()->push_back(n);
			Editor->Rebuild();
		}




		FeatureEditorRemoveColliderButton::FeatureEditorRemoveColliderButton(int i, FeatureEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			Index = i;
			Editor = parent;
		}

		FeatureEditorRemoveColliderButton::~FeatureEditorRemoveColliderButton() {}

		void FeatureEditorRemoveColliderButton::OnClick(int button)
		{
			auto c = Editor->Feature->GetColliders();
			(*c)[Index] = c->back();
			c->resize(c->size() - 1);
			Editor->Rebuild();
		}

	}
}