#include "GuiItem.h"
#include "Window.h"
#include "Assets.h"

namespace Core
{
	namespace Gui
	{

		Item::Item(Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
		{
			Parent = parent;
			PRA = alignment;
			Offset = position;
			Size = size;
			Label = label;
			Image = image;
			MouseOverImage = mouseOverImage;
			MouseDownImage = mouseDownImage;
			
			MoveElapsed = 0.0f;
			MoveDuration = -1.0f;
			MoveType = MoveStyle::Instant;

			StartDrag = false;

			glGenVertexArrays(1, &quad_VertexArrayID);
			glGenBuffers(1, &quad_vertexbuffer);

			Invalidate();
		}


		Item::~Item()
		{
			if (Label != nullptr)
			{
				delete Label;
			}
			glDeleteVertexArrays(1, &quad_VertexArrayID);
			glDeleteBuffers(1, &quad_vertexbuffer);
		}


		void Item::BuildGeometry()
		{
			if (Label != nullptr)
			{
				Label->Move(Position);
			}

			// Convert from pixel counts to opengl coords
			GLfloat wWidth = (float)Settings::Window::Width;
			GLfloat wHeight = (float)Settings::Window::Height;
			GLfloat wHalfWidth = wWidth / 2.0f;
			GLfloat wHalfHeight = wHeight / 2.0f;

			glm::vec2 pos((Position.x - wHalfWidth) / wHalfWidth, (wHeight - Position.y - wHalfHeight) / wHalfHeight);
			//GLfloat finalX = x / wWidth;
			//GLfloat finalY = y / wHeight;
			glm::vec2 dim(Size.x / wHalfWidth, Size.y / wHalfHeight);

			GLfloat g_quad_vertex_buffer_data[] = {
				pos.x, pos.y - dim.y, 0.0f, 1.0f,
				pos.x + dim.x, pos.y - dim.y, 1.0f, 1.0f,
				pos.x, pos.y, 0.0f, 0.0f,
				pos.x, pos.y, 0.0f, 0.0f,
				pos.x + dim.x, pos.y - dim.y, 1.0f, 1.0f,
				pos.x + dim.x, pos.y, 1.0f, 0.0f
			};

			// Update gpu memory
			glBindVertexArray(quad_VertexArrayID);
			glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_DYNAMIC_DRAW);
		}


		void Item::DrawQuad(Shader* shader, Texture* texture)
		{
			shader->MakeCurrent();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture->GetID());
			glUniform1i(shader->GetUL("GuiTexture"), 0);

			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
			glVertexAttribPointer(
				0,                                // attribute
				4,                                // size
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				(void*)0                          // array buffer offset
				);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glDisableVertexAttribArray(0);
		}


		void Item::Move(glm::vec2 position, float duration, MoveStyle style)
		{
			if (style == MoveStyle::Instant && MoveDuration > -0.5f)
			{
				TargetPosition += position;
				OriginalPosition += position;

				for (auto c : Children)
				{
					c->Move(position);
				}
			}
			else if (style == MoveStyle::Instant)
			{
				MoveElapsed = 0.0f;
				MoveDuration = duration;
				MoveType = MoveStyle::Linear;
				TargetPosition = position + Position;
				OriginalPosition = Position;
				Position = TargetPosition;

				Invalidate(false);

				for (auto c : Children)
				{
					c->Move(position);
				}
			}
			else
			{
				MoveElapsed = 0.0f;
				MoveDuration = duration;
				MoveType = style;
				TargetPosition = position + Position;
				OriginalPosition = Position;
			}
		}


		bool Item::IsMoving()
		{
			return MoveDuration > -0.5f;
		}


		void Item::SetLabel(std::string label)
		{
			Label->UpdateText(label);
		}


