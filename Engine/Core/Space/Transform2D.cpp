#include "Transform2D.h"

using namespace Core::Space;

Transform2D::Transform2D()
{
	Position = glm::vec2(0.0f, 0.0f);
	Rotation = 0.0f;
	Scaling = glm::vec2(1.0f, 1.0f);
}
Transform2D::~Transform2D() {}

void Transform2D::Translate(const glm::vec2& delta)
{
	Position += delta;
}
void Transform2D::Rotate(float delta)
{
	Rotation += delta;
}
void Transform2D::Scale(const glm::vec2& delta)
{
	Scaling = glm::vec2(Scaling.x * delta.x, Scaling.y * delta.y);
}
const glm::vec2& Transform2D::GetPosition() { return Position; }
const float& Transform2D::GetRotation() { return Rotation; }
const glm::vec2& Transform2D::GetScale() { return Scaling; }

void Transform2D::UpdateMatrix()
{
	auto localTranslate = glm::mat3(
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		Position.x, Position.y, 1.0f
		);

	auto cosTheta = cos(Rotation);
	auto sinTheta = sin(Rotation);
	auto localRotation = glm::mat3(
		cosTheta, -sinTheta, 0.0f,
		sinTheta, cosTheta, 0.0f,
		0.0f, 0.0f, 1.0f
		);

	auto localScale = glm::mat3(
		Scaling.x, 0.0f, 0.0f,
		0.0f, Scaling.y, 0.0f,
		0.0f, 0.0f, 1.0f
		);
	
	if (auto p = GetParent())
	{
		auto m = p->GetUnscaledMatrix();
		Unscaled = m * localTranslate * localRotation;
		LocalSpace = Unscaled * localScale;
	}
	else
	{
		Unscaled = localTranslate * localRotation * localScale;
	}
}

const glm::mat3& Transform2D::GetMatrix()
{
	return LocalSpace;
}

const glm::mat3& Transform2D::GetUnscaledMatrix()
{
	return Unscaled;
}

glm::vec2 Transform2D::Down()
{
	auto m = GetMatrix();
	auto result = m * glm::vec3(0, -1, 0);
	return glm::vec2(result.x, result.y);
}
glm::vec2 Transform2D::Right()
{
	auto m = GetMatrix();
	auto result = m * glm::vec3(1, 0, 0);
	return glm::vec2(result.x, result.y);
}

void Transform2D::AddChild(std::shared_ptr<Transform2DIF> child)
{
	for (auto& c : Children)
	{
		if (c == child)
			return;
	}
	Children.push_back(child);
	child->SetParent(shared_from_this());
}
void Transform2D::RemoveChild(std::shared_ptr<Transform2DIF> child)
{
	for (size_t i = 0; i < Children.size(); i++)
	{
		if (Children[i] == child)
		{
			std::swap(Children[i], Children.back());
			Children.pop_back();
			i--;
		}
	}
}
std::shared_ptr<Transform2DIF> Transform2D::GetChild(size_t i)
{
	return Children.at(i);
}
size_t Transform2D::GetChildCount()
{
	return Children.size();
}
std::shared_ptr<Transform2DIF> Transform2D::GetParent()
{
	if (auto p = Parent.lock())
	{
		return p;
	}
	return nullptr;
}
void Transform2D::SetParent(std::shared_ptr<Transform2DIF> parent)
{
	Parent = parent;
}