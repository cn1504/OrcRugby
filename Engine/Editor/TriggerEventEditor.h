#pragma once
#include "Core.h"
#include "TriggerEvent.h"
#include "GuiItem.h"


namespace Core
{
	namespace Gui
	{

		void BuildTriggerEventGui(std::vector<TriggerEvent*>* tes, TriggerEvent* tePtr, Window* windowPtr, Item* parent, float& y);


		class AudioFileButton
			: public Item
		{
		protected:
			Window* WindowPtr;
			std::string* File;

		public:
			AudioFileButton(std::string* file, Window* window, Item* parent, Item::Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label);
			virtual ~AudioFileButton();

			virtual void OnClick(int button);
		};

		class AudioBrowser
			: public FileBrowser
		{
		protected:
			std::string* File;
			Item* Opener;

		public:
			AudioBrowser(std::string* file, Window* window, Item* opener);
			virtual ~AudioBrowser();

			virtual void Resolve(std::string name);
		};



		class TriggerTypeButton
			: public Item
		{
		protected:
			Window* WindowPtr;
			TriggerEvent* tePtr;
			std::vector<TriggerEvent*>* teCollection;

		public:
			TriggerTypeButton(std::vector<TriggerEvent*>* tes, TriggerEvent* te, Window* window, Item* parent, Item::Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~TriggerTypeButton();

			virtual void OnClick(int button);
			virtual void Rebuild();
		};

		class TriggerTypeBrowser
			: public Item
		{
		protected:
			Window* WindowPtr;
			Item* Opener;
			TriggerEvent* tePtr;
			std::vector<TriggerEvent*>* teCollection;

			void BuildList();

		public:
			TriggerTypeBrowser(std::vector<TriggerEvent*>* tes, TriggerEvent* te, Window* window, Item* opener);
			virtual ~TriggerTypeBrowser();

			virtual void Resolve(std::string name);
		};

		class TriggerTypeBrowserButton
			: public Item
		{
		protected:
			TriggerTypeBrowser* Parent;

		public:
			TriggerTypeBrowserButton(TriggerTypeBrowser* fbrowser, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~TriggerTypeBrowserButton();

			virtual void OnClick(int button);
		};
	}
}