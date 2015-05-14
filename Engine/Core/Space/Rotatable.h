#pragma once
#include <Core.h>

namespace Core
{
	namespace Space
	{
		class Rotatable3D
		{
		public:
			virtual ~Rotatable3D();
			virtual void Rotate(const glm::quat& delta) = 0;
			virtual void Rotate(const glm::vec3& deltaEuler) = 0;
		};

		class Rotatable2D
		{
		public:
			virtual ~Rotatable2D();
			virtual void Rotate(float delta) = 0;
		};
		


		class Rotate3DAction
		{
		public:
			virtual ~Rotate3DAction();
			virtual void Perform(const glm::quat& delta) = 0;
		};

		class Rotate3D : public Rotate3DAction
		{
		private:
			Rotatable3D* Target;
		public:
			Rotate3D(Rotatable3D* target);
			virtual ~Rotate3D();
			virtual void Perform(const glm::quat& delta);
		};

		class DontRotate3D : public Rotate3DAction
		{
		private:
			Rotatable3D* Target;
		public:
			DontRotate3D(Rotatable3D* target);
			virtual ~DontRotate3D();
			virtual void Perform(const glm::quat& delta);
		};



		class Rotate3DEulerAction
		{
		public:
			virtual ~Rotate3DEulerAction();
			virtual void Perform(const glm::vec3& delta) = 0;
		};

		class Rotate3DEuler : public Rotate3DEulerAction
		{
		private:
			Rotatable3D* Target;
		public:
			Rotate3DEuler(Rotatable3D* target);
			virtual ~Rotate3DEuler();
			virtual void Perform(const glm::vec3& delta);
		};

		class DontRotate3DEuler : public Rotate3DEulerAction
		{
		private:
			Rotatable3D* Target;
		public:
			DontRotate3DEuler(Rotatable3D* target);
			virtual ~DontRotate3DEuler();
			virtual void Perform(const glm::vec3& delta);
		};



		class Rotate2DAction
		{
		public:
			virtual ~Rotate2DAction();
			virtual void Perform(float delta) = 0;
		};

		class Rotate2D : public Rotate2DAction
		{
		private:
			Rotatable2D* Target;
		public:
			Rotate2D(Rotatable2D* target);
			virtual ~Rotate2D();
			virtual void Perform(float delta);
		};

		class DontRotate2D : public Rotate2DAction
		{
		private:
			Rotatable2D* Target;
		public:
			DontRotate2D(Rotatable2D* target);
			virtual ~DontRotate2D();
			virtual void Perform(float delta);
		};
	}
}