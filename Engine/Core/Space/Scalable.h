#pragma once
#include <Core.h>

namespace Core
{
	namespace Space
	{
		class Scalable3D
		{
		public:
			virtual ~Scalable3D();
			virtual void Scale(const glm::vec3& delta) = 0;
		};

		class Scalable2D
		{
		public:
			virtual ~Scalable2D();
			virtual void Scale(const glm::vec2& delta) = 0;
		};

		class Scalable2DInt
		{
		public:
			virtual ~Scalable2DInt();
			virtual void Scale(const glm::ivec2& delta) = 0;
		};



		class Scale3DAction
		{
		public:
			virtual ~Scale3DAction();
			virtual void Perform(const glm::vec3& delta) = 0;
		};

		class Scale3D : public Scale3DAction
		{
		private:
			Scalable3D* Target;
		public:
			Scale3D(Scalable3D* target);
			virtual ~Scale3D();
			virtual void Perform(const glm::vec3& delta);
		};

		class DontScale3D : public Scale3DAction
		{
		private:
			Scalable3D* Target;
		public:
			DontScale3D(Scalable3D* target);
			virtual ~DontScale3D();
			virtual void Perform(const glm::vec3& delta);
		};


		class Scale2DAction
		{
		public:
			virtual ~Scale2DAction();
			virtual void Perform(const glm::vec2& delta) = 0;
		};

		class Scale2D : public Scale2DAction
		{
		private:
			Scalable2D* Target;
		public:
			Scale2D(Scalable2D* target);
			virtual ~Scale2D();
			virtual void Perform(const glm::vec2& delta);
		};

		class DontScale2D : public Scale2DAction
		{
		private:
			Scalable2D* Target;
		public:
			DontScale2D(Scalable2D* target);
			virtual ~DontScale2D();
			virtual void Perform(const glm::vec2& delta);
		};



		class Scale2DIntAction
		{
		public:
			virtual ~Scale2DIntAction();
			virtual void Perform(const glm::ivec2& delta) = 0;
		};

		class Scale2DInt : public Scale2DIntAction
		{
		private:
			Scalable2DInt* Target;
		public:
			Scale2DInt(Scalable2DInt* target);
			virtual ~Scale2DInt();
			virtual void Perform(const glm::ivec2& delta);
		};

		class DontScale2DInt : public Scale2DIntAction
		{
		private:
			Scalable2DInt* Target;
		public:
			DontScale2DInt(Scalable2DInt* target);
			virtual ~DontScale2DInt();
			virtual void Perform(const glm::ivec2& delta);
		};
	}
}