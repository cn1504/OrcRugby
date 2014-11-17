#pragma once
#include "Core.h"
#include "GuiItem.h"
#include "Input.h"
#include "SkeletalAnimation.h"


namespace Core
{
	namespace Gui
	{

		class ImportButton
			: public Item
		{
		protected:
			Window* WindowPtr;

		public:
			ImportButton(Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~ImportButton();

			virtual void OnClick(int button);
		};

		class AssetBrowser
			: public FileBrowser
		{
		public:
			AssetBrowser(std::string title, std::string path, std::string targetExt, Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~AssetBrowser();

			virtual void Resolve(std::string name);
		};



		class CameraToggleButton
			: public Item
		{
		protected:
			Window* WindowPtr;
			Input::CameraMode CurrentMode;

			Texture* Cam1Image;
			Texture* Cam1MouseOverImage;
			Texture* Cam1MouseDownImage;
			Texture* Cam2Image;
			Texture* Cam2MouseOverImage;
			Texture* Cam2MouseDownImage;

		public:
			CameraToggleButton(Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* cam1Image = nullptr, Texture* cam1MouseOverImage = nullptr, Texture* cam1MouseDownImage = nullptr, Texture* cam2Image = nullptr, Texture* cam2MouseOverImage = nullptr, Texture* cam2MouseDownImage = nullptr);
			virtual ~CameraToggleButton();

			virtual void OnClick(int button);
		};



		class CharacterBrowser
			: public FileBrowser
		{
		public:
			CharacterBrowser(std::string title, std::string path, std::string targetExt, Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~CharacterBrowser();

			virtual void Resolve(std::string name);
		};


		class CharacterButton
			: public Item
		{
		protected:
			Window* WindowPtr;

		public:
			CharacterButton(Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~CharacterButton();

			virtual void OnClick(int button);
		};


		class CharacterBrowserNewButton
			: public Item
		{
		protected:
			Window* WindowPtr;
			CharacterBrowser* CB;

		public:
			CharacterBrowserNewButton(Window* window, CharacterBrowser* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~CharacterBrowserNewButton();

			virtual void OnClick(int button);
		};



		
		class CharacterCreationDialog;

		class CharacterCreationNameTextBox
			: public TextBox
		{
		public:
			CharacterCreationNameTextBox(Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label);
			virtual ~CharacterCreationNameTextBox();
		};

		class CharacterCreationAcceptButton
			: public Item
		{
		protected:
			CharacterCreationDialog* Dialog;

		public:
			CharacterCreationAcceptButton(CharacterCreationDialog* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			~CharacterCreationAcceptButton();

			virtual void OnClick(int button);
		};

		class CharacterCreationDialog
			: public Item
		{
		protected:
			Window* WindowPtr;
			std::string Path;
			CharacterCreationNameTextBox* NameTextBox;

		public:
			CharacterCreationDialog(std::string path, Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			~CharacterCreationDialog();

			virtual void Accept();
		};




		class CharacterEditor
			: public Item
		{
		public:
			enum class Category { Categories, Meshes, Animations, FrameEvents };
			Category OpenCategory;

		protected:
			Window* WindowPtr;
			bool ShouldClose;

			Entity* Character;
			
		public:
			CharacterEditor(Entity* character, Window* window, Item* parent);
			virtual ~CharacterEditor();

			virtual void Invalidate(bool includeChildren = true);
			virtual void Update(glm::vec2 mousePosition);
			virtual void Destroy();

			virtual void Rebuild();
		};

		class CharacterEditorCategoryButton
			: public Item
		{
		protected:
			CharacterEditor* Editor;
			CharacterEditor::Category Target;

		public:
			CharacterEditorCategoryButton(CharacterEditor::Category target, CharacterEditor* parent, Item::Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~CharacterEditorCategoryButton();

			virtual void OnClick(int button);
		};


		class CharacterEditorScaleTextBox
			: public TextBox
		{
		protected:
			Entity* Character;

		public:
			CharacterEditorScaleTextBox(Entity* character, Window* window, Item* parent, Item::Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr, Texture* editImage = nullptr, Texture* editMouseOverImage = nullptr, Texture* editMouseDownImage = nullptr);
			virtual ~CharacterEditorScaleTextBox();

			virtual void SaveValue();
		};

		class CharacterEditorSaveButton
			: public Item
		{
		protected:
			Window* WindowPtr;
			Entity* Character;

		public:
			CharacterEditorSaveButton(Entity* character, Window* window, Item* parent);
			virtual ~CharacterEditorSaveButton();

			virtual void OnClick(int button);
		};



		class SkeletonBrowser
			: public FileBrowser
		{
		protected:
			Core::SkeletalAnimation* SA;
			Item* Opener;

		public:
			SkeletonBrowser(Core::SkeletalAnimation* sa, Item* opener, Window* window);
			virtual ~SkeletonBrowser();

			virtual void Resolve(std::string name);
		};

		class CharacterEditorSkeletonButton
			: public Item
		{
		protected:
			Window* WindowPtr;
			Entity* Character;

		public:
			CharacterEditorSkeletonButton(Entity* character, Window* window, Item* parent, Item::Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~CharacterEditorSkeletonButton();

			virtual void OnClick(int button);
		};	



		class AnimationBrowser
			: public FileBrowser
		{
		protected:
			Core::SkeletalAnimation* SA;
			Item* Opener;

		public:
			AnimationBrowser(Core::SkeletalAnimation* sa, Item* opener, Window* window);
			virtual ~AnimationBrowser();

			virtual void Resolve(std::string name);
		};

		class CharacterEditorAnimationButton
			: public Item
		{
		protected:
			Window* WindowPtr;
			Entity* Character;

		public:
			CharacterEditorAnimationButton(Entity* character, Window* window, Item* parent, Item::Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~CharacterEditorAnimationButton();

			virtual void OnClick(int button);
		};



		class MeshBrowser
			: public FileBrowser
		{
		protected:
			std::pair<std::string, std::string>* Mesh;
			Item* Opener;

		public:
			MeshBrowser(std::pair<std::string, std::string>* mesh, Window* window, Item* opener);
			virtual ~MeshBrowser();

			virtual void Resolve(std::string name);
		};


		class CharacterEditorMeshButton
			: public Item
		{
		protected:
			Window* WindowPtr; 
			std::pair<std::string, std::string>* Mesh;

		public:
			CharacterEditorMeshButton(std::pair<std::string, std::string>* mesh, Window* window, Item* parent, glm::vec2 position, glm::vec2 size, Text* label);
			virtual ~CharacterEditorMeshButton();

			virtual void OnClick(int button);
		};




		class CharacterEditorMaterialButton
			: public Item
		{
		protected:
			Window* WindowPtr;
			std::pair<std::string, std::string>* Material;

		public:
			CharacterEditorMaterialButton(std::pair<std::string, std::string>* material, Window* window, Item* parent, glm::vec2 position, glm::vec2 size, Text* label);
			virtual ~CharacterEditorMaterialButton();

			virtual void OnClick(int button);
		};




		class CharacterEditorAddMeshButton
			: public Item
		{
		protected:
			SkeletalAnimation* SA;
			CharacterEditor* Editor;

		public:
			CharacterEditorAddMeshButton(SkeletalAnimation* sa, CharacterEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~CharacterEditorAddMeshButton();

			virtual void OnClick(int button);
		};

		class CharacterEditorRemoveMeshButton
			: public Item
		{
		protected:
			SkeletalAnimation* SA;
			CharacterEditor* Editor;
			int Index;

		public:
			CharacterEditorRemoveMeshButton(SkeletalAnimation* sa, int i, CharacterEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~CharacterEditorRemoveMeshButton();

			virtual void OnClick(int button);
		};



		class CharacterEditorAnimationLine;

		class CharacterEditorAnimationNameTextBox
			: public TextBox
		{
		protected:
			CharacterEditorAnimationLine* Line;
			SkeletalAnimation* SA;
			std::string Name;

		public:
			CharacterEditorAnimationNameTextBox(SkeletalAnimation* sa, std::string name, Window* window, CharacterEditorAnimationLine* parent, Item::Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label);
			virtual ~CharacterEditorAnimationNameTextBox();

			virtual void SaveValue();
			void RefreshData(std::string name);
		};

		class CharacterEditorAnimationFirstTextBox
			: public TextBox
		{
		protected:
			CharacterEditorAnimationLine* Line;
			SkeletalAnimation* SA;
			std::string Name;

		public:
			CharacterEditorAnimationFirstTextBox(SkeletalAnimation* sa, std::string name, Window* window, CharacterEditorAnimationLine* parent, Item::Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label);
			virtual ~CharacterEditorAnimationFirstTextBox();

			virtual void SaveValue();
			void RefreshData(std::string name);
		};

		class CharacterEditorAnimationLastTextBox
			: public TextBox
		{
		protected:
			CharacterEditorAnimationLine* Line;
			SkeletalAnimation* SA;
			std::string Name;

		public:
			CharacterEditorAnimationLastTextBox(SkeletalAnimation* sa, std::string name, Window* window, CharacterEditorAnimationLine* parent, Item::Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label);
			virtual ~CharacterEditorAnimationLastTextBox();

			virtual void SaveValue();
			void RefreshData(std::string name);
		};

		class CharacterEditorAnimationTimeScaleTextBox
			: public TextBox
		{
		protected:
			CharacterEditorAnimationLine* Line;
			SkeletalAnimation* SA;
			std::string Name;

		public:
			CharacterEditorAnimationTimeScaleTextBox(SkeletalAnimation* sa, std::string name, Window* window, CharacterEditorAnimationLine* parent, Item::Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label);
			virtual ~CharacterEditorAnimationTimeScaleTextBox();

			virtual void SaveValue();
			void RefreshData(std::string name);
		};

		class CharacterEditorRemoveAnimationButton
			: public Item
		{
		protected:
			SkeletalAnimation* SA;
			CharacterEditor* Editor;
			std::string Name;

		public:
			CharacterEditorRemoveAnimationButton(SkeletalAnimation* sa, std::string name, CharacterEditor* editor, CharacterEditorAnimationLine* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~CharacterEditorRemoveAnimationButton();

			virtual void OnClick(int button);
			void RefreshData(std::string name);
		};

		class CharacterEditorPlayAnimationButton
			: public Item
		{
		protected:
			SkeletalAnimation* SA;
			std::string Name;

		public:
			CharacterEditorPlayAnimationButton(SkeletalAnimation* sa, std::string name, CharacterEditorAnimationLine* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~CharacterEditorPlayAnimationButton();

			virtual void OnClick(int button);
			void RefreshData(std::string name);
		};

		class CharacterEditorAnimationLine
			: public Item
		{
		protected:
			CharacterEditorAnimationNameTextBox* NameTextBox;
			CharacterEditorAnimationFirstTextBox* FirstTextBox;
			CharacterEditorAnimationLastTextBox* LastTextBox;
			CharacterEditorAnimationTimeScaleTextBox* TimeScaleTextBox;
			CharacterEditorPlayAnimationButton* PlayButton;
			CharacterEditorRemoveAnimationButton* RemoveButton;

		public:
			CharacterEditorAnimationLine(SkeletalAnimation* sa, std::string name, Window* window, CharacterEditor* parent, Item::Alignment alignment, glm::vec2 position, glm::vec2 size);
			virtual ~CharacterEditorAnimationLine();

			void RefreshData(std::string name);
		};

		class CharacterEditorAddAnimationButton
			: public Item
		{
		protected:
			SkeletalAnimation* SA;
			CharacterEditor* Editor;

		public:
			CharacterEditorAddAnimationButton(SkeletalAnimation* sa, CharacterEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~CharacterEditorAddAnimationButton();

			virtual void OnClick(int button);
		};

		class CharacterEditorAddFrameEventButton
			: public Item
		{
		protected:
			SkeletalAnimation* SA;
			CharacterEditor* Editor;

		public:
			CharacterEditorAddFrameEventButton(SkeletalAnimation* sa, CharacterEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~CharacterEditorAddFrameEventButton();

			virtual void OnClick(int button);
		};
		
		class CharacterEditorFrameEventFrameNumberTextBox
			: public TextBox
		{
		protected:
			CharacterEditor* Editor;
			SkeletalAnimation* SA;
			unsigned int Frame;
			TriggerEvent* TE;

		public:
			CharacterEditorFrameEventFrameNumberTextBox(unsigned int frame, TriggerEvent* te, SkeletalAnimation* sa, Window* window, CharacterEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr, Texture* editImage = nullptr, Texture* editMouseOverImage = nullptr, Texture* editMouseDownImage = nullptr);
			virtual ~CharacterEditorFrameEventFrameNumberTextBox();

			virtual void SaveValue();
		};

		class CharacterEditorRemoveFrameEventButton
			: public Item
		{
		protected:
			SkeletalAnimation* SA;
			CharacterEditor* Editor;
			unsigned int Frame;
			TriggerEvent* TE;

		public:
			CharacterEditorRemoveFrameEventButton(unsigned int frame, TriggerEvent* te, SkeletalAnimation* sa, CharacterEditor* editor, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~CharacterEditorRemoveFrameEventButton();

			virtual void OnClick(int button);
			void RefreshData(std::string name);
		};

	}
}