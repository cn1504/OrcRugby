#include "TriggerEventEditor.h"
#include "Assets.h"
#include "Window.h"

namespace Core
{
	namespace Gui
	{

		void BuildTriggerEventGui(std::vector<TriggerEvent*>* tes, TriggerEvent* tePtr, Window* windowPtr, Item* parent, float& y)
		{
			float c1 = 10.0f;
			float c2 = 100.0f;

			parent->AddItem(new Item(parent, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(70, 20), new Text("Type:", 4, 3, Assets::Fonts["Consolas16"])));
			parent->AddItem(new TriggerTypeButton(tes, tePtr, windowPtr, parent, Item::Alignment::TopLeft, glm::vec2(c2, y), glm::vec2(100, 20), new Text("", 4, 3, Assets::Fonts["Consolas16"]), nullptr, Assets::Textures["DialogBackgroundMO"]));
			y += 20.0f;

			if (tePtr->Type == TRIGGER_SFX)
			{
				auto tep = (TriggerEventSFX*)(tePtr);
				parent->AddItem(new Item(parent, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(70, 20), new Text("Audio File:", 4, 3, Assets::Fonts["Consolas16"])));
				parent->AddItem(new AudioFileButton(&tep->AudioTrack, windowPtr, parent, Item::Alignment::TopLeft, glm::vec2(c2, y), glm::vec2(100, 20), new Text(tep->AudioTrack, 4, 3, Assets::Fonts["Consolas16"])));
				y += 20.0f;
				parent->AddItem(new Item(parent, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(70, 20), new Text("Volume:", 4, 3, Assets::Fonts["Consolas16"])));
				parent->AddItem(new TextBoxFloatZeroPlus(&tep->Volume, windowPtr, parent, Item::Alignment::TopLeft, glm::vec2(c2, y), glm::vec2(100, 20), new Text("", 4, 3, Assets::Fonts["Consolas16"]), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
				y += 20.0f;
			}
			else if (tePtr->Type == TRIGGER_PLAY_ANIMATION)
			{
				auto tep = (TriggerEventPlayAnimation*)(tePtr);
				parent->AddItem(new Item(parent, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(70, 20), new Text("Animation:", 4, 3, Assets::Fonts["Consolas16"])));
				parent->AddItem(new TextBoxString(&tep->AnimationName, windowPtr, parent, Item::Alignment::TopLeft, glm::vec2(c2, y), glm::vec2(100, 20), new Text("", 4, 3, Assets::Fonts["Consolas16"]), nullptr, Assets::Textures["TextBoxMO"], nullptr, Assets::Textures["TextBoxEdit"]));
				y += 20.0f;
				parent->AddItem(new Item(parent, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(70, 20), new Text("Loop:", 4, 3, Assets::Fonts["Consolas16"])));
				parent->AddItem(new CheckBox(&tep->Loop, parent, Item::Alignment::TopLeft, glm::vec2(c2, y), glm::vec2(17.0f, 17.0f), Assets::Textures["CloseButton"], Assets::Textures["CloseButtonMO"], nullptr, Assets::Textures["CheckSmall"], Assets::Textures["CheckSmallMO"]));
				y += 20.0f;
			}
			else
			{
				parent->AddItem(new Item(parent, Item::Alignment::TopLeft, glm::vec2(c1, y), glm::vec2(70, 20), new Text("Invalid Event Type", 4, 3, Assets::Fonts["Consolas16"])));
				y += 20.0f;
			}
			
		}


		


		AudioFileButton::AudioFileButton(std::string* file, Window* window, Item* parent, Item::Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label)
			: Item(parent, alignment, position, size, label, nullptr, Assets::Textures["DialogBackgroundMO"])
		{
			WindowPtr = window;
			File = file;
		}

		AudioFileButton::~AudioFileButton() {}

		void AudioFileButton::OnClick(int button)
		{
			WindowPtr->Scene->Gui->AddItem(new AudioBrowser(File, WindowPtr, this));
		}





		AudioBrowser::AudioBrowser(std::string* file, Window* window, Item* opener)
			: FileBrowser("Audio Files", "Resources/Audio/", "ogg", window, window->Scene->Gui, Item::Alignment::Center, glm::vec2(), glm::vec2(220, 150), nullptr, Assets::Textures["DialogBackground"])
		{
			File = file;
			Opener = opener;
		}

		AudioBrowser::~AudioBrowser()	{}

		void AudioBrowser::Resolve(std::string name)
		{
			std::string file = name + "." + TargetExt;
			if (FilePaths.count(file) > 0)
			{
				std::string relPath = std::string(&Path[16]) + name;
				*File = relPath;
				Opener->SetLabel(relPath);

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


		


		TriggerTypeButton::TriggerTypeButton(std::vector<TriggerEvent*>* tes, TriggerEvent* te, Window* window, Item* parent, Item::Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			WindowPtr = window;
			tePtr = te;
			teCollection = tes;

			label->UpdateText(te->Type);
		}

		TriggerTypeButton::~TriggerTypeButton() {}

		void TriggerTypeButton::OnClick(int button)
		{
			WindowPtr->Scene->Gui->AddItem(new TriggerTypeBrowser(teCollection, tePtr, WindowPtr, Parent));
		}

		void TriggerTypeButton::Rebuild()
		{
			Parent->Rebuild();
		}





		TriggerTypeBrowser::TriggerTypeBrowser(std::vector<TriggerEvent*>* tes, TriggerEvent* te, Window* window, Item* opener)
			: Item(window->Scene->Gui, Item::Alignment::Center, glm::vec2(), glm::vec2(220, 150), nullptr, Assets::Textures["DialogBackground"])
		{
			WindowPtr = window;
			tePtr = te;
			teCollection = tes;
			Opener = opener;

			BuildList();
		}


		TriggerTypeBrowser::~TriggerTypeBrowser() {}


		void TriggerTypeBrowser::Resolve(std::string name)
		{
			int i = 0;
			for (i = 0; i < teCollection->size() && tePtr != (*teCollection)[i]; i++) {}

			if (i != teCollection->size())
			{
				auto op = (*teCollection)[i];
				(*teCollection)[i] = CreateTriggerEvent(name);
				delete op;

				if (Opener != nullptr)
					Opener->Rebuild();
			}

			Parent->RemoveItem(this);
			delete this;
		}


		void TriggerTypeBrowser::BuildList()
		{
			auto Title = new Text("Trigger Events", 4, 4, Assets::Fonts["Consolas24"], glm::vec4(0.9, 0.9, 0.9, 1.0));
			AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(5, 0), glm::vec2(150, 28), Title));
			
			float y = 40.0f;
			float lh = 16.0f;
			float lw = 214.0f;

			AddItem(new TriggerTypeBrowserButton(this, Item::Alignment::TopLeft, glm::vec2(3, y), glm::vec2(lw, lh), new Text("SFX", 7, 0, Assets::Fonts["Consolas16"], glm::vec4(0.9, 0.9, 0.9, 1.0)), nullptr, Assets::Textures["DialogBackgroundMO"]));
			y += lh;
			AddItem(new TriggerTypeBrowserButton(this, Item::Alignment::TopLeft, glm::vec2(3, y), glm::vec2(lw, lh), new Text("PlayAnimation", 7, 0, Assets::Fonts["Consolas16"], glm::vec4(0.9, 0.9, 0.9, 1.0)), nullptr, Assets::Textures["DialogBackgroundMO"]));
			y += lh;


			Resize(glm::vec2(Size.x, 28 + lh * Children.size()));

			AddItem(new Border(Border::Orientation::Horizontal, this, Item::Alignment::TopLeft, glm::vec2(1, 1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
			AddItem(new Border(Border::Orientation::Vertical, this, Item::Alignment::TopLeft, glm::vec2(1, 1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
			AddItem(new Border(Border::Orientation::Horizontal, this, Item::Alignment::BottomRight, glm::vec2(-1, -1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
			AddItem(new Border(Border::Orientation::Vertical, this, Item::Alignment::BottomRight, glm::vec2(-1, -1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));

			AddItem(new Border(Border::Orientation::Horizontal, this, Item::Alignment::TopLeft, glm::vec2(1, 28), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
			AddItem(new DialogCloseButton(this, this, Item::Alignment::TopRight, glm::vec2(-6, 6), glm::vec2(17, 17), nullptr, Assets::Textures["CloseButton"], Assets::Textures["CloseButtonMO"]));

			Invalidate();
		}





		TriggerTypeBrowserButton::TriggerTypeBrowserButton(TriggerTypeBrowser* fbrowser, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(fbrowser, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			Parent = fbrowser;
		}


		TriggerTypeBrowserButton::~TriggerTypeBrowserButton()
		{
		}


		void TriggerTypeBrowserButton::OnClick(int button)
		{
			Parent->Resolve(Label->GetText());
		}
	}
}