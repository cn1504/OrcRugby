#include "MaterialEditor.h"
#include "Assets.h"
#include "Window.h"

namespace Core
{
	namespace Gui
	{

		MaterialBrowser::MaterialBrowser(std::pair<std::string, std::string>* mat, Window* window, Item* opener)
			: Item(window->Scene->Gui, Item::Alignment::Center, glm::vec2(), glm::vec2(220, 150), nullptr, Assets::Textures["DialogBackground"])
		{
			WindowPtr = window;
			Mat = mat;
			Opener = opener;

			BuildList();
		}


		MaterialBrowser::~MaterialBrowser()
		{
		}


		void MaterialBrowser::Resolve(std::string name)
		{
			if (Assets::Materials.count(name) > 0)
			{
				Mat->second = name;
				if (Opener != nullptr)
					Opener->SetLabel(name);

				Parent->RemoveItem(this);
				delete this;
			}
		}


		void MaterialBrowser::BuildList()
		{
			auto Title = new Text("Materials", 4, 4, Assets::Fonts["Consolas24"], glm::vec4(0.9, 0.9, 0.9, 1.0));
			AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(5, 0), glm::vec2(150, 28), Title));


			std::map<std::string, Material*> sorted(Assets::Materials.begin(), Assets::Materials.end());
			for (auto m : sorted)
			{
				auto text = new Text(m.first, 7, 0, Assets::Fonts["Consolas16"], glm::vec4(0.9, 0.9, 0.9, 1.0));
				AddItem(new MaterialBrowserButton(this, Item::Alignment::TopLeft, glm::vec2(3, 20 + 16 * Children.size()), glm::vec2(214, 16), text, nullptr, Assets::Textures["DialogBackgroundMO"]));
			}


			Resize(glm::vec2(Size.x, 28 + 16 * Children.size()));