		void Item::Update(glm::vec2 mousePosition)
		{
			// Update position if moving
			if (MoveDuration > -0.5f)
			{
				MoveElapsed += Time::RealtimeDelta;
				glm::vec2 moveVector;

				if (MoveElapsed > MoveDuration)
				{
					// Stop Moving
					MoveDuration = -1.0f;
					Position = TargetPosition;
					moveVector = TargetPosition - Position;					
				}
				else
				{
					glm::vec2 oldPosition = Position;
					if (MoveType == MoveStyle::Linear)
					{
						Position = glm::mix(OriginalPosition, TargetPosition, MoveElapsed / MoveDuration);
					}
					else if (MoveType == MoveStyle::ExponentialIn)
					{
						float t = MoveElapsed / MoveDuration;
						float y = sqrt(t);
						Position = OriginalPosition * (1.0f - y) + TargetPosition * (y);
					}
					else if (MoveType == MoveStyle::ExponentialOut)
					{
						float t = MoveElapsed / MoveDuration;
						float y = t * t;
						Position = OriginalPosition * (1.0f - y) + TargetPosition * (y);
					}
					else if (MoveType == MoveStyle::Sinosoidal)
					{
						float t = MoveElapsed / MoveDuration;
						float y = (glm::cos(t * glm::pi<float>()) + 1.0f) * 0.5f;
						Position = OriginalPosition * y + TargetPosition * (1.0f - y);
					}
					moveVector = Position - oldPosition;
				}
				Offset += moveVector;
				
				for (auto c : Children)
				{
					c->Move(moveVector);
				}

				Invalidate(false);
			}


			// Update mouse relative state
			if (mousePosition.x < Position.x + Size.x && mousePosition.x > Position.x && mousePosition.y < Position.y + Size.y && mousePosition.y > Position.y)
			{
				if (State == Item::State::InActive)
				{
					State = Item::State::MouseOver;
				}
			}
			else
			{
				if (State == Item::State::MouseOver || State == Item::State::MouseDown)
				{
					State = Item::State::InActive;
				}
			}

			if (State == State::MouseDown)
			{	
				if (StartDrag)
				{
					OriginalMousePosition = mousePosition;
					StartDrag = false;
				}
				auto delta = mousePosition - OriginalMousePosition;
				OriginalMousePosition = mousePosition;
				OnDrag(delta);
			}

			for (auto c : Children)
			{
				c->Update(mousePosition);
			}
		}


		bool Item::OnMouseDown(int button)
		{
			if (State != State::MouseOver)
			{
				return false;
			}
			
			// If any child fails to catch event
			bool result = false;
			for (auto c : Children)
			{
				bool r = c->OnMouseDown(button);
				if (r == true)
					result = true;
			}
			if (result == false)
			{
				State = State::MouseDown;
				StartDrag = true;
			}
			return true;
		}


		bool Item::OnMouseRelease(int button)
		{
			if (State != State::MouseOver && State != State::MouseDown)
			{
				return false;
			}

			// If any child fails to catch event
			bool result = false;
			for (auto c : Children)
			{
				bool r = c->OnMouseRelease(button);
				if (r == true)
					result = true;
			}
			if (result == false)
			{
				State = Item::State::MouseOver;
				OnClick(button);		
			}
			return true;
		}


		void Item::Render(Shader* textureShader, Shader* fontShader)
		{
			if (State == Item::State::InActive)
			{
				if (Image != nullptr)
				{
					DrawQuad(textureShader, Image);
				}

				if (Label != nullptr)
				{
					fontShader->MakeCurrent();
					Label->Render(fontShader);
				}
			}

			else if (State == Item::State::MouseOver)
			{
				if (MouseOverImage != nullptr)
				{
					DrawQuad(textureShader, MouseOverImage);
				}
				else if (Image != nullptr)
				{
					DrawQuad(textureShader, Image);
				}

				if (Label != nullptr)
				{
					fontShader->MakeCurrent();
					Label->Render(fontShader);
				}
			}

			else if (State == Item::State::MouseDown)
			{
				if (MouseDownImage != nullptr)
				{
					DrawQuad(textureShader, MouseDownImage);
				}
				else if (MouseOverImage != nullptr)
				{
					DrawQuad(textureShader, MouseOverImage);
				}
				else if (Image != nullptr)
				{
					DrawQuad(textureShader, Image);
				}

				if (Label != nullptr)
				{
					fontShader->MakeCurrent();
					Label->Render(fontShader);
				}
			}

			for (auto c : Children)
			{
				c->Render(textureShader, fontShader);
			}
		}


		void Item::AddItem(Item* item)
		{
			Children.push_back(item);
		}

