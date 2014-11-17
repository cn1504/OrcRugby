#include "TileEditor.h"
#include "Assets.h"
#include "Window.h"

namespace Core
{
	namespace Gui
	{

		EditableTile::EditableTile(std::string name)
			: Tile(name)
		{
			Load();
		}

		EditableTile::~EditableTile() {}

		std::string* EditableTile::GetName() { return &Name; }

		int* EditableTile::GetTileWeight() { return &TileWeight; }
		int* EditableTile::GetTileAdjustment() { return &TileAdjustment; }

		bool* EditableTile::GetOnlySpawnOnce() { return &OnlySpawnOnce; }

		glm::vec3* EditableTile::GetLowerBound() { return &LowerBound; }
		glm::vec3* EditableTile::GetUpperBound() { return &UpperBound; }
		
		std::vector<Node*>* EditableTile::GetTransitions() { return &Transitions; }
		std::vector<Node*>* EditableTile::GetFeatures() { return &Features; }
		std::vector<Node*>* EditableTile::GetPlaceables() { return &Placeables; }
		std::vector<Node*>* EditableTile::GetCreatures() { return &Creatures; }
		std::vector<LightNode*>* EditableTile::GetLights() { return &Lights; }
		std::vector<Node*>* EditableTile::GetAudioSources() { return &AudioSources; }
		std::vector<std::pair<std::string, std::string>>* EditableTile::GetMeshes() { return &Meshes; }
		std::vector<Collider*>* EditableTile::GetColliders() { return &Colliders; }





		TileButton::TileButton(Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			WindowPtr = window;
		}

		TileButton::~TileButton()	{}

		void TileButton::OnClick(int button)
		{
			WindowPtr->Scene->Gui->AddItem(new TileBrowser("Tiles", "Resources/Tiles/", "", WindowPtr, WindowPtr->Scene->Gui, Item::Alignment::Center, glm::vec2(), glm::vec2(220, 150), nullptr, Assets::Textures["DialogBackground"]));
		}





		TileBrowser::TileBrowser(std::string title, std::string path, std::string targetExt, Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: FileBrowser(title, path, targetExt, window, parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			AddItem(new TileBrowserNewButton(WindowPtr, this, Alignment::TopRight, glm::vec2(-27, 9), glm::vec2(10, 10), Assets::Textures["Plus"], Assets::Textures["PlusMO"]));
		}

		TileBrowser::~TileBrowser() {}

		void TileBrowser::Resolve(std::string name)
		{
			std::string file = name;
			if (FilePaths.count(file) > 0)
			{
				Debug::Log("Calling scene->loadtile " + Path + " " + ((Path.length() > 16) ? std::string(&Path[16]) + file : file));
				WindowPtr->Scene->LoadTile((Path.length() > 16) ? std::string(&Path[16]) + file : file);
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
				AddItem(new TileBrowserNewButton(WindowPtr, this, Alignment::TopRight, glm::vec2(-27, 9), glm::vec2(10, 10), Assets::Textures["Plus"], Assets::Textures["PlusMO"]));
			}
		}


		


		TileBrowserNewButton::TileBrowserNewButton(Window* window, TileBrowser* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, nullptr, image, mouseOverImage, mouseDownImage)
		{
			WindowPtr = window;
			CB = parent;
		}

		TileBrowserNewButton::~TileBrowserNewButton() {}

		void TileBrowserNewButton::OnClick(int button)
		{
			auto p = CB->GetCurrentPath();
			if (p.length() > 16)
				p = std::string(&p[16]);
			else
				p = "";
			WindowPtr->Scene->Gui->AddItem(new TileCreationDialog(p, WindowPtr, WindowPtr->Scene->Gui, Alignment::Center, glm::vec2(0, 0), glm::vec2(220, 75), Assets::Textures["DialogBackground"]));
			Parent->Destroy();
		}





		TileCreationNameTextBox::TileCreationNameTextBox(Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label)
			: TextBox(window, parent, alignment, position, size, label, nullptr, Assets::Textures["DialogBackgroundMO"], nullptr, Assets::Textures["TextBoxEdit"])
		{
		}

		TileCreationNameTextBox::~TileCreationNameTextBox() {}





		TileCreationAcceptButton::TileCreationAcceptButton(TileCreationDialog* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			Dialog = parent;
		}

		TileCreationAcceptButton::~TileCreationAcceptButton() {}

		void TileCreationAcceptButton::OnClick(int button)
		{
			Dialog->Accept();
		}





		TileCreationDialog::TileCreationDialog(std::string path, Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, nullptr, image, mouseOverImage, mouseDownImage)
		{
			Debug::Log(path);

			WindowPtr = window;
			Path = path;
			NameTextBox = new TileCreationNameTextBox(window, this, Alignment::BottomCenter, glm::vec2(-10, -13), glm::vec2(160, 20), new Text("TileName", 4, 3, Assets::Fonts["Consolas16"]));
			AddItem(NameTextBox);
			AddItem(new TileCreationAcceptButton(this, Alignment::BottomCenter, glm::vec2(85, -10), glm::vec2(26, 26), nullptr, Assets::Textures["Check"], Assets::Textures["CheckMO"]));

			AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(5, 0), glm::vec2(150, 28), new Text("New Tile", 4, 4, Assets::Fonts["Consolas24"], glm::vec4(0.9, 0.9, 0.9, 1.0))));

