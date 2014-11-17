#include "Core.h"
#include "GuiItem.h"
#include "Tile.h"
#include "CharacterEditor.h"

namespace Core
{
	namespace Gui
	{

		class EditableTile
			: public Tile
		{			
		public:
			EditableTile(std::string name);
			virtual ~EditableTile();
			
			std::string* GetName();

			int* GetTileWeight();
			int* GetTileAdjustment();

			bool* GetOnlySpawnOnce();

			glm::vec3* GetLowerBound();
			glm::vec3* GetUpperBound();

			std::vector<Node*>* GetTransitions();
			std::vector<Node*>* GetFeatures();
			std::vector<Node*>* GetPlaceables();
			std::vector<Node*>* GetCreatures();
			std::vector<LightNode*>* GetLights();
			std::vector<Node*>* GetAudioSources();
			std::vector<std::pair<std::string, std::string>>* GetMeshes();
			std::vector<Collider*>* GetColliders();
		};





		class TileButton
			: public Item
		{
		protected:
			Window* WindowPtr;

		public:
			TileButton(Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~TileButton();

			virtual void OnClick(int button);
		};





		class TileBrowser
			: public FileBrowser
		{
		public:
			TileBrowser(std::string title, std::string path, std::string targetExt, Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~TileBrowser();

			virtual void Resolve(std::string name);
		};


		class TileBrowserNewButton
			: public Item
		{
		protected:
			Window* WindowPtr;
			TileBrowser* CB;

		public:
			TileBrowserNewButton(Window* window, TileBrowser* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~TileBrowserNewButton();

			virtual void OnClick(int button);
		};





		class TileCreationDialog;

		class TileCreationNameTextBox
			: public TextBox
		{
		public:
			TileCreationNameTextBox(Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label);
			virtual ~TileCreationNameTextBox();
		};

		class TileCreationAcceptButton
			: public Item
		{
		protected:
			TileCreationDialog* Dialog;

		public:
			TileCreationAcceptButton(TileCreationDialog* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			~TileCreationAcceptButton();

			virtual void OnClick(int button);
		};

		class TileCreationDialog
			: public Item
		{
		protected:
			Window* WindowPtr;
			std::string Path;
			TileCreationNameTextBox* NameTextBox;

		public:
			TileCreationDialog(std::string path, Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			~TileCreationDialog();

			virtual void Accept();
		};


		
		

		class TileEditor
			: public Item
		{
		protected:
			Window* WindowPtr;
			bool ShouldClose;

			std::vector<Refreshable*> Perishables;

		public:
			enum class Category { Categories, Transitions, Features, Placeables, Creatures, Lights, AudioSources, Meshes, Colliders };
			Category OpenCategory;
			float ScrollPosition;

			EditableTile* Tile;

			TileEditor(std::string tile, Window* window, Item* parent);
			virtual ~TileEditor();

			virtual void Invalidate(bool includeChildren = true);
			virtual void Update(glm::vec2 mousePosition);
			virtual void Destroy();

			virtual void Rebuild();
			void RefreshData(std::string material);
		};



		class TileEditorSaveButton
			: public Item
		{
		protected:
			Window* WindowPtr;
			TileEditor* Editor;

		public:
			TileEditorSaveButton(TileEditor* editor, Window* window, Alignment alignment, glm::vec2 position, glm::vec2 size, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~TileEditorSaveButton();

			virtual void OnClick(int button);
		};
		
		

		class TileEditorCategoryButton
			: public Item
		{
		protected:
			TileEditor::Category Target;
			TileEditor* Editor;

		public:
			TileEditorCategoryButton(TileEditor::Category target, TileEditor* editor, Alignment alignment, glm::vec2 position, glm::vec2 size, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~TileEditorCategoryButton();

			virtual void OnClick(int button);
		};



		class TileEditorScrollButton
			: public Item
		{
		protected:
			float Delta;
			TileEditor* Editor;

		public:
			TileEditorScrollButton(float delta, TileEditor* editor, Alignment alignment, glm::vec2 position, glm::vec2 size, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~TileEditorScrollButton();

			virtual void OnClick(int button);
		};





		class TileEditorAddTransitionButton
			: public Item
		{
		protected:
			TileEditor* Editor;

		public:
			TileEditorAddTransitionButton(TileEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~TileEditorAddTransitionButton();

			virtual void OnClick(int button);
		};

		class TileEditorRemoveTransitionButton
			: public Item
		{
		protected:
			TileEditor* Editor;
			int Index;

		public:
			TileEditorRemoveTransitionButton(int i, TileEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~TileEditorRemoveTransitionButton();

			virtual void OnClick(int button);
		};





		class TileEditorAddFeatureButton
			: public Item
		{
		protected:
			TileEditor* Editor;

		public:
			TileEditorAddFeatureButton(TileEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~TileEditorAddFeatureButton();

			virtual void OnClick(int button);
		};

		class TileEditorRemoveFeatureButton
			: public Item
		{
		protected:
			TileEditor* Editor;
			int Index;

		public:
			TileEditorRemoveFeatureButton(int i, TileEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~TileEditorRemoveFeatureButton();

			virtual void OnClick(int button);
		};





		class TileEditorAddLightButton
			: public Item
		{
		protected:
			TileEditor* Editor;

		public:
			TileEditorAddLightButton(TileEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~TileEditorAddLightButton();

			virtual void OnClick(int button);
		};

		class TileEditorRemoveLightButton
			: public Item
		{
		protected:
			TileEditor* Editor;
			int Index;

		public:
			TileEditorRemoveLightButton(int i, TileEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~TileEditorRemoveLightButton();

			virtual void OnClick(int button);
		};
		




		class TileEditorAddMeshButton
			: public Item
		{
		protected:
			TileEditor* Editor;

		public:
			TileEditorAddMeshButton(TileEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~TileEditorAddMeshButton();

			virtual void OnClick(int button);
		};

		class TileEditorRemoveMeshButton
			: public Item
		{
		protected:
			TileEditor* Editor;
			int Index;

		public:
			TileEditorRemoveMeshButton(int i, TileEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~TileEditorRemoveMeshButton();

			virtual void OnClick(int button);
		};





		class TileEditorAddColliderButton
			: public Item
		{
		protected:
			TileEditor* Editor;

		public:
			TileEditorAddColliderButton(TileEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~TileEditorAddColliderButton();

			virtual void OnClick(int button);
		};

		class TileEditorRemoveColliderButton
			: public Item
		{
		protected:
			TileEditor* Editor;
			int Index;

		public:
			TileEditorRemoveColliderButton(int i, TileEditor* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~TileEditorRemoveColliderButton();

			virtual void OnClick(int button);
		};

	}
}