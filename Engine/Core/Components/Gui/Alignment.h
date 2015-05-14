#pragma once

#include <Space/Transform2DIF.h>

namespace Core
{
	namespace Components
	{
		namespace Gui
		{
			class Alignment
			{
			protected:
				glm::vec2 Margin;

			private:
				virtual glm::vec2 CalculatePosition(const glm::vec2& parentSize, const glm::vec2& targetSize) = 0;

			public:
				Alignment(const glm::vec2& margin) : Margin(margin * 2.0f) {}
				virtual ~Alignment() {}

				void Realign(std::shared_ptr<Core::Space::Transform2DIF> target)
				{
					auto p = target->GetParent();
					if (p != nullptr)
					{
						auto pSize = p->GetScale();
						auto tSize = target->GetScale();
						target->Translate(CalculatePosition(pSize, tSize) - target->GetPosition());
					}
				}
			};


			class AlignTopLeft : public Alignment
			{
			public:
				AlignTopLeft(const glm::vec2& margin) : Alignment(margin) {}
				virtual ~AlignTopLeft() {}

			private:
				glm::vec2 CalculatePosition(const glm::vec2& parentSize, const glm::vec2& targetSize)
				{
					auto delta = parentSize - Margin - targetSize;
					return glm::vec2(-delta.x, delta.y);
				}
			};


			class AlignTop : public Alignment
			{
			public:
				AlignTop(const glm::vec2& margin) : Alignment(margin) {}
				virtual ~AlignTop() {}

			private:
				glm::vec2 CalculatePosition(const glm::vec2& parentSize, const glm::vec2& targetSize)
				{
					auto delta = parentSize - Margin - targetSize;
					return glm::vec2(0.0f, delta.y);
				}
			};


			class AlignTopRight : public Alignment
			{
			public:
				AlignTopRight(const glm::vec2& margin) : Alignment(margin) {}
				virtual ~AlignTopRight() {}

			private:
				glm::vec2 CalculatePosition(const glm::vec2& parentSize, const glm::vec2& targetSize)
				{
					auto delta = parentSize - Margin - targetSize;
					return delta;
				}
			};


			class AlignLeft : public Alignment
			{
			public:
				AlignLeft(const glm::vec2& margin) : Alignment(margin) {}
				virtual ~AlignLeft() {}

			private:
				glm::vec2 CalculatePosition(const glm::vec2& parentSize, const glm::vec2& targetSize)
				{
					auto delta = parentSize - Margin - targetSize;
					return glm::vec2(-delta.x, 0.0f);
				}
			};


			class AlignRight : public Alignment
			{
			public:
				AlignRight(const glm::vec2& margin) : Alignment(margin) {}
				virtual ~AlignRight() {}

			private:
				glm::vec2 CalculatePosition(const glm::vec2& parentSize, const glm::vec2& targetSize)
				{
					auto delta = parentSize - Margin - targetSize;
					return glm::vec2(delta.x, 0.0f);
				}
			};


			class AlignBottomLeft : public Alignment
			{
			public:
				AlignBottomLeft(const glm::vec2& margin) : Alignment(margin) {}
				virtual ~AlignBottomLeft() {}

			private:
				glm::vec2 CalculatePosition(const glm::vec2& parentSize, const glm::vec2& targetSize)
				{
					auto delta = parentSize - Margin - targetSize;
					return -delta;
				}
			};


			class AlignBottom : public Alignment
			{
			public:
				AlignBottom(const glm::vec2& margin) : Alignment(margin) {}
				virtual ~AlignBottom() {}

			private:
				glm::vec2 CalculatePosition(const glm::vec2& parentSize, const glm::vec2& targetSize)
				{
					auto delta = parentSize - Margin - targetSize;
					return glm::vec2(0.0f, -delta.y);
				}
			};
			

			class AlignBottomRight : public Alignment
			{
			public:
				AlignBottomRight(const glm::vec2& margin) : Alignment(margin) {}
				virtual ~AlignBottomRight() {}

			private:
				glm::vec2 CalculatePosition(const glm::vec2& parentSize, const glm::vec2& targetSize)
				{
					auto delta = parentSize - Margin - targetSize;
					return glm::vec2(delta.x, -delta.y);
				}
			};
		}
	}
}