		void Item::RemoveItem(Item* item)
		{
			int i = 0;
			while (i < Children.size())
			{
				if (Children[i] == item)
					break;
				i++;
			}
			if (i == Children.size())
				return;

			Children[i] = Children.back();
			Children.resize(Children.size() - 1);
		}
		
		void Item::Destroy()
		{
			Parent->RemoveItem(this);
			delete this;
		}


		void Item::OnClick(int button) {}
		void Item::OnDrag(glm::vec2 deltaPosition) {}

		
		void Item::Resize(glm::vec2 size)
		{
			Size = size;

			BuildGeometry();
		}


		void Item::Invalidate(bool includeChildren)
		{
			Position = (Parent == nullptr) ? Offset :
				(PRA == Alignment::TopLeft) ? Parent->Position + Offset :
				(PRA == Alignment::TopCenter) ? Parent->Position + glm::vec2((Parent->Size.x * 0.5f) - (Size.x * 0.5f), 0.0f) + Offset :
				(PRA == Alignment::TopRight) ? Parent->Position + glm::vec2(Parent->Size.x - Size.x, 0.0f) + Offset :
				(PRA == Alignment::Left) ? Parent->Position + glm::vec2(0.0f, (Parent->Size.y * 0.5f) - (Size.y * 0.5f)) + Offset :
				(PRA == Alignment::Center) ? Parent->Position + glm::vec2((Parent->Size.x * 0.5f) - (Size.x * 0.5f), (Parent->Size.y * 0.5f) - (Size.y * 0.5f)) + Offset :
				(PRA == Alignment::Right) ? Parent->Position + glm::vec2(Parent->Size.x - Size.x, (Parent->Size.y * 0.5f) - (Size.y * 0.5f)) + Offset :
				(PRA == Alignment::BottomLeft) ? Parent->Position + glm::vec2(0.0f, Parent->Size.y - Size.y) + Offset :
				(PRA == Alignment::BottomCenter) ? Parent->Position + glm::vec2((Parent->Size.x * 0.5f) - (Size.x * 0.5f), Parent->Size.y - Size.y) + Offset :
				(PRA == Alignment::BottomRight) ? Parent->Position + glm::vec2(Parent->Size.x - Size.x, Parent->Size.y - Size.y) + Offset :
				Offset;

			BuildGeometry();

			if (includeChildren)
				for (auto c : Children)
				{
					c->Invalidate();
				}
		}


		glm::vec4 Item::GetDimensions()
		{
			return glm::vec4(Position.x, Position.y, Size.x, Size.y);
		}


		std::string Item::GetLabel()
		{
			return (Label == nullptr) ? "" : Label->GetText();
		}


		void Item::Rebuild() {}




		Border::Border(Orientation orientation, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			or = orientation;
		}


		Border::~Border()
		{
		}


		void Border::Invalidate(bool includeChildren)
		{
			if (or == Orientation::Horizontal)
			{
				Size.x = Parent->GetDimensions().z - abs(Offset.x * 2);
			}
			else
			{
				Size.y = Parent->GetDimensions().w - abs(Offset.y * 2);
			}
			Item::Invalidate(includeChildren);
		}




		TextBox::TextBox(Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage, Texture* editImage, Texture* editMouseOverImage, Texture* editMouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			WindowPtr = window;

			DefaultImage = image;
			DefaultMouseOverImage = mouseOverImage;
			DefaultMouseDownImage = mouseDownImage;
			EditImage = editImage;
			EditMouseOverImage = editMouseOverImage;
			EditMouseDownImage = editMouseDownImage;
		}


		TextBox::~TextBox()
		{
		}


		void TextBox::OnClick(int button)
		{
			WindowPtr->Input->ActivateTextInput(this);

			Image = EditImage;
			MouseOverImage = EditMouseOverImage;
			MouseDownImage = EditMouseDownImage;
		}


		void TextBox::SaveValue()
		{
			Image = DefaultImage;
			MouseOverImage = DefaultMouseOverImage;
			MouseDownImage = DefaultMouseDownImage;
		}