			AddItem(new Border(Border::Orientation::Horizontal, this, Item::Alignment::TopLeft, glm::vec2(1, 1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
			AddItem(new Border(Border::Orientation::Vertical, this, Item::Alignment::TopLeft, glm::vec2(1, 1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
			AddItem(new Border(Border::Orientation::Horizontal, this, Item::Alignment::BottomRight, glm::vec2(-1, -1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
			AddItem(new Border(Border::Orientation::Vertical, this, Item::Alignment::BottomRight, glm::vec2(-1, -1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));

			AddItem(new Border(Border::Orientation::Horizontal, this, Item::Alignment::TopLeft, glm::vec2(1, 28), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
			AddItem(new DialogCloseButton(this, this, Item::Alignment::TopRight, glm::vec2(-6, 6), glm::vec2(17, 17), nullptr, Assets::Textures["CloseButton"], Assets::Textures["CloseButtonMO"]));

			Invalidate();
		}





		MaterialBrowserButton::MaterialBrowserButton(MaterialBrowser* fbrowser, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(fbrowser, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			Parent = fbrowser;
		}


		MaterialBrowserButton::~MaterialBrowserButton()
		{
		}


		void MaterialBrowserButton::OnClick(int button)
		{
			Parent->Resolve(Label->GetText());
		}





		MaterialButton::MaterialButton(Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			WindowPtr = window;
		}

		MaterialButton::~MaterialButton()	{}

		void MaterialButton::OnClick(int button)
		{
			WindowPtr->Scene->Gui->AddItem(new MaterialEditorPicker(WindowPtr));
		}




		MaterialEditorPicker::MaterialEditorPicker(Window* window)
			: Item(window->Scene->Gui, Alignment::Center, glm::vec2(), glm::vec2(220,100), nullptr, Assets::Textures["DialogBackground"])
		{
			WindowPtr = window;
			BuildList();
		}

		MaterialEditorPicker::~MaterialEditorPicker() {}

		void MaterialEditorPicker::Resolve(std::string name)
		{
			if (Assets::Materials.count(name) > 0)
			{
				WindowPtr->Scene->Gui->AddItem(new MaterialEditor(name, WindowPtr, WindowPtr->Scene->Gui));

				Parent->RemoveItem(this);
				delete this;
			}
		}

		void MaterialEditorPicker::Invalidate(bool includeChildren)
		{
			Size.y = 50 + (Assets::Materials.size()) * 16.0f;
			Item::Invalidate(includeChildren);
		}

		void MaterialEditorPicker::BuildList()
		{
			for (auto c : Children)
				c->Destroy();
			Children.clear();

			// Build title bar and borders
			AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(5, 0), glm::vec2(150, 28), new Text("Materials", 4, 4, Assets::Fonts["Consolas24"], glm::vec4(0.9, 0.9, 0.9, 1.0))));

			AddItem(new Border(Border::Orientation::Horizontal, this, Item::Alignment::TopLeft, glm::vec2(1, 1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
			AddItem(new Border(Border::Orientation::Vertical, this, Item::Alignment::TopLeft, glm::vec2(1, 1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
			AddItem(new Border(Border::Orientation::Horizontal, this, Item::Alignment::BottomRight, glm::vec2(-1, -1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
			AddItem(new Border(Border::Orientation::Vertical, this, Item::Alignment::BottomRight, glm::vec2(-1, -1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));

			AddItem(new Border(Border::Orientation::Horizontal, this, Item::Alignment::TopLeft, glm::vec2(1, 28), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
			AddItem(new DialogCloseButton(this, this, Item::Alignment::TopRight, glm::vec2(-6, 6), glm::vec2(17, 17), nullptr, Assets::Textures["CloseButton"], Assets::Textures["CloseButtonMO"]));

			// Build material list
			float y = 38;
			int i = 0;
			std::map<std::string, Material*> sorted(Assets::Materials.begin(), Assets::Materials.end());
			for (auto m : sorted)
			{
				AddItem(new MaterialEditorPickerButton(this, Alignment::TopLeft, glm::vec2(3,y + i * 16), glm::vec2(214, 16),
					new Text(m.first, 7, 0, Assets::Fonts["Consolas16"]), nullptr, Assets::Textures["DialogBackgroundMO"]));
				i++;
			}

			Invalidate();
		}

		

		
	
		MaterialEditorPickerButton::MaterialEditorPickerButton(MaterialEditorPicker* picker, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(picker, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			Picker = picker;
		}

		MaterialEditorPickerButton::~MaterialEditorPickerButton() {}

		void MaterialEditorPickerButton::OnClick(int button)
		{
			Picker->Resolve(Label->GetText());
		}





		MaterialEditor::MaterialEditor(std::string material, Window* window, Item* parent)
			: Item(parent, Item::Alignment::TopRight, glm::vec2(280, 100), glm::vec2(280, parent->GetDimensions().w - 160), nullptr, Assets::Textures["DialogBackground"])
		{
			WindowPtr = window;
			ShouldClose = false;
			Name = material;
			Material = Assets::Materials[material];

			Rebuild();

			Move(glm::vec2(-Size.x, 0), 0.3f, MoveStyle::Sinosoidal);
		}

		MaterialEditor::~MaterialEditor() {}

		void MaterialEditor::Invalidate(bool includeChildren)
		{
			Size.y = Parent->GetDimensions().w - 160;
			Item::Invalidate(includeChildren);
		}

		void MaterialEditor::Update(glm::vec2 mousePosition)
		{
			if (ShouldClose && !IsMoving())
			{
				Item::Destroy();
			}

			Item::Update(mousePosition);
		}
		
		void MaterialEditor::Destroy()
		{
			Move(glm::vec2(Size.x, 0), 0.3f, MoveStyle::Sinosoidal);
			ShouldClose = true;
		}

		void MaterialEditor::Rebuild()
		{
			for (auto c : Children)
			{
				delete c;
			}
			Children.clear();

			// Build Title Bar and Borders
			auto Title = new Text(Material->Name, 4, 2, Assets::Fonts["Consolas24"], glm::vec4(0.9, 0.9, 0.9, 1.0));
			auto p = new TextBoxString(&Material->Name, WindowPtr, this, Item::Alignment::TopLeft, glm::vec2(3, 3), glm::vec2(160, 24), Title, nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]);
			AddItem(p);

			AddItem(new DialogCloseButton(this, this, Alignment::TopRight, glm::vec2(-7, 5), glm::vec2(20, 20), nullptr, Assets::Textures["CloseButton"], Assets::Textures["CloseButtonMO"]));
			AddItem(new MaterialEditorSaveButton(this, Alignment::TopRight, glm::vec2(-34, 5), glm::vec2(20, 20), nullptr, Assets::Textures["Save"], Assets::Textures["SaveMO"]));
			AddItem(new MaterialEditorCopyButton(this, Alignment::TopRight, glm::vec2(-63, 5), glm::vec2(20, 20), nullptr, Assets::Textures["Copy"], Assets::Textures["CopyMO"]));
			AddItem(new MaterialEditorDeleteButton(this, Alignment::TopRight, glm::vec2(-92, 5), glm::vec2(20, 20), nullptr, Assets::Textures["Trash"], Assets::Textures["TrashMO"]));

			AddItem(new Border(Border::Orientation::Horizontal, this, Alignment::TopLeft, glm::vec2(1, 1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
			AddItem(new Border(Border::Orientation::Vertical, this, Alignment::TopLeft, glm::vec2(1, 1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
			AddItem(new Border(Border::Orientation::Horizontal, this, Alignment::BottomRight, glm::vec2(-1, -1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
			AddItem(new Border(Border::Orientation::Vertical, this, Alignment::BottomRight, glm::vec2(-1, -1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));

			AddItem(new Border(Border::Orientation::Horizontal, this, Alignment::TopLeft, glm::vec2(1, 28), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));

			// Build Option Controls
			float y = 40.0f;
			float g1 = 168;
			float g2 = 203;
			float g3 = 238;

			AddItem(new Item(this, Alignment::TopLeft, glm::vec2(3, y), glm::vec2(110, 20), new Text("Noise Algorithm:", 4, 3, Assets::Fonts["Consolas16"])));
			auto na = new MaterialEditorNoiseAlgorithmBox(WindowPtr, this, Alignment::TopLeft, glm::vec2(g1, y), glm::vec2(30, 20), new Text("v", 4, 3, Assets::Fonts["Consolas16"]));
			AddItem(na);
			y += 20.0f;

			AddItem(new Item(this, Alignment::TopLeft, glm::vec2(3, y), glm::vec2(110, 20), new Text("      Scale:", 4, 3, Assets::Fonts["Consolas16"])));
			AddItem(new TextBoxFloatZeroPlus(&Material->NoiseScale, WindowPtr, this, Alignment::TopLeft, glm::vec2(g1, y), glm::vec2(100, 20), new Text("v", 4, 3, Assets::Fonts["Consolas16"]), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			y += 20.0f;

			AddItem(new Item(this, Alignment::TopLeft, glm::vec2(3, y), glm::vec2(110, 20), new Text("      Offset:", 4, 3, Assets::Fonts["Consolas16"])));
			AddItem(new TextBoxFloat(&Material->NoiseOffset.x, WindowPtr, this, Alignment::TopLeft, glm::vec2(g1, y), glm::vec2(100, 20), new Text("v", 4, 3, Assets::Fonts["Consolas16"]), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			y += 20.0f;
			AddItem(new TextBoxFloat(&Material->NoiseOffset.y, WindowPtr, this, Alignment::TopLeft, glm::vec2(g1, y), glm::vec2(100, 20), new Text("v", 4, 3, Assets::Fonts["Consolas16"]), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			y += 20.0f;

			AddItem(new Item(this, Alignment::TopLeft, glm::vec2(3, y), glm::vec2(110, 20), new Text("Material 1 Cieling:", 4, 3, Assets::Fonts["Consolas16"])));
			AddItem(new TextBoxFloatZeroToOne(&Material->Mat1Cieling, WindowPtr, this, Alignment::TopLeft, glm::vec2(g1, y), glm::vec2(100, 20), new Text("v", 4, 3, Assets::Fonts["Consolas16"]), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			y += 20.0f;
			AddItem(new Item(this, Alignment::TopLeft, glm::vec2(3, y), glm::vec2(110, 20), new Text("Material 2 Floor:", 4, 3, Assets::Fonts["Consolas16"])));
			AddItem(new TextBoxFloatZeroToOne(&Material->Mat2Floor, WindowPtr, this, Alignment::TopLeft, glm::vec2(g1, y), glm::vec2(100, 20), new Text("v", 4, 3, Assets::Fonts["Consolas16"]), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			y += 30.0f;

			AddItem(new Item(this, Alignment::TopLeft, glm::vec2(3, y), glm::vec2(110, 20), new Text("Is Translucent:", 4, 3, Assets::Fonts["Consolas16"])));
			AddItem(new CheckBox(&Material->IsTranslucent, this, Alignment::TopLeft, glm::vec2(g1, y), glm::vec2(17.0f, 17.0f), Assets::Textures["CloseButton"], Assets::Textures["CloseButtonMO"], nullptr, Assets::Textures["CheckSmall"], Assets::Textures["CheckSmallMO"]));
			y += 20.0f;
			AddItem(new Item(this, Alignment::TopLeft, glm::vec2(3, y), glm::vec2(110, 20), new Text("Diffuse Alpha 1:", 4, 3, Assets::Fonts["Consolas16"])));
			AddItem(new TextBoxFloatZeroToOne(&Material->DiffuseAlpha1, WindowPtr, this, Alignment::TopLeft, glm::vec2(g1, y), glm::vec2(100, 20), new Text("v", 4, 3, Assets::Fonts["Consolas16"]), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			y += 20.0f;
			AddItem(new Item(this, Alignment::TopLeft, glm::vec2(3, y), glm::vec2(110, 20), new Text("              2:", 4, 3, Assets::Fonts["Consolas16"])));
			AddItem(new TextBoxFloatZeroToOne(&Material->DiffuseAlpha2, WindowPtr, this, Alignment::TopLeft, glm::vec2(g1, y), glm::vec2(100, 20), new Text("v", 4, 3, Assets::Fonts["Consolas16"]), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			y += 30.0f; 

			AddItem(new Item(this, Alignment::TopLeft, glm::vec2(3, y), glm::vec2(110, 20), new Text("Base Material:", 4, 3, Assets::Fonts["Consolas16"])));
			y += 20.0f;

			AddItem(new Item(this, Alignment::TopLeft, glm::vec2(3, y), glm::vec2(110, 20), new Text("Diffuse Color (RGB):", 4, 3, Assets::Fonts["Consolas16"])));
			AddItem(new TextBoxRGB(&Material->DiffuseColor.x, WindowPtr, this, Alignment::TopLeft, glm::vec2(g1, y), glm::vec2(30, 20), new Text("v", 4, 3, Assets::Fonts["Consolas16"]), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			AddItem(new TextBoxRGB(&Material->DiffuseColor.y, WindowPtr, this, Alignment::TopLeft, glm::vec2(g2, y), glm::vec2(30, 20), new Text("v", 4, 3, Assets::Fonts["Consolas16"]), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			AddItem(new TextBoxRGB(&Material->DiffuseColor.z, WindowPtr, this, Alignment::TopLeft, glm::vec2(g3, y), glm::vec2(30, 20), new Text("v", 4, 3, Assets::Fonts["Consolas16"]), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			y += 20.0f;

			AddItem(new Item(this, Alignment::TopLeft, glm::vec2(3, y), glm::vec2(110, 20), new Text("Diffuse Intensity:", 4, 3, Assets::Fonts["Consolas16"])));
			AddItem(new TextBoxFloatZeroPlus(&Material->DiffuseIntensity, WindowPtr, this, Alignment::TopLeft, glm::vec2(g1, y), glm::vec2(100, 20), new Text("v", 4, 3, Assets::Fonts["Consolas16"]), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			y += 30.0f;


			AddItem(new Item(this, Alignment::TopLeft, glm::vec2(3, y), glm::vec2(110, 20), new Text("Specular Color (RGB):", 4, 3, Assets::Fonts["Consolas16"])));
			AddItem(new TextBoxRGB(&Material->SpecularColor.x, WindowPtr, this, Alignment::TopLeft, glm::vec2(g1, y), glm::vec2(30, 20), new Text("v", 4, 3, Assets::Fonts["Consolas16"]), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			AddItem(new TextBoxRGB(&Material->SpecularColor.y, WindowPtr, this, Alignment::TopLeft, glm::vec2(g2, y), glm::vec2(30, 20), new Text("v", 4, 3, Assets::Fonts["Consolas16"]), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			AddItem(new TextBoxRGB(&Material->SpecularColor.z, WindowPtr, this, Alignment::TopLeft, glm::vec2(g3, y), glm::vec2(30, 20), new Text("v", 4, 3, Assets::Fonts["Consolas16"]), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			y += 20.0f;

			AddItem(new Item(this, Alignment::TopLeft, glm::vec2(3, y), glm::vec2(110, 20), new Text("Specular Intensity:", 4, 3, Assets::Fonts["Consolas16"])));
			AddItem(new TextBoxFloatZeroPlus(&Material->SpecularIntensity, WindowPtr, this, Alignment::TopLeft, glm::vec2(g1, y), glm::vec2(100, 20), new Text("v", 4, 3, Assets::Fonts["Consolas16"]), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			y += 20.0f;

			AddItem(new Item(this, Alignment::TopLeft, glm::vec2(3, y), glm::vec2(110, 20), new Text("Specular Hardness:", 4, 3, Assets::Fonts["Consolas16"])));
			AddItem(new TextBoxFloatZeroPlus(&Material->SpecularHardness, WindowPtr, this, Alignment::TopLeft, glm::vec2(g1, y), glm::vec2(100, 20), new Text("v", 4, 3, Assets::Fonts["Consolas16"]), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			y += 30.0f;

			AddItem(new Item(this, Alignment::TopLeft, glm::vec2(3, y), glm::vec2(110, 20), new Text("Emanative Color (RGB):", 4, 3, Assets::Fonts["Consolas16"])));
			AddItem(new TextBoxRGB(&Material->EmanativeColor.x, WindowPtr, this, Alignment::TopLeft, glm::vec2(g1, y), glm::vec2(30, 20), new Text("v", 4, 3, Assets::Fonts["Consolas16"]), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			AddItem(new TextBoxRGB(&Material->EmanativeColor.y, WindowPtr, this, Alignment::TopLeft, glm::vec2(g2, y), glm::vec2(30, 20), new Text("v", 4, 3, Assets::Fonts["Consolas16"]), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			AddItem(new TextBoxRGB(&Material->EmanativeColor.z, WindowPtr, this, Alignment::TopLeft, glm::vec2(g3, y), glm::vec2(30, 20), new Text("v", 4, 3, Assets::Fonts["Consolas16"]), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			y += 20.0f;

			AddItem(new Item(this, Alignment::TopLeft, glm::vec2(3, y), glm::vec2(110, 20), new Text("Emanative Intensity:", 4, 3, Assets::Fonts["Consolas16"])));
			AddItem(new TextBoxRGB(&Material->EmanativeIntensity, WindowPtr, this, Alignment::TopLeft, glm::vec2(g1, y), glm::vec2(100, 20), new Text("v", 4, 3, Assets::Fonts["Consolas16"]), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			y += 30.0f;

			AddItem(new Item(this, Alignment::TopLeft, glm::vec2(3, y), glm::vec2(110, 20), new Text("Material 2:", 4, 3, Assets::Fonts["Consolas16"])));
			y += 20.0f;

			AddItem(new Item(this, Alignment::TopLeft, glm::vec2(3, y), glm::vec2(110, 20), new Text("Diffuse Color (RGB):", 4, 3, Assets::Fonts["Consolas16"])));
			AddItem(new TextBoxRGB(&Material->DiffuseColor2.x, WindowPtr, this, Alignment::TopLeft, glm::vec2(g1, y), glm::vec2(30, 20), new Text("v", 4, 3, Assets::Fonts["Consolas16"]), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			AddItem(new TextBoxRGB(&Material->DiffuseColor2.y, WindowPtr, this, Alignment::TopLeft, glm::vec2(g2, y), glm::vec2(30, 20), new Text("v", 4, 3, Assets::Fonts["Consolas16"]), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			AddItem(new TextBoxRGB(&Material->DiffuseColor2.z, WindowPtr, this, Alignment::TopLeft, glm::vec2(g3, y), glm::vec2(30, 20), new Text("v", 4, 3, Assets::Fonts["Consolas16"]), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			y += 20.0f;

			AddItem(new Item(this, Alignment::TopLeft, glm::vec2(3, y), glm::vec2(110, 20), new Text("Diffuse Intensity:", 4, 3, Assets::Fonts["Consolas16"])));
			AddItem(new TextBoxFloatZeroPlus(&Material->DiffuseIntensity2, WindowPtr, this, Alignment::TopLeft, glm::vec2(g1, y), glm::vec2(100, 20), new Text("v", 4, 3, Assets::Fonts["Consolas16"]), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			y += 30.0f;


			AddItem(new Item(this, Alignment::TopLeft, glm::vec2(3, y), glm::vec2(110, 20), new Text("Specular Color (RGB):", 4, 3, Assets::Fonts["Consolas16"])));
			AddItem(new TextBoxRGB(&Material->SpecularColor2.x, WindowPtr, this, Alignment::TopLeft, glm::vec2(g1, y), glm::vec2(30, 20), new Text("v", 4, 3, Assets::Fonts["Consolas16"]), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			AddItem(new TextBoxRGB(&Material->SpecularColor2.y, WindowPtr, this, Alignment::TopLeft, glm::vec2(g2, y), glm::vec2(30, 20), new Text("v", 4, 3, Assets::Fonts["Consolas16"]), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			AddItem(new TextBoxRGB(&Material->SpecularColor2.z, WindowPtr, this, Alignment::TopLeft, glm::vec2(g3, y), glm::vec2(30, 20), new Text("v", 4, 3, Assets::Fonts["Consolas16"]), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			y += 20.0f;

			AddItem(new Item(this, Alignment::TopLeft, glm::vec2(3, y), glm::vec2(110, 20), new Text("Specular Intensity:", 4, 3, Assets::Fonts["Consolas16"])));
			AddItem(new TextBoxFloatZeroPlus(&Material->SpecularIntensity2, WindowPtr, this, Alignment::TopLeft, glm::vec2(g1, y), glm::vec2(100, 20), new Text("v", 4, 3, Assets::Fonts["Consolas16"]), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			y += 20.0f;

			AddItem(new Item(this, Alignment::TopLeft, glm::vec2(3, y), glm::vec2(110, 20), new Text("Specular Hardness:", 4, 3, Assets::Fonts["Consolas16"])));
			AddItem(new TextBoxFloatZeroPlus(&Material->SpecularHardness2, WindowPtr, this, Alignment::TopLeft, glm::vec2(g1, y), glm::vec2(100, 20), new Text("v", 4, 3, Assets::Fonts["Consolas16"]), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			y += 30.0f;

			AddItem(new Item(this, Alignment::TopLeft, glm::vec2(3, y), glm::vec2(110, 20), new Text("Emanative Color (RGB):", 4, 3, Assets::Fonts["Consolas16"])));
			AddItem(new TextBoxRGB(&Material->EmanativeColor2.x, WindowPtr, this, Alignment::TopLeft, glm::vec2(g1, y), glm::vec2(30, 20), new Text("v", 4, 3, Assets::Fonts["Consolas16"]), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			AddItem(new TextBoxRGB(&Material->EmanativeColor2.y, WindowPtr, this, Alignment::TopLeft, glm::vec2(g2, y), glm::vec2(30, 20), new Text("v", 4, 3, Assets::Fonts["Consolas16"]), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			AddItem(new TextBoxRGB(&Material->EmanativeColor2.z, WindowPtr, this, Alignment::TopLeft, glm::vec2(g3, y), glm::vec2(30, 20), new Text("v", 4, 3, Assets::Fonts["Consolas16"]), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			y += 20.0f;

			AddItem(new Item(this, Alignment::TopLeft, glm::vec2(3, y), glm::vec2(110, 20), new Text("Emanative Intensity:", 4, 3, Assets::Fonts["Consolas16"])));
			AddItem(new TextBoxFloatZeroPlus(&Material->EmanativeIntensity2, WindowPtr, this, Alignment::TopLeft, glm::vec2(g1, y), glm::vec2(100, 20), new Text("v", 4, 3, Assets::Fonts["Consolas16"]), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			y += 35.0f;

			AddItem(new Border(Border::Orientation::Horizontal, this, Alignment::TopLeft, glm::vec2(1, y), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
			y += 15.0f;

			AddItem(new Item(this, Alignment::TopLeft, glm::vec2(3, y), glm::vec2(110, 20), new Text("Index Of Refraction:", 4, 3, Assets::Fonts["Consolas16"])));
			AddItem(new TextBoxFloatZeroPlus(&Material->IndexOfRefraction, WindowPtr, this, Alignment::TopLeft, glm::vec2(g1, y), glm::vec2(100, 20), new Text("v", 4, 3, Assets::Fonts["Consolas16"]), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
			y += 30.0f;

			AddItem(new Item(this, Alignment::TopLeft, glm::vec2(3, y), glm::vec2(110, 20), new Text("Density (e3):", 4, 3, Assets::Fonts["Consolas16"])));
			auto den = new MaterialEditorDensityBox(WindowPtr, this, Alignment::TopLeft, glm::vec2(g1, y), glm::vec2(100, 20), new Text("v", 4, 3, Assets::Fonts["Consolas16"]));
			AddItem(den);
			y += 30.0f;

			AddItem(new Item(this, Alignment::TopLeft, glm::vec2(3, y), glm::vec2(110, 20), new Text("Young's Modulus (e9):", 4, 3, Assets::Fonts["Consolas16"])));
			auto ym = new MaterialEditorYoungsModulusBox(WindowPtr, this, Alignment::TopLeft, glm::vec2(g1, y), glm::vec2(100, 20), new Text("v", 4, 3, Assets::Fonts["Consolas16"]));
			AddItem(ym);
			y += 20.0f;

			AddItem(new Item(this, Alignment::TopLeft, glm::vec2(3, y), glm::vec2(110, 20), new Text("Bulk Modulus (e9):", 4, 3, Assets::Fonts["Consolas16"])));
			auto bm = new MaterialEditorBulkModulusBox(WindowPtr, this, Alignment::TopLeft, glm::vec2(g1, y), glm::vec2(100, 20), new Text("v", 4, 3, Assets::Fonts["Consolas16"]));
			AddItem(bm);
			y += 20.0f;

			AddItem(new Item(this, Alignment::TopLeft, glm::vec2(3, y), glm::vec2(110, 20), new Text("Shear Modulus (e9):", 4, 3, Assets::Fonts["Consolas16"])));
			auto sm = new MaterialEditorShearModulusBox(WindowPtr, this, Alignment::TopLeft, glm::vec2(g1, y), glm::vec2(100, 20), new Text("v", 4, 3, Assets::Fonts["Consolas16"]));
			AddItem(sm);
			y += 30.0f;

			AddItem(new Item(this, Alignment::TopLeft, glm::vec2(3, y), glm::vec2(110, 20), new Text("Yield Strength (e6):", 4, 3, Assets::Fonts["Consolas16"])));
			auto ys = new MaterialEditorYieldStrengthBox(WindowPtr, this, Alignment::TopLeft, glm::vec2(g1, y), glm::vec2(100, 20), new Text("v", 4, 3, Assets::Fonts["Consolas16"]));
			AddItem(ys);
			y += 20.0f;

			AddItem(new Item(this, Alignment::TopLeft, glm::vec2(3, y), glm::vec2(110, 20), new Text("Ultimate Strength (e6):", 4, 3, Assets::Fonts["Consolas16"])));
			auto us = new MaterialEditorUltimateStrengthBox(WindowPtr, this, Alignment::TopLeft, glm::vec2(g1, y), glm::vec2(100, 20), new Text("v", 4, 3, Assets::Fonts["Consolas16"]));
			AddItem(us);


			Invalidate();
		}





		MaterialEditorSaveButton::MaterialEditorSaveButton(MaterialEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			Editor = parent;
		}

		MaterialEditorSaveButton::~MaterialEditorSaveButton() {}

		void MaterialEditorSaveButton::OnClick(int button)
		{
			if (Editor->Name != Editor->Material->Name)
			{
				Assets::Materials[Editor->Material->Name] = Editor->Material;
				Assets::Materials.erase(Editor->Name);
				Editor->Name = Editor->Material->Name;
			}
			Assets::SaveMaterials();
		}





		MaterialEditorCopyButton::MaterialEditorCopyButton(MaterialEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			Editor = parent;
		}

		MaterialEditorCopyButton::~MaterialEditorCopyButton() {}

		void MaterialEditorCopyButton::OnClick(int button)
		{
			if (Editor->Name != Editor->Material->Name)
			{
				Assets::Materials[Editor->Material->Name] = Editor->Material;
				Assets::Materials.erase(Editor->Name);
				Editor->Name = Editor->Material->Name;
			}

			std::string newMat = Editor->Material->Name + "_Copy";
			Assets::Materials[newMat] = new Material(*Editor->Material);
			Assets::Materials[newMat]->Name = newMat;
			Editor->Rebuild();
			Assets::SaveMaterials();
		}





		MaterialEditorDeleteButton::MaterialEditorDeleteButton(MaterialEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			Editor = parent;
		}

		MaterialEditorDeleteButton::~MaterialEditorDeleteButton() {}

		void MaterialEditorDeleteButton::OnClick(int button)
		{
			if (Editor->Name != Editor->Material->Name)
			{
				Assets::Materials[Editor->Material->Name] = Editor->Material;
				Assets::Materials.erase(Editor->Name);
				Editor->Name = Editor->Material->Name;
			}
			Assets::Materials.erase(Editor->Material->Name);
			delete Editor->Material;
			Assets::SaveMaterials();
			Editor->Destroy();
		}





		MaterialEditorNoiseAlgorithmBox::MaterialEditorNoiseAlgorithmBox(Window* window, MaterialEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label)
			: TextBox(window, parent, alignment, position, size, label, nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"])
		{
			Editor = parent;
			RefreshData();
		}

		MaterialEditorNoiseAlgorithmBox::~MaterialEditorNoiseAlgorithmBox() {}

		void MaterialEditorNoiseAlgorithmBox::SaveValue()
		{
			int s = (int)Editor->Material->Noise;
			try	{
				s = std::stoi(Label->GetText());
				Editor->Material->Noise =
					(s <= 0) ? Material::NoiseType::None :
					(s == 1) ? Material::NoiseType::Value2D :
					(s == 2) ? Material::NoiseType::Perlin2D : 
					(s == 3) ? Material::NoiseType::SimplexPerlin2D : 
					(s == 4) ? Material::NoiseType::Cellular2D :
					(s == 5) ? Material::NoiseType::TurbulentSine :
					Material::NoiseType::None;
			}
			catch (std::invalid_argument &e) { (void*)&e; }
			catch (std::out_of_range &e) { (void*)&e; }
			RefreshData();
			TextBox::SaveValue();
		}

		void MaterialEditorNoiseAlgorithmBox::RefreshData()
		{
			auto v = (int)Editor->Material->Noise;
			Label->UpdateText(std::to_string(v));
		}





		MaterialEditorDensityBox::MaterialEditorDensityBox(Window* window, MaterialEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label)
			: TextBox(window, parent, alignment, position, size, label, nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"])
		{
			Editor = parent;
			RefreshData();
		}

		MaterialEditorDensityBox::~MaterialEditorDensityBox() {}

		void MaterialEditorDensityBox::SaveValue()
		{
			float s = Editor->Material->Density / 1.0e3f;
			try	{
				s = std::stof(Label->GetText());
				Editor->Material->Density = (s < 0) ? 0 : s * 1.0e3f;
			}
			catch (std::invalid_argument &e) { (void*)&e; }
			catch (std::out_of_range &e) { (void*)&e; }
			RefreshData();
			TextBox::SaveValue();
		}

		void MaterialEditorDensityBox::RefreshData()
		{
			auto v = Editor->Material->Density / 1.0e3f;
			Label->UpdateText(std::to_string(v));
		}





		MaterialEditorYoungsModulusBox::MaterialEditorYoungsModulusBox(Window* window, MaterialEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label)
			: TextBox(window, parent, alignment, position, size, label, nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"])
		{
			Editor = parent;
			RefreshData();
		}

		MaterialEditorYoungsModulusBox::~MaterialEditorYoungsModulusBox() {}

		void MaterialEditorYoungsModulusBox::SaveValue()
		{
			float s = Editor->Material->YoungsModulus / 1.0e9f;
			try	{
				s = std::stof(Label->GetText());
				Editor->Material->YoungsModulus = (s < 0) ? 0 : s * 1.0e9f;
			}
			catch (std::invalid_argument &e) { (void*)&e; }
			catch (std::out_of_range &e) { (void*)&e; }
			RefreshData();
			TextBox::SaveValue();
		}

		void MaterialEditorYoungsModulusBox::RefreshData()
		{
			auto v = Editor->Material->YoungsModulus / 1.0e9f;
			Label->UpdateText(std::to_string(v));
		}





		MaterialEditorBulkModulusBox::MaterialEditorBulkModulusBox(Window* window, MaterialEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label)
			: TextBox(window, parent, alignment, position, size, label, nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"])
		{
			Editor = parent;
			RefreshData();
		}

		MaterialEditorBulkModulusBox::~MaterialEditorBulkModulusBox() {}

		void MaterialEditorBulkModulusBox::SaveValue()
		{
			float s = Editor->Material->BulkModulus / 1.0e9f;
			try	{
				s = std::stof(Label->GetText());
				Editor->Material->BulkModulus = (s < 0) ? 0 : s * 1.0e9f;
			}
			catch (std::invalid_argument &e) { (void*)&e; }
			catch (std::out_of_range &e) { (void*)&e; }
			RefreshData();
			TextBox::SaveValue();
		}

		void MaterialEditorBulkModulusBox::RefreshData()
		{
			auto v = Editor->Material->BulkModulus / 1.0e9f;
			Label->UpdateText(std::to_string(v));
		}





		MaterialEditorShearModulusBox::MaterialEditorShearModulusBox(Window* window, MaterialEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label)
			: TextBox(window, parent, alignment, position, size, label, nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"])
		{
			Editor = parent;
			RefreshData();
		}

		MaterialEditorShearModulusBox::~MaterialEditorShearModulusBox() {}

		void MaterialEditorShearModulusBox::SaveValue()
		{
			float s = Editor->Material->ShearModulus / 1.0e9f;
			try	{
				s = std::stof(Label->GetText());
				Editor->Material->ShearModulus = (s < 0) ? 0 : s * 1.0e9f;
			}
			catch (std::invalid_argument &e) { (void*)&e; }
			catch (std::out_of_range &e) { (void*)&e; }
			RefreshData();
			TextBox::SaveValue();
		}

		void MaterialEditorShearModulusBox::RefreshData()
		{
			auto v = Editor->Material->ShearModulus / 1.0e9f;
			Label->UpdateText(std::to_string(v));
		}





		MaterialEditorYieldStrengthBox::MaterialEditorYieldStrengthBox(Window* window, MaterialEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label)
			: TextBox(window, parent, alignment, position, size, label, nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"])
		{
			Editor = parent;
			RefreshData();
		}

		MaterialEditorYieldStrengthBox::~MaterialEditorYieldStrengthBox() {}

		void MaterialEditorYieldStrengthBox::SaveValue()
		{
			float s = Editor->Material->YieldStrength / 1.0e6f;
			try	{
				s = std::stof(Label->GetText());
				Editor->Material->YieldStrength = (s < 0) ? 0 : s * 1.0e6f;
			}
			catch (std::invalid_argument &e) { (void*)&e; }
			catch (std::out_of_range &e) { (void*)&e; }
			RefreshData();
			TextBox::SaveValue();
		}

		void MaterialEditorYieldStrengthBox::RefreshData()
		{
			auto v = Editor->Material->YieldStrength / 1.0e6f;
			Label->UpdateText(std::to_string(v));
		}





		MaterialEditorUltimateStrengthBox::MaterialEditorUltimateStrengthBox(Window* window, MaterialEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label)
			: TextBox(window, parent, alignment, position, size, label, nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"])
		{
			Editor = parent;
			RefreshData();
		}

		MaterialEditorUltimateStrengthBox::~MaterialEditorUltimateStrengthBox() {}

		void MaterialEditorUltimateStrengthBox::SaveValue()
		{
			float s = Editor->Material->UltimateStrength / 1.0e6f;
			try	{
				s = std::stof(Label->GetText());
				Editor->Material->UltimateStrength = (s < 0) ? 0 : s * 1.0e6f;
			}
			catch (std::invalid_argument &e) { (void*)&e; }
			catch (std::out_of_range &e) { (void*)&e; }
			RefreshData();
			TextBox::SaveValue();
		}

		void MaterialEditorUltimateStrengthBox::RefreshData()
		{
			auto v = Editor->Material->UltimateStrength / 1.0e6f;
			Label->UpdateText(std::to_string(v));
		}

	}
}