			AddItem(new Border(Border::Orientation::Horizontal, this, Item::Alignment::TopLeft, glm::vec2(1, 1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
			AddItem(new Border(Border::Orientation::Vertical, this, Item::Alignment::TopLeft, glm::vec2(1, 1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
			AddItem(new Border(Border::Orientation::Horizontal, this, Item::Alignment::BottomRight, glm::vec2(-1, -1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
			AddItem(new Border(Border::Orientation::Vertical, this, Item::Alignment::BottomRight, glm::vec2(-1, -1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));

			AddItem(new Border(Border::Orientation::Horizontal, this, Item::Alignment::TopLeft, glm::vec2(1, 28), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
			AddItem(new DialogCloseButton(this, this, Item::Alignment::TopRight, glm::vec2(-6, 6), glm::vec2(17, 17), nullptr, Assets::Textures["CloseButton"], Assets::Textures["CloseButtonMO"]));

			Invalidate();
		}

		TileCreationDialog::~TileCreationDialog() {}

		void TileCreationDialog::Accept()
		{
			auto name = NameTextBox->GetLabel();

			Debug::Log("Creating new tile: " + name);
			WindowPtr->Scene->AppendConsole("Creating new tile: " + name);

			Tile tile(Path + name);
			tile.LoadDefaults();
			tile.Save();

			WindowPtr->Scene->LoadTile(Path + name);
			Destroy();
		}





		TileEditor::TileEditor(std::string tile, Window* window, Item* parent)
			: Item(parent, Item::Alignment::TopLeft, glm::vec2(-500, 100), glm::vec2(500, parent->GetDimensions().w - 160), nullptr, Assets::Textures["DialogBackground"])
		{
			WindowPtr = window;
			ShouldClose = false;
			Tile = new EditableTile(tile);

			OpenCategory = Category::Categories;
			ScrollPosition = 0.0f;

			Rebuild();

			Move(glm::vec2(Size.x, 0), 0.3f, MoveStyle::Sinosoidal);
		}

		TileEditor::~TileEditor()	
		{
			delete Tile;
		}

		void TileEditor::Destroy()
		{
			Move(glm::vec2(-Size.x, 0), 0.3f, MoveStyle::Sinosoidal);
			ShouldClose = true;
		}

		void TileEditor::Update(glm::vec2 mousePosition)
		{
			if (ShouldClose && !IsMoving())
			{
				if (WindowPtr->Scene->TileEditor == this)
					WindowPtr->Scene->TileEditor = nullptr;
				Item::Destroy();
			}

			Item::Update(mousePosition);
		}

		void TileEditor::Invalidate(bool includeChildren)
		{
			Size.y = Parent->GetDimensions().w - 160;
			Item::Invalidate(includeChildren);
		}

		void TileEditor::Rebuild()
		{
			for (auto c : Children)
			{
				c->Destroy();
			}
			Children.clear();
			
			auto Title = new Text(*Tile->GetName(), 4, 4, Assets::Fonts["Consolas24"], glm::vec4(0.9, 0.9, 0.9, 1.0));
			AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(5, 0), glm::vec2(150, 28), Title));
			// AddItem(new TileEditorSaveButton(Character, WindowPtr, this));
			AddItem(new DialogCloseButton(this, this, Alignment::TopRight, glm::vec2(-7, 5), glm::vec2(20, 20), nullptr, Assets::Textures["CloseButton"], Assets::Textures["CloseButtonMO"]));

			AddItem(new TileEditorSaveButton(this, WindowPtr, Alignment::TopRight, glm::vec2(-34, 5), glm::vec2(20, 20), Assets::Textures["Save"], Assets::Textures["SaveMO"]));

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

			AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(lblw, ls), new Text("Spawn Weight", 4, 3, Assets::Fonts["Consolas16"])));
			AddItem(new TextBoxIntZeroPlus(Tile->GetTileWeight(), WindowPtr, this, Alignment::TopLeft, glm::vec2(c2, y), glm::vec2(cw1, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			y += ls;

			AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(lblw, ls), new Text("Spawn Adjustment", 4, 3, Assets::Fonts["Consolas16"])));
			AddItem(new TextBoxIntZeroPlus(Tile->GetTileAdjustment(), WindowPtr, this, Alignment::TopLeft, glm::vec2(c2, y), glm::vec2(cw1, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			y += ls;

			AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(lblw, ls), new Text("Only Spawn Once", 4, 3, Assets::Fonts["Consolas16"])));
			AddItem(new CheckBox(Tile->GetOnlySpawnOnce(), this, Alignment::TopLeft, glm::vec2(c2, y + 2.0f), glm::vec2(17.0f, 17.0f), Assets::Textures["CloseButton"], Assets::Textures["CloseButtonMO"], nullptr, Assets::Textures["CheckSmall"], Assets::Textures["CheckSmallMO"]));
			y += ps;

			AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(lblw, ls), new Text("Lower Bound: X", 4, 3, Assets::Fonts["Consolas16"])));
			AddItem(new TextBoxFloat(&Tile->GetLowerBound()->x, WindowPtr, this, Alignment::TopLeft, glm::vec2(c2, y), glm::vec2(cw1, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			y += ls;
			AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(lblw, ls), new Text("             Y", 4, 3, Assets::Fonts["Consolas16"])));
			AddItem(new TextBoxFloat(&Tile->GetLowerBound()->y, WindowPtr, this, Alignment::TopLeft, glm::vec2(c2, y), glm::vec2(cw1, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			y += ls;
			AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(lblw, ls), new Text("             Z", 4, 3, Assets::Fonts["Consolas16"])));
			AddItem(new TextBoxFloat(&Tile->GetLowerBound()->z, WindowPtr, this, Alignment::TopLeft, glm::vec2(c2, y), glm::vec2(cw1, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			y += ps;

			AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(lblw, ls), new Text("Upper Bound: X", 4, 3, Assets::Fonts["Consolas16"])));
			AddItem(new TextBoxFloat(&Tile->GetUpperBound()->x, WindowPtr, this, Alignment::TopLeft, glm::vec2(c2, y), glm::vec2(cw1, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			y += ls;
			AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(lblw, ls), new Text("             Y", 4, 3, Assets::Fonts["Consolas16"])));
			AddItem(new TextBoxFloat(&Tile->GetUpperBound()->y, WindowPtr, this, Alignment::TopLeft, glm::vec2(c2, y), glm::vec2(cw1, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			y += ls;
			AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(lblw, ls), new Text("             Z", 4, 3, Assets::Fonts["Consolas16"])));
			AddItem(new TextBoxFloat(&Tile->GetUpperBound()->z, WindowPtr, this, Alignment::TopLeft, glm::vec2(c2, y), glm::vec2(cw1, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			y += ps;

			AddItem(new Border(Border::Orientation::Horizontal, this, Item::Alignment::TopLeft, glm::vec2(1, y), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
			y += 10.0f;
			
			float maxHeight = Size.y - ls + ScrollPosition;
			float minHeight = y + ScrollPosition + ls;

			if (OpenCategory == Category::Categories)
			{
				AddItem(new TileEditorCategoryButton(Category::Transitions, this, Item::Alignment::TopLeft, glm::vec2(3.0f, y), glm::vec2(Size.x - 6.0f, ls), nullptr, Assets::Textures["DialogBackgroundMO"]));
				AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(lblw, ls), new Text("Transitions", 4, 3, Assets::Fonts["Consolas16"])));
				y += ls;

				AddItem(new TileEditorCategoryButton(Category::Features, this, Item::Alignment::TopLeft, glm::vec2(3.0f, y), glm::vec2(Size.x - 6.0f, ls), nullptr, Assets::Textures["DialogBackgroundMO"]));
				AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(lblw, ls), new Text("Features", 4, 3, Assets::Fonts["Consolas16"])));
				y += ls;

				AddItem(new TileEditorCategoryButton(Category::Lights, this, Item::Alignment::TopLeft, glm::vec2(3.0f, y), glm::vec2(Size.x - 6.0f, ls), nullptr, Assets::Textures["DialogBackgroundMO"]));
				AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(lblw, ls), new Text("Lights", 4, 3, Assets::Fonts["Consolas16"])));
				y += ls;

				AddItem(new TileEditorCategoryButton(Category::Meshes, this, Item::Alignment::TopLeft, glm::vec2(3.0f, y), glm::vec2(Size.x - 6.0f, ls), nullptr, Assets::Textures["DialogBackgroundMO"]));
				AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(lblw, ls), new Text("Meshes", 4, 3, Assets::Fonts["Consolas16"])));
				y += ls;

				AddItem(new TileEditorCategoryButton(Category::Colliders, this, Item::Alignment::TopLeft, glm::vec2(3.0f, y), glm::vec2(Size.x - 6.0f, ls), nullptr, Assets::Textures["DialogBackgroundMO"]));
				AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(lblw, ls), new Text("Colliders", 4, 3, Assets::Fonts["Consolas16"])));
				y += ls;
			}
			else if (OpenCategory == Category::Transitions)
			{
				AddItem(new TileEditorCategoryButton(Category::Categories, this, Item::Alignment::TopRight, glm::vec2(-7, y), glm::vec2(20, 20), Assets::Textures["CloseButton"], Assets::Textures["CloseButtonMO"]));
				AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(lblw, ls), new Text("Transitions:", 4, 3, Assets::Fonts["Consolas16"])));
				AddItem(new TileEditorAddTransitionButton(this, Item::Alignment::TopRight, glm::vec2(-34, y + 4.0f), glm::vec2(12, 12), nullptr, Assets::Textures["Plus"], Assets::Textures["PlusMO"]));
				y += ps;

				auto transitions = Tile->GetTransitions();
				for (i = 0; i < transitions->size(); i++)
				{
					if (y < maxHeight)
					{
						if (y > minHeight)
						{
							AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y - ScrollPosition), glm::vec2(lblw, ls), new Text("Tag:", 4, 3, Assets::Fonts["Consolas16"])));
							AddItem(new TextBoxString(&(*transitions)[i]->Tag, WindowPtr, this, Alignment::TopLeft, glm::vec2(c2, y - ScrollPosition), glm::vec2(cw1M, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
							AddItem(new TileEditorRemoveTransitionButton(i, this, Item::Alignment::TopRight, glm::vec2(-27, y - ScrollPosition), glm::vec2(20, 20), nullptr, nullptr, Assets::Textures["TrashMO"]));
						}
						y += ls;
					}

					if (y < maxHeight)
					{
						if (y > minHeight)
						{
							AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y - ScrollPosition), glm::vec2(lblw, ls), new Text("Position: X", 4, 3, Assets::Fonts["Consolas16"])));
							AddItem(new TextBoxFloat(&(*transitions)[i]->Transform.Position.x, WindowPtr, this, Alignment::TopLeft, glm::vec2(c2, y - ScrollPosition), glm::vec2(cw1, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
						}
						y += ls;
					}
					if (y - ScrollPosition < maxHeight)
					{
						if (y > minHeight)
						{
							AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y - ScrollPosition), glm::vec2(lblw, ls), new Text("          Y", 4, 3, Assets::Fonts["Consolas16"])));
							AddItem(new TextBoxFloat(&(*transitions)[i]->Transform.Position.y, WindowPtr, this, Alignment::TopLeft, glm::vec2(c2, y - ScrollPosition), glm::vec2(cw1, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
						}
						y += ls;
					}
					if (y < maxHeight)
					{
						if (y > minHeight)
						{
							AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y - ScrollPosition), glm::vec2(lblw, ls), new Text("          Z", 4, 3, Assets::Fonts["Consolas16"])));
							AddItem(new TextBoxFloat(&(*transitions)[i]->Transform.Position.z, WindowPtr, this, Alignment::TopLeft, glm::vec2(c2, y - ScrollPosition), glm::vec2(cw1, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
						}
						y += ps;
					}
				}
			}		
			else if (OpenCategory == Category::Features)
			{
				AddItem(new TileEditorCategoryButton(Category::Categories, this, Item::Alignment::TopRight, glm::vec2(-7, y), glm::vec2(20, 20), Assets::Textures["CloseButton"], Assets::Textures["CloseButtonMO"]));
				AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(lblw, ls), new Text("Features:", 4, 3, Assets::Fonts["Consolas16"])));
				AddItem(new TileEditorAddFeatureButton(this, Item::Alignment::TopRight, glm::vec2(-34, y + 4.0f), glm::vec2(12, 12), nullptr, Assets::Textures["Plus"], Assets::Textures["PlusMO"]));
				y += ps;

				auto features = Tile->GetFeatures();
				Debug::Log(std::to_string(features->size()));
				for (i = 0; i < features->size(); i++)
				{
					if (y < maxHeight)
					{
						if (y > minHeight)
						{
							AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y - ScrollPosition), glm::vec2(lblw, ls), new Text("Tag:", 4, 3, Assets::Fonts["Consolas16"])));
							AddItem(new TextBoxString(&(*features)[i]->Tag, WindowPtr, this, Alignment::TopLeft, glm::vec2(c2, y - ScrollPosition), glm::vec2(cw1M, 20.0f), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
							AddItem(new TileEditorRemoveFeatureButton(i, this, Item::Alignment::TopRight, glm::vec2(-27, y - ScrollPosition), glm::vec2(20, 20), nullptr, nullptr, Assets::Textures["TrashMO"]));
						}
						y += ls;
					}

					if (y < maxHeight)
					{
						if (y > minHeight)
						{
							AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y - ScrollPosition), glm::vec2(lblw, ls), new Text("Position: X", 4, 3, Assets::Fonts["Consolas16"])));
							AddItem(new TextBoxFloat(&(*features)[i]->Transform.Position.x, WindowPtr, this, Alignment::TopLeft, glm::vec2(c2, y - ScrollPosition), glm::vec2(cw1, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
						}
						y += ls;
					}
					if (y < maxHeight)
					{
						if (y > minHeight)
						{
							AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y - ScrollPosition), glm::vec2(lblw, ls), new Text("          Y", 4, 3, Assets::Fonts["Consolas16"])));
							AddItem(new TextBoxFloat(&(*features)[i]->Transform.Position.y, WindowPtr, this, Alignment::TopLeft, glm::vec2(c2, y - ScrollPosition), glm::vec2(cw1, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
						}
						y += ls;
					}
					if (y < maxHeight)
					{
						if (y > minHeight)
						{
							AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y - ScrollPosition), glm::vec2(lblw, ls), new Text("          Z", 4, 3, Assets::Fonts["Consolas16"])));
							AddItem(new TextBoxFloat(&(*features)[i]->Transform.Position.z, WindowPtr, this, Alignment::TopLeft, glm::vec2(c2, y - ScrollPosition), glm::vec2(cw1, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
						}
						y += ls;
					}

					if (y < maxHeight)
					{
						if (y > minHeight)
						{
							AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y - ScrollPosition), glm::vec2(lblw, ls), new Text("Rotation: Y", 4, 3, Assets::Fonts["Consolas16"])));
							AddItem(new TextBoxRotationY(&(*features)[i]->Transform.Rotation, WindowPtr, this, Alignment::TopLeft, glm::vec2(c2, y - ScrollPosition), glm::vec2(cw1, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
						}
						y += ps;
					}
				}
			}
			else if (OpenCategory == Category::Lights)
			{
				AddItem(new TileEditorCategoryButton(Category::Categories, this, Item::Alignment::TopRight, glm::vec2(-7, y), glm::vec2(20, 20), Assets::Textures["CloseButton"], Assets::Textures["CloseButtonMO"]));
				AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(lblw, ls), new Text("Lights:", 4, 3, Assets::Fonts["Consolas16"])));
				AddItem(new TileEditorAddLightButton(this, Item::Alignment::TopRight, glm::vec2(-34, y + 4.0f), glm::vec2(12, 12), nullptr, Assets::Textures["Plus"], Assets::Textures["PlusMO"]));
				y += ps;

				auto c = Tile->GetLights();
				for (i = 0; i < c->size(); i++)
				{
					if (y < maxHeight)
					{
						if (y > minHeight)
						{
							AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y - ScrollPosition), glm::vec2(lblw, ls), new Text("Radius:", 4, 3, Assets::Fonts["Consolas16"])));
							AddItem(new TextBoxFloatZeroPlus(&(*c)[i]->Radius, WindowPtr, this, Alignment::TopLeft, glm::vec2(c2, y - ScrollPosition), glm::vec2(cw1M, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
							AddItem(new TileEditorRemoveLightButton(i, this, Item::Alignment::TopRight, glm::vec2(-27, y + 2.0f - ScrollPosition), glm::vec2(20, 20), nullptr, nullptr, Assets::Textures["TrashMO"]));
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
							AddItem(new CheckBox(&(*c)[i]->CastsShadow, this, Alignment::TopLeft, glm::vec2(c2, y + 2.0f), glm::vec2(17.0f, 17.0f), Assets::Textures["CloseButton"], Assets::Textures["CloseButtonMO"], nullptr, Assets::Textures["CheckSmall"], Assets::Textures["CheckSmallMO"]));
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
				AddItem(new TileEditorCategoryButton(Category::Categories, this, Item::Alignment::TopRight, glm::vec2(-7, y), glm::vec2(20, 20), Assets::Textures["CloseButton"], Assets::Textures["CloseButtonMO"]));
				AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(lblw, ls), new Text("Meshes:", 4, 3, Assets::Fonts["Consolas16"])));
				AddItem(new TileEditorAddMeshButton(this, Item::Alignment::TopRight, glm::vec2(-34, y + 4.0f), glm::vec2(12, 12), nullptr, Assets::Textures["Plus"], Assets::Textures["PlusMO"]));
				y += ps;

				auto c = Tile->GetMeshes();
				for (i = 0; i < c->size(); i++)
				{
					if (y < maxHeight)
					{
						if (y > minHeight)
						{
							AddItem(new CharacterEditorMeshButton(&(*c)[i], WindowPtr, this, glm::vec2(2, y - ScrollPosition), glm::vec2(350, ls), new Text((*c)[i].first, 12, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))));
							AddItem(new CharacterEditorMaterialButton(&(*c)[i], WindowPtr, this, glm::vec2(352, y - ScrollPosition), glm::vec2(100, ls), new Text((*c)[i].second, 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))));
							AddItem(new TileEditorRemoveMeshButton(i, this, Item::Alignment::TopRight, glm::vec2(-27, y - ScrollPosition), glm::vec2(20, 20), nullptr, nullptr, Assets::Textures["TrashMO"]));
						}
						y += ls;
					}
				}
			}
			else if (OpenCategory == Category::Colliders)
			{
				AddItem(new TileEditorCategoryButton(Category::Categories, this, Item::Alignment::TopRight, glm::vec2(-7, y), glm::vec2(20, 20), Assets::Textures["CloseButton"], Assets::Textures["CloseButtonMO"]));
				AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(lblw, ls), new Text("Colliders:", 4, 3, Assets::Fonts["Consolas16"])));
				AddItem(new TileEditorAddColliderButton(this, Item::Alignment::TopRight, glm::vec2(-34, y + 4.0f), glm::vec2(12, 12), nullptr, Assets::Textures["Plus"], Assets::Textures["PlusMO"]));
				y += ps;