		TextBoxString::TextBoxString(std::string* var, Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage, Texture* editImage, Texture* editMouseOverImage, Texture* editMouseDownImage)
			: TextBox(window, parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage, editImage, editMouseOverImage, editMouseDownImage)
		{
			Var = var;
			label->UpdateText(*Var);
		}

		TextBoxString::~TextBoxString() {}

		void TextBoxString::SaveValue()
		{
			auto r = Label->GetText();
			if (r != "") *Var = r;
			Label->UpdateText(*Var);

			TextBox::SaveValue();
		}





		TextBoxFloat::TextBoxFloat(float* var, Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage, Texture* editImage, Texture* editMouseOverImage, Texture* editMouseDownImage)
			: TextBox(window, parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage, editImage, editMouseOverImage, editMouseDownImage)
		{
			Var = var;
			label->UpdateText(std::to_string(*Var));
		}
		
		TextBoxFloat::~TextBoxFloat() {}

		void TextBoxFloat::SaveValue()
		{
			try	{
				*Var = std::stof(Label->GetText());
			}
			catch (std::invalid_argument &e) { (void*)&e; }
			catch (std::out_of_range &e) { (void*)&e; }

			Label->UpdateText(std::to_string(*Var));

			TextBox::SaveValue();
		}





		TextBoxFloatZeroPlus::TextBoxFloatZeroPlus(float* var, Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage, Texture* editImage, Texture* editMouseOverImage, Texture* editMouseDownImage)
			: TextBox(window, parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage, editImage, editMouseOverImage, editMouseDownImage)
		{
			Var = var;
			label->UpdateText(std::to_string(*Var));
		}

		TextBoxFloatZeroPlus::~TextBoxFloatZeroPlus() {}

		void TextBoxFloatZeroPlus::SaveValue()
		{
			try	{
				auto v = std::stof(Label->GetText());
				*Var = (v < 0) ? 0.0f : v;
			}
			catch (std::invalid_argument &e) { (void*)&e; }
			catch (std::out_of_range &e) { (void*)&e; }

			Label->UpdateText(std::to_string(*Var));

			TextBox::SaveValue();
		}





		TextBoxFloatZeroToOne::TextBoxFloatZeroToOne(float* var, Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage, Texture* editImage, Texture* editMouseOverImage, Texture* editMouseDownImage)
			: TextBox(window, parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage, editImage, editMouseOverImage, editMouseDownImage)
		{
			Var = var;
			label->UpdateText(std::to_string(*Var));
		}

		TextBoxFloatZeroToOne::~TextBoxFloatZeroToOne() {}

		void TextBoxFloatZeroToOne::SaveValue()
		{
			try	{
				auto v = std::stof(Label->GetText());
				*Var = (v < 0.0f) ? 0.0f : (v > 1.0f) ? 1.0f : v;
			}
			catch (std::invalid_argument &e) { (void*)&e; }
			catch (std::out_of_range &e) { (void*)&e; }

			Label->UpdateText(std::to_string(*Var));

			TextBox::SaveValue();
		}





		TextBoxRGB::TextBoxRGB(float* var, Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage, Texture* editImage, Texture* editMouseOverImage, Texture* editMouseDownImage)
			: TextBox(window, parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage, editImage, editMouseOverImage, editMouseDownImage)
		{
			Var = var;
			auto i = (int)(*Var * 256.0f);
			Label->UpdateText(std::to_string(i));
		}

		TextBoxRGB::~TextBoxRGB() {}

		void TextBoxRGB::SaveValue()
		{
			auto v = *Var;
			int s = (int)(v * 255.0f);
			try	{
				s = std::stoi(Label->GetText());
				*Var = ((float)((s < 0) ? 0 : (s > 256) ? 256 : s)) / 256.0f;
			}
			catch (std::invalid_argument &e) { (void*)&e; }
			catch (std::out_of_range &e) { (void*)&e; }

			auto i = (int)(*Var * 256.0f);
			Label->UpdateText(std::to_string(i));

			TextBox::SaveValue();
		}





		TextBoxInt::TextBoxInt(int* var, Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage, Texture* editImage, Texture* editMouseOverImage, Texture* editMouseDownImage)
			: TextBox(window, parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage, editImage, editMouseOverImage, editMouseDownImage)
		{
			Var = var;
			label->UpdateText(std::to_string(*Var));
		}

