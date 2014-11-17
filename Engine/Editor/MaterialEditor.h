#pragma once
#include "Core.h"
#include "GuiItem.h"
#include "Material.h"

namespace Core
{
	namespace Gui
	{

		class MaterialBrowser
			: public Item
		{
		protected:
			std::pair<std::string, std::string>* Mat;
			Window* WindowPtr;
			Item* Opener;

			void BuildList();

		public:
			MaterialBrowser(std::pair<std::string, std::string>* mat, Window* window, Item* opener);
			virtual ~MaterialBrowser();

			virtual void Resolve(std::string name);
		};

		class MaterialBrowserButton
			: public Item
		{
		protected:
			MaterialBrowser* Parent;

		public:
			MaterialBrowserButton(MaterialBrowser* fbrowser, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~MaterialBrowserButton();

			virtual void OnClick(int button);
		};





		class MaterialButton
			: public Item
		{
		protected:
			Window* WindowPtr;

		public:
			MaterialButton(Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~MaterialButton();

			virtual void OnClick(int button);
		};
		
		class MaterialEditorPicker
			: public Item
		{
		protected:
			Window* WindowPtr;

			void BuildList();

		public:
			MaterialEditorPicker(Window* window);
			~MaterialEditorPicker();

			virtual void Invalidate(bool includeChildren = true);
			virtual void Resolve(std::string name);
		};

		class MaterialEditorPickerButton
			: public Item
		{
		protected:
			MaterialEditorPicker* Picker;

		public:
			MaterialEditorPickerButton(MaterialEditorPicker* picker, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~MaterialEditorPickerButton();

			virtual void OnClick(int button);
		};



				
		
		class MaterialEditor
			: public Item
		{
		protected:
			Window* WindowPtr;
			bool ShouldClose;

		public:
			std::string Name;
			Core::Material* Material;

			MaterialEditor(std::string material, Window* window, Item* parent);
			virtual ~MaterialEditor();

			virtual void Invalidate(bool includeChildren = true);
			virtual void Update(glm::vec2 mousePosition);
			virtual void Destroy();

			virtual void Rebuild();
		};

		class MaterialEditorSaveButton
			: public Item
		{
		protected:
			MaterialEditor* Editor;

		public:
			MaterialEditorSaveButton(MaterialEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~MaterialEditorSaveButton();

			virtual void OnClick(int button);
		};

		class MaterialEditorCopyButton
			: public Item
		{
		protected:
			MaterialEditor* Editor;

		public:
			MaterialEditorCopyButton(MaterialEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~MaterialEditorCopyButton();

			virtual void OnClick(int button);
		};

		class MaterialEditorDeleteButton
			: public Item
		{
		protected:
			MaterialEditor* Editor;

		public:
			MaterialEditorDeleteButton(MaterialEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~MaterialEditorDeleteButton();

			virtual void OnClick(int button);
		};		


		class MaterialEditorNoiseAlgorithmBox
			: public TextBox, public Refreshable
		{
		protected:
			MaterialEditor* Editor;

		public:
			MaterialEditorNoiseAlgorithmBox(Window* window, MaterialEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label);
			virtual ~MaterialEditorNoiseAlgorithmBox();

			virtual void SaveValue();
			virtual void RefreshData();
		};



		class MaterialEditorDensityBox
			: public TextBox, public Refreshable
		{
		protected:
			MaterialEditor* Editor;

		public:
			MaterialEditorDensityBox(Window* window, MaterialEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label);
			virtual ~MaterialEditorDensityBox();

			virtual void SaveValue();
			virtual void RefreshData();
		};

		class MaterialEditorYoungsModulusBox
			: public TextBox, public Refreshable
		{
		protected:
			MaterialEditor* Editor;

		public:
			MaterialEditorYoungsModulusBox(Window* window, MaterialEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label);
			virtual ~MaterialEditorYoungsModulusBox();

			virtual void SaveValue();
			virtual void RefreshData();
		};

		class MaterialEditorBulkModulusBox
			: public TextBox, public Refreshable
		{
		protected:
			MaterialEditor* Editor;

		public:
			MaterialEditorBulkModulusBox(Window* window, MaterialEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label);
			virtual ~MaterialEditorBulkModulusBox();

			virtual void SaveValue();
			virtual void RefreshData();
		};

		class MaterialEditorShearModulusBox
			: public TextBox, public Refreshable
		{
		protected:
			MaterialEditor* Editor;

		public:
			MaterialEditorShearModulusBox(Window* window, MaterialEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label);
			virtual ~MaterialEditorShearModulusBox();

			virtual void SaveValue();
			virtual void RefreshData();
		};

		class MaterialEditorYieldStrengthBox
			: public TextBox, public Refreshable
		{
		protected:
			MaterialEditor* Editor;

		public:
			MaterialEditorYieldStrengthBox(Window* window, MaterialEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label);
			virtual ~MaterialEditorYieldStrengthBox();

			virtual void SaveValue();
			virtual void RefreshData();
		};

		class MaterialEditorUltimateStrengthBox
			: public TextBox, public Refreshable
		{
		protected:
			MaterialEditor* Editor;

		public:
			MaterialEditorUltimateStrengthBox(Window* window, MaterialEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label);
			virtual ~MaterialEditorUltimateStrengthBox();

			virtual void SaveValue();
			virtual void RefreshData();
		};

	}
}