				auto colliders = Tile->GetColliders();
				for (i = 0; i < colliders->size(); i++)
				{
					if (y < maxHeight)
					{
						if (y > minHeight)
						{
							AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y - ScrollPosition), glm::vec2(lblw, ls), new Text("Shape:", 4, 3, Assets::Fonts["Consolas16"])));
							AddItem(new Item(this, Alignment::TopLeft, glm::vec2(c2, y - ScrollPosition), glm::vec2(cw1M, ls), new Text("Box", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))));
							//AddItem(new TextBoxInt(&f->ShapeType, WindowPtr, this, Alignment::TopLeft, glm::vec2(c2, y - ScrollPosition), glm::vec2(cw1M, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
							AddItem(new TileEditorRemoveColliderButton(i, this, Item::Alignment::TopRight, glm::vec2(-27, y - ScrollPosition), glm::vec2(20, 20), nullptr, nullptr, Assets::Textures["TrashMO"]));
						}
						y += ls;
					}

					if (y < maxHeight)
					{
						if (y > minHeight)
						{
							AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y - ScrollPosition), glm::vec2(lblw, ls), new Text("Position: X", 4, 3, Assets::Fonts["Consolas16"])));
							AddItem(new TextBoxFloat(&(*colliders)[i]->Transform.Position.x, WindowPtr, this, Alignment::TopLeft, glm::vec2(c2, y - ScrollPosition), glm::vec2(cw1, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
						}
						y += ls;
					}
					if (y < maxHeight)
					{
						if (y > minHeight)
						{
							AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y - ScrollPosition), glm::vec2(lblw, ls), new Text("          Y", 4, 3, Assets::Fonts["Consolas16"])));
							AddItem(new TextBoxFloat(&(*colliders)[i]->Transform.Position.y, WindowPtr, this, Alignment::TopLeft, glm::vec2(c2, y - ScrollPosition), glm::vec2(cw1, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
						}
						y += ls;
					}
					if (y < maxHeight)
					{
						if (y > minHeight)
						{
							AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y - ScrollPosition), glm::vec2(lblw, ls), new Text("          Z", 4, 3, Assets::Fonts["Consolas16"])));
							AddItem(new TextBoxFloat(&(*colliders)[i]->Transform.Position.z, WindowPtr, this, Alignment::TopLeft, glm::vec2(c2, y - ScrollPosition), glm::vec2(cw1, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
						}
						y += ls;
					}

					if (y < maxHeight)
					{
						if (y > minHeight)
						{
							AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y - ScrollPosition), glm::vec2(lblw, ls), new Text("Rotation: X", 4, 3, Assets::Fonts["Consolas16"])));
							AddItem(new TextBoxFloat(&(*colliders)[i]->Transform.Rotation.x, WindowPtr, this, Alignment::TopLeft, glm::vec2(c2, y - ScrollPosition), glm::vec2(cw1, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
						}
						y += ls;
					}
					if (y < maxHeight)
					{
						if (y > minHeight)
						{
							AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y - ScrollPosition), glm::vec2(lblw, ls), new Text("          Y", 4, 3, Assets::Fonts["Consolas16"])));
							AddItem(new TextBoxFloat(&(*colliders)[i]->Transform.Rotation.y, WindowPtr, this, Alignment::TopLeft, glm::vec2(c2, y - ScrollPosition), glm::vec2(cw1, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
						}
						y += ls;
					}
					if (y < maxHeight)
					{
						if (y > minHeight)
						{
							AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y - ScrollPosition), glm::vec2(lblw, ls), new Text("          Z", 4, 3, Assets::Fonts["Consolas16"])));
							AddItem(new TextBoxFloat(&(*colliders)[i]->Transform.Rotation.z, WindowPtr, this, Alignment::TopLeft, glm::vec2(c2, y - ScrollPosition), glm::vec2(cw1, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
						}
						y += ls;
					}
					if (y < maxHeight)
					{
						if (y > minHeight)
						{
							AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(c1, y - ScrollPosition), glm::vec2(lblw, ls), new Text("          W", 4, 3, Assets::Fonts["Consolas16"])));
							AddItem(new TextBoxFloat(&(*colliders)[i]->Transform.Rotation.w, WindowPtr, this, Alignment::TopLeft, glm::vec2(c2, y - ScrollPosition), glm::vec2(cw1, ls), new Text("0", 4, 3, Assets::Fonts["Consolas16"], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
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
				AddItem(new TileEditorScrollButton(-40.0f, this, Alignment::TopRight, glm::vec2(-7, minHeight - ScrollPosition + 7), glm::vec2(16, 16), Assets::Textures["Up"], Assets::Textures["UpMO"]));
				AddItem(new TileEditorScrollButton(40.0f, this, Alignment::BottomRight, glm::vec2(-7, -7), glm::vec2(16, 16), Assets::Textures["Down"], Assets::Textures["DownMO"]));
			}

			Invalidate();
		}





		TileEditorSaveButton::TileEditorSaveButton(TileEditor* editor, Window* window, Alignment alignment, glm::vec2 position, glm::vec2 size, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(editor, alignment, position, size, nullptr, image, mouseOverImage, mouseDownImage)
		{
			WindowPtr = window;
			Editor = editor;
		}

		TileEditorSaveButton::~TileEditorSaveButton()	{}

		void TileEditorSaveButton::OnClick(int button)
		{
			WindowPtr->Scene->AppendConsole("Saving tile: " + *Editor->Tile->GetName());
			Editor->Tile->Save();
			WindowPtr->Scene->LoadTile(*Editor->Tile->GetName());
		}





		TileEditorCategoryButton::TileEditorCategoryButton(TileEditor::Category target, TileEditor* editor, Alignment alignment, glm::vec2 position, glm::vec2 size, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(editor, alignment, position, size, nullptr, image, mouseOverImage, mouseDownImage)
		{
			Editor = editor;
			Target = target;
		}

		TileEditorCategoryButton::~TileEditorCategoryButton()	{}

		void TileEditorCategoryButton::OnClick(int button)
		{
			Editor->OpenCategory = Target;
			Editor->ScrollPosition = 0.0f;
			Editor->Rebuild();
		}





		TileEditorScrollButton::TileEditorScrollButton(float delta, TileEditor* editor, Alignment alignment, glm::vec2 position, glm::vec2 size, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(editor, alignment, position, size, nullptr, image, mouseOverImage, mouseDownImage)
		{
			Editor = editor;
			Delta = delta;
		}

		TileEditorScrollButton::~TileEditorScrollButton()	{}

		void TileEditorScrollButton::OnClick(int button)
		{
			Editor->ScrollPosition += Delta;
			if (Editor->ScrollPosition < 0.0f)
			{
				Editor->ScrollPosition = 0.0f;
			}
			Editor->Rebuild();
		}





		TileEditorAddTransitionButton::TileEditorAddTransitionButton(TileEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			Editor = parent;
		}

		TileEditorAddTransitionButton::~TileEditorAddTransitionButton() {}

		void TileEditorAddTransitionButton::OnClick(int button)
		{
			auto n = new Node;
			n->Tag = "Tag";
			Editor->Tile->GetTransitions()->push_back(n);
			Editor->Rebuild();
		}




		TileEditorRemoveTransitionButton::TileEditorRemoveTransitionButton(int i, TileEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			Index = i;
			Editor = parent;
		}

		TileEditorRemoveTransitionButton::~TileEditorRemoveTransitionButton() {}

		void TileEditorRemoveTransitionButton::OnClick(int button)
		{
			auto c = Editor->Tile->GetTransitions();
			(*c)[Index] = c->back();
			c->resize(c->size() - 1);
			Editor->Rebuild();
		}





		TileEditorAddFeatureButton::TileEditorAddFeatureButton(TileEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			Editor = parent;
		}

		TileEditorAddFeatureButton::~TileEditorAddFeatureButton() {}

		void TileEditorAddFeatureButton::OnClick(int button)
		{
			auto n = new Node;
			n->Tag = "Tag";
			Editor->Tile->GetFeatures()->push_back(n);
			Editor->Rebuild();
		}




		TileEditorRemoveFeatureButton::TileEditorRemoveFeatureButton(int i, TileEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			Index = i;
			Editor = parent;
		}

		TileEditorRemoveFeatureButton::~TileEditorRemoveFeatureButton() {}

		void TileEditorRemoveFeatureButton::OnClick(int button)
		{
			auto c = Editor->Tile->GetFeatures();
			(*c)[Index] = c->back();
			c->resize(c->size() - 1);
			Editor->Rebuild();
		}





		TileEditorAddLightButton::TileEditorAddLightButton(TileEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			Editor = parent;
		}

		TileEditorAddLightButton::~TileEditorAddLightButton() {}

		void TileEditorAddLightButton::OnClick(int button)
		{
			auto n = new LightNode;
			n->Color = glm::vec3(1.0f);
			n->Radius = 1.0f;
			n->CastsShadow = false;
			n->Transform.Scale = glm::vec3(0.05f, 0.05f, 0.05f);
			Editor->Tile->GetLights()->push_back(n);
			Editor->Rebuild();
		}




		TileEditorRemoveLightButton::TileEditorRemoveLightButton(int i, TileEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			Index = i;
			Editor = parent;
		}

		TileEditorRemoveLightButton::~TileEditorRemoveLightButton() {}

		void TileEditorRemoveLightButton::OnClick(int button)
		{
			auto c = Editor->Tile->GetLights();
			(*c)[Index] = c->back();
			c->resize(c->size() - 1);
			Editor->Rebuild();
		}

		



		TileEditorAddMeshButton::TileEditorAddMeshButton(TileEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			Editor = parent;
		}

		TileEditorAddMeshButton::~TileEditorAddMeshButton() {}

		void TileEditorAddMeshButton::OnClick(int button)
		{
			Editor->Tile->GetMeshes()->push_back(std::pair<std::string, std::string>(std::string("Cube"), std::string("Gold")));
			Editor->Rebuild();
		}




		TileEditorRemoveMeshButton::TileEditorRemoveMeshButton(int i, TileEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			Index = i;
			Editor = parent;
		}

		TileEditorRemoveMeshButton::~TileEditorRemoveMeshButton() {}

		void TileEditorRemoveMeshButton::OnClick(int button)
		{
			auto c = Editor->Tile->GetMeshes();
			(*c)[Index] = c->back();
			c->resize(c->size() - 1);
			Editor->Rebuild();
		}





		TileEditorAddColliderButton::TileEditorAddColliderButton(TileEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			Editor = parent;
		}

		TileEditorAddColliderButton::~TileEditorAddColliderButton() {}

		void TileEditorAddColliderButton::OnClick(int button)
		{
			auto n = new Collider;
			n->ShapeType = Collider::Type::Box;
			Editor->Tile->GetColliders()->push_back(n);
			Editor->Rebuild();
		}




		TileEditorRemoveColliderButton::TileEditorRemoveColliderButton(int i, TileEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			Index = i;
			Editor = parent;
		}

		TileEditorRemoveColliderButton::~TileEditorRemoveColliderButton() {}

		void TileEditorRemoveColliderButton::OnClick(int button)
		{
			auto c = Editor->Tile->GetColliders();
			(*c)[Index] = c->back();
			c->resize(c->size() - 1);
			Editor->Rebuild();
		}

	}
}