		TextBoxInt::~TextBoxInt() {}

		void TextBoxInt::SaveValue()
		{
			try	{
				*Var = std::stoi(Label->GetText());
			}
			catch (std::invalid_argument &e) { (void*)&e; }
			catch (std::out_of_range &e) { (void*)&e; }

			Label->UpdateText(std::to_string(*Var));

			TextBox::SaveValue();
		}





		TextBoxIntZeroPlus::TextBoxIntZeroPlus(int* var, Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage, Texture* editImage, Texture* editMouseOverImage, Texture* editMouseDownImage)
			: TextBox(window, parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage, editImage, editMouseOverImage, editMouseDownImage)
		{
			Var = var;
			label->UpdateText(std::to_string(*Var));
		}

		TextBoxIntZeroPlus::~TextBoxIntZeroPlus() {}

		void TextBoxIntZeroPlus::SaveValue()
		{
			try	{
				auto v = std::stoi(Label->GetText());
				*Var = (v < 0) ? 0 : v;
			}
			catch (std::invalid_argument &e) { (void*)&e; }
			catch (std::out_of_range &e) { (void*)&e; }

			Label->UpdateText(std::to_string(*Var));

			TextBox::SaveValue();
		}





		TextBoxRotationY::TextBoxRotationY(glm::quat* var, Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage, Texture* editImage, Texture* editMouseOverImage, Texture* editMouseDownImage)
			: TextBox(window, parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage, editImage, editMouseOverImage, editMouseDownImage)
		{
			Var = var;
			Rot = glm::degrees(glm::asin(Var->y) * 2.0f);

			label->UpdateText(std::to_string(Rot));
		}

		TextBoxRotationY::~TextBoxRotationY() {}

		void TextBoxRotationY::SaveValue()
		{
			try	{
				Rot = std::stof(Label->GetText());
				*Var = glm::quat(glm::vec3(0.0f, glm::radians(Rot), 0.0f));
			}
			catch (std::invalid_argument &e) { (void*)&e; }
			catch (std::out_of_range &e) { (void*)&e; }

			Debug::Log("Rotation: y - " + std::to_string(Var->y) + ", w - " + std::to_string(Var->w));
			Label->UpdateText(std::to_string(Rot));

			TextBox::SaveValue();
		}





		CheckBox::CheckBox(bool* value, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Texture* falseImage, Texture* falseMouseOverImage, Texture* falseMouseDownImage, Texture* trueImage, Texture* trueMouseOverImage, Texture* trueMouseDownImage)
			: Item(parent, alignment, position, size, nullptr, falseImage, falseMouseOverImage, falseMouseDownImage)
		{
			FalseImage = falseImage;
			FalseMouseOverImage = falseMouseOverImage;
			FalseMouseDownImage = falseMouseDownImage;
			TrueImage = trueImage;
			TrueMouseOverImage = trueMouseOverImage;
			TrueMouseDownImage = trueMouseDownImage;

			Value = value;

			if (*Value)
			{
				Image = trueImage;
				MouseOverImage = trueMouseOverImage;
				MouseDownImage = trueMouseDownImage;
			}
		}

		CheckBox::~CheckBox() {}

		void CheckBox::OnClick(int button)
		{
			*Value = !(*Value);

			if (*Value)
			{
				Image = TrueImage;
				MouseOverImage = TrueMouseOverImage;
				MouseDownImage = TrueMouseDownImage;
			}
			else
			{
				Image = FalseImage;
				MouseOverImage = FalseMouseOverImage;
				MouseDownImage = FalseMouseDownImage;
			}
		}





		DialogCloseButton::DialogCloseButton(Item* target, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			Target = target;
		}


		DialogCloseButton::~DialogCloseButton()
		{
		}


		void DialogCloseButton::OnClick(int button)
		{
			if (Target != nullptr)
			{
				Target->Destroy();
			}
		}


		


		Refreshable::~Refreshable() {}





