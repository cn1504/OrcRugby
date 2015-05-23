#include "Transform.h"

using namespace Core::Space;

Transform::Transform() : Scaling(1.0f, 1.0f, 1.0f)
{
}

Transform::~Transform()
{

}

void Transform::Translate(const glm::vec3& delta)
{
	Position += delta;
}

void Transform::Rotate(const glm::quat& delta)
{
	Rotation = delta * Rotation;
}

void Transform::Rotate(const glm::vec3& deltaEuler)
{
	Rotation = glm::quat(deltaEuler) * Rotation;
}

void Transform::Scale(const glm::vec3& delta)
{
	Scaling *= delta;
}

const glm::vec3& Transform::GetPosition()
{	
	return Position;
}

const glm::quat& Transform::GetRotation()
{
	return Rotation;
}

const glm::vec3& Transform::GetScale()
{
	return Scaling;
}

void Transform::UpdateMatrix()
{
	Matrix = glm::translate(Position) * glm::toMat4(Rotation) * glm::scale(Scaling);

	if (auto p = Parent.lock())
	{
		Matrix = p->GetMatrix() * Matrix;
	}
}

const glm::mat4& Transform::GetMatrix()
{
	return Matrix;
}

glm::vec3 Transform::Forward()
{
	auto v = Matrix * glm::vec4(0, 0, 1, 0);
	return glm::normalize(glm::vec3(v.x, v.y, v.z));
}
glm::vec3 Transform::Right()
{
	auto v = Matrix * glm::vec4(-1, 0, 0, 0);
	return glm::normalize(glm::vec3(v.x, v.y, v.z));
}
glm::vec3 Transform::Up()
{
	auto v = Matrix * glm::vec4(0, 1, 0, 0);
	return glm::normalize(glm::vec3(v.x, v.y, v.z));
}

void Transform::AddChild(std::shared_ptr<TransformIF> child)
{
	for (auto& c : Children)
	{
		if (c == child)
			return;
	}
	Children.push_back(child);
	child->SetParent(shared_from_this());
}

void Transform::RemoveChild(std::shared_ptr<TransformIF> child)
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

std::shared_ptr<TransformIF> Transform::GetChild(size_t i)
{
	return Children.at(i);
}

size_t Transform::GetChildCount()
{
	return Children.size();
}
std::shared_ptr<TransformIF> Transform::GetParent()
{
	if (auto p = Parent.lock())
	{
		return p;
	}
	return nullptr;
}
void Transform::SetParent(std::shared_ptr<TransformIF> parent)
{
	Parent = parent;
}