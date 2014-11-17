#include "Core.h"
#include "GuiItem.h"
#include "CharacterEditor.h"
#include "Feature.h"

namespace Core
{
	namespace Gui
	{

		class EditableFeature
			: public Feature
		{
		public:
			EditableFeature(std::string name);
			virtual ~EditableFeature();
			
			std::string* GetTagPtr();
			int* GetSpawnWeightPtr();
			int* GetSpawnWeightAdjustmentPtr();
			
			std::vector<Node*>* GetPlaceables();
			std::vector<Node*>* GetCreatures();
			std::vector<LightNode*>* GetLights();
			std::vector<Node*>* GetAudioSources();
			std::vector<std::pair<std::string, std::string>>* GetMeshes();
			std::vector<Collider*>* GetColliders();
		};





		class FeatureButton
			: public Item
		{
		protected:
			Window* WindowPtr;

		public:
			FeatureButton(Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~FeatureButton();

			virtual void OnClick(int button);
		};





		class FeatureBrowser
			: public FileBrowser
		{
		public:
			FeatureBrowser(std::string title, std::string path, std::string targetExt, Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~FeatureBrowser();

			virtual void Resolve(std::string name);
		};


		class FeatureBrowserNewButton
			: public Item
		{
		protected:
			Window* WindowPtr;
			FeatureBrowser* CB;

		public:
			FeatureBrowserNewButton(Window* window, FeatureBrowser* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~FeatureBrowserNewButton();

			virtual void OnClick(int button);
		};





		class FeatureCreationDialog;

		class FeatureCreationAcceptButton
			: public Item
		{
		protected:
			FeatureCreationDialog* Dialog;

		public:
			FeatureCreationAcceptButton(FeatureCreationDialog* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			~FeatureCreationAcceptButton();

			virtual void OnClick(int button);
		};

		class FeatureCreationDialog
			: public Item
		{
		protected:
			Window* WindowPtr;
			std::string Path;
			std::string Name;
			TextBoxString* NameTextBox;

		public:
			FeatureCreationDialog(std::string path, Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			~FeatureCreationDialog();

			virtual void Accept();
		};





		class FeatureEditor
			: public Item
		{
		protected:
			Window* WindowPtr;
			bool ShouldClose;

			std::vector<Refreshable*> Perishables;

		public:
			enum class Category { Categories, Placeables, Creatures, Lights, AudioSources, Meshes, Colliders };
			Category OpenCategory;
			float ScrollPosition;

			EditableFeature* Feature;

			FeatureEditor(std::string tile, Window* window, Item* parent);
			virtual ~FeatureEditor();

			virtual void Invalidate(bool includeChildren = true);
			virtual void Update(glm::vec2 mousePosition);
			virtual void Destroy();

			virtual void Rebuild();
			void RefreshData(std::string material);
		};



		class FeatureEditorSaveButton
			: public Item
		{
		protected:
			Window* WindowPtr;
			FeatureEditor* Editor;

		public:
			FeatureEditorSaveButton(FeatureEditor* editor, Window* window, Alignment alignment, glm::vec2 position, glm::vec2 size, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~FeatureEditorSaveButton();

			virtual void OnClick(int button);
		};



		class FeatureEditorCategoryButton
			: public Item
		{
		protected:
			FeatureEditor::Category Target;
			FeatureEditor* Editor;

		public:
			FeatureEditorCategoryButton(FeatureEditor::Category target, FeatureEditor* editor, Alignment alignment, glm::vec2 position, glm::vec2 size, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~FeatureEditorCategoryButton();

			virtual void OnClick(int button);
		};



		class FeatureEditorScrollButton
			: public Item
		{
		protected:
			float Delta;
			FeatureEditor* Editor;

		public:
			FeatureEditorScrollButton(float delta, FeatureEditor* editor, Alignment alignment, glm::vec2 position, glm::vec2 size, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~FeatureEditorScrollButton();

			virtual void OnClick(int button);
		};





		class FeatureEditorAddLightButton
			: public Item
		{
		protected:
			FeatureEditor* Editor;

		public:
			FeatureEditorAddLightButton(FeatureEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~FeatureEditorAddLightButton();

			virtual void OnClick(int button);
		};

		class FeatureEditorRemoveLightButton
			: public Item
		{
		protected:
			FeatureEditor* Editor;
			int Index;

		public:
			FeatureEditorRemoveLightButton(int i, FeatureEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~FeatureEditorRemoveLightButton();

			virtual void OnClick(int button);
		};





		class FeatureEditorAddMeshButton
			: public Item
		{
		protected:
			FeatureEditor* Editor;

		public:
			FeatureEditorAddMeshButton(FeatureEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~FeatureEditorAddMeshButton();

			virtual void OnClick(int button);
		};

		class FeatureEditorRemoveMeshButton
			: public Item
		{
		protected:
			FeatureEditor* Editor;
			int Index;

		public:
			FeatureEditorRemoveMeshButton(int i, FeatureEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~FeatureEditorRemoveMeshButton();

			virtual void OnClick(int button);
		};





		class FeatureEditorAddColliderButton
			: public Item
		{
		protected:
			FeatureEditor* Editor;

		public:
			FeatureEditorAddColliderButton(FeatureEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~FeatureEditorAddColliderButton();

			virtual void OnClick(int button);
		};

		class FeatureEditorRemoveColliderButton
			: public Item
		{
		protected:
			FeatureEditor* Editor;
			int Index;

		public:
			FeatureEditorRemoveColliderButton(int i, FeatureEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~FeatureEditorRemoveColliderButton();

			virtual void OnClick(int button);
		};

	}
}