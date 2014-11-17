#pragma once
#include "Core.h"
#include "Texture.h"
#include "Text.h"
#include "Shader.h"

namespace Core
{
	namespace Gui
	{

		class Item
		{
		public:
			enum class State { InActive, MouseOver, MouseDown };
			enum class MoveStyle { Instant, Linear, Sinosoidal, ExponentialIn, ExponentialOut };
			enum class Alignment { TopLeft, TopCenter, TopRight, Left, Center, Right, BottomLeft, BottomCenter, BottomRight };

		protected:
			Item* Parent;
			std::vector<Item*> Children;

			Texture* Image;
			Texture* MouseOverImage;
			Texture* MouseDownImage;
			Text* Label;
			Alignment PRA;
			glm::vec2 Offset;
			glm::vec2 Position;
			glm::vec2 Size;
			State State;

			glm::vec2 TargetPosition;
			glm::vec2 OriginalPosition;
			float MoveDuration;
			float MoveElapsed;
			MoveStyle MoveType;

			bool StartDrag;
			glm::vec2 OriginalMousePosition;

			GLuint quad_VertexArrayID;
			GLuint quad_vertexbuffer;
			void BuildGeometry();
			void DrawQuad(Shader* shader, Texture* texture);

		public:
			Item(Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~Item();

			glm::vec4 GetDimensions();
			void Resize(glm::vec2 size);
			void Move(glm::vec2 deltaPosition, float duration = 0.0f, MoveStyle style = MoveStyle::Instant);
			bool IsMoving();

			void SetLabel(std::string label);
			std::string GetLabel();

			virtual void Update(glm::vec2 mousePosition);
			virtual void Invalidate(bool includeChildren = true);
			virtual void Rebuild();
			virtual void Render(Shader* textureShader, Shader* fontShader);
			virtual void Destroy();

			void AddItem(Item* item);
			void RemoveItem(Item* item);
			bool OnMouseDown(int button);
			bool OnMouseRelease(int button);
			
			virtual void OnClick(int button);
			virtual void OnDrag(glm::vec2 deltaPosition);
		};
		

		class Border
			: public Item
		{
		public:
			enum class Orientation { Horizontal, Vertical };
			Border(Orientation orientation, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~Border();
			virtual void Invalidate(bool includeChildren = true);

		protected:
			Orientation or;
		};


		class TextBox
			: public Item
		{
		protected:
			Window* WindowPtr;

			Texture* DefaultImage;
			Texture* DefaultMouseOverImage;
			Texture* DefaultMouseDownImage;
			Texture* EditImage;
			Texture* EditMouseOverImage;
			Texture* EditMouseDownImage;

		public:
			TextBox(Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr, Texture* editImage = nullptr, Texture* editMouseOverImage = nullptr, Texture* editMouseDownImage = nullptr);
			virtual ~TextBox();

			virtual void OnClick(int button);
			virtual void SaveValue();
		};
		
		class TextBoxString
			: public TextBox
		{
		protected:
			std::string* Var;

		public:
			TextBoxString(std::string* var, Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr, Texture* editImage = nullptr, Texture* editMouseOverImage = nullptr, Texture* editMouseDownImage = nullptr);
			virtual ~TextBoxString();

			virtual void SaveValue();
		};

		class TextBoxFloat
			: public TextBox
		{
		protected:
			float* Var;

		public:
			TextBoxFloat(float* var, Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr, Texture* editImage = nullptr, Texture* editMouseOverImage = nullptr, Texture* editMouseDownImage = nullptr);
			virtual ~TextBoxFloat();

			virtual void SaveValue();
		};
		
		class TextBoxFloatZeroPlus
			: public TextBox
		{
		protected:
			float* Var;

		public:
			TextBoxFloatZeroPlus(float* var, Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr, Texture* editImage = nullptr, Texture* editMouseOverImage = nullptr, Texture* editMouseDownImage = nullptr);
			virtual ~TextBoxFloatZeroPlus();

			virtual void SaveValue();
		};

		class TextBoxFloatZeroToOne
			: public TextBox
		{
		protected:
			float* Var;

		public:
			TextBoxFloatZeroToOne(float* var, Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr, Texture* editImage = nullptr, Texture* editMouseOverImage = nullptr, Texture* editMouseDownImage = nullptr);
			virtual ~TextBoxFloatZeroToOne();

			virtual void SaveValue();
		};

		class TextBoxRGB
			: public TextBox
		{
		protected:
			float* Var;

		public:
			TextBoxRGB(float* var, Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr, Texture* editImage = nullptr, Texture* editMouseOverImage = nullptr, Texture* editMouseDownImage = nullptr);
			virtual ~TextBoxRGB();

			virtual void SaveValue();
		};

		class TextBoxInt
			: public TextBox
		{
		protected:
			int* Var;

		public:
			TextBoxInt(int* var, Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr, Texture* editImage = nullptr, Texture* editMouseOverImage = nullptr, Texture* editMouseDownImage = nullptr);
			virtual ~TextBoxInt();

			virtual void SaveValue();
		};

		class TextBoxIntZeroPlus
			: public TextBox
		{
		protected:
			int* Var;

		public:
			TextBoxIntZeroPlus(int* var, Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr, Texture* editImage = nullptr, Texture* editMouseOverImage = nullptr, Texture* editMouseDownImage = nullptr);
			virtual ~TextBoxIntZeroPlus();

			virtual void SaveValue();
		};

		class TextBoxRotationY
			: public TextBox
		{
		protected:
			float Rot;
			glm::quat* Var;

		public:
			TextBoxRotationY(glm::quat* var, Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr, Texture* editImage = nullptr, Texture* editMouseOverImage = nullptr, Texture* editMouseDownImage = nullptr);
			virtual ~TextBoxRotationY();

			virtual void SaveValue();
		};


		class CheckBox
			: public Item
		{
		protected:
			Texture* FalseImage;
			Texture* FalseMouseOverImage;
			Texture* FalseMouseDownImage;
			Texture* TrueImage;
			Texture* TrueMouseOverImage;
			Texture* TrueMouseDownImage;

			bool* Value;

		public:
			CheckBox(bool* value, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Texture* falseImage = nullptr, Texture* falseMouseOverImage = nullptr, Texture* falseMouseDownImage = nullptr, Texture* trueImage = nullptr, Texture* trueMouseOverImage = nullptr, Texture* trueMouseDownImage = nullptr);
			virtual ~CheckBox();

			virtual void OnClick(int button);
		};


		class DialogCloseButton
			: public Item
		{
		protected:
			Item* Target;

		public:
			DialogCloseButton(Item* target, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~DialogCloseButton();

			virtual void OnClick(int button);
		};
		

		class Refreshable
		{
		public:
			virtual ~Refreshable();
			virtual void RefreshData() = 0;
		};


		class FileBrowser
			: public Item
		{
		protected:
			Window* WindowPtr;
			std::string TitleText;
			Text* Title;
			std::string Path;
			std::string TargetExt;
			std::unordered_map<std::string, std::string> FilePaths;

			void BuildList();

		public:
			FileBrowser(std::string title, std::string path, std::string targetExt, Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label = nullptr, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~FileBrowser();

			virtual void Resolve(std::string name);
			std::string GetCurrentPath();
		};


		class FileBrowserButton
			: public Item
		{
		protected:
			FileBrowser* Parent;

		public:
			FileBrowserButton(FileBrowser* fbrowser, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image = nullptr, Texture* mouseOverImage = nullptr, Texture* mouseDownImage = nullptr);
			virtual ~FileBrowserButton();

			virtual void OnClick(int button);
		};

	}
}