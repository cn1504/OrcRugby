#pragma once
#include <Core.h>

namespace Core
{
	namespace Space
	{
		class Translatable3D
		{
		public:
			virtual ~Translatable3D();
			virtual void Translate(const glm::vec3& delta) = 0;
		};

		class Translatable2D
		{
		public:
			virtual ~Translatable2D();
			virtual void Translate(const glm::vec2& delta) = 0;
		};

		class Translatable2DInt
		{
		public:
			virtual ~Translatable2DInt();
			virtual void Translate(const glm::ivec2& delta) = 0;
		};


		class Translate3DAction
		{
		public:
			virtual ~Translate3DAction();
			virtual void Perform(const glm::vec3& delta) = 0;
		};

		class Translate3D : public Translate3DAction
		{
		private:
			Translatable3D* Target;
		public:
			Translate3D(Translatable3D* target);
			virtual ~Translate3D();
			virtual void Perform(const glm::vec3& delta);
		};

		class DontTranslate3D : public Translate3DAction
		{
		private:
			Translatable3D* Target;
		public:
			DontTranslate3D(Translatable3D* target);
			virtual ~DontTranslate3D();
			virtual void Perform(const glm::vec3& delta);
		};


		class Translate2DAction
		{
		public:
			virtual ~Translate2DAction();
			virtual void Perform(const glm::vec2& delta) = 0;
		};

		class Translate2D : public Translate2DAction
		{
		private:
			Translatable2D* Target;
		public:
			Translate2D(Translatable2D* target);
			virtual ~Translate2D();
			virtual void Perform(const glm::vec2& delta);
		};

		class DontTranslate2D : public Translate2DAction
		{
		private:
			Translatable2D* Target;
		public:
			DontTranslate2D(Translatable2D* target);
			virtual ~DontTranslate2D();
			virtual void Perform(const glm::vec2& delta);
		};



		class Translate2DIntAction
		{
		public:
			virtual ~Translate2DIntAction();
			virtual void Perform(const glm::ivec2& delta) = 0;
		};

		class Translate2DInt : public Translate2DIntAction
		{
		private:
			Translatable2DInt* Target;
		public:
			Translate2DInt(Translatable2DInt* target);
			virtual ~Translate2DInt();
			virtual void Perform(const glm::ivec2& delta);
		};

		class DontTranslate2DInt : public Translate2DIntAction
		{
		private:
			Translatable2DInt* Target;
		public:
			DontTranslate2DInt(Translatable2DInt* target);
			virtual ~DontTranslate2DInt();
			virtual void Perform(const glm::ivec2& delta);
		};
	}
}