		FileBrowser::FileBrowser(std::string title, std::string path, std::string targetExt, Window* window, Item* parent, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(parent, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			WindowPtr = window;
			Path = path;
			TargetExt = targetExt;
			TitleText = title;

			BuildList();
		}

		FileBrowser::~FileBrowser()	{}

		void FileBrowser::Resolve(std::string name)
		{
			std::string file = name + "." + TargetExt;
			if (FilePaths.count(file) > 0)
			{
				WindowPtr->Scene->AppendConsole("Importing File: " + file);
				//AssetImporter::LoadFile(FilePaths[file].c_str());
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

		void FileBrowser::BuildList()
		{
			Title = new Text(TitleText, 4, 4, Assets::Fonts["Consolas24"], glm::vec4(0.9, 0.9, 0.9, 1.0));
			AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(5, 0), glm::vec2(Size.x - 33.0f, 28), Title));

			float y = 36.0f;

			// Open ImportDialog Window
			tinydir_dir dir;
			auto result = tinydir_open(&dir, Path.c_str());
			if (result == 0)
			{
				Path = dir.path;
				while (dir.has_next)
				{
					tinydir_file file;
					tinydir_readfile(&dir, &file);

					if (file.name[0] == '.')
					{
					}
					else if (file.is_dir)
					{
						// Add dir item
						auto text = new Text(file.name, 27, 0, Assets::Fonts["Consolas16"], glm::vec4(0.9, 0.9, 0.9, 1.0));
						AddItem(new FileBrowserButton(this, Item::Alignment::TopLeft, glm::vec2(3.0f, y), glm::vec2(Size.x - 6.0f, 16), text, nullptr, Assets::Textures["DialogBackgroundMO"]));
						AddItem(new Item(this, Item::Alignment::TopLeft, glm::vec2(10, y), glm::vec2(16, 16), nullptr, Assets::Textures["Folder"]));
						y += 16;
					}
					else if (TargetExt == "" || std::string(file.extension) == TargetExt)
					{
						// Add item
						auto s = std::string(file.name);
						if (TargetExt != "")
							s.resize(s.length() - TargetExt.length() - 1);
						auto text = new Text(s, 7, 0, Assets::Fonts["Consolas16"], glm::vec4(0.9, 0.9, 0.9, 1.0)); 
						AddItem(new FileBrowserButton(this, Item::Alignment::TopLeft, glm::vec2(3, y), glm::vec2(Size.x - 6.0f, 16), text, nullptr, Assets::Textures["DialogBackgroundMO"]));
						FilePaths[file.name] = file.path;
						y += 16;
					}

					tinydir_next(&dir);
				}

				tinydir_close(&dir);

				Resize(glm::vec2(Size.x, y + 8.0f));
			}
			else
			{
				Title->UpdateText("Invalid Directory");
			}

			AddItem(new Border(Border::Orientation::Horizontal, this, Item::Alignment::TopLeft, glm::vec2(1, 1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
			AddItem(new Border(Border::Orientation::Vertical, this, Item::Alignment::TopLeft, glm::vec2(1, 1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
			AddItem(new Border(Border::Orientation::Horizontal, this, Item::Alignment::BottomRight, glm::vec2(-1, -1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
			AddItem(new Border(Border::Orientation::Vertical, this, Item::Alignment::BottomRight, glm::vec2(-1, -1), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));

			AddItem(new Border(Border::Orientation::Horizontal, this, Item::Alignment::TopLeft, glm::vec2(1, 28), glm::vec2(1, 1), nullptr, Assets::Textures["Border"]));
			AddItem(new DialogCloseButton(this, this, Item::Alignment::TopRight, glm::vec2(-6, 6), glm::vec2(17, 17), nullptr, Assets::Textures["CloseButton"], Assets::Textures["CloseButtonMO"]));

			Invalidate();
		}

		std::string FileBrowser::GetCurrentPath()
		{
			return Path;
		}




		FileBrowserButton::FileBrowserButton(FileBrowser* fbrowser, Alignment alignment, glm::vec2 position, glm::vec2 size, Text* label, Texture* image, Texture* mouseOverImage, Texture* mouseDownImage)
			: Item(fbrowser, alignment, position, size, label, image, mouseOverImage, mouseDownImage)
		{
			Parent = fbrowser;
		}

		FileBrowserButton::~FileBrowserButton() {}

		void FileBrowserButton::OnClick(int button)
		{
			Parent->Resolve(Label->GetText());
		}